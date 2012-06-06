/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012 thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "PicDataBase.h"

#include <iostream>
#include <algorithm>
#include "../common/StringUtils.h"

const std::string PicData::cEmptyVector = "(empty)";
const std::string PicData::cNoTags = "(none)";

void mergeSets(std::set<std::string>& dest, const std::set<std::string>& other)
{
  std::set<std::string>::const_iterator dest_iter, other_iter;
  dest_iter = dest.begin();
  other_iter = other.begin();
  while (dest_iter!=dest.end() and other_iter!=other.end())
  {
    if (*dest_iter<*other_iter)
    {
      ++dest_iter;
    }
    else if (*dest_iter>*other_iter)
    {
      dest_iter = dest.insert(*other_iter).first;
      ++other_iter;
    }
    else
    {
      ++dest_iter;
      ++other_iter;
    }
  }//while
  //check for remainder
  while (other_iter!=other.end())
  {
    dest.insert(*other_iter);
    ++other_iter;
  }
}


void PicData::show() const
{
  std::cout << "  name: "<<name<<"\n  artist: "<<artist <<"\n  who: ";
  if (who.size()==0)
  {
    std::cout <<cEmptyVector;
  }
  else
  { //list all
    std::set<std::string>::const_iterator iter;
    for (iter=who.begin(); iter!=who.end(); ++iter)
    {
      std::cout << *iter <<" ";
    }//for
  } //else-branch
  std::cout <<"\n";
  //tags
  std::cout << "  " << DataBase::cTagPrefix<<": ";
  if (tags.size()==0)
  {
    std::cout <<cNoTags;
  }
  else
  { //list all
    std::set<std::string>::const_iterator iter;
    for (iter=tags.begin(); iter!=tags.end(); ++iter)
    {
      std::cout << *iter <<" ";
    }//for
  } //else-branch
  std::cout <<"\n";
}

void PicData::clear()
{
  name.clear();
  artist.clear();
  who.clear();
  tags.clear();
  hash_sha256.setToNull();
}

void PicData::mergeWith(const PicData& other)
{
  mergeSets(who, other.who);
  mergeSets(tags, other.tags);
  //only get the other name, if this one is empty
  if (name.empty() and not other.name.empty())
  {
    name = other.name;
  }
  //only get the other artist, if this one is empty
  if ((artist.empty() or (artist==Splitter::cUnknownArtist))
    and (!other.artist.empty() and (other.artist!=Splitter::cUnknownArtist)))
  {
    artist = other.artist;
  }

  //hrmpf;
  #warning Incomplete!
}

bool wsr_compare(const WhoStatRec& a, const WhoStatRec& b)
{
  return (a.count> b.count) or ((a.count==b.count) and (a.who<b.who));
}

const std::string DataBase::cFilePrefix = "file";
const std::string DataBase::cNamePrefix = "name";
const std::string DataBase::cArtistPrefix = "artist";
const std::string DataBase::cWhoPrefix = "who";
const std::string DataBase::cTagPrefix = "tags";
const std::string DataBase::cHashPrefix = "sha256";

DataBase& DataBase::getSingleton()
{
  static DataBase Instance;
  return Instance;
}

DataBase::~DataBase()
{
  m_Files.clear();
}

DataBase::DataBase()
{
  //empty
}

void DataBase::clearAllData()
{
  m_Files.clear();
}

bool DataBase::getFilesFromDirectory(const std::string& directory)
{
  std::vector<FileEntry> temp;
  if (!getDirectoryFileList(directory, temp, "", false))
  {
    return false;
  }
  if (temp.size()==0)
  {
    return false;
  }
  PicData temp_data;
  temp_data.artist = Splitter::cUnknownArtist;
  temp_data.name = "";
  temp_data.tags.clear();
  temp_data.who.clear();
  bool result = false;
  unsigned int i;
  for (i=0; i<temp.size(); i=i+1)
  {
    if (!(temp.at(i).IsDirectory))
    {
      m_Files[temp.at(i).FileName] = temp_data;
      result = true;
    }//if
  }//for
  return result;
}

void DataBase::addFile(const std::string& FileName, const PicData& data)
{
  if (!FileName.empty())
  {
    //delete old hash index entry, if present
    const std::map<std::string, PicData>::const_iterator iter = m_Files.find(FileName);
    if (iter!=m_Files.end())
    {
      if (!data.hash_sha256.isNull() and !iter->second.hash_sha256.isNull())
      {
        removeFromHashIndex(iter->second.hash_sha256, FileName);
      }
    }
    //set or update file entry
    m_Files[FileName] = data;
    //set new hash index entry
    if (!data.hash_sha256.isNull())
    {
      m_HashIndex[data.hash_sha256].insert(FileName);
    }
  }
}

bool DataBase::hasFile(const std::string& FileName) const
{
  return (m_Files.find(FileName) != m_Files.end());
}

const PicData& DataBase::getData(const std::string& FileName) const
{
  std::map<std::string, PicData>::const_iterator iter;
  iter = m_Files.find(FileName);
  if (iter != m_Files.end())
  {
    return iter->second;
  }
  throw 12345;
}

bool DataBase::hasHash(const SHA256::MessageDigest& hash) const
{
  return (m_HashIndex.find(hash)!=m_HashIndex.end());
}

const std::set<std::string>& DataBase::getFilesForHash(const SHA256::MessageDigest& hash) const
{
  const std::map<SHA256::MessageDigest, std::set<std::string> >::const_iterator iter
     = m_HashIndex.find(hash);
  if (iter!=m_HashIndex.end())
  {
    return iter->second;
  }
  //no entry present, throw exception
  throw 42;
}

void DataBase::AutoTag_Splitter()
{
  std::vector<TwoStrings> ts;
  PicData temp_data;

  //build FileEntry vector
  std::vector<FileEntry> fe_vec;
  fe_vec.clear();
  std::map<std::string, PicData>::const_iterator iter;
  iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    fe_vec.push_back((FileEntry){iter->first, false});
    ++iter;
  }//while
  ts = Splitter::splitFileNames(fe_vec);

  unsigned int i;
  i = fe_vec.size();
  if (i!= ts.size())
  {
    std::cout << "AutoTag_Splitter: Error: vectors are not of same size.\n";
    return;
  }
  for (i=0; i<ts.size(); i=i+1)
  {
    temp_data = getData(fe_vec.at(i).FileName);
    temp_data.artist = ts.at(i).Artist;
    temp_data.name = ts.at(i).Name;
    m_Files[fe_vec.at(i).FileName] = temp_data;
  }//for
  return;
}

void DataBase::ListData() const
{
  std::map<std::string, PicData>::const_iterator iter;
  iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    std::cout << cFilePrefix <<": "<<iter->first<<"\n";
    iter->second.show();
    iter++;
  }//while
}

unsigned int DataBase::getNumEntries() const
{
  return m_Files.size();
}

std::vector<std::string> DataBase::getUntaggedFiles() const
{
  std::vector<std::string> result;
  result.clear();
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    if (iter->second.tags.size() == 0)
    {
      result.push_back(iter->first);
    }
    ++iter;
  }
  return result;
}

std::vector<std::string> DataBase::getUnknownArtistFiles() const
{
  std::vector<std::string> result;
  result.clear();
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    if (iter->second.artist == Splitter::cUnknownArtist)
    {
      result.push_back(iter->first);
    }
    ++iter;
  }
  return result;
}

std::vector<std::string> DataBase::getUnknownWhoFiles() const
{
  std::vector<std::string> result;
  result.clear();
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    if (iter->second.who.size() == 0)
    {
      result.push_back(iter->first);
    }
    ++iter;
  }
  return result;
}

std::vector<std::string> DataBase::getNonexistingFiles(const std::string& BaseDir) const
{
  std::vector<std::string> result;
  result.clear();
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    if (!FileExists(BaseDir+iter->first))
    {
      result.push_back(iter->first);
    }
    ++iter;
  }
  return result;
}

bool DataBase::deleteFile(const std::string& FileName)
{
  return (m_Files.erase(FileName)>0);
}

void DataBase::purgeNonexistingFiles(const std::string& BaseDir)
{
  std::vector<std::string> deletionList = getNonexistingFiles(BaseDir);
  unsigned int i;
  for (i=0; i<deletionList.size(); i=i+1)
  {
    m_Files.erase(deletionList[i]);
  }//for
}

void DataBase::showTagStatistics() const
{
  std::map<std::string, unsigned int> tagList;
  tagList.clear();
  unsigned int total_tags=0;
  std::set<std::string>::const_iterator setIter;
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    for (setIter=iter->second.tags.begin(); setIter!=iter->second.tags.end(); ++setIter)
    {
      tagList[*setIter]++;
      ++total_tags;
    }//for
    ++iter;
  }
  //print it
  std::cout<<"Present tags in database:\n";
  std::map<std::string, unsigned int>::const_iterator listIter = tagList.begin();
  while (listIter != tagList.end())
  {
    std::cout << "  "<<listIter->first<<": "<<listIter->second<<"\n";
    ++listIter;
  }//while
  if (tagList.size()==0)
  {
    std::cout << "  none at all\n";
  }
  else
  {
    std::cout << "Total: "<<tagList.size()<<" distinct tags.\n";
  }
  std::cout << "A total of "<<total_tags<<" tags on "<<m_Files.size()
            <<" files, that means "<<static_cast<float>(total_tags)/m_Files.size()
            <<" tags per file.\n";
}

void DataBase::showWhoStatistics() const
{
  std::map<std::string, unsigned int> whoList;
  whoList.clear();
  std::set<std::string>::const_iterator it;
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    for (it=iter->second.who.begin(); it!=iter->second.who.end(); ++it)
    {
      whoList[*it]++;
    }//for
    ++iter;
  }
  //print it
  std::cout<<"Present person names in database:\n";
  std::map<std::string, unsigned int>::const_iterator listIter = whoList.begin();
  while (listIter != whoList.end())
  {
    std::cout << "  "<<listIter->first<<": "<<listIter->second<<"\n";
    ++listIter;
  }//while
  if (whoList.size()==0)
  {
    std::cout << "  none at all\n";
  }
  else
  {
    std::cout << "Total: "<<whoList.size()<<" persons.\n";
    //order them by number
    std::vector<WhoStatRec> sortedList;
    WhoStatRec wsr;
    listIter = whoList.begin();
    while (listIter != whoList.end())
    {
      wsr.who = listIter->first;
      wsr.count = listIter->second;
      sortedList.push_back(wsr);
      ++listIter;
    }//while
    sort(sortedList.begin(), sortedList.end(), wsr_compare);
    std::cout<<"\nPresent person names in database by count:\n";
    unsigned int i;
    for (i=0; i<sortedList.size(); ++i)
    {
      std::cout << "  "<<sortedList[i].who<<": "<<sortedList[i].count<<"\n";
    }//for
  }//else
}

bool DataBase::saveToFile(const std::string& FileName) const
{
  std::ofstream output;
  std::set<std::string>::const_iterator set_i;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cout << "Error: Unable to open file \""<<FileName<<"\".\n";
    return false;
  }
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    //filename
    output<<cFilePrefix<<":"<<iter->first<<"\n";
    //pic name
    output<<cNamePrefix<<":"<<iter->second.name<<"\n";
    //artist's/photographer's name
    output<<cArtistPrefix<<":"<<iter->second.artist<<"\n";
    //people on pic
    output<<cWhoPrefix<<":";
    if (iter->second.who.empty())
    {
      output << PicData::cEmptyVector<<"\n";
    }
    else
    {
      for (set_i=iter->second.who.begin(); set_i!=iter->second.who.end(); ++set_i)
      {
        output << *set_i+" ";
      }//for
      output << "\n";
    }//else branch
    //tags
    output<<cTagPrefix<<":";
    if (iter->second.tags.size()==0)
    {
      output << PicData::cNoTags<<"\n";
    }
    else
    {
      for (set_i=iter->second.tags.begin(); set_i!=iter->second.tags.end(); ++set_i)
      {
        output << *set_i +" ";
      }//for
      output << "\n";
    }//else branch
    //SHA-256 hash
    if (!(iter->second.hash_sha256.isNull()))
    {
      output << cHashPrefix << ":"<<iter->second.hash_sha256.toHexString()<<"\n";
    }
    iter++;
  }//while
  output.close();
  return output.good();
}

bool DataBase::loadFromFile(const std::string& FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "Error: Unable to open file \""<<FileName<<"\" for reading.\n";
    return false;
  }
  unsigned int FileSize;
  std::string cur_line = "";
  std::string db_file = "";
  char buffer[256];
  PicData temp_data;
  input.seekg(0, std::ios_base::end);
  FileSize = input.tellg();
  input.seekg(0, std::ios_base::beg);
  while (input.tellg()<FileSize)
  { //read loop
    input.getline(buffer, 255);
    buffer[255] = '\0';
    cur_line = std::string(buffer);

    //check for new file entry
    if (cur_line.size()>cFilePrefix.size())
    {
      if (cur_line.substr(0,cFilePrefix.size()+1) == cFilePrefix+":")
      { //got new file entry
        //... so save old data first
        if (db_file != "")
        {
          m_Files[db_file] = temp_data;
        }
        temp_data.clear();
        db_file = cur_line.substr(cFilePrefix.size()+1);
      }
    }//if file

    //name entry?
    if (cur_line.size()>cNamePrefix.size())
    {
      if (cur_line.substr(0,cNamePrefix.size()+1) == cNamePrefix+":")
      { //got name entry
        temp_data.name = cur_line.substr(cNamePrefix.size()+1);
      }
    }//if name

    //artist entry?
    if (cur_line.size()>cArtistPrefix.size())
    {
      if (cur_line.substr(0,cArtistPrefix.size()+1) == cArtistPrefix+":")
      { //got artist entry
        temp_data.artist = cur_line.substr(cArtistPrefix.size()+1);
      }
    }//if artist

    //people entry?
    if (cur_line.size()>cWhoPrefix.size())
    {
      if (cur_line.substr(0,cWhoPrefix.size()+1) == cWhoPrefix+":")
      { //got people entry
        std::string who_line = cur_line.substr(cWhoPrefix.size()+1);
        if (who_line == PicData::cEmptyVector)
        {
          temp_data.who.clear();
        }
        else
        {
          temp_data.who = Splitter::splitAtSpace(who_line);
        }
      }
    }//if people

    //tag entry?
    if (cur_line.size()>cTagPrefix.size())
    {
      if (cur_line.substr(0,cTagPrefix.size()+1) == cTagPrefix+":")
      { //got tag entry
        std::string tag_line = cur_line.substr(cTagPrefix.size()+1);
        if (tag_line == PicData::cNoTags)
        {
          temp_data.tags.clear();
        }
        else
        {
          temp_data.tags = Splitter::splitAtSpace(tag_line);
        }
      }
    }//if tag

    //sha256 entry?
    if (cur_line.size()>cHashPrefix.size())
    {
      if (cur_line.substr(0,cHashPrefix.size()+1) == cHashPrefix+":")
      { //got hash entry
        if (!temp_data.hash_sha256.fromHexString(cur_line.substr(cHashPrefix.size()+1)))
        {
          //reset value in case of failure
          temp_data.hash_sha256.setToNull();
        }
      }
    }//if name


  }//while

  //care for last data set
  if (db_file != "")
  {
    m_Files[db_file] = temp_data;
  }
  input.close();
  return true;
}

void DataBase::Query::clear()
{
  plusWho.clear();
  plusTags.clear();
  nameSubstrings.clear();
  isArtist = "";
}

void DataBase::Query::fromString(std::string queryString)
{
  clear();
  trim(queryString);
  if (queryString=="") return;
  const std::set<std::string> sv = Splitter::splitAtSpace(queryString);
  std::set<std::string>::const_iterator set_i;
  bool lineProcessed = false;
  for (set_i=sv.begin(); set_i!=sv.end(); ++set_i)
  {
    lineProcessed = false;
    //who?
    if (set_i->length()>2)
    {
      if (set_i->substr(0,2) == "w:")
      {
        plusWho.push_back(toLowerString(set_i->substr(2)));
        lineProcessed = true;
      }
    }
    //artist?
    if (set_i->length()>3 && !lineProcessed)
    {
      if (set_i->substr(0,3) == "by:")
      {
        isArtist = set_i->substr(3);
        lineProcessed = true;
      }
    }
    //tag?
    if (set_i->length()>4 && !lineProcessed)
    {
      if (set_i->substr(0,4) == "tag:")
      {
        plusTags.push_back(toLowerString(set_i->substr(4)));
        lineProcessed = true;
      }
    }
    if (!lineProcessed)
    {
      nameSubstrings.push_back(*set_i);
    }
  }//for
}

bool DataBase::Query::fulfilledBy(const PicData& pic) const
{
  //check artist
  if (isArtist!="" and isArtist!=pic.artist) return false;
  //check name substrings
  unsigned int i;
  for (i=0; i<nameSubstrings.size(); i=i+1)
  {
    if (pic.name.find(nameSubstrings[i])==std::string::npos)
    {
      return false;
    }
  }//for
  //check tags
  std::set<std::string>::const_iterator j;
  bool found;
  for (i=0; i<plusTags.size(); i=i+1)
  {
    found = false;
    for (j=pic.tags.begin(); (j!=pic.tags.end()) and !found; ++j)
    {
      found = (plusTags[i] == *j);
    }//for j
    if (!found) return false;
  }//for i

  //check who
  for (i=0; i<plusWho.size(); i=i+1)
  {
    found = false;
    for (j=pic.who.begin(); (j!=pic.who.end()) and !found; ++j)
    {
      found = (plusWho[i] == *j);
    }//for
    if (!found) return false;
  }//for i

  //finished, all conditions are met
  return true;
}

std::vector<std::string> DataBase::executeQuery(const std::string& query) const
{
  const std::string::size_type union_pos = query.find(":UNION:");
  const std::string::size_type inter_pos = query.find(":INTERSECT:");
  //make sure the first operation from the lef is considered first
  if ((union_pos!=std::string::npos) and (inter_pos!=std::string::npos))
  {
    //union is first?
    if (union_pos<inter_pos)
    {
      return getQueryResultUnion(executeQuery(query.substr(0, union_pos)),
             executeQuery(query.substr(union_pos+7)));
    }
    //intersection is first
    return getQueryResultIntersection(executeQuery(query.substr(0, inter_pos)),
             executeQuery(query.substr(inter_pos+11)));
  }//if
  //union present?
  if (union_pos!=std::string::npos)
  {
    return getQueryResultUnion(executeQuery(query.substr(0, union_pos)),
             executeQuery(query.substr(union_pos+7)));
  }
  //intersection present?
  if (inter_pos!=std::string::npos)
  {
    return getQueryResultIntersection(executeQuery(query.substr(0, inter_pos)),
             executeQuery(query.substr(inter_pos+11)));
  }

  //else: go on with normal query
  std::vector<std::string> result;
  Query newQuery;
  newQuery.fromString(query);
  result.clear();
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter!=m_Files.end())
  {
    if (newQuery.fulfilledBy(iter->second))
    {
      result.push_back(iter->first);
    }
    ++iter;
  }//while
  return result;
}

std::vector<std::string> DataBase::getQueryResultUnion(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two) const
{
  std::vector<std::string> result;
  std::vector<std::string>::const_iterator one_iter, two_iter;
  one_iter = query_one.begin();
  two_iter = query_two.begin();
  while (one_iter!=query_one.end() and two_iter!=query_two.end())
  {
    if (*one_iter<*two_iter)
    {
      result.push_back(*one_iter);
      ++one_iter;
    }
    else if (*one_iter>*two_iter)
    {
      result.push_back(*two_iter);
      ++two_iter;
    }
    else
    {
      result.push_back(*one_iter);
      ++one_iter;
      ++two_iter;
    }
  }//while
  //check for remainder
  if (one_iter!=query_one.end())
  {
    //set end iter
    two_iter = query_one.end();
    //start iter is already known: one_iter
  }
  else if (two_iter!=query_two.end())
  {
    //set start iterator
    one_iter = two_iter;
    //set end iterator
    two_iter = query_two.end();
  }
  else
  {
    return result;
  }
  //copy remaining stuff
  while (one_iter!=two_iter)
  {
    result.push_back(*one_iter);
    ++one_iter;
  }//while
  return result;
}

std::vector<std::string> DataBase::getQueryResultIntersection(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two) const
{
  std::vector<std::string> result;
  std::vector<std::string>::const_iterator one_iter, two_iter;
  one_iter = query_one.begin();
  two_iter = query_two.begin();
  while (one_iter!=query_one.end() and two_iter!=query_two.end())
  {
    if (*one_iter<*two_iter)
    {
      ++one_iter;
    }
    else if (*one_iter>*two_iter)
    {
      ++two_iter;
    }
    else
    {
      result.push_back(*one_iter);
      ++one_iter;
      ++two_iter;
    }
  }//while
  return result;
}

void DataBase::removeFromHashIndex(const SHA256::MessageDigest& hash, const std::string& FileName)
{
  std::map<SHA256::MessageDigest, std::set<std::string> >::iterator iter = m_HashIndex.find(hash);
  if (iter!=m_HashIndex.end())
  {
    iter->second.erase(FileName);
    if (iter->second.empty())
    {
      m_HashIndex.erase(iter);
    }
  }
}

DataBase::Iterator DataBase::getFirst() const
{
  return m_Files.begin();
}

DataBase::Iterator DataBase::getEnd() const
{
  return m_Files.end();
}

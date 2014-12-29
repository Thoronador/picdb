/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2011, 2012, 2013, 2014  Thoronador

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

#include "DataBase.hpp"
#include <iostream>
#include <algorithm>
#include "../../libthoro/common/StringUtils.h"
#include "Query.hpp"
#include "WhoStatRecord.hpp"

const std::string DataBase::cFilePrefix = "file";
const std::string DataBase::cNamePrefix = "name";
const std::string DataBase::cArtistPrefix = "artist";
const std::string DataBase::cWhoPrefix = "who";
const std::string DataBase::cTagPrefix = "tags";
const std::string DataBase::cHashPrefix = "sha256";


DataBase::~DataBase()
{
  clearAllData();
}

DataBase::DataBase()
: m_Files(std::map<std::string, PicData>()),
  m_Data(std::map<SHA256::MessageDigest, PicData>()),
  m_FileToHash(std::map<std::string, SHA256::MessageDigest>())
{
  //empty
}

void DataBase::clearAllData()
{
  m_Files.clear();
  m_FileToHash.clear();
  m_Data.clear();
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
    //if no valid hash, use old index
    if (data.hash_sha256.isNull())
    {
      //set or update file entry
      m_Files[FileName] = data;
    }
    else
    {
      //valid hash
      // --> add to filename-to-hash map
      m_FileToHash[FileName] = data.hash_sha256;
      //add/replace data
      m_Data[data.hash_sha256] = data;
      //remove it from old index, if present
      std::map<std::string, PicData>::iterator files_iter = m_Files.find(FileName);
      if (files_iter!=m_Files.end())
      {
        m_Files.erase(files_iter);
      }
    }//else (outer)
  }//if file name not empty
}

bool DataBase::hasFile(const std::string& FileName) const
{
  return ((m_Files.find(FileName)!= m_Files.end())
    or (m_FileToHash.find(FileName)!=m_FileToHash.end()));
}

std::set<std::string> DataBase::getListedFiles() const
{
  std::set<std::string> result;
  std::map<std::string, PicData>::const_iterator f_iter = m_Files.begin();
  while (f_iter!= m_Files.end())
  {
    result.insert(f_iter->first);
    ++f_iter;
  }
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter!=m_FileToHash.end())
  {
    result.insert(fth_iter->first);
    ++fth_iter;
  }
  return result;
}

const PicData& DataBase::getData(const std::string& FileName) const
{
  //check old-style index
  const std::map<std::string, PicData>::const_iterator iter = m_Files.find(FileName);;
  if (iter != m_Files.end())
  {
    return iter->second;
  }
  //check new index
  const std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter
    = m_FileToHash.find(FileName);
  if (fth_iter!=m_FileToHash.end())
  {
    const std::map<SHA256::MessageDigest, PicData>::const_iterator data_iter
      = m_Data.find(fth_iter->second);
    if (m_Data.end()!=data_iter)
      return data_iter->second;
  }
  throw 12345;
}

const PicData& DataBase::getData(const SHA256::MessageDigest& hash) const
{
  const std::map<SHA256::MessageDigest, PicData>::const_iterator data_iter
      = m_Data.find(hash);
  if (m_Data.end()!=data_iter)
  {
    return data_iter->second;
  }
  throw 12345;
}

bool DataBase::hasDataForHash(const SHA256::MessageDigest& hash) const
{
  return (m_Data.find(hash)!=m_Data.end());
}

std::set<std::string> DataBase::getAllFilesForHash(const SHA256::MessageDigest& hash) const
{
  std::set<std::string> result;
  if (hash.isNull()) return result;
  std::map<std::string, SHA256::MessageDigest>::const_iterator iter
     = m_FileToHash.begin();
  if (iter!=m_FileToHash.end())
  {
    if (iter->second==hash) result.insert(result.end(), iter->first);
    ++iter;
  }
  return result;
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

void DataBase::hashUnhashedFiles(const std::string& baseDir, unsigned int limit)
{
  //max set to zero? zero means hash all!
  if (limit==0)
  {
    limit = m_Files.size();
  }
  else
  {
    if (limit>m_Files.size()) limit = m_Files.size();
  }
  //now do it
  std::map<std::string, PicData>::iterator iter = m_Files.begin();
  while ((limit>0) and (iter!=m_Files.end()))
  {
    if (FileExists(baseDir+iter->first))
    {
      SHA256::MessageDigest md256 = SHA256::computeFromFile(baseDir+iter->first);
      if (!md256.isNull())
      {
        PicData data = iter->second;
        data.hash_sha256 = md256;
        addFile(iter->first, data);
        iter = m_Files.begin();
      }
      else
      {
        ++iter;
      }
    }
    else
    {
      //file does not exist, delete it
      m_Files.erase(iter);
      iter = m_Files.begin();
    }
    --limit;
  }//while
}

unsigned int DataBase::getNumUnhashed() const
{
  return m_Files.size();
}

unsigned int DataBase::getNumHashed() const
{
  return m_FileToHash.size();
}

unsigned int DataBase::getNumEntries() const
{
  return m_Files.size() + m_FileToHash.size();
}

std::vector<std::string> DataBase::getUntaggedFiles() const
{
  std::vector<std::string> result;
  result.clear();
  //search old index
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    if (iter->second.tags.size() == 0)
    {
      result.push_back(iter->first);
    }
    ++iter;
  }
  //search new index
  std::vector<std::string> newResult;
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter != m_FileToHash.end())
  {
    if (m_Data.find(fth_iter->second)->second.tags.size() == 0)
    {
      newResult.push_back(fth_iter->first);
    }
    ++fth_iter;
  }
  return getQueryResultUnion(result, newResult);
}

std::vector<std::string> DataBase::getUnknownArtistFiles() const
{
  //search old index
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
  //search new index
  std::vector<std::string> newResult;
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter != m_FileToHash.end())
  {
    if (m_Data.find(fth_iter->second)->second.artist == Splitter::cUnknownArtist)
    {
      newResult.push_back(fth_iter->first);
    }
    ++fth_iter;
  }
  return getQueryResultUnion(result, newResult);
}

std::vector<std::string> DataBase::getUnknownWhoFiles() const
{
  //search old index
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
  //search new index
  std::vector<std::string> newResult;
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter != m_FileToHash.end())
  {
    if (m_Data.find(fth_iter->second)->second.who.size() == 0)
    {
      newResult.push_back(fth_iter->first);
    }
    ++fth_iter;
  }
  return getQueryResultUnion(result, newResult);
}

std::vector<std::string> DataBase::getNonexistingFiles(const std::string& BaseDir) const
{
  std::vector<std::string> result;
  result.clear();
  //search old index
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    if (!FileExists(BaseDir+iter->first))
    {
      result.push_back(iter->first);
    }
    ++iter;
  }
  //search new index
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter != m_FileToHash.end())
  {
    if (!FileExists(BaseDir+fth_iter->first))
    {
      result.push_back(fth_iter->first);
    }
    ++fth_iter;
  }
  return result;
}

bool DataBase::deleteFile(const std::string& FileName)
{
  return ((m_Files.erase(FileName)>0) or (m_FileToHash.erase(FileName)>0));
}

void DataBase::purgeNonexistingFiles(const std::string& BaseDir)
{
  std::vector<std::string> deletionList = getNonexistingFiles(BaseDir);
  unsigned int i;
  for (i=0; i<deletionList.size(); i=i+1)
  {
    deleteFile(deletionList[i]);
  }//for
}

void DataBase::showTagStatistics() const
{
  std::map<std::string, unsigned int> tagList;
  tagList.clear();
  unsigned int total_tags=0;
  std::set<std::string>::const_iterator setIter;
  //search first index
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
  //search second index
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter != m_FileToHash.end())
  {
    const std::map<SHA256::MessageDigest, PicData>::const_iterator aux_iter = m_Data.find(fth_iter->second);
    for (setIter=aux_iter->second.tags.begin(); setIter!=aux_iter->second.tags.end(); ++setIter)
    {
      tagList[*setIter]++;
      ++total_tags;
    }//for
    ++fth_iter;
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
  const unsigned int file_count = m_Files.size() + m_FileToHash.size();
  std::cout << "A total of "<<total_tags<<" tags on "<<file_count
            <<" files, that means "<<static_cast<float>(total_tags)/(file_count==0 ? 1 : file_count)
            <<" tags per file.\n";
}

void DataBase::showWhoStatistics() const
{
  std::map<std::string, unsigned int> whoList;
  whoList.clear();
  std::set<std::string>::const_iterator it;
  //search old index
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    for (it=iter->second.who.begin(); it!=iter->second.who.end(); ++it)
    {
      whoList[*it]++;
    }//for
    ++iter;
  }
  //search new index
  //search second index
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter != m_FileToHash.end())
  {
    const std::map<SHA256::MessageDigest, PicData>::const_iterator aux_iter = m_Data.find(fth_iter->second);
    for (it=aux_iter->second.who.begin(); it!=aux_iter->second.who.end(); ++it)
    {
      whoList[*it]++;
    }//for
    ++fth_iter;
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
  //save old-style index
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter != m_Files.end())
  {
    saveFileEntry(output, iter->first, iter->second);
    ++iter;
  }//while
  //save new index
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter!=m_FileToHash.end())
  {
    saveFileEntry(output, fth_iter->first, m_Data.find(fth_iter->second)->second);
    ++fth_iter;
  }//while
  output.close();
  return output.good();
}

void DataBase::saveFileEntry(std::ofstream& output, const std::string& fileName, const PicData& data) const
{
  std::set<std::string>::const_iterator set_i;
  //filename
  output<<cFilePrefix<<":"<<fileName<<"\n";
  //pic name
  output<<cNamePrefix<<":"<<data.name<<"\n";
  //artist's/photographer's name
  output<<cArtistPrefix<<":"<<data.artist<<"\n";
  //people on pic
  output<<cWhoPrefix<<":";
  if (data.who.empty())
  {
    output << PicData::cEmptyVector<<"\n";
  }
  else
  {
    for (set_i=data.who.begin(); set_i!=data.who.end(); ++set_i)
    {
      output << *set_i+" ";
    }//for
    output << "\n";
  }//else branch
  //tags
  output<<cTagPrefix<<":";
  if (data.tags.size()==0)
  {
    output << PicData::cNoTags<<"\n";
  }
  else
  {
    for (set_i=data.tags.begin(); set_i!=data.tags.end(); ++set_i)
    {
      output << *set_i +" ";
    }//for
    output << "\n";
  }//else branch
  //SHA-256 hash
  if (!(data.hash_sha256.isNull()))
  {
    output << cHashPrefix << ":"<<data.hash_sha256.toHexString()<<"\n";
  }
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
          addFile(db_file, temp_data);
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
    addFile(db_file, temp_data);
  }
  input.close();
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
  //search old index
  std::map<std::string, PicData>::const_iterator iter = m_Files.begin();
  while (iter!=m_Files.end())
  {
    if (newQuery.fulfilledBy(iter->second))
    {
      result.push_back(iter->first);
    }
    ++iter;
  }//while
  //search new index
  std::vector<std::string> newResult;
  std::map<std::string, SHA256::MessageDigest>::const_iterator fth_iter = m_FileToHash.begin();
  while (fth_iter!=m_FileToHash.end())
  {
    const std::map<SHA256::MessageDigest, PicData>::const_iterator aux_iter = m_Data.find(fth_iter->second);
    if (newQuery.fulfilledBy(aux_iter->second))
    {
      newResult.push_back(fth_iter->first);
    }
    ++fth_iter;
  }//while
  return getQueryResultUnion(result, newResult);
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

void DataBase::removeFromFileToHashIndex(const std::string& FileName)
{
  std::map<std::string, SHA256::MessageDigest>::iterator iter = m_FileToHash.find(FileName);
  if (iter!=m_FileToHash.end())
  {
    m_FileToHash.erase(iter);
  }
}

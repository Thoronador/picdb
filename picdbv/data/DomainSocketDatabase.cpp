/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2015  Thoronador

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

#include "DomainSocketDatabase.hpp"
#include <stdexcept>  // for "standard" exception types
#include <utility>
#include "../../libthoro/common/StringUtils.h"
#include "../sockets/UnixDomainSocketClient.hpp"
#include "../common/escaping.hpp"
#include "../daemon/functions.hpp"
#include "Serializer.hpp"

DomainSocketDatabase::DomainSocketDatabase(const std::string& db, const std::string& socketFile)
: db_name(db),
  socket_file(socketFile)
{

}

DomainSocketDatabase::~DomainSocketDatabase()
{
  //empty
}

std::string DomainSocketDatabase::sendRequestToServer(const std::string& request) const
{
  UnixDomainSocketClient socketCli;
  if (!socketCli.connectToServer(socket_file))
  {
    //Could not connect to server!
    return "";
  }
  if (!socketCli.sendString(request))
  {
    //Could not send request to server!
    socketCli.closeConnection();
    return "";
  }

  std::string response = "";
  if (!socketCli.receiveString(response))
  {
    // Could not receive answer from server.
    socketCli.closeConnection();
    return "";
  }
  socketCli.closeConnection();
  unescapeInPlace(response);
  if (response.empty())
  {
    //Received empty answer from server, this should not happen!
    return "";
  }
  return response;
}

void DomainSocketDatabase::clearAllData()
{
  const std::string resp = sendRequestToServer("clear_db "+db_name);
  if (resp.empty())
    throw std::runtime_error("Could not send request to server!");
  const int code = extractStatusCodeFromResponse(resp);
  if (code != codeOKInt)
  {
    throw std::runtime_error("Got unexpected server response: "+resp);
    return;
  }
}

bool DomainSocketDatabase::getFilesFromDirectory(const std::string& directory)
{
  throw std::runtime_error("Function DomainSocketDatabase::getFilesFromDirectory() is not implemented yet.");
  return false;
}

void DomainSocketDatabase::addFile(const std::string& FileName, const PicData& data)
{
  if (!FileName.empty())
  {
    const std::string response = sendRequestToServer("add_file "+db_name+" "+FileName+"\n"
                                                     +Serializer::serialize(data));
    const int code = extractStatusCodeFromResponse(response);
    if (code != codeOKInt)
    {
      throw std::runtime_error("Got unexpected server response: "+response);
      return;
    }
  }//if file name not empty
}

bool DomainSocketDatabase::hasFile(const std::string& FileName) const
{
  const std::string response = sendRequestToServer("file_exists "+db_name+" "+FileName);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    return true;
  }
  if (code == codeNoContentInt)
  {
    return false;
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

std::set<std::string> DomainSocketDatabase::getListedFiles() const
{
  const std::string response = sendRequestToServer("list_files "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeNoContentInt)
  {
    return std::set<std::string>();
  }
  if (code == codeOKInt)
  {
    const std::vector<std::string> splitted = Splitter::splitAtSeparator(response, '\n');
    std::vector<std::string>::const_iterator iter = splitted.begin();
    ++iter; //skip first element, because that's the status code message

    std::set<std::string> result;
    while (iter != splitted.end())
    {
      result.insert(/*std::move(*/*iter/*)*/);
      ++iter;
    } //while
    return result;
  }
  //unexpected code
  throw std::runtime_error("Got unexpected server response: "+response);
}

const PicData& DomainSocketDatabase::getData(const std::string& FileName) const
{
  const std::string response = sendRequestToServer("file_data "+db_name+" "+FileName);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    std::string::size_type lnBreak = response.find("\n");
    PicData result;
    if (Serializer::unserialize(response.substr(lnBreak+1), result))
    {
      return result;
    }
    else
    {
      throw std::runtime_error("Server response could not be deserialized!\nResponse: "+response);
    }
  }
  throw std::runtime_error("Got unexpected server response: "+response);
}

const PicData& DomainSocketDatabase::getData(const SHA256::MessageDigest& hash) const
{
  const std::string response = sendRequestToServer("hash_data "+db_name+" "+hash.toHexString());
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    std::string::size_type lnBreak = response.find("\n");
    PicData result;
    if (Serializer::unserialize(response.substr(lnBreak+1), result))
    {
      return result;
    }
    else
    {
      throw std::runtime_error("Server response could not be deserialized!\nResponse: "+response);
    }
  }
  throw std::runtime_error("Got unexpected server response: "+response);
}

bool DomainSocketDatabase::hasDataForHash(const SHA256::MessageDigest& hash) const
{
  const std::string response = sendRequestToServer("hash_exists "+db_name+" "+hash.toHexString());
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    return true;
  }
  if (code == codeNoContentInt)
  {
    return false;
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

std::set<std::string> DomainSocketDatabase::getAllFilesForHash(const SHA256::MessageDigest& hash) const
{
  const std::string response = sendRequestToServer("files_for_hash "+db_name+" "+hash.toHexString());
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeNoContentInt)
  {
    return std::set<std::string>();
  }
  if (code == codeOKInt)
  {
    std::vector<std::string> splitted = Splitter::splitAtSeparator(response, '\n');
    std::vector<std::string>::const_iterator iter = splitted.begin();
    ++iter; //skip first element, because that's the status code message
    std::set<std::string> result;
    while (iter != splitted.end())
    {
      result.insert(/*std::move(*/*iter/*)*/);
      ++iter;
    } //while
    return result;
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

bool DomainSocketDatabase::AutoTag_Splitter()
{
  const std::string response = sendRequestToServer("autotag_db "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    return true;
  }
  if (code == codeInternalServerErrorInt)
  {
    return false;
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

void DomainSocketDatabase::hashUnhashedFiles(const std::string& baseDir, unsigned int limit)
{
  #warning Not implemented yet!
  throw std::runtime_error("Function DomainSocketDatabase::hashUnhashedFiles() is not implemented yet.");
  return;
}

unsigned int DomainSocketDatabase::getNumUnhashed() const
{
  std::string response = sendRequestToServer("num_unhashed "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    //remove first four characters (status code + following space)
    response.erase(0, 4);
    std::string::size_type space_pos = response.find(' ');
    if (space_pos != std::string::npos)
    {
      response.erase(space_pos);
      unsigned int result = 0;
      if (stringToUnsignedInt(response, result))
        return result;
      //conversion failed
      throw std::runtime_error("String \""+response+"\" does not contain a valid integral number!");
    }
    //no space -> malformed response
    throw std::runtime_error("Response does not contain a valid number!\nResponse: "+codeOK+" "+response);
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

unsigned int DomainSocketDatabase::getNumHashed() const
{
  std::string response = sendRequestToServer("num_hashed "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    //remove first four characters (status code + following space)
    response.erase(0, 4);
    std::string::size_type space_pos = response.find(' ');
    if (space_pos != std::string::npos)
    {
      response.erase(space_pos);
      unsigned int result = 0;
      if (stringToUnsignedInt(response, result))
        return result;
      //conversion failed
      throw std::runtime_error("String \""+response+"\" does not contain a valid integral number!");
    }
    //no space -> malformed response
    throw std::runtime_error("Response does not contain a valid number!\nResponse: "+codeOK+" "+response);
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

unsigned int DomainSocketDatabase::getNumEntries() const
{
  std::string response = sendRequestToServer("num_entries "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    //remove first four characters (status code + following space)
    response.erase(0, 4);
    std::string::size_type space_pos = response.find(' ');
    if (space_pos != std::string::npos)
    {
      response.erase(space_pos);
      unsigned int result = 0;
      if (stringToUnsignedInt(response, result))
        return result;
      //conversion failed
      throw std::runtime_error("String \""+response+"\" does not contain a valid integral number!");
    }
    //no space -> malformed response
    throw std::runtime_error("Response does not contain a valid number!\nResponse: "+codeOK+" "+response);
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

std::vector<std::string> DomainSocketDatabase::getUntaggedFiles() const
{
  std::string response = sendRequestToServer("untagged "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeNoContentInt)
    return std::vector<std::string>();
  if (code == codeOKInt)
  {
    //remove first four characters (status code + following space)
    response.erase(0, 4);
    return Splitter::splitAtSeparator(response, '\0');
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

std::vector<std::string> DomainSocketDatabase::getUnknownArtistFiles() const
{
  std::string response = sendRequestToServer("no_artist "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeNoContentInt)
    return std::vector<std::string>();
  if (code == codeOKInt)
  {
    //remove first four characters (status code + following space)
    response.erase(0, 4);
    return Splitter::splitAtSeparator(response, '\0');
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

std::vector<std::string> DomainSocketDatabase::getUnknownWhoFiles() const
{
  std::string response = sendRequestToServer("no_who "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeNoContentInt)
    return std::vector<std::string>();
  if (code == codeOKInt)
  {
    //remove first four characters (status code + following space)
    response.erase(0, 4);
    return Splitter::splitAtSeparator(response, '\0');
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

std::vector<std::string> DomainSocketDatabase::getNonexistingFiles(const std::string& BaseDir) const
{
  #warning Not implemented yet!
  throw std::runtime_error("Function DomainSocketDatabase::getUnknownArtistFiles() is not implemented yet.");
  return std::vector<std::string>();
}

bool DomainSocketDatabase::deleteFile(const std::string& FileName)
{
  std::string response = sendRequestToServer("delete_file "+db_name+" "+FileName);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    return true;
  }
  if (code == codeNoContentInt)
  {
    return false;
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

void DomainSocketDatabase::purgeNonexistingFiles(const std::string& BaseDir)
{
  #warning Not implemented yet!
  throw std::runtime_error("Function DomainSocketDatabase::getUnknownArtistFiles() is not implemented yet.");
  return;
}

void DomainSocketDatabase::showTagStatistics(std::ostream& outStream) const
{
  const std::string response = sendRequestToServer("tag_stats "+db_name);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeOKInt)
  {
    response.erase(0, 4); //remove status code + space
    outStream << response;
    return;
  }
  //unexpected code
  throw std::runtime_error("Got unexpected server response: "+response);
}

void DomainSocketDatabase::showWhoStatistics(std::ostream& outStream) const
{
  #warning Not implemented yet!
  throw std::runtime_error("Function DomainSocketDatabase::showWhoStatistics() is not implemented yet.");
  return;
}

bool DomainSocketDatabase::saveToFile(const std::string& FileName) const
{
  #warning Not implemented yet!
  return false;
}

bool DomainSocketDatabase::loadFromFile(const std::string& FileName)
{
  const std::string response = sendRequestToServer("load_db "+db_name+" "+FileName);
  return (codeOKInt == extractStatusCodeFromResponse(response));
}

std::vector<std::string> DomainSocketDatabase::executeQuery(const std::string& query) const
{
  std::string response = sendRequestToServer("query "+db_name+" "+query);
  const int code = extractStatusCodeFromResponse(response);
  if (code == codeNoContentInt)
    return std::vector<std::string>();
  if (code == codeOKInt)
  {
    std::vector<std::string> vec = Splitter::splitAtSeparator(response, '\n');
    //erase first element from vector, because that's the status code + message
    vec.erase(vec.begin());
    return vec;
  }
  //some kind of error occurred
  throw std::runtime_error("Got unexpected server response: "+response);
}

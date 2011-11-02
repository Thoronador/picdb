/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

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

#ifndef PICDATABASE_H
#define PICDATABASE_H

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Splitter.h"
#include "../common/DirectoryFileList.h"

struct PicData
{
  std::string name, artist;
  std::set<std::string> who;
  std::set<std::string> tags;

  /* writes the structure's data to the standard output */
  void show() const;
  void clear();
  static const std::string cEmptyVector;
  static const std::string cNoTags;
}; //struct

struct WhoStatRec
{
  std::string who;
  unsigned int count;
};//struct

bool wsr_compare(const WhoStatRec& a, const WhoStatRec& b);

class DataBase
{
  public:
    //iterator type
    typedef std::map<std::string, PicData>::const_iterator Iterator;

    /* singleton access */
    static DataBase& getSingleton();

    /* destructor */
    virtual ~DataBase();

    /* clears all entries within the database */
    void clearAllData();

    /* adds all files from the given directory to the database and returns true,
       if at least one file was added

       parameters:
           directory - the directory that will be searched for files
    */
    bool getFilesFromDirectory(const std::string& directory);

    /* tries to guess names of pictures and their artists from the file names
       of the pictures by searching for certain dividers.
    */
    void AutoTag_Splitter();

    /* adds a new file entry to the database. Any existing entry with the same
       file name will be overwritten. Trying to add an empty file name will be
       ignored.

       parameters:
           FileName - name of the file that will be added
           data     - the data of that file
    */
    void addFile(const std::string& FileName, const PicData& data);

    /* returns true, if a file with the given file name is in the database

       parameters:
           FileName - name of the file that shall be searched
    */
    bool hasFile(const std::string& FileName) const;

    /* deletes an entry from the database and returns true, if such an entry
       was deleted

       parameters:
           FileName - name of the file whose DB entry shall be deleted
    */
    bool deleteFile(const std::string& FileName);

    /* tries to get the data entry for a given file and returns it, if such a
       file is present in the database. If no such file can be found, the
       function will throw an exception. Thus make sure the requested file is
       present by calling hasFile() before.

       parameters:
           FileName - name of the file whose entry shall be returned
    */
    const PicData& getData(const std::string& FileName) const;

    /* writes the data of all files within the DB to the standard output */
    void ListData() const;

    /* returns the number of entries in the database */
    unsigned int getNumEntries() const;

    /* returns a vector containing all files in the db that have not been tagged
       yet
    */
    std::vector<std::string> getUntaggedFiles() const;

    /* returns a vector containing all files in the db that have an unknown
       artist entry
    */
    std::vector<std::string> getUnknownArtistFiles() const;

    /* returns a vector containing all file that do not show any persons (or
       unknown persons for that matter)
    */
    std::vector<std::string> getUnknownWhoFiles() const;

    /* returns a list of files that are in the database but do not exist on the
       physical medium. BaseDir is the directory the files are assumed to be in

       parameters:
           BaseDir - directory containing the files
    */
    std::vector<std::string> getNonexistingFiles(const std::string& BaseDir) const;

    /* removes entries about all non-existing files from the database

       parameters:
           BaseDir - directory that should contain the files (but doesn't)
    */
    void purgeNonexistingFiles(const std::string& BaseDir);
    void showTagStatistics() const;
    void showWhoStatistics() const;
    bool saveToFile(const std::string& FileName) const;
    bool loadFromFile(const std::string& FileName);

    /* translates the given string into query conditions and returns a list of
       all files that match the given query conditions

       parameters:
           query - the query string
    */
    std::vector<std::string> executeQuery(const std::string& query) const;

    /* returns iterator to the beginning of the internal map */
    Iterator getFirst() const;

    /* returns iterator to the end of the internal map */
    Iterator getEnd() const;

    static const std::string cFilePrefix;
    static const std::string cNamePrefix;
    static const std::string cArtistPrefix;
    static const std::string cWhoPrefix;
    static const std::string cTagPrefix;
  private:
    //structure to hold criteria for a DB query
    struct Query
    {
      std::vector<std::string> plusWho;
      std::vector<std::string> plusTags;
      std::vector<std::string> nameSubstrings;
      std::string isArtist;

      /*resets/clears all data members of the query structure

        remarks:
            Be aware that a cleared query's criteria (i.e. none at all) will be
            fulfilled by any possible data set. (See fulfilledBy().)
      */
      void clear();

      /* sets query criteria as specified by the given string

         parameters:
             queryString - the string holding the criteria
      */
      void fromString(std::string queryString);

      /* returns true, if the provided data set fulfills all criteria specified
         for this query

         parameters:
             pic - the data set to be checked
      */
      bool fulfilledBy(const PicData& pic) const;
    }; //struct

    /* returns the union of two query results

       parameters:
           query_one - result of first query
           query_two - result of second query
    */
    std::vector<std::string> getQueryResultUnion(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two) const;

    /* returns the intersection of two query results

       parameters:
           query_one - result of first query
           query_two - result of second query
    */
    std::vector<std::string> getQueryResultIntersection(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two) const;

    //constructor
    DataBase();

    //empty copy constructor
    DataBase(const DataBase& op) {}

    //internal file map - key is file name, value is data record
    std::map<std::string, PicData> m_Files;
}; //class

#endif // PICDATABASE_H

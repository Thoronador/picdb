/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2014  Thoronador

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

#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "PicData.hpp"
#include "Splitter.hpp"
#include "../../libthoro/common/DirectoryFileList.h"
#include "../../libthoro/hashfunctions/sha-256.h"

class DataBase
{
  public:
    /** \brief default constructor */
    DataBase();


    /** \brief destructor */
    virtual ~DataBase();


    /** \brief clears all entries within the database */
    void clearAllData();


    /** \brief adds all files from the given directory to the database
     *         and returns true, if at least one file was added
     *
     * \param directory the directory that will be searched for files
     * \return Returns true, if at least one file was added.
     */
    bool getFilesFromDirectory(const std::string& directory);


    /** \brief tries to guess names of pictures and their artists from the file names
     *         of the pictures by searching for certain dividers.
     */
    void AutoTag_Splitter();


    /** \brief adds a new file entry to the database.
     *         Any existing entry with the same file name will be overwritten.
     *         Trying to add an empty file name will be ignored.
     *
     * \param FileName name of the file that will be added
     * \param data the data of that file
     */
    void addFile(const std::string& FileName, const PicData& data);


    /** \brief checks, whether a file with the given file name is in the database
     *
     * \param FileName name of the file that shall be searched for
     * \return Returns true, if a file with the given file name is in the
     *         database.
     */
    bool hasFile(const std::string& FileName) const;


    /** \brief returns a set containing all files in the database
     * \return set of all file names in the database
     */
    std::set<std::string> getListedFiles() const;


    /** \brief deletes an entry from the database
     *
     * \param FileName name of the file whose DB entry shall be deleted
     * \return Returns true, if an entry was deleted.
     */
    bool deleteFile(const std::string& FileName);


    /** \brief tries to get the data entry for a given file and returns it,
     * if such a file is present in the database. If no such file can be found, the
     * function will throw an exception. Thus make sure the requested file is
     * present by calling hasFile() before.
     * \param FileName name of the file whose entry shall be returned
     */
    const PicData& getData(const std::string& FileName) const;


    /** \brief tries to get the data entry for a given hash and returns it, if such a
       hash is present in the database. If no such hash can be found, the
       function will throw an exception. Thus make sure the requested hash is
       present by calling hasDataForHash() before.

     * \param hash the SHA-256 hash of the file whose entry shall be returned
     */
    const PicData& getData(const SHA256::MessageDigest& hash) const;


    /** \brief Checks, whether there is a database entry for the file with the given hash value.
     * \remarks The "null" hash will never have a DB entry.
     * \param hash the SHA-256 hash value
     * \return Return true, if an entry for the given hash value exists.
     */
    bool hasDataForHash(const SHA256::MessageDigest& hash) const;


    /** \brief returns all file names for a given hash, if an entry is present.
     * If no entry for that hash is present, the function will return an empty list.
     * If the hash is the null hash, it will return an empty list, too.
     *
     * \param hash the SHA-256 hash value of the files
     * \return list (i.e. std::set) of all matching file names
     */
    std::set<std::string> getAllFilesForHash(const SHA256::MessageDigest& hash) const;


    /** \brief writes the data of all files within the DB to the standard output */
    void ListData() const;


    /** \brief tries to calculate the hashes for all files in the DB that do not have a valid hash yet.
     *
     * \param baseDir  the base directory for the file names in the DB
     * \param limit    the maximum number of files to be hashed - zero means all
     */
    void hashUnhashedFiles(const std::string& baseDir, unsigned int limit);


    /** \brief gets the number of files without hash in database
     * \return returns the number of files without hash in database
     */
    unsigned int getNumUnhashed() const;


    /** \brief gets the number of files with hash in database
     * \return returns the number of files with hash in database
     */
    unsigned int getNumHashed() const;


    /** returns the number of entries in the database */
    unsigned int getNumEntries() const;


    /** \brief returns a vector containing all files in the db that have not been tagged yet
     * \returns vector containing the names of untagged files
     */
    std::vector<std::string> getUntaggedFiles() const;


    /** \brief returns a vector of files with unknown artist
     * \return returns a vector containing all files in the db that have an unknown
     *  artist entry
     */
    std::vector<std::string> getUnknownArtistFiles() const;


    /** \return returns a vector containing all file that do not show any persons
     * (or unknown persons for that matter)
     */
    std::vector<std::string> getUnknownWhoFiles() const;


    /** \brief returns a list of files that are in the database but do not exist on the
     * physical medium.
     * BaseDir is the directory the files are assumed to be in.
     *
     * \param BaseDir directory containing the files
     */
    std::vector<std::string> getNonexistingFiles(const std::string& BaseDir) const;


    /** \brief removes entries about all non-existing files from the database
     *
     * \param BaseDir  directory that should contain the files (but doesn't)
     */
    void purgeNonexistingFiles(const std::string& BaseDir);


    void showTagStatistics() const;

    void showWhoStatistics() const;

    bool saveToFile(const std::string& FileName) const;
    bool loadFromFile(const std::string& FileName);

    /** \brief translates the given string into query conditions and returns a list of
     * all files that match the given query conditions
     *
     * \param query  the query string
     */
    std::vector<std::string> executeQuery(const std::string& query) const;


    static const std::string cFilePrefix;
    static const std::string cNamePrefix;
    static const std::string cArtistPrefix;
    static const std::string cWhoPrefix;
    static const std::string cTagPrefix;
    static const std::string cHashPrefix;
  private:
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


    /* removes one file entry from filename-to-hash index, if present.

       parameters:
           FileName - associated file name
    */
    void removeFromFileToHashIndex(const std::string& FileName);

    ///aux. function to save one single db entry to file
    void saveFileEntry(std::ofstream& output, const std::string& fileName, const PicData& data) const;

    ///internal file map - key is file name, value is data record - deprecated
    std::map<std::string, PicData> m_Files;

    ///internal "database" - key is a file's digest, value is associated record
    std::map<SHA256::MessageDigest, PicData> m_Data;
    ///maps known file names to their last known hash
    std::map<std::string, SHA256::MessageDigest> m_FileToHash;
}; //class

#endif // DATABASE_H

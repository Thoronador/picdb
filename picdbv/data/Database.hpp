/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2014, 2015  Thoronador

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

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "PicData.hpp"
#include "../common/Splitter.hpp"
#include "../../libthoro/common/DirectoryFileList.h"
#include "../../libthoro/hashfunctions/sha-256.h"


/** \brief abstract class Database - serves as interface for all real database classes
 */
class Database
{
  public:
    /** \brief destructor */
    virtual ~Database();


    /** \brief clears all entries within the database */
    virtual void clearAllData() = 0;


    /** \brief adds all files from the given directory to the database
     *         and returns true, if at least one file was added
     *
     * \param directory the directory that will be searched for files
     * \return Returns true, if at least one file was added.
     */
    virtual bool getFilesFromDirectory(const std::string& directory) = 0;


    /** \brief tries to guess names of pictures and their artists from the file names
     *         of the pictures by searching for certain dividers.
     * \return Returns true, if the values could be changed or updated.
     *         Returns false otherwise.
     */
    virtual bool AutoTag_Splitter() = 0;


    /** \brief adds a new file entry to the database.
     *         Any existing entry with the same file name will be overwritten.
     *         Trying to add an empty file name will be ignored.
     *
     * \param FileName name of the file that will be added
     * \param data the data of that file
     */
    virtual void addFile(const std::string& FileName, const PicData& data) = 0;


    /** \brief checks, whether a file with the given file name is in the database
     *
     * \param FileName name of the file that shall be searched for
     * \return Returns true, if a file with the given file name is in the
     *         database.
     */
    virtual bool hasFile(const std::string& FileName) const = 0;


    /** \brief returns a set containing all files in the database
     * \return set of all file names in the database
     */
    virtual std::set<std::string> getListedFiles() const = 0;


    /** \brief deletes an entry from the database
     *
     * \param FileName name of the file whose DB entry shall be deleted
     * \return Returns true, if an entry was deleted.
     */
    virtual bool deleteFile(const std::string& FileName) = 0;


    /** \brief tries to get the data entry for a given file and returns it,
     * if such a file is present in the database. If no such file can be found, the
     * function will throw an exception. Thus make sure the requested file is
     * present by calling hasFile() before.
     * \param FileName name of the file whose entry shall be returned
     */
    virtual const PicData& getData(const std::string& FileName) const = 0;


    /** \brief tries to get the data entry for a given hash and returns it, if such a
       hash is present in the database. If no such hash can be found, the
       function will throw an exception. Thus make sure the requested hash is
       present by calling hasDataForHash() before.

     * \param hash the SHA-256 hash of the file whose entry shall be returned
     */
    virtual const PicData& getData(const SHA256::MessageDigest& hash) const = 0;


    /** \brief Checks, whether there is a database entry for the file with the given hash value.
     * \remarks The "null" hash will never have a DB entry.
     * \param hash the SHA-256 hash value
     * \return Return true, if an entry for the given hash value exists.
     */
    virtual bool hasDataForHash(const SHA256::MessageDigest& hash) const = 0;


    /** \brief returns all file names for a given hash, if an entry is present.
     * If no entry for that hash is present, the function will return an empty list.
     * If the hash is the null hash, it will return an empty list, too.
     *
     * \param hash the SHA-256 hash value of the files
     * \return list (i.e. std::set) of all matching file names
     */
    virtual std::set<std::string> getAllFilesForHash(const SHA256::MessageDigest& hash) const = 0;


    /** \brief writes the data of all files within the DB to the standard output */
    virtual void ListData() const = 0;


    /** \brief tries to calculate the hashes for all files in the DB that do not have a valid hash yet.
     *
     * \param baseDir  the base directory for the file names in the DB
     * \param limit    the maximum number of files to be hashed - zero means all
     */
    virtual void hashUnhashedFiles(const std::string& baseDir, unsigned int limit) = 0;


    /** \brief gets the number of files without hash in database
     * \return returns the number of files without hash in database
     */
    virtual unsigned int getNumUnhashed() const = 0;


    /** \brief gets the number of files with hash in database
     * \return returns the number of files with hash in database
     */
    virtual unsigned int getNumHashed() const = 0;


    /** returns the number of entries in the database */
    virtual unsigned int getNumEntries() const = 0;


    /** \brief returns a vector containing all files in the db that have not been tagged yet
     * \returns vector containing the names of untagged files
     */
    virtual std::vector<std::string> getUntaggedFiles() const = 0;


    /** \brief returns a vector of files with unknown artist
     * \return returns a vector containing all files in the db that have an unknown
     *  artist entry
     */
    virtual std::vector<std::string> getUnknownArtistFiles() const = 0;


    /** \return returns a vector containing all file that do not show any persons
     * (or unknown persons for that matter)
     */
    virtual std::vector<std::string> getUnknownWhoFiles() const = 0;


    /** \brief returns a list of files that are in the database but do not exist on the
     * physical medium.
     * BaseDir is the directory the files are assumed to be in.
     *
     * \param BaseDir directory containing the files
     */
    virtual std::vector<std::string> getNonexistingFiles(const std::string& BaseDir) const = 0;


    /** \brief removes entries about all non-existing files from the database
     *
     * \param BaseDir  directory that should contain the files (but doesn't)
     */
    virtual void purgeNonexistingFiles(const std::string& BaseDir) = 0;


    virtual void showTagStatistics() const = 0;

    virtual void showWhoStatistics() const = 0;

    virtual bool saveToFile(const std::string& FileName) const = 0;
    virtual bool loadFromFile(const std::string& FileName) = 0;

    /** \brief translates the given string into query conditions and returns a list of
     * all files that match the given query conditions
     *
     * \param query  the query string
     */
    virtual std::vector<std::string> executeQuery(const std::string& query) const = 0;


    static const std::string cFilePrefix;
    static const std::string cNamePrefix;
    static const std::string cArtistPrefix;
    static const std::string cWhoPrefix;
    static const std::string cTagPrefix;
    static const std::string cHashPrefix;
  protected:
    /* returns the union of two query results

       parameters:
           query_one - result of first query
           query_two - result of second query
    */
    static std::vector<std::string> getQueryResultUnion(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two);

    /* returns the intersection of two query results

       parameters:
           query_one - result of first query
           query_two - result of second query
    */
    static std::vector<std::string> getQueryResultIntersection(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two);

    ///aux. function to save one single db entry to file
    static void saveFileEntry(std::ofstream& output, const std::string& fileName, const PicData& data);
}; //class

#endif // DATABASE_HPP

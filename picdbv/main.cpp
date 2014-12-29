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

#include <iostream>
#include <string>
#include "Configuration.h"
#include "common/Splitter.hpp"
#include "PicDataBase.h"
#include "GUI.h"

//the configuration
Configuration& config = GUI::getSingleton().config;

void showHelp()
{
  std::cout << "picdbv - Help\n\n"
            << "parameters:\n"
            << "  All parameters are optional. If no parameters are given, the programme will\n"
            << "  behave as if some default parameters were given. These are marked with\n"
            << "  \"(default)\" in the following text.\n"
            << "  --help      - show this help message and exits the programme\n"
            << "  -?          - same as --help\n"
            << "  --search    - search for new files in the directory (default)\n"
            << "  --nosearch  - do not search for new files\n"
            << "  --auto-tagging (default)\n"
            << "              - tries to guess artist's name and picture name from file name\n"
            << "  --noauto-tagging\n"
            << "              - disables auto-tagging\n"
            << "  --load      - loads database from predefined file (default)\n"
            << "  --noload    - does not load database from file\n"
            << "  --list      - lists all files in the database after all operations are\n"
            << "                finished\n"
            << "  --nolist    - surpress listing (default)\n"
            << "  --save      - saves database to predefined file\n"
            << "  --nosave    - does not save database (default)\n"
            << "  --purge     - removes not existing files from database\n"
            << "  --nopurge   - does not remove files (default)\n"
            << "  --stats-only\n"
            << "              - just shows DB statistics and does not start the graphical user\n"
            << "                interface\n";
}

int main(int argc, char **argv)
{
  bool doList = false;
  bool doLoad = true;
  bool doSave = false;
  bool doTagging = true;
  bool doSearchNewFiles = true;
  bool doPurge = false;
  bool onlyStats = false;

  config.initialiseValues();
  const std::string configFileName = "picdbv.ini";
  //load configuration, if present
  std::cout <<"Loading configuration file...";
  if (FileExists(configFileName))
  {
    if (config.loadFromFile(configFileName))
    {
      std::cout << "success.\n";
    }
    else { std::cout << "Loading configuration file failed.\n"; }
  }
  else
  {
    std::cout <<" file doesn't exist. Skipping.\n";
  }

  int arg_i = 1; //zeroth parameter is file of executable, so skip it
  while (arg_i<argc)
  {
    const std::string arg_string = std::string(argv[arg_i]);
    //check for valid parameters
    if (arg_string == "--help" || arg_string == "-?")
    {
      showHelp();
      return 0;
    }
    else if (arg_string == "--list" || arg_string == "-ls")
    {
      doList = true;
    }
    else if (arg_string == "--nolist" || arg_string == "-nls")
    {
      doList = false;
    }
    else if (arg_string == "--load" || arg_string == "-l")
    {
      doLoad = true;
    }
    else if (arg_string == "--noload")
    {
      doLoad = false;
    }
    else if (arg_string == "--save" || arg_string == "-s")
    {
      doSave = true;
    }
    else if (arg_string == "--nosave")
    {
      doSave = false;
    }
    else if (arg_string == "--auto-tagging" || arg_string == "-t")
    {
      doTagging = true;
    }
    else if (arg_string == "--noauto-tagging" || arg_string == "-nt")
    {
      doTagging = false;
    }
    else if (arg_string == "--search")
    {
      doSearchNewFiles = true;
    }
    else if (arg_string == "--nosearch")
    {
      doSearchNewFiles = false;
    }
    else if (arg_string == "--purge")
    {
      doPurge = true;
    }
    else if (arg_string == "--nopurge")
    {
      doPurge = false;
    }
    else if (arg_string == "--stats-only")
    {
      onlyStats = true;
    }
    else
    {
      //unrecognised parameter
      std::cout <<"Invalid parameter supplied: \""<<arg_string<<"\".\n";
      std::cout <<"Use the parameter --help to get a list of valid parameters.\n";
      return 0;
    }
    arg_i = arg_i +1;
  }//while
  std::cout << "Processed command line parameters: "<<argc-1<<"\n";

  std::cout << "DataBase starts now.\n";
  DataBase& db = DataBase::getSingleton();
  db.clearAllData();
  if (doSearchNewFiles)
  {
    std::cout <<"Getting new files... ";
    std::cout.flush();
    if (db.getFilesFromDirectory(config.Directory))
    {
      std::cout << "OK\n";
    }
    else
    {
      std::cout << "failed or no new files.\n";
    }
  }//search
  if (doTagging)
  {
    std::cout <<"AutoTagging...\n";
    db.AutoTag_Splitter();
  }
  //loading DB from file
  if (doLoad)
  {
    std::cout <<"Loading DB file...";
    if (FileExists(config.DB_File))
    {
      if (db.loadFromFile(config.DB_File))
      {
        std::cout << "success.\n";
      }
      else { std::cout << "failed.\n"; }
    }
    else
    {
      std::cout <<" file doesn't exist. Skipping.\n";
    }
  }//load

  if (doList)
  {
    std::cout<<"Listing files:\n";
    db.ListData();
  }
  std::cout <<"Number of DB entries: "<<db.getNumEntries()<<"\n";

  if (doPurge)
  {
    std::cout << "Purging files (this may take a while)...\n";
    std::vector<std::string> sv = db.getNonexistingFiles(config.Directory);
    std::cout <<"Number of purgable files: "<<sv.size()<<"\n";
    std::cout << "Purging...";
    db.purgeNonexistingFiles(config.Directory);
    std::cout << "done.\n";
  }

  if (doSave)
  {
    if (db.saveToFile(config.DB_File))
    {
      std::cout << "DB saved successfully to \""<<config.DB_File<<"\".\n";
    }
    else
    {
      std::cout << "DB not saved to \""<<config.DB_File<<"\" due to error.\n";
    }
  }

  if (!onlyStats)
  {
    //setup GLUT/GLFW library
    std::cout << "Initializing GUI. Used library is "<<GUI::getSingleton().getType()<<".\n";
    GUI::getSingleton().init();
    if (!GUI::getSingleton().createWindow(640, 400, 0, 0, "picdbv window"))
    {
      std::cout << "ERROR: Could not create GUI window.\n";
      return 0;
    }
    #ifdef APP_USING_FREEGLUT
    //If we use freeglut API instead of the usual one, we like to return from the
    //main loop instead of exiting without prior notice.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    #endif
    GUI::getSingleton().initGL();
    GUI::getSingleton().getAllFiles();

    bool success = GUI::getSingleton().setCurrentImageByIndex(0);
    unsigned int i = 1;
    while (!success and (i<GUI::getSingleton().getNumberOfFilesInList()))
    {
      //set another image
      success = GUI::getSingleton().setCurrentImageByIndex(i);
      ++i;
    }//while

    GUI::getSingleton().setCallbacks();

    //Starting GLUT
    std::cout << "start GUI main loop\n";
    GUI::getSingleton().start();
  }//if not only stats

  db.showTagStatistics();
  db.showWhoStatistics();
  std::cout << "untagged files: "<<db.getUntaggedFiles().size()<<"\n";
  std::cout << "unknown artist files: " << db.getUnknownArtistFiles().size()<<"\n";
  std::cout << "unknown who files: " << db.getUnknownWhoFiles().size()<<"\n";

  return 0;
}

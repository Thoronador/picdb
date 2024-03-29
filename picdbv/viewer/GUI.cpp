/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2014, 2023  Dirk Stolle

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

#include "GUI.hpp"
#include <iostream>
#include <GL/gl.h>
#include "../../libstriezel/common/graphics/ImageLoader.hpp"
#include "../../libstriezel/common/graphics/GLfunctions.hpp"
#include "../../libstriezel/common/StringUtils.hpp"
#include "../../libstriezel/filesystem/file.hpp"
#include "../../libstriezel/hash/sha256/FileSourceUtility.hpp"
#include "PicDatabase.hpp"
#include "GUIMultiLinePanel.hpp"

GUI::GUI()
: image_tex(0),
  m_CurrentTextureTarget(GL_TEXTURE_2D),
  selectedFiles(std::vector<std::string>()),
  currentIndex(0),
  m_Panels(std::vector<GUITextPanel*>()),
  m_showInput(false),
  m_InputLine("")
{
  glis.setHeight(0);
  glis.setWidth(0);
  glis.setBuffer(nullptr);
  glis.setFormat(0);
  nonFullscreenData.isFullscreen = false;
  nonFullscreenData.window_width = 50;
  nonFullscreenData.window_height = 50;
  nonFullscreenData.pos_x = 0;
  nonFullscreenData.pos_y = 0;
}

GUI& GUI::getSingleton()
{
  static GUI Instance;
  return Instance;
}

GUI::~GUI()
{
  glis.freeBuffer();
  freeGLTexture();
  removeAllPanels();
}

void GUI::initGL()
{
  glLoadIdentity();
  glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
  //enable GL stuff
  // Enable backface culling
  glEnable(GL_CULL_FACE);
  //enable depth test
  //glEnable(GL_DEPTH_TEST);
}

void GUI::keyPressed(int Key)
{
  if (isEnterKeyCode(Key))
  {
    m_showInput = !m_showInput;
    if (!m_showInput)
    {
      //input window not active means input "window" closed and have some kind
      // of stuff to do
      processInput();
    }
    requestRedisplay();
    return;
  }

  if (m_showInput)
  {
    // change displayed text according to pressed key
    // backspace or del
    if (isBackspaceKeyCode(Key) or isDeleteKeyCode(Key))
    {
      if (!m_InputLine.empty())
        m_InputLine.erase(m_InputLine.length()-1);
    }
    // escape
    else if (isESCKeyCode(Key))
    {
      m_showInput = false;
    }
    // some other printable character
    else if (Key >= 32)
    {
      m_InputLine.append(1, Key);
    }

    requestRedisplay();
    return;
  }//if input box is shown

  if (isESCKeyCode(Key) || (Key == 'Q') || (Key == 'q'))
  {
    //Ende Gel�nde
    std::cout << "Quit ";
    glis.freeBuffer();
    terminate();
    return;
  }
  else if ((Key == 'R') || (Key == 'r'))
  {
    // force redisplay
    requestRedisplay();
  }
  else if (isRightKeyCode(Key) && (!selectedFiles.empty()))
  {
    std::cout << "right pressed\n";
    showNext();
    requestRedisplay();
  }
  else if (isLeftKeyCode(Key) && (!selectedFiles.empty()))
  {
    std::cout << "left pressed\n";
    showPrevious();
    requestRedisplay();
  }
}

void GUI::addPanel(GUITextPanel* panel)
{
  if (panel != nullptr)
    m_Panels.push_back(panel);
}

void GUI::removeAllPanels()
{
  while (!m_Panels.empty())
  {
    GUITextPanel* p = m_Panels.back();
    m_Panels.pop_back();
    if (p != nullptr)
    {
      delete p;
      p = nullptr;
    }
  }
}

void GUI::drawPanels()
{
  for (auto panel: m_Panels)
  {
    if (panel != nullptr)
      panel->draw();
  }
}

void GUI::drawInput()
{
  const std::string::size_type len = m_InputLine.length() + 2;
  const float dx = 2.0f / static_cast<float>(getWindowWidth());
  const float dy = 2.0f / static_cast<float>(getWindowHeight());
  glLineWidth(2.0f);
  const float left_bg = -dx * (len * 4 + 3 + 2 + 3);
  const float right_bg = dx * (len * 4 + 3 + 2 + 3);
  const float top_bg = dy * (8 + 3 + 2 + 3);
  const float bottom_bg = -dy * (8 + 3 + 2 + 3);
  // background
  glBegin(GL_QUADS);
    glColor3f(1.0, 0.5, 0.25);
    glVertex3f(left_bg, top_bg, 0.0);
    glVertex3f(left_bg, bottom_bg, 0.0);
    glVertex3f(right_bg, bottom_bg, 0.0);
    glVertex3f(right_bg, top_bg, 0.0);
  glEnd();
  // border
  glBegin(GL_LINE_LOOP);
    glColor3f(0.25, 0.25, 1.0);
    glVertex3f(left_bg + 3 * dx, top_bg - 4 * dy, 0.0);
    glVertex3f(left_bg + 3 * dx, bottom_bg + 3 * dy, 0.0);
    glVertex3f(right_bg - 4 * dx, bottom_bg + 3 * dy, 0.0);
    glVertex3f(right_bg - 4 * dx, top_bg - 4 * dy, 0.0);
  glEnd();
  writeText(m_InputLine, left_bg + 12 * dx, bottom_bg + 10 * dy, 0.0f);
}

void GUI::processInput()
{
  trim(m_InputLine);
  if (m_InputLine.empty())
  {
    std::cout << "Got empty input line after trim, nothing to do here.\n";
    return;
  }

  //check for administrative tasks
  if ('?' == m_InputLine[0])
  {
    //save requested?
    if ("?save" == m_InputLine)
    {
      std::cout << "Saving DB requested...\n";
      if (PicDatabase::getSingleton().saveToFile(config.DB_File))
      {
        std::cout << "DB saved successfully to \"" << config.DB_File << "\".\n";
      }
      else
      {
        std::cout << "DB not saved to \"" << config.DB_File << "\" due to error.\n";
      }
    }//if save
    else if ("?load" == m_InputLine)
    {
      std::cout << "Loading DB requested...\n";
      if (libstriezel::filesystem::file::exists(config.DB_File))
      {
        if (PicDatabase::getSingleton().loadFromFile(config.DB_File))
        {
          std::cout << "success.\n";
        }
        else
        {
          std::cout << "failed.\n";
        }
      }
      else
      {
        std::cout << "DB file doesn't exist. Skipping.\n";
      }
    }//if save
    else if ("?purge" == m_InputLine)
    {
      std::cout << "Purging files (this may take a while)...\n";
      std::vector<std::string> sv = PicDatabase::getSingleton().getNonexistingFiles(config.Directory);
      std::cout << "Number of purgable files: " << sv.size() <<"\n";
      std::cout << "Purging...";
      PicDatabase::getSingleton().purgeNonexistingFiles(config.Directory);
      std::cout << "done.\n";
    }//if purge
    else if ("?all" == m_InputLine)
    {
      std::cout << "Resetting file list to all files...";
      getAllFiles();
      std::cout << "done.\n";
      showNext();
    }//if all
    else if ("?untagged" == m_InputLine)
    {
      std::cout << "Requesting untagged files...\n"  ;
      std::vector<std::string> sv = PicDatabase::getSingleton().getUntaggedFiles();
      std::cout << "Number of untagged files: " << sv.size() << "\n";
      if (!sv.empty())
      {
        selectedFiles = sv;
        setCurrentImageByIndex(0);
      }
    }//if untagged
    else if ("?unknown" == m_InputLine)
    {
      std::cout << "Requesting files with unknown persons...\n"  ;
      std::vector<std::string> sv = PicDatabase::getSingleton().getUnknownWhoFiles();
      std::cout << "Number of files: " << sv.size() << "\n";
      if (!sv.empty())
      {
        selectedFiles = sv;
        setCurrentImageByIndex(0);
      }
    }//if unknown who
    else if (("?unknown_artist" == m_InputLine) || ("?unknownartist" == m_InputLine))
    {
      std::cout << "Requesting files with unknown artist...\n"  ;
      std::vector<std::string> sv = PicDatabase::getSingleton().getUnknownArtistFiles();
      std::cout << "Number of files: " << sv.size() << "\n";
      if (!sv.empty())
      {
        selectedFiles = sv;
        setCurrentImageByIndex(0);
      }
    }//if unknown artist
    else if (("?fullscreen" == m_InputLine) || (m_InputLine == "?fs"))
    {
      std::cout << "Fullscreen mode requested... ";
      if (nonFullscreenData.isFullscreen)
      {
        std::cout << "We are already in fullscreen mode!\n";
      }
      else
      {
        nonFullscreenData.pos_x = getWindowPosX();
        nonFullscreenData.pos_y = getWindowPosY();
        nonFullscreenData.window_width = getWindowWidth();
        nonFullscreenData.window_height = getWindowHeight();
        nonFullscreenData.isFullscreen = true;
        makeFullScreen();
        std::cout << "done.\n";
      }
    }//if fullscreen
    else if (("?no-fullscreen" == m_InputLine) || ("?normal" == m_InputLine))
    {
      std::cout << "Returning from fullscreen mode requested... ";
      if (!nonFullscreenData.isFullscreen)
      {
        std::cout << "We are already out of fullscreen mode!\n";
      }
      else
      {
        setWindowDimensions(nonFullscreenData.window_width, nonFullscreenData.window_height);
        setWindowPosition(nonFullscreenData.pos_x, nonFullscreenData.pos_y);
        nonFullscreenData.isFullscreen = false;
        std::cout << "done.\n";
      }
    }//if not fullscreen
    else if (((m_InputLine.substr(0,5) == "?jump") || (m_InputLine.substr(0,5) == "?goto"))
          && (m_InputLine.length() > 5))
    {
      std::string jump_index = m_InputLine.substr(5);
      trim(jump_index);
      int newIndex = 0;
      if (stringToInt(jump_index, newIndex))
      {
        if ((newIndex < 1) || (newIndex > selectedFiles.size()))
        {
          std::cout << "Warning; jump index " << newIndex << " is out of range. "
                    << "Valid range is [1;" << selectedFiles.size() << "].\n";
        }
        else
        {
          setCurrentImageByIndex(newIndex - 1);
        }
      }//if conversion was successful
      else
      {
        // conversion failed
        std::cout << "Error: The string \"" << jump_index << "\" does not represent a integer value.\n";
      }
    }//jump/go to specified index
    else
    {
      std::cout << "Unknown operation \"" << m_InputLine << "\" requested!\n";
      return;
    }
    m_InputLine = "";
    return;
  }//if '?'

  if (m_InputLine.at(0) == '+')
  {
    //request to add something to current picture's data
    if (m_InputLine.length() > 3)
    {
      if (m_InputLine.substr(0, 3) == "+t ")
      {
        std::cout<< "Adding tags requested...\n";
        if (PicDatabase::getSingleton().hasFile(selectedFiles.at(currentIndex)))
        {
          //get copy of current data
          PicData data = PicDatabase::getSingleton().getData(selectedFiles[currentIndex]);
          //split line into separate tags
          std::set<std::string> new_tags = Splitter::splitAtSpace(m_InputLine.substr(3));
          std::set<std::string>::const_iterator i;
          for (i = new_tags.begin(); i != new_tags.end(); ++i)
          {
            if (!(i->empty()))
              data.tags.insert(*i);
          }//for
          PicDatabase::getSingleton().addFile(selectedFiles[currentIndex], data);
          updateInfoPanels(data.who, data.tags, data.artist);
          requestRedisplay();
          std::cout << "Tags \"" << m_InputLine.substr(3) << "\" added successfully.\n";
        }//if file present
      }//if +t
      else if (m_InputLine.substr(0, 3) == "+w ")
      {
        std::cout<< "Adding who/what requested...\n";
        if (PicDatabase::getSingleton().hasFile(selectedFiles.at(currentIndex)))
        {
          //get copy of current data
          PicData data = PicDatabase::getSingleton().getData(selectedFiles[currentIndex]);
          //split line into separate tags
          std::set<std::string> new_whos = Splitter::splitAtSpace(m_InputLine.substr(3));
          std::set<std::string>::const_iterator i;
          for (i = new_whos.begin(); i != new_whos.end(); ++i)
          {
            if (!(i->empty()))
              data.who.insert(*i);
          }//for
          PicDatabase::getSingleton().addFile(selectedFiles[currentIndex], data);
          updateInfoPanels(data.who, data.tags, data.artist);
          requestRedisplay();
          std::cout << "Who: \"" << m_InputLine.substr(3) << "\" added successfully.\n";
        }//if file present
      }//if +w
      else
      {
        std::cout << "Unknown operation \"" << m_InputLine << "\" requested!\n";
        return;
      }
      return;
    }//if line's length > 3
  }//if '+'

  if (m_InputLine.at(0) == '-')
  {
    //request to remove something from current picture's data
    if (m_InputLine.length() > 3)
    {
      if (m_InputLine.substr(0, 3) == "-t ")
      {
        std::cout << "Removing tags requested...\n";
        if (PicDatabase::getSingleton().hasFile(selectedFiles.at(currentIndex)))
        {
          //get copy of current data
          PicData data = PicDatabase::getSingleton().getData(selectedFiles[currentIndex]);
          //split line into separate tags
          std::set<std::string> del_tags = Splitter::splitAtSpace(m_InputLine.substr(3));
          std::set<std::string>::const_iterator i;
          for (i = del_tags.begin(); i != del_tags.end(); ++i)
          {
            if (!(i->empty()))
              data.tags.erase(*i);
          }
          PicDatabase::getSingleton().addFile(selectedFiles[currentIndex], data);
          updateInfoPanels(data.who, data.tags, data.artist);
          requestRedisplay();
          std::cout << "Tags \"" << m_InputLine.substr(3) << "\" removed.\n";
        }//if file present
      }//if -t
      else if (m_InputLine.substr(0, 3) == "-w ")
      {
        std::cout<< "Removing who/what requested...\n";
        if (PicDatabase::getSingleton().hasFile(selectedFiles.at(currentIndex)))
        {
          //get copy of current data
          PicData data = PicDatabase::getSingleton().getData(selectedFiles[currentIndex]);
          //split line into separate tags
          std::set<std::string> del_whos = Splitter::splitAtSpace(m_InputLine.substr(3));
          std::set<std::string>::const_iterator i;
          for (i = del_whos.begin(); i != del_whos.end(); ++i)
          {
            if (!(i->empty()))
              data.who.erase(*i);
          }//for
          PicDatabase::getSingleton().addFile(selectedFiles[currentIndex], data);
          updateInfoPanels(data.who, data.tags, data.artist);
          requestRedisplay();
          std::cout << "Who: \"" << m_InputLine.substr(3) << "\" removed successfully.\n";
        }//if file present
      }//if +w
      else
      {
        std::cout << "Unknown removal operation \"" << m_InputLine << "\" requested!\n";
        return;
      }
      return;
    }//if line's length > 3
  }//if '-'

  //must be a query instead
  std::cout << "You have entered the query \"" << m_InputLine << "\".\n";
  std::cout << "Performing query...";
  std::vector<std::string> query_result = PicDatabase::getSingleton().executeQuery(m_InputLine);
  std::cout << "done.\nResults: " << query_result.size() << "\n";
  if (!query_result.empty())
  {
    // set result as new selected files
    selectedFiles = query_result;
    // set new first image
    if (setCurrentImageByIndex(0))
    {
      if (PicDatabase::getSingleton().hasFile(selectedFiles.at(0)))
      {
        const PicData& data = PicDatabase::getSingleton().getData(selectedFiles.at(0));
        updateInfoPanels(data.who, data.tags, data.artist);
      }//if file in DB
    }//if new image was set
  }
  else
    std::cout << "The query returned zero results. Selection was not changed.\n";
}//function processInput

void GUI::getAllFiles()
{
  selectedFiles.clear();
  currentIndex = -1;
  const std::set<std::string> fileset = PicDatabase::getSingleton().getListedFiles();
  std::set<std::string>::const_iterator iter = fileset.begin();
  while (iter != fileset.end())
  {
    selectedFiles.push_back(*iter);
    ++iter;
  }
  freeGLTexture();
  glis.freeBuffer();
}

unsigned int GUI::getNumberOfFilesInList() const
{
  return selectedFiles.size();
}

void GUI::showNext()
{
  ++currentIndex;
  // Are we out of range?
  if ((currentIndex >= selectedFiles.size()) || (currentIndex < 0))
    currentIndex = 0; // reset to begin

  if (!selectedFiles.empty())
  {
    // check for presence of file and get next file
    while (currentIndex<selectedFiles.size())
    {
      if (!libstriezel::filesystem::file::exists(config.Directory + selectedFiles[currentIndex]))
      {
        // delete file from the DB, it does not exist any more
        PicDatabase::getSingleton().deleteFile(selectedFiles[currentIndex]);
        // delete file from the selected files vector, too
        selectedFiles.erase(selectedFiles.begin() + currentIndex);
      }//if
      else
      {
        break; //break out of while loop
      }
    }//while

    if (currentIndex >= selectedFiles.size())
    {
      // no image
      std::cout << "No next image in list.\n";
      freeGLTexture();
      glis.freeBuffer();
      removeAllPanels();
      return;
    }
    // try to display image
    if (PicDatabase::getSingleton().hasFile(selectedFiles[currentIndex]))
    {
      const PicData& info = PicDatabase::getSingleton().getData(selectedFiles[currentIndex]);
      if (setCurrentImageByIndex(currentIndex))
      {
        // update info stuff
        updateInfoPanels(info.who, info.tags, info.artist);
      }
    }
    else
    {
      // no such file in DB, no new image
      std::cout << "No image named \"" << selectedFiles[currentIndex] << "\" in DB.\n";
      freeGLTexture();
      glis.freeBuffer();
      removeAllPanels();
    }
  }
}

void GUI::showPrevious()
{
  std::cout << "DBG: showPrev\n";
  --currentIndex;
  //are we out of range?
  if ((currentIndex < 0) || (currentIndex >= selectedFiles.size()))
    currentIndex = selectedFiles.size() - 1; //reset to end

  if (!selectedFiles.empty())
  {
    //check for presence of file and get next file
    while (currentIndex<selectedFiles.size())
    {
      if (!libstriezel::filesystem::file::exists(config.Directory + selectedFiles[currentIndex]))
      {
        // delete file from the DB, it does not exist any more
        PicDatabase::getSingleton().deleteFile(selectedFiles[currentIndex]);
        // delete file from the selected files vector, too
        selectedFiles.erase(selectedFiles.begin()+currentIndex);
      }
      else
      {
        break; //break out of loop
      }
    }//while

    if (currentIndex >= selectedFiles.size())
    {
      //no image
      removeAllPanels();
      std::cout << "No previous image in list.\n";
      freeGLTexture();
      glis.freeBuffer();
      return;
    }
    //try to display image
    if (PicDatabase::getSingleton().hasFile(selectedFiles[currentIndex]))
    {
      std::cout << "DBG: Accessing database...\n";
      const PicData& info = PicDatabase::getSingleton().getData(selectedFiles[currentIndex]);
      std::cout << "DBG: Setting image...\n";
      if (setCurrentImageByIndex(currentIndex))
      {
        //update info stuff
        updateInfoPanels(info.who, info.tags, info.artist);
      }
    }
    else
    {
      //no such file in DB, no new image
      removeAllPanels();
      std::cout << "No image named \"" << selectedFiles[currentIndex] << "\" in DB.\n";
      freeGLTexture();
      glis.freeBuffer();
    }
  }//if not empty
}

/*
void GUI::specialKeyPressed(int Key)
{
  if (isRightKeyCode(Key) && (!selectedFiles.empty()))
  {
    std::cout << "right pressed\n";
    showNext();
    requestRedisplay();
  }
  else if (isLeftKeyCode(Key) && (!selectedFiles.empty()))
  {
    std::cout << "left pressed\n";
    showPrevious();
    requestRedisplay();
  }
}
*/

void GUI::draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  if ((image_tex != 0) && glis.isLoaded())
  {
    // we have an image, draw it
    GLfloat l, r, b, t;
    if (getCorners(getWindowWidth(), getWindowHeight(), glis.getWidth(), glis.getHeight(), l, r, b, t))
    {
      //draw it, we got the coordinates
      glEnable(m_CurrentTextureTarget);
      glEnable(GL_ALPHA_TEST);
      glBindTexture(GL_TEXTURE_2D, image_tex);
      glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0);
        if ( m_CurrentTextureTarget == GL_TEXTURE_2D)
        {
          glTexCoord2f(0.0, 1.0);//upper left
          glVertex3f(l, t, 0.0);
          glTexCoord2f(0.0, 0.0);//lower left
          glVertex3f(l, b, 0.0);
          glTexCoord2f(1.0, 0.0);//lower right
          glVertex3f(r, b, 0.0);
          glTexCoord2f(1.0, 1.0);//upper right
          glVertex3f(r, t, 0.0);
        }
        else
        {
          // we've got rectangular target here
          glTexCoord2f(0.0, glis.getHeight());//upper left
          glVertex3f(l, t, 0.0);
          glTexCoord2f(0.0, 0.0);//lower left
          glVertex3f(l, b, 0.0);
          glTexCoord2f(glis.getWidth(), 0.0);//lower right
          glVertex3f(r, b, 0.0);
          glTexCoord2f(glis.getWidth(), glis.getHeight());//upper right
          glVertex3f(r, t, 0.0);
        }
      glEnd();
      glDisable(GL_ALPHA_TEST);
      glDisable(m_CurrentTextureTarget);
    }
    else
    {
      std::cout << "Could not get image coordinates!\n!";
    }
  }
  drawPanels();
  if (m_showInput) drawInput();
  glFlush();
  swapBuffers();
}

void GUI::performIdleTasks()
{
  // hash checks or updates
  if (!m_IdleHashUpdateFiles.empty())
  {
    const std::string currFile = *(m_IdleHashUpdateFiles.begin());
    const SHA256::MessageDigest sha_md = SHA256::computeFromFile(currFile);
    if (!sha_md.isNull())
    {
      // hash found
      if (PicDatabase::getSingleton().hasFile(currFile))
      {
        PicData tempData = PicDatabase::getSingleton().getData(currFile);
        if (tempData.hash_sha256 != sha_md)
        {
          // hash difference found
          if (tempData.hash_sha256.isNull())
          {
            //no previous hash set, silent update
            tempData.hash_sha256 = sha_md;
            const bool could_use_update = (tempData.tags.empty() || tempData.who.empty() || tempData.artist.empty());
            bool dataUpdated = false;
            if (could_use_update and PicDatabase::getSingleton().hasDataForHash(sha_md))
            {

                const PicData& otherData = PicDatabase::getSingleton().getData(sha_md);
                //update tags, who and artist
                if (tempData.tags.empty())
                {
                  tempData.tags = otherData.tags;
                  dataUpdated = true;
                }
                if (tempData.who.empty())
                {
                  tempData.who = otherData.who;
                  dataUpdated = true;
                }
                if (tempData.artist.empty())
                {
                  tempData.artist = otherData.artist;
                  dataUpdated = true;
                }

            }//if update needed and file with same hash present
            //set new data
            PicDatabase::getSingleton().addFile(currFile, tempData);
            //do we need to update the panels?
            if (dataUpdated && (currFile == selectedFiles[currentIndex]))
            {
              updateInfoPanels(tempData.who, tempData.tags, tempData.artist);
            }
          }
          else
          {
            //file has changed since last DB update - notify user!
            // TODO
          }
        }//if
      }
    }
    //remove file from list of hash updates
    m_IdleHashUpdateFiles.erase(currFile);
  }//if not empty
  else
  {
    // update some unhashed files instead
    const unsigned int uh = PicDatabase::getSingleton().getNumUnhashed();
    if (uh > 0)
    {
      const unsigned int info_step = 100;
      static unsigned int next_info_at = uh - (uh % info_step);
      if (uh < next_info_at)
      {
        std::cout << "Info: unhashed files: " << uh << "\n";
        next_info_at = next_info_at > info_step ? next_info_at-info_step : 1;
      }
      PicDatabase::getSingleton().hashUnhashedFiles(config.Directory, 2);
    }
  }
}//func performing idle task

void GUI::writeText(const std::string& text, const float x, const float y, const float z)
{
  protectedWriteText(text, x, y, z);
}

void GUI::centerText(const std::string& text, const float y, const float z)
{
  const float dx = 2.0f / static_cast<float>(getWindowWidth());
  writeText(text, -dx * text.length() * 4, y, z);
}

bool GUI::setCurrentImage(const std::string& FileName, const std::string& shortName, const int32_t index_first, const int32_t index_total)
{
  freeGLTexture();
  glis.freeBuffer();

  const ImageLoader::ImageType i_type = ImageLoader::getImageType(FileName);

  if (!ImageLoader::isSupportedImage(i_type))
  {
    std::cout << "File \"" << FileName << "\" seems to be neither BMP, nor JPEG, nor PNG, nor PPM.\n";
    setWindowTitle("picdbv");
    removeAllPanels();
    updateCenterTopPanel(shortName);
    return false;
  }

  glis = ImageLoader::readImage(FileName, i_type);

  if (!glis.isLoaded())
  {
    std::cout << "File \"" << FileName << "\" could not be loaded.\n";
    setWindowTitle("picdbv");
    removeAllPanels();
    updateCenterTopPanel(shortName);
    return false;
  }

  //file is ready, I guess
  glGenTextures(1, &image_tex);
  GLenum texTarget = GL_TEXTURE_2D;
  GLenum proxyTarget = GL_PROXY_TEXTURE_2D;
  if (glis.needsNPOTExtension())
  {
    getNPOTTextureTargets(getNPOTSupport(), texTarget, proxyTarget);
  }

  // make sure our texture target is enabled
  if (!glIsEnabled(texTarget))
  {
    glEnable(texTarget);
  }

  glBindTexture(texTarget, image_tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  if ((glis.getFormatGL() == GL_RGB) || (glis.getFormatGL() == GL_BGR))
  {
    // proxy stuff
    glTexImage2D(proxyTarget, 0, 3, glis.getWidth(), glis.getHeight(), 0, glis.getFormatGL(), GL_UNSIGNED_BYTE, glis.getBufferPointer());
    GLint test_width = 0;
    glGetTexLevelParameteriv(proxyTarget, 0, GL_TEXTURE_WIDTH, &test_width);
    if (test_width != 0)
    {
      // the real stuff
      glTexImage2D(texTarget, 0, 3, glis.getWidth(), glis.getHeight(), 0, glis.getFormatGL(), GL_UNSIGNED_BYTE, glis.getBufferPointer());
    }
    else
    {
      // proxy test failed
      std::cout << "Your GL implementation cannot handle this texture size ("
                << glis.getWidth() << "x" << glis.getHeight() << "px) properly! Trying"
                << " half size.\n";
      if (glis.resizeToHalf())
      {
        // the real stuff
        glTexImage2D(texTarget, 0, 3, glis.getWidth(), glis.getHeight(), 0, glis.getFormatGL(), GL_UNSIGNED_BYTE, glis.getBufferPointer());
      }
      else
      {
        std::cout << "Error during resize attempt!\n";
        return false;
      }
    }
  }
  else if (glis.getFormatGL() == GL_RGBA)
  {
    //proxy stuff
    glTexImage2D(proxyTarget, 0, 4, glis.getWidth(), glis.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glis.getBufferPointer());
    GLint test_width = 0;
    glGetTexLevelParameteriv(proxyTarget, 0, GL_TEXTURE_WIDTH, &test_width);
    if (test_width != 0)
    {
      // the real stuff
      glTexImage2D(texTarget, 0, 4, glis.getWidth(), glis.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glis.getBufferPointer());
    }
    else
    {
      // proxy test failed
      std::cout << "Your GL implementation cannot handle this texture size ("
                << glis.getWidth() << "x" << glis.getHeight() << "px) properly!"
                << " Trying half size.\n";
      if (glis.resizeToHalf())
      {
        // the real stuff
        glTexImage2D(texTarget, 0, 4, glis.getWidth(), glis.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glis.getBufferPointer());
      }
      else
      {
        std::cout << "Error during resize attempt!\n";
        return false;
      }
      return false;
    }
  }
  else
  {
    std::cout << "Image has unknown GL format.\n";
    glBindTexture(texTarget, 0);
    glDeleteTextures(1, &image_tex);
    image_tex = 0;
    return false;
  }
  glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  updateCenterTopPanel(shortName);
  if ((index_first <= 0) || (index_total <= 0))
  {
    setWindowTitle("picdbv - " + shortName + " (" + std::to_string(glis.getWidth()) + "x" + std::to_string(glis.getHeight()) + "px)");
  }
  else
  {
    // show title with index
    setWindowTitle("picdbv - " + shortName + " (" + std::to_string(glis.getWidth()) + "x" + std::to_string(glis.getHeight()) + "px) "
                      + std::to_string(index_first) + " of " + std::to_string(index_total));
  }
  requestRedisplay();
  std::cout << "Info: Successfully set new image \"" << shortName << "\" as active image.\n";
  // mark file for hash update
  m_IdleHashUpdateFiles.insert(FileName);
  // save current texture target
  m_CurrentTextureTarget = texTarget;
  return true;
}

bool GUI::setCurrentImageByIndex(const int32_t idx)
{
  // check range
  if ((idx < 0) || (idx >= selectedFiles.size()))
    return false;
  if (setCurrentImage(config.Directory+selectedFiles.at(idx), selectedFiles.at(idx), idx + 1, selectedFiles.size()))
  {
    currentIndex = idx;
    std::cout << "Info: Successfully set new image index " << idx << " as active image.\n";
    return true;
  }
  return false;
}

void GUI::updateCenterTopPanel(const std::string& new_msg)
{
  for (auto* panel: m_Panels)
  {
    GUIAdjustedTextPanel* atp = dynamic_cast<GUIAdjustedTextPanel*>(panel);
    if (atp != nullptr)
    {
      if (atp->getAdjustment() == GUIAdjustedTextPanel::paCenterTop)
      {
        // it's the one we are looking for!
        atp->setString(new_msg);
        requestRedisplay();
        return;
      }
    }
  }
  // if we get to this place, then there is no such panel yet
  addPanel(new GUIAdjustedTextPanel(new_msg, GUIAdjustedTextPanel::paCenterTop, 0.2, 1.0, 0.2));
  requestRedisplay();
  return;
}

void GUI::updateMultiLinePanel(const GUIAdjustedTextPanel::PanelAdjustment adj, const std::string& head, const std::set<std::string>& items)
{
  for (auto* panel: m_Panels)
  {
    GUIMultiLineAdjustedPanel* mlap = dynamic_cast<GUIMultiLineAdjustedPanel*>(panel);
    if (mlap != nullptr)
    {
      if (mlap->getAdjustment() == adj)
      {
        // it's the one we are looking for!
        mlap->setString(head);
        mlap->addLines(items);
        requestRedisplay();
        return;
      }
    }
  }
  // if we get to this place, then there is no such panel yet
  GUIMultiLineAdjustedPanel* mlap = new GUIMultiLineAdjustedPanel(head, adj);
  mlap->setString(head);
  mlap->addLines(items);
  mlap->setAdjustment(adj);
  mlap->setColour(0.2, 1.0, 0.2);
  addPanel(mlap);
  requestRedisplay();
  return;
}

void GUI::updateInfoPanels(const std::set<std::string>& who, const std::set<std::string>& tag, const std::string& artist)
{
  if (!who.empty())
  {
    updateMultiLinePanel(GUIAdjustedTextPanel::paLeftTop, "who:", who);
  }
  else
  {
    std::set<std::string> temp;
    temp.insert("(unknown)");
    updateMultiLinePanel(GUIAdjustedTextPanel::paLeftTop, "who:", temp);
  }
  if (!tag.empty())
  {
    updateMultiLinePanel(GUIAdjustedTextPanel::paRightTop, "tags (" + std::to_string(tag.size()) + "):", tag);
  }
  else
  {
    std::set<std::string> temp;
    temp.insert("(none)");
    updateMultiLinePanel(GUIAdjustedTextPanel::paRightTop, "tags:", temp);
  }
  if (!artist.empty())
  {
    std::set<std::string> temp;
    temp.insert(artist);
    updateMultiLinePanel(GUIAdjustedTextPanel::paLeftBottom, "artist:", temp);
  }
  else
  {
    std::set<std::string> temp;
    temp.insert("(unknown)");
    updateMultiLinePanel(GUIAdjustedTextPanel::paLeftBottom, "artist:", temp);
  }
}

void GUI::freeGLTexture()
{
  if ((image_tex != 0) && (glIsTexture(image_tex) == GL_TRUE))
  {
    glDeleteTextures(1, &image_tex);
    image_tex = 0;
  }
}

bool getCorners(const int window_width, const int window_height, const int img_width,
                const int img_height, GLfloat& left, GLfloat&right, GLfloat& bottom,
                GLfloat& top)
{
  if ((window_width == 0) || (window_height == 0) || (img_width == 0) || (img_height == 0))
    return false;

  if ((window_width >= img_width) && (window_height >= img_height))
  {
    // image fits in without resizing
    left = -1.0f + 0.5 * ((1.0 - static_cast<float>(img_width) / static_cast<float>(window_width)) * 2.0f);
    right = 1.0f - 0.5 * ((1.0 - static_cast<float>(img_width) / static_cast<float>(window_width)) * 2.0f);
    bottom = -1.0f + 0.5* ((1.0 - static_cast<float>(img_height) / static_cast<float>(window_height)) * 2.0f);
    top = 1.0f - 0.5* ((1.0 - static_cast<float>(img_height) / static_cast<float>(window_height)) * 2.0f);
    return true;
  }

  const float img_ratio = static_cast<float>(img_width) / static_cast<float>(img_height);
  const float wnd_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);

  if (img_ratio > wnd_ratio)
  {
    // width is limiting stuff here
    // so left and right are window border
    left = -1.0f;
    right = 1.0f;
    // get new actual height of image in pixels
    const float h = static_cast<float>(window_width) / img_ratio;
    bottom = -1.0f + 0.5 * ((1.0 - h / static_cast<float>(window_height)) * 2.0f);
    top = 1.0f - 0.5 * ((1.0 - h / static_cast<float>(window_height)) * 2.0f);
    return true;
  }
  else
  {
    // height is limiting stuff here
    // so top and bottom are clear
    top = 1.0f;
    bottom = -1.0f;
    // get new actual width of image in pixels
    const float w = window_height*img_ratio;
    left = -1.0f + 0.5 * ((1.0 - w / static_cast<float>(window_width)) * 2.0f);
    right = 1.0f - 0.5 * ((1.0 - w / static_cast<float>(window_width)) * 2.0f);
    return true;
  }
}

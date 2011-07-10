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

#ifndef PICDBV_GUI_H
#define PICDBV_GUI_H

#include <string>
#include <vector>
#include <set>
#include "../common/graphics/GLImageStructure.h"
#include "Configuration.h"
#include "GUITextPanel.h"

class GUI
{
  public:
    /* constructor */
    GUI();

    /* destructor */
    ~GUI();

    /* sets some initial stuff for the GL */
    void initGL();

    /* key wrapper that will be called by GLUT whenever a key is pressed */
    void keyWrapper(unsigned char Key, int x, int y);

    /* key wrapper that will be called by GLUT whenever a special key is pressed */
    void specialWrapper(int Key, int x, int y);

    /* draw wrapper that will be called whenever a window needs to be (re-)drawn */
    void drawWrapper(void);

    /* tries to set the given image to the image currently displayed. Returns
       true on success, false on failure.

       parameters:
           FileName  - full file name of the current image
           shortName - short file name
    */
    bool setCurrentImage(const std::string& FileName, const std::string& shortName);

    bool setCurrentImageByIndex(const int32_t idx);

    //writes out a text at the given location
    static void writeText(const std::string& text, const float x, const float y, const float z);

    //same as write text, but centers the text around x-axis
    void centerText(const std::string& text, const float y, const float z);

    void freeGLTexture();

    void getAllFiles();

    unsigned int getNumberOfFilesInList() const;

    void addPanel(GUITextPanel* panel);

    void removeAllPanels();

    Configuration config;
  private:
    //internal image structure
    GLImageStructure glis;
    //internal image name
    GLuint image_tex;

    void showNext();
    void showPrevious();

    void updateCenterTopPanel(const std::string& new_msg);

    void updateMultiLinePanel(const GUIAdjustedTextPanel::PanelAdjustment adj, const std::string& head, const std::set<std::string>& items);

    void updateInfoPanels(const std::set<std::string>& who, const std::set<std::string>& tag, const std::string& artist);

    std::vector<std::string> selectedFiles;
    int32_t currentIndex;

    std::vector<GUITextPanel*> m_Panels;

    //input "window"
    bool m_showInput;
    std::string m_InputLine;

    struct {
      bool isFullscreen;
      int window_width;
      int window_height;
      int pos_x, pos_y;
    } nonFullscreenData;

    /* draws all present panels */
    void drawPanels();

    /* draws the input line "window" */
    void drawInput();

    /* processes the input */
    void processInput();
};//class

bool getCorners(const int window_width, const int window_height, const int img_width,
                const int img_height, GLfloat& left, GLfloat&right, GLfloat& bottom,
                GLfloat& top);

#endif // PICDBV_GUI_H

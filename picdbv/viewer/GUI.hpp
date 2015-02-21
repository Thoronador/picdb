/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
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

#ifndef PICDBV_GUI_HPP
#define PICDBV_GUI_HPP

#include <string>
#include <vector>
#include <set>
#include "../../libthoro/common/graphics/GLImageStructure.h"
#ifdef USE_GLFW_GUI
  #include "../../libthoro/common/gui/glfwGUI.h"
#elif defined(USE_GLUT_GUI)
  #include "../../libthoro/common/gui/glutGUI.h"
#else
  #error No GUI defined!
#endif
#include "Configuration.hpp"
#include "GUITextPanel.hpp"

#ifdef USE_GLFW_GUI
class GUI: public glfwGUI
#else
class GUI: public glutGUI
#endif
{
  public:
    static GUI& getSingleton();

    /* destructor */
    ~GUI();

    /* sets some initial stuff for the GL */
    void initGL();

    /* function that will be called whenever a key is pressed

       parameters:
           key - the pressed key or generated character
    */
    virtual void keyPressed(int key);

    /* function that will be called whenever a special key (e.g. system key) is pressed

       parameters:
           key - the pressed key
    */
    //virtual void specialKeyPressed(int key);

    /* draw wrapper that will be called whenever a window needs to be (re-)drawn */
    virtual void draw(void);

    /* let's the GUI perform tasks that can/should (only) be done during idle
       time - currently that's just the hash update */
    void performIdleTasks();

    virtual void idle(void)
    {
      performIdleTasks();
    }

    /* tries to set the given image to the image currently displayed. Returns
       true on success, false on failure.

       parameters:
           FileName    - full file name of the current image
           shortName   - short file name
           index_first - one-based index of the image (for display purpose only)
           index_total - total number of files in index ( "   "       "      " )
    */
    bool setCurrentImage(const std::string& FileName, const std::string& shortName, const int32_t index_first=-1, const int32_t index_total=-1);

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
    /* constructor */
    GUI();

    /* copy constructor */
    GUI(const GUI& op) {}

    //internal image structure
    GLImageStructure glis;
    //internal image name
    GLuint image_tex;
    //internal texture target;
    GLenum m_CurrentTextureTarget;

    void showNext();
    void showPrevious();

    void updateCenterTopPanel(const std::string& new_msg);

    void updateMultiLinePanel(const GUIAdjustedTextPanel::PanelAdjustment adj, const std::string& head, const std::set<std::string>& items);

    void updateInfoPanels(const std::set<std::string>& who, const std::set<std::string>& tag, const std::string& artist);

    std::vector<std::string> selectedFiles;
    int32_t currentIndex;

    //holds all GUI panels
    std::vector<GUITextPanel*> m_Panels;

    //contains the files whose hash values should be updated during idle time
    std::set<std::string> m_IdleHashUpdateFiles;

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

#endif // PICDBV_GUI_HPP

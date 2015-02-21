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

#ifndef GUIMULTILINEPANEL_HPP
#define GUIMULTILINEPANEL_HPP

#include <set>
#include <string>
#include <vector>
#include "GUITextPanel.hpp"

class GUIMultiLinePanel : virtual public GUITextPanel
{
  public:
    /* constructor */
    GUIMultiLinePanel(const std::string& msg);

    /* alternative constructor */
    GUIMultiLinePanel(const std::string& msg, const float red, const float green, const float blue);

    /* destructor */
    ~GUIMultiLinePanel();

    /* sets the new string of the panel */
    virtual void setString(const std::string& new_string);

    //returns reference to the internal lines vector
    const std::vector<std::string>& getLines() const;

    /* appends the given lines to the already present ones

       parameters:
           moreLines - the vector of lines that have to be added
    */
    void addLines(const std::vector<std::string>& moreLines);

    /* appends the given lines to the already present ones

       parameters:
           moreLines - the set of lines that have to be added
    */
    void addLines(const std::set<std::string>& moreLines);

    /* clears all lines, effectively emptying the panel */
    void clearLines();
  protected:
    //lines of text in the panel
    std::vector<std::string> m_Lines;

    /* splits the string at new line characters and puts those splitted strings
       as new lines of the panel

       parameters:
           str1 - the new string
    */
    void explodeStringToLines(std::string str1);
};//class

class GUIMultiLineAdjustedPanel : public GUIMultiLinePanel, public GUIAdjustedTextPanel
{
  public:
    /* constructor */
    GUIMultiLineAdjustedPanel(const std::string& msg, const PanelAdjustment adj);

    /* alternative constructor */
    GUIMultiLineAdjustedPanel(const std::string& msg, const PanelAdjustment adj, const float red, const float green, const float blue);

    /* draws the panel's text */
    virtual void draw();
};//class

#endif // GUIMULTILINEPANEL_HPP

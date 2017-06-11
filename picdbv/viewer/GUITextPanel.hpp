/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011  Dirk Stolle

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

#ifndef GUITEXTPANEL_HPP
#define GUITEXTPANEL_HPP

#include <string>

//basic class for all other types of text panels
class GUITextPanel
{
  public:
    /* constructor */
    GUITextPanel(const std::string& msg);

    /* alternative constructor */
    GUITextPanel(const std::string& msg, const float red, const float green, const float blue);

    /* destructor */
    virtual ~GUITextPanel();

    void setColour(const float red, const float green, const float blue);

    float getRed() const;
    float getGreen() const;
    float getBlue() const;

    /* returns the panel's string */
    const std::string& getString() const;

    /* sets the new string of the panel */
    virtual void setString(const std::string& new_string);

    /* draws the panel's text */
    virtual void draw() = 0;
  protected:
    std::string m_string;
    float r, g, b;
}; //class


//panel with special adjustmend
class GUIAdjustedTextPanel : virtual public GUITextPanel
{
  public:
    enum PanelAdjustment {paCenterTop, paLeftTop, paRightTop, paCenterBottom,
                          paLeftBottom, paRightBottom, paCenter };

    /* constructor */
    GUIAdjustedTextPanel(const std::string& msg, const PanelAdjustment adj);

    /* alternative constructor */
    GUIAdjustedTextPanel(const std::string& msg, const PanelAdjustment adj, const float red, const float green, const float blue);

    /* destructor */
    ~GUIAdjustedTextPanel();

    void setAdjustment(const PanelAdjustment new_adjust);

    PanelAdjustment getAdjustment() const;

    /* draws the panel's text */
    virtual void draw();
  protected:
    PanelAdjustment m_adjust;
};//class

#endif // GUITEXTPANEL_HPP

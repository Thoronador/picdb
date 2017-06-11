/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2013  Dirk Stolle

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

#include "GUITextPanel.hpp"
#include "GUI.hpp"

GUITextPanel::GUITextPanel(const std::string& msg)
: m_string(msg),
  r(1.0f),
  g(1.0f),
  b(1.0f)
{

}

GUITextPanel::GUITextPanel(const std::string& msg, const float red, const float green, const float blue)
: m_string(msg),
  r(red),
  g(green),
  b(blue)
{

}

GUITextPanel::~GUITextPanel()
{
  //empty
}

const std::string& GUITextPanel::getString() const
{
  return m_string;
}

void GUITextPanel::setString(const std::string& new_string)
{
  m_string = new_string;
}

void GUITextPanel::setColour(const float red, const float green, const float blue)
{
  r = red;
  g = green;
  b = blue;
}

float GUITextPanel::getRed() const
{
  return r;
}

float GUITextPanel::getGreen() const
{
  return g;
}

float GUITextPanel::getBlue() const
{
  return b;
}

// adjustment panel's stuff

GUIAdjustedTextPanel::GUIAdjustedTextPanel(const std::string& msg, const PanelAdjustment adj)
: GUITextPanel(msg),
  m_adjust(adj)
{

}

GUIAdjustedTextPanel::GUIAdjustedTextPanel(const std::string& msg, const PanelAdjustment adj, const float red, const float green, const float blue)
: GUITextPanel(msg, red, green, blue),
  m_adjust(adj)
{

}

GUIAdjustedTextPanel::~GUIAdjustedTextPanel()
{
  // empty
}

void GUIAdjustedTextPanel::setAdjustment(const PanelAdjustment new_adjust)
{
  m_adjust = new_adjust;
}

GUIAdjustedTextPanel::PanelAdjustment GUIAdjustedTextPanel::getAdjustment() const
{
  return m_adjust;
}

void GUIAdjustedTextPanel::draw()
{
  const float dx = 2.0f/static_cast<float>(GUI::getSingleton().getWindowWidth());
  const float dy = 2.0f/ static_cast<float>(GUI::getSingleton().getWindowHeight());
  glColor3f(r, g, b);

  switch (m_adjust)
  {
    case paCenterTop:
         GUI::writeText(m_string, -dx*m_string.length()*4, 1.0-24*dy, 0.0f);
         break;
    case paLeftTop:
         GUI::writeText(m_string, -1.0, 1.0-24*dy, 0.0f);
         break;
    case paRightTop:
         GUI::writeText(m_string, 1.0-dx*m_string.length()*8, 1.0-24*dy, 0.0f);
         break;
    case paLeftBottom:
         GUI::writeText(m_string, -1.0, -1.0+8*dy, 0.0f);
         break;
    case paCenterBottom:
         GUI::writeText(m_string, -dx*m_string.length()*4, -1.0+8*dy, 0.0f);
         break;
    case paRightBottom:
         GUI::writeText(m_string, 1.0-dx*m_string.length()*8, -1.0+8*dy, 0.0f);
         break;
    case paCenter:
         GUI::writeText(m_string, -dx*m_string.length()*4, 0.0-8*dy, 0.0f);
         break;
  }//swi
}

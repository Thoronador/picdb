/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012  thoronador

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

#include "GUIMultiLinePanel.h"
#include "GUI.h"

GUIMultiLinePanel::GUIMultiLinePanel(const std::string& msg)
: GUITextPanel(msg)
{
  m_string = msg;
  setColour(1.0, 1.0, 1.0);
  explodeStringToLines(msg);
}

GUIMultiLinePanel::GUIMultiLinePanel(const std::string& msg, const float red, const float green, const float blue)
: GUITextPanel(msg, red, green, blue)
{
  m_string = msg;
  setColour(red, green, blue);
  explodeStringToLines(msg);
}

GUIMultiLinePanel::~GUIMultiLinePanel()
{
  m_Lines.clear();
}

void GUIMultiLinePanel::setString(const std::string& new_string)
{
  explodeStringToLines(new_string);
  m_string = new_string;
}

void GUIMultiLinePanel::explodeStringToLines(std::string str1)
{
  m_Lines.clear();
  if (str1.empty())
  {
    m_Lines.push_back("");
    return;
  }
  std::string::size_type nl_pos = str1.find('\n');
  while (nl_pos != std::string::npos)
  {
    m_Lines.push_back(str1.substr(0, nl_pos));
    str1 = str1.substr(nl_pos+1);
    nl_pos = str1.find('\n');
  }//while
  if (!str1.empty())
  {
    m_Lines.push_back(str1);
  }
  return;
}

const std::vector<std::string>& GUIMultiLinePanel::getLines() const
{
  return m_Lines;
}

void GUIMultiLinePanel::addLines(const std::vector<std::string>& moreLines)
{
  m_Lines.insert(m_Lines.end(), moreLines.begin(), moreLines.end());
  //add lines to internal single-string representation
  if (!moreLines.empty())
  {
    //first string
    if (m_string.empty())
    {
      m_string = moreLines.at(0);
    }
    else m_string.append("\n"+moreLines.at(0));
    //...and the rest
    unsigned int i;
    for (i=1; i<moreLines.size(); ++i)
    {
      m_string.append("\n"+moreLines.at(i));
    }
  }//moreLines not empty
}

void GUIMultiLinePanel::addLines(const std::set<std::string>& moreLines)
{
  if (!moreLines.empty())
  {
    const std::set<std::string>::const_iterator iter_end = moreLines.end();
    std::set<std::string>::const_iterator iter = moreLines.begin();
    while (iter!=iter_end)
    {
      //append stuff to m_string
      if (m_string.empty())
      {
        m_string = *iter;
      }
      else
      {
        m_string.append("\n"+ *iter);
      }
      //add it to the vector of lines
      m_Lines.push_back(*iter);
      ++iter;
    }//while

  }//moreLines not empty
}

void GUIMultiLinePanel::clearLines()
{
  m_string = "";
  m_Lines.clear();
}


GUIMultiLineAdjustedPanel::GUIMultiLineAdjustedPanel(const std::string& msg, const PanelAdjustment adj)
: GUITextPanel(msg), GUIMultiLinePanel(msg), GUIAdjustedTextPanel(msg, adj)
{
  m_string = msg;
  explodeStringToLines(msg);
  setColour(1.0, 1.0, 1.0);
  m_adjust = adj;
}

GUIMultiLineAdjustedPanel::GUIMultiLineAdjustedPanel(const std::string& msg, const PanelAdjustment adj, const float red, const float green, const float blue)
: GUITextPanel(msg, red, green, blue), GUIMultiLinePanel(msg, red, green, blue), GUIAdjustedTextPanel(msg, adj, red, green, blue)
{
  m_string = msg;
  explodeStringToLines(msg);
  setColour(red, green, blue);
  m_adjust = adj;
}

void GUIMultiLineAdjustedPanel::draw()
{
  const float dx = 2.0f/static_cast<float>(GUI::getSingleton().getWindowWidth());
  const float dy = 2.0f/ static_cast<float>(GUI::getSingleton().getWindowHeight());
  const unsigned int pixel_per_line = 16;
  glColor3f(r, g, b);
  //get length of line with most characters
  unsigned int longest = 0;
  unsigned int i;
  for (i=0; i<m_Lines.size(); ++i)
  {
    if (m_Lines[i].length()>longest)
      longest = m_Lines[i].length();
  }//for

  switch (m_adjust)
  {
    case paLeftTop:
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], -1.0, 1.0-24*dy-pixel_per_line*dy*i, 0.0f);
         }//for
         break;
    case paLeftBottom:
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], -1.0, -1.0+8*dy+dy*pixel_per_line*(m_Lines.size()-1-i), 0.0f);
         }//for
         break;
    case paCenterTop:
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], -dx*longest*4, 1.0-24*dy-pixel_per_line*dy*i, 0.0f);
         }//for
         break;
    case paCenterBottom:
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], -dx*longest*4, -1.0+8*dy+dy*pixel_per_line*(m_Lines.size()-1-i), 0.0f);
         }//for
         break;
    case paRightTop:
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], 1.0-dx*longest*8, 1.0-24*dy-pixel_per_line*dy*i, 0.0f);
         }//for
         break;
    case paRightBottom:
         GUI::writeText(m_string, 1.0-dx*m_string.length()*8, -1.0+8*dy, 0.0f);
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], 1.0-dx*longest*8, -1.0+8*dy+dy*pixel_per_line*(m_Lines.size()-1-i), 0.0f);
         }//for
         break;
    case paCenter:
         for (i=0; i<m_Lines.size(); ++i)
         {
           GUI::writeText(m_Lines[i], -dx*longest*4, 0.0+0.5*pixel_per_line*dy /* top of box */
                                                    -pixel_per_line*dy*(i+1),
                                                    0.0f);
         }//for
         break;
  }//swi
}

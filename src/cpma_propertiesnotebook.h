// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef CPMAPROPERTIESNOTEBOOK_H
#define CPMAPROPERTIESNOTEBOOK_H

#include "propertiesnotebookbase.h"


class CPMAVisibilityPropertiesCtrl;
class FontPropertiesCtrl;
class ColorPropertiesCtrl;
class MiscPropertiesCtrl;
class ImagePropertiesCtrl;

class CPMAPropertiesNotebook : public PropertiesNotebookBase
{
  public:
    CPMAPropertiesNotebook( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    CPMAVisibilityPropertiesCtrl  *m_vis;
    FontPropertiesCtrl      *m_font;
    ColorPropertiesCtrl     *m_color;
    ImagePropertiesCtrl     *m_image;
    MiscPropertiesCtrl        *m_misc;

};

#endif

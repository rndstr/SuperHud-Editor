// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#include "q4max_displayctrl.h"

#include "common.h"
#include "pakmanager.h"
#include "model.h"
#include "hudfilebase.h"
#include "prefs.h"
#include "elementsctrlbase.h"
#include "font.h"
#include "texture.h"
#include "model.h"

#include <algorithm>

Q4MAXDisplayCtrl::Q4MAXDisplayCtrl( wxWindow *parent ) :
  DisplayCtrlBase(parent)
{
  
}

Q4MAXDisplayCtrl::~Q4MAXDisplayCtrl()
{
  cleanup();
}

void Q4MAXDisplayCtrl::init()
{
  DisplayCtrlBase::init();

  // set up fonts
  /*
  m_fonts[wxT("cpma")] = new Q4MAXFont(wxT("cpma"));
  m_fonts[wxT("id")] = new Q4MAXFont(wxT("id"));
  m_fonts[wxT("threewave")] = new Q4MAXFont(wxT("threewave"));
  m_fonts[wxT("idblock")] = new Q4MAXFont(wxT("idblock"));
  m_fonts[wxT("sansman")] = new Q4MAXFont(wxT("sansman"));
  */

  // we could enumerate during startup but initialization order is not working out
  /*
  pakbrowser_dirs_type dirs;
  pakbrowser_files_type files;
  PakManager::get().enumerate_pakdircontents(wxT("/fonts/"), &dirs, &files);
  for( pakbrowser_files_type::const_iterator it = files.begin(); it != files.end(); ++it )
  {
    if( it->first.Matches(wxT("*.tga")) )
    {
      wxString name = it->first.SubString(0, it->first.length()-5);
      if( m_fonts.find(name) == m_fonts.end() )
        m_fonts[name] = new Q4MAXFont(name);
    }
  }
  */

  // load fonts
  for( fonts_type::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it )
    if( !it->second->load() )
      wxLogError( _("Cannot find font: ") + it->first + wxT("\n\n") + 
      _("Either the game directory is wrong or you have an outdated Q4MAX version installed,\nmake sure you got at least 1.44") );

  PakManager::get().debug();
}


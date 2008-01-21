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

#include "cpma_hudfile.h"
#include "common.h"
#include "elementsctrlbase.h"
#include "pakmanager.h"


#include "cpma_element.h"

#include <wx/txtstrm.h>
#include <wx/datetime.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/tokenzr.h>


//#include <wx/stdpaths.h>
//#include <wx/dir.h>
//#include <algorithm>


CPMAHudFile::CPMAHudFile() : 
  HudFileBase()
{
}


bool CPMAHudFile::save( const wxString& filename )
{
  int decoratecount = 0;
  for( cit_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( (*it)->name() == HF_POSTDECORATE_PREFIX || (*it)->name() == HF_PREDECORATE_PREFIX )
      ++decoratecount;
  }
  if( decoratecount > CPMA_HF_MAX_PREPOSTDECORATE )
    wxLogWarning( _("You have more than %d combined PreDecorate/PostDecorate elements which CPMA does not support"), CPMA_HF_MAX_PREPOSTDECORATE );
  return HudFileBase::save(filename);
}

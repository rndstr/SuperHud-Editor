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
#ifndef Q4MAXHUDFILE_H
#define Q4MAXHUDFILE_H

#include "hudfilebase.h"
#include "q4max_element.h"

// -- parsing

#ifdef BACKWARD_COMPATIBILITY_142
  const wxString HF_POSTDECORATE_PREFIX_142 = wxT("_PostDecorate");
  const wxString HF_PREDECORATE_PREFIX_142 = wxT("!PreDecorate");
#endif

class Q4MAXHudFile : public HudFileBase
{
  public:
    Q4MAXHudFile();
    virtual ~Q4MAXHudFile() {}

    wxString default_hudfilename() const { return wxT("hud/hud.cfg"); }

  protected:
    bool          read_properties( ElementBase *hi, const wxString& props );
    ElementBase*  create_element( const wxString& name ) const { return new Q4MAXElement(name); }
    ElementBase*  create_element( const hsitem_s& defit ) const { return new Q4MAXElement(defit); }
};


#endif

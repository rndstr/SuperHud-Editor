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
#ifndef CPMAHUDFILE_H
#define CPMAHUDFILE_H

#include "hudfilebase.h"
#include "cpma_element.h"



/// maximuum number of pre and postdecorates
const int CPMA_HF_MAX_PREPOSTDECORATE = 64;

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile();
    virtual ~CPMAHudFile() {}

    bool save( const wxString& filename );
    wxString default_hudfilename() const { return wxT("hud/hud.cfg"); }

  protected:
    ElementBase*  create_element( const wxString& name ) const { return new CPMAElement(name); }
    ElementBase*  create_element( const hsitem_s& defit ) const { return new CPMAElement(defit); }
};


#endif

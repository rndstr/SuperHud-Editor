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

#include  "../hudfilebase.h"

#define BACKWARD_COMPATIBILITY_142

/// Used for algos trying to iterate through all PostDecorate elements.
const wxString HF_POSTDECORATE_PREFIX = wxT("PostDecorate");
/// Used for algos trying to iterate through all PreDecorate elements.
const wxString HF_PREDECORATE_PREFIX = wxT("PreDecorate");
/// maximuum number of pre and postdecorates
const int HF_MAX_PREPOSTDECORATE = 64;

// -- parsing


/// possible property(command) delimiters: newline or semicolon.
const wxString HF_PROPERTY_DELIM = wxT("\n;");
/// what we trim for.
const wxString HF_PROPERTY_TRIM = wxT(" \n\t\r");

#ifdef BACKWARD_COMPATIBILITY_142
  const wxString HF_POSTDECORATE_PREFIX_142 = wxT("_PostDecorate");
  const wxString HF_PREDECORATE_PREFIX_142 = wxT("!PreDecorate");
#endif

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile();
    virtual ~CPMAHudFile() {}


    void load_default_elements();
    bool save( const wxString& filename );
    wxString default_hudfilename() const { return wxT("hud/hud.cfg"); }

    const notuniqs_type& notuniq_elements() const;
    ElementBase*  create_element( const wxString& name ) const;

    bool parse_item( wxString s );

  private:
    
    bool read_properties( ElementBase *hi, const wxString& props );
};


#endif

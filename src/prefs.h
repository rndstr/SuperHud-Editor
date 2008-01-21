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
#ifndef PREFS_H
#define PREFS_H

#include "common.h"

#include "varcontainer.h"
#include "color4.h"

#include <wx/string.h>


class Prefs : public varcont_type
{
    
  public:
    
    /// initializes the config object
    /// @returns bool True if the configfile didn't yet exist aka "first start" otherwise false
    bool init();
    void cleanup();

    void load();
    /// stores all preferences in config file
    void save();
    void read_var( var_type& var );
    void write_var( const var_type& var ) const;


    /// returns the initial path to be used in a filedialog for the current game
    wxString filedialog_path() const;

  // singleton
  public:
    static Prefs& get();
  protected:
    Prefs() {}
    Prefs(const Prefs&);
    Prefs& operator= (const Prefs&);
    virtual ~Prefs() {}
};

#endif // PREFS_H


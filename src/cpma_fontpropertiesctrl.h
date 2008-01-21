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
#ifndef CPMA_FONTPROPERTIESCTRL_H
#define CPMA_FONTPROPERTIESCTRL_H

#include "cpma_propertygrid.h"

class ElementBase;

class FontPropertiesCtrl : public CPMAPropertyGrid
{
  public:
    FontPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    /// @arg reset Whether to reset the items (delete&append)
    void update_layout( bool reset = true );

    void ExpandAll( bool expand = true );

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    static wxString textalign_element_to_ui( const wxChar& ta );
    static wxChar textalign_ui_to_element( const wxString& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif

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
#ifndef CPMAPROPERTYGRID_H
#define CPMAPROPERTYGRID_H

#include "../propertygridbase.h"


/// Provides a few functions that are in use on all
/// the various grids
class CPMAPropertyGrid : public PropertyGridBase
{
  public:
    CPMAPropertyGridBase( 
      wxWindow *parent, 
      wxWindowID id = wxID_ANY, 
		  const wxPoint& pos = wxDefaultPosition, 
		  const wxSize& size = wxDefaultSize, 
		  long style = wxPGMAN_DEFAULT_STYLE, 
		  const wxChar* name = wxPropertyGridManagerNameStr
      ) : PropertyGridBase(parent, id, pos, size, style, name)
    {
    }


  protected:
    /// returns the currently selected element (as stored in CPMAPropertiesNotebook::update_from_element)
    /// @returns The current element or 0 if none selected
    const CPMAElement* current_element() const
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - PropertiesNotebook is not yet available"));
        return 0;
      }
      const CPMAElement* el = static_cast<const CPMAElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - there is no element selected"));
        return 0;
      }
      return el;
    }
    CPMAElement* current_element()
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - PropertiesNotebook is not yet available"));
        return 0;
      }
      CPMAElement *el = static_cast<CPMAElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - there is no element selected"));
        return 0;
      }
      return el;
    }
};

#endif

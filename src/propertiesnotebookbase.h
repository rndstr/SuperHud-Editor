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
#ifndef PROPERTIESNOTEBOOKBASE_H
#define PROPERTIESNOTEBOOKBASE_H

#include "common.h"

#include "superhudeditor.h"
#include "hudfilebase.h"
#include <wx/aui/auibook.h>

class ElementBase;

class PropertiesNotebookBase : public wxAuiNotebook
{
  public:
    PropertiesNotebookBase( wxWindow* parent ) :
      wxAuiNotebook( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, wxDefaultSize, 
        wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TOP | wxAUI_NB_SCROLL_BUTTONS ),
        m_curel(0)
    {
    }

    virtual void update_from_element( const elements_type& els ) = 0;

    ElementBase*       curel() { return m_curel; }
    const ElementBase* curel() const { return m_curel; }

  protected:
    ElementBase   *m_curel;///< element that is currently being displayed



};

#endif

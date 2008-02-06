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
#include "q4max_propertiesnotebook.h"

#include "q4max_visibilitypropertiesctrl.h"
#include "q4max_textpropertiesctrl.h"
#include "prefs.h"

#include "q4max_element.h"
#include <wx/propgrid/propgrid.h>



Q4MAXPropertiesNotebook::Q4MAXPropertiesNotebook( wxWindow *parent ) : 
  PropertiesNotebookBase(parent)
{
  wxPropertyGrid::SetBoolChoices(_("On"), _("Off"));

  wxColour col = Prefs::get().var(wxT("props_color")).wxcval();
  wxColour bgcol = Prefs::get().var(wxT("props_bgcolor")).wxcval();
  wxColour icol = Prefs::get().var(wxT("props_inheritcolor")).wxcval();
  wxColour ibgcol = Prefs::get().var(wxT("props_inheritbgcolor")).wxcval();

  m_vis = new Q4MAXVisibilityPropertiesCtrl(this);
  m_text = new Q4MAXTextPropertiesCtrl(this);
  m_vis->set_colors(icol, ibgcol, col, bgcol);
  m_text->set_colors(icol, ibgcol, col, bgcol);
  AddPage( m_vis, _("Visibility") );
  AddPage( m_text, _("Text") );

  // some initial size so it isn't smashed without an existing perspective
  SetSize(300, -1);
}

void Q4MAXPropertiesNotebook::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  { // no properties selection
    
    // CollapseAll/ExpandAll trigger EVT_PG_CHANGING/CHANGED if we have been editing
    // a value that wasn't yet saved. so we have to make sure that we set
    // m_curel _afterwards_*

    m_vis->CollapseAll();
    m_text->CollapseAll();

    m_vis->ClearSelection();
    m_text->ClearSelection();

    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_ENABLE, false );
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_DISABLE, false );
    Disable();
    m_curel = 0; // *
  }
  else
  {
    Enable();

    m_vis->ExpandAll();
    m_text->ExpandAll();

    m_curel = els.front();

    m_vis->from_element(m_curel);
    m_text->from_element(m_curel);

    m_vis->ClearModifiedStatus();
    m_text->ClearModifiedStatus();
  }
}


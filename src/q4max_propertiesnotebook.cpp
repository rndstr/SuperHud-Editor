#include "q4max_propertiesnotebook.h"

#include "q4max_visibilitypropertiesctrl.h"
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
  m_vis->set_colors(icol, ibgcol, col, bgcol);
  AddPage( m_vis, _("Visibility") );

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
    m_vis->ClearSelection();
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_ENABLE, false );
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_DISABLE, false );
    Disable();
    m_curel = 0; // *
  }
  else
  {
    Enable();

    m_vis->ExpandAll();

    m_curel = els.front();

    m_vis->from_element(m_curel);
    m_vis->ClearModifiedStatus();
  }
}


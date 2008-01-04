#include "propertiesnotebook.h"

#include "visibilitypropertiesctrl.h"
#include "fontpropertiesctrl.h"
#include "colorpropertiesctrl.h"
#include "imagepropertiesctrl.h"
#include "miscpropertiesctrl.h"
#include "../prefs.h"

#include "element.h"
#include <wx/propgrid/propgrid.h>



CPMAPropertiesNotebook::CPMAPropertiesNotebook( wxWindow *parent ) : 
  PropertiesNotebookBase(parent)
{
  wxPropertyGrid::SetBoolChoices(_("On"), _("Off"));

  wxColour col = Prefs::get().var(wxT("props_color")).wxcolorval();
  wxColour bgcol = Prefs::get().var(wxT("props_bgcolor")).wxcolorval();
  wxColour icol = Prefs::get().var(wxT("props_inheritcolor")).wxcolorval();
  wxColour ibgcol = Prefs::get().var(wxT("props_inheritbgcolor")).wxcolorval();

  m_vis = new VisibilityPropertiesCtrl(this);
  m_font = new FontPropertiesCtrl(this);
  m_color = new ColorPropertiesCtrl(this);
  m_image = new ImagePropertiesCtrl(this);
  m_misc = new MiscPropertiesCtrl(this);
  m_vis->set_colors(icol, ibgcol, col, bgcol);
  m_font->set_colors(icol, ibgcol, col, bgcol);
  m_color->set_colors(icol, ibgcol, col, bgcol);
  m_image->set_colors(icol, ibgcol, col, bgcol);
  m_misc->set_colors(icol, ibgcol, col, bgcol);

  AddPage( m_vis, _("Visibility") );
  AddPage( m_font, _("Font") );
  AddPage( m_color, _("Color") );
  AddPage( m_image, _("Image") );
  AddPage( m_misc, _("Misc") );
  // some initial size so it isn't smashed without an existing perspective
  SetSize(300, -1);
}

void CPMAPropertiesNotebook::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  { // no properties selection
    
    // CollapseAll/ExpandAll trigger EVT_PG_CHANGING/CHANGED if we have been editing
    // a value that wasn't yet saved. so we have to make sure that we set
    // m_curel _afterwards_
    /*
    m_vis->CollapseAll();
    m_font->CollapseAll();
    m_color->CollapseAll();
    m_image->CollapseAll();
    m_misc->CollapseAll();
    */
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_ENABLE, false );
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_DISABLE, false );
    Disable();
    m_curel = 0;
  }
  else
  {
    Enable();
    /*
    m_vis->ExpandAll();
    m_font->ExpandAll();
    m_color->ExpandAll();
    m_image->ExpandAll();
    m_misc->ExpandAll();
    */
    m_curel = els.front();

    m_vis->from_element(m_curel);
    m_font->from_element(m_curel);
    m_color->from_element(m_curel);
    m_image->from_element(m_curel);
    m_misc->from_element(m_curel);

    m_vis->ClearModifiedStatus();
    m_font->ClearModifiedStatus();
    m_color->ClearModifiedStatus();
    m_image->ClearModifiedStatus();
    m_misc->ClearModifiedStatus();
  }
}





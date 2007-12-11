#include "propertiesctrl.h"

#include "../visibilitypropertiesctrl.h"
#include "../fontpropertiesctrl.h"
#include "../colorpropertiesctrl.h"
#include "../miscpropertiesctrl.h"

#include "element.h"
#include <wx/propgrid/propgrid.h>



CPMAPropertiesCtrl::CPMAPropertiesCtrl( wxWindow *parent ) : 
  PropertiesCtrlBase(parent)
{
  m_vis = new VisibilityPropertiesCtrl(this);
  m_font = new FontPropertiesCtrl(this);
  m_color = new ColorPropertiesCtrl(this);
  m_misc = new MiscPropertiesCtrl(this);
  AddPage( m_vis, _("Visibility") );
  AddPage( m_font, _("Font") );
  AddPage( m_color, _("Color") );
  AddPage( m_misc, _("Misc") );
}

void CPMAPropertiesCtrl::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  { // no properties selection
    m_curel = 0;
    m_vis->CollapseAll();
    m_font->CollapseAll();
    m_color->CollapseAll();
    m_misc->CollapseAll();
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_ENABLE, false );
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_DISABLE, false );
    Disable();
  }
  else
  {
    m_curel = els.front();
    Enable();
    m_vis->ExpandAll();
    m_vis->from_element(m_curel);
    m_font->ExpandAll();
    m_font->from_element(m_curel);
    m_color->ExpandAll();
    m_color->from_element(m_curel);
    m_misc->ExpandAll();
    m_misc->from_element(m_curel);
  }
}





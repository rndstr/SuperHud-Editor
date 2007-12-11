#include "propertiesctrl.h"

#include "../positionpropertiesctrl.h"
#include "../fontpropertiesctrl.h"
#include "../colorpropertiesctrl.h"

#include "element.h"
#include <wx/propgrid/propgrid.h>



CPMAPropertiesCtrl::CPMAPropertiesCtrl( wxWindow *parent ) : 
  PropertiesCtrlBase(parent)
{
  m_pos = new PositionPropertiesCtrl(this);
  m_font = new FontPropertiesCtrl(this);
  m_color = new ColorPropertiesCtrl(this);
  AddPage( m_pos, _("Position") );
  AddPage( m_font, _("Font") );
  AddPage( m_color, _("Color") );
}

void CPMAPropertiesCtrl::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  { // no properties selection
    m_curel = 0;
    m_pos->CollapseAll();
    m_font->CollapseAll();
    m_color->CollapseAll();
    m_pos->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_ENABLE, false );
    m_pos->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_DISABLE, false );
    Disable();
  }
  else
  {
    m_curel = els.front();
    Enable();
    m_pos->ExpandAll();
    m_pos->from_element(m_curel);
    m_font->ExpandAll();
    m_font->from_element(m_curel);
    m_color->ExpandAll();
    m_color->from_element(m_curel);
  }
}




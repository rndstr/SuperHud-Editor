#include "propertiesctrl.h"

#include "../positionpropertiesctrl.h"
#include "../fontpropertiesctrl.h"

#include "element.h"
#include <wx/propgrid/propgrid.h>



CPMAPropertiesCtrl::CPMAPropertiesCtrl( wxWindow *parent ) : 
  PropertiesCtrlBase(parent)
{
  m_pos = new PositionPropertiesCtrl(this);
  m_font = new FontPropertiesCtrl(this);
  AddPage( m_pos, _("Position/Size") );
  AddPage( m_font, _("Font") );
}

void CPMAPropertiesCtrl::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  {
    m_curel = 0;
    m_pos->CollapseAll();
    m_font->CollapseAll();
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
  }
}




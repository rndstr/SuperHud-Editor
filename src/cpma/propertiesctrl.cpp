#include "propertiesctrl.h"

#include "../PositionPropertiesCtrl.h"

#include "element.h"
#include <wx/propgrid/propgrid.h>



CPMAPropertiesCtrl::CPMAPropertiesCtrl( wxWindow *parent ) : 
  PropertiesCtrlBase(parent)
{
  m_pos = new PositionPropertiesCtrl(this);
  AddPage( m_pos, _("Position/Size") );
}

void CPMAPropertiesCtrl::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  {
    m_curel = 0;
    m_pos->CollapseAll();
    Disable();
  }
  else
  {
    m_curel = els.front();
    Enable();
    m_pos->ExpandAll();
    m_pos->from_element(m_curel);
    // FIXME change wxAuiNotebook window title
    // this should happen the following way:
    // - OnSelectionChanged should be propagated to MainFrame
    // - m_mgr.GetPane(), then change wxAuiPaneInfo
    // - call MainFrame::DoUpdate
    /*
    bool ov = m_curel->m_has & E_HAS_RECT;
    m_pos->SetPropertyValue( wxT("overwrite-rect"), ov );
    m_pos->SetPropertyValue( wxT("X"), m_curel->m_rect.GetX() );
    m_pos->SetPropertyValue( wxT("Y"), m_curel->m_rect.GetY() );
    m_pos->SetPropertyValue( wxT("Width"), m_curel->m_rect.GetWidth() );
    m_pos->SetPropertyValue( wxT("Height"), m_curel->m_rect.GetHeight() );
    */
  }
}




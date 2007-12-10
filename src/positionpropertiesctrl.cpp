#include "positionpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "propertiesctrlbase.h"

BEGIN_EVENT_TABLE(PositionPropertiesCtrl, wxPropertyGridManager)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, PositionPropertiesCtrl::OnItemChanged)
END_EVENT_TABLE()

PositionPropertiesCtrl::PositionPropertiesCtrl( wxWindow *parent ) :
  wxPropertyGridManager( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Position/Size"));
  Append( wxBoolProperty( _("Overwrite"), wxT("overwrite-rect"), false) );

  Append( wxPropertyCategory( _("Position")) );
  SetPropertyAttribute(wxT("overwrite-rect"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("overwrite-rect"), _("By default an element is drawn at position (0,0) with width 64 and height 32. Check this box to specify your own values.") );

  Append( wxIntProperty( wxT("X"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("X"), _("This sets where the element is drawn, how many pixels from left") );
  SetPropertyEditor(wxT("X"),wxPG_EDITOR(TextCtrl));

  Append( wxIntProperty( _("Y"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("Y"), _("This sets where the element is drawn, how many pixels from top") );

  Append( wxPropertyCategory( _("Size")) );
  Append( wxIntProperty( _("Width"), wxPG_LABEL, 64) );

  Append( wxIntProperty( _("Height"), wxPG_LABEL, 32) );

  SetTargetPage(0);
}


void PositionPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("PositionPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();
  wxString name = ev.GetPropertyName();
  if( name == wxT("overwrite-rect") )
  {
    if( ev.GetPropertyValueAsBool() )
      el->m_has |= E_HAS_RECT;
    else
      el->m_has &= ~E_HAS_RECT;

    update_layout();
  }
  else if( name == wxT("X") || name == wxT("Y") || name == wxT("Height") || name == wxT("Width") )
  {
    if( !(el->m_has & E_HAS_RECT) )
    { // user was starting to edit while seeing the inherited values, copy them over
      el->m_rect = el->iget_rect();
      SetPropertyValue( wxT("overwrite-rect"), true );
      el->m_has |= E_HAS_RECT;
    }
    if( name == wxT("X") )
      el->m_rect.x = ev.GetPropertyValueAsInt();
    else if( name == wxT("Y") )
      el->m_rect.y = ev.GetPropertyValueAsInt();
    else if( name == wxT("Width") )
      el->m_rect.width = ev.GetPropertyValueAsInt();
    else if( name == wxT("Height") )
      el->m_rect.height = ev.GetPropertyValueAsInt();
    update_layout();
  }
  else
    return; // nothing changed

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}
void PositionPropertiesCtrl::from_element( ElementBase *el )
{
  SetPropertyValue( wxT("overwrite-rect"), el->has() & E_HAS_RECT );
  SetPropertyValue( wxT("X"), el->m_rect.GetX() );
  SetPropertyValue( wxT("Y"), el->m_rect.GetY() );
  SetPropertyValue( wxT("Width"), el->m_rect.GetWidth() );
  SetPropertyValue( wxT("Height"), el->m_rect.GetHeight() );
  update_layout();
}

void PositionPropertiesCtrl::update_layout()
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("PositionPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();

  bool ov = GetPropertyValueAsBool( wxT("overwrite-rect") );
  
  wxRect r;
  if( !ov )
  {
    r = el->iget_rect();
    SetPropertyTextColour( wxT("X"), PROPS_COLOR_INHERITED );
    SetPropertyTextColour( wxT("Y"), PROPS_COLOR_INHERITED );
    SetPropertyTextColour( wxT("Width"), PROPS_COLOR_INHERITED );
    SetPropertyTextColour( wxT("Height"), PROPS_COLOR_INHERITED );
  }
  else
  {
    r = el->rect();
    SetPropertyTextColour( wxT("X"), PROPS_COLOR_NORMAL );
    SetPropertyTextColour( wxT("Y"), PROPS_COLOR_NORMAL );
    SetPropertyTextColour( wxT("Width"), PROPS_COLOR_NORMAL );
    SetPropertyTextColour( wxT("Height"), PROPS_COLOR_NORMAL );
  }
  SetPropertyValue( wxT("X"), r.x );
  SetPropertyValue( wxT("Y"), r.y );
  SetPropertyValue( wxT("Width"), r.width );
  SetPropertyValue( wxT("Height"), r.height );
}

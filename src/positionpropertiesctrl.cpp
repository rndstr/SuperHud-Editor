#include "positionpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "elementsctrlbase.h"
#include "propertiesctrlbase.h"

BEGIN_EVENT_TABLE(PositionPropertiesCtrl, wxPropertyGridManager)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, PositionPropertiesCtrl::OnItemChanged)
  EVT_TOOL_RANGE(ID_BTN_ELEMENT_ENABLE, ID_BTN_ELEMENT_DISABLE,
    PositionPropertiesCtrl::OnElementVisibility)
END_EVENT_TABLE()

PositionPropertiesCtrl::PositionPropertiesCtrl( wxWindow *parent ) :
  wxPropertyGridManager( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPG_TOOLBAR|wxPGMAN_DEFAULT_STYLE )
{
  SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Position/Size"));

  Append( wxBoolProperty( _("Use"), wxT("overwrite-rect"), false) );
  SetPropertyAttribute(wxT("overwrite-rect"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("overwrite-rect"), _("By default an element is drawn at position (0,0) with width 64 and height 32. Check this box to specify your own values.") );

  Append( wxPropertyCategory( _("Position")) );

  Append( wxIntProperty( wxT("X"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("X"), _("This sets where the element is drawn, how many pixels from left") );
  SetPropertyEditor(wxT("X"),wxPG_EDITOR(TextCtrl));

  Append( wxIntProperty( _("Y"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("Y"), _("This sets where the element is drawn, how many pixels from top") );

  Append( wxPropertyCategory( _("Size")) );
  Append( wxIntProperty( _("Width"), wxPG_LABEL, 64) );

  Append( wxIntProperty( _("Height"), wxPG_LABEL, 32) );
  wxToolBar *tb = GetToolBar();
  // delete all
  while( tb->GetToolsCount() )
    tb->DeleteToolByPos(0);
  tb->AddRadioTool( ID_BTN_ELEMENT_ENABLE, _("Enable element"), 
    wxArtProvider::GetBitmap(ART_ELEMENT_ENABLED, wxART_TOOLBAR, wxSize(16,16)),
    wxArtProvider::GetBitmap(ART_ELEMENT_ENABLED, wxART_TOOLBAR, wxSize(16,16)),
    _("Enable element")
    );
  tb->AddRadioTool( ID_BTN_ELEMENT_DISABLE, _("Disable element"), 
    wxArtProvider::GetBitmap(ART_ELEMENT_DISABLED, wxART_TOOLBAR, wxSize(16,16)), 
    wxArtProvider::GetBitmap(ART_ELEMENT_DISABLED, wxART_TOOLBAR, wxSize(16,16)),
    _("Disable element")
    );
  tb->Realize();


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

  wxToolBar *tb = GetToolBar();
  tb->ToggleTool( (el->is_enabled() ? ID_BTN_ELEMENT_ENABLE : ID_BTN_ELEMENT_DISABLE), true );
  tb->Enable( !(el->flags() & E_ENABLEALWAYS) );
    
  /*
  SetPropertyValue( wxT("X"), el->m_rect.GetX() );
  SetPropertyValue( wxT("Y"), el->m_rect.GetY() );
  SetPropertyValue( wxT("Width"), el->m_rect.GetWidth() );
  SetPropertyValue( wxT("Height"), el->m_rect.GetHeight() );
  */
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
    SetPropertyColour( wxT("X"), PROPS_BGCOLOR_INHERITED );
    SetPropertyTextColour( wxT("Y"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("Y"), PROPS_BGCOLOR_INHERITED );
    SetPropertyTextColour( wxT("Width"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("Width"), PROPS_BGCOLOR_INHERITED );
    SetPropertyTextColour( wxT("Height"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("Height"), PROPS_BGCOLOR_INHERITED );
  }
  else
  {
    r = el->rect();
    SetPropertyTextColour( wxT("X"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("X"), PROPS_BGCOLOR_NORMAL );
    SetPropertyTextColour( wxT("Y"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("Y"), PROPS_BGCOLOR_NORMAL );
    SetPropertyTextColour( wxT("Width"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("Width"), PROPS_BGCOLOR_NORMAL );
    SetPropertyTextColour( wxT("Height"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("Height"), PROPS_BGCOLOR_NORMAL );
  }
  SetPropertyValue( wxT("X"), r.x );
  SetPropertyValue( wxT("Y"), r.y );
  SetPropertyValue( wxT("Width"), r.width );
  SetPropertyValue( wxT("Height"), r.height );
}

void PositionPropertiesCtrl::OnElementVisibility( wxCommandEvent& ev )
{
  if( ev.GetId() != ID_BTN_ELEMENT_ENABLE && ev.GetId() != ID_BTN_ELEMENT_DISABLE )
    return;
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("PositionPropertiesCtrl::OnElementVisibility() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();
  el->set_enabled( ev.GetId() == ID_BTN_ELEMENT_ENABLE );

  wxGetApp().mainframe()->elementsctrl()->elementslistctrl()->update_item(el);
}


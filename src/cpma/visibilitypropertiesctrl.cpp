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
#include "visibilitypropertiesctrl.h"

#include "../common.h"
#include "../mainframe.h"
#include "../hudfilebase.h"
#include "../elementsctrlbase.h"
#include "../propertiesnotebookbase.h"
#include "../artprovider.h"
#include "element.h"



BEGIN_EVENT_TABLE(VisibilityPropertiesCtrl, CPMAPropertyGridBase)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, VisibilityPropertiesCtrl::OnItemChanged)
  EVT_TOOL_RANGE(ID_BTN_ALIGN_LEFT, ID_BTN_ALIGN_BOTTOM,
    VisibilityPropertiesCtrl::OnAlign)
  EVT_TOOL_RANGE(ID_BTN_ELEMENT_ENABLE, ID_BTN_ELEMENT_DISABLE,
    VisibilityPropertiesCtrl::OnElementVisibility)
END_EVENT_TABLE()

VisibilityPropertiesCtrl::VisibilityPropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGridBase( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPG_TOOLBAR|wxPGMAN_DEFAULT_STYLE )
{
  // needed for `time' (duration) to give the user possibility to revert back to inherital value
  // TODO we could also add a [x] button that goes back to inherited!
  SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES); 

  AddPage(_("Visibility"));

  // a value of 0 (=disable) isn't the same as unspecified (=inherit)
  Append( new wxIntProperty( _("Duration"), wxT("time"), 0) );
  SetPropertyEditor(wxT("time"),wxPG_EDITOR(SpinCtrl));
  SetPropertyHelpString( wxT("time"), _("How long the element will be displayed for if it doesn't update again. Generally used for item pickups, frag messages, chat, etc.\n\nClear to disable.") );


  Append( new wxPropertyCategory( _("Rectangle"), wxT("cat-rect")) );

  Append( new wxBoolProperty( _("Use"), wxT("overwrite-rect"), false) );
  SetPropertyHelpString( wxT("overwrite-rect"), _("By default an element is drawn at position (0,0) with width 64 and height 32. Check this box to specify your own values.") );

  Append( new wxIntProperty( wxT("X"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("X"), _("This sets where the element is drawn, how many pixels from left") );
  SetPropertyEditor(wxT("X"),wxPG_EDITOR(SpinCtrl));

  Append( new wxIntProperty( wxT("Y"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("Y"), _("This sets where the element is drawn, how many pixels from top") );
  SetPropertyEditor(wxT("Y"),wxPG_EDITOR(SpinCtrl));

  Append( new wxIntProperty( _("Width"), wxT("width"), 64) );
  SetPropertyEditor(wxT("width"),wxPG_EDITOR(SpinCtrl));

  Append( new wxIntProperty( _("Height"), wxT("height"), 32) );
  SetPropertyEditor(wxT("height"),wxPG_EDITOR(SpinCtrl));

  SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);

  wxToolBar *tb = GetToolBar();
  // we ain't interested in the default buttons :o
  while( tb->GetToolsCount() )
    tb->DeleteToolByPos(0);
  tb->AddRadioTool( ID_BTN_ELEMENT_ENABLE, _("Enable element"), wxArtProvider::GetBitmap(ART_ELEMENT_ENABLED, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ELEMENT_ENABLE, _("Enable element") );
  tb->AddRadioTool( ID_BTN_ELEMENT_DISABLE, _("Disable element"), wxArtProvider::GetBitmap(ART_ELEMENT_DISABLED, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ELEMENT_DISABLE, _("Disable element") );
  tb->AddSeparator();
  tb->AddTool( ID_BTN_ALIGN_LEFT, _("Align element left"), wxArtProvider::GetBitmap(ART_ALIGN_LEFT, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_LEFT, _("Align element left") );
  tb->AddTool( ID_BTN_ALIGN_CENTER, _("Align element center"), wxArtProvider::GetBitmap(ART_ALIGN_CENTER, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_CENTER, _("Align element center") );
  tb->AddTool( ID_BTN_ALIGN_RIGHT, _("Align element right"), wxArtProvider::GetBitmap(ART_ALIGN_RIGHT, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_RIGHT, _("Align element right") );
  tb->AddSeparator();
  tb->AddTool( ID_BTN_ALIGN_TOP, _("Align element top"), wxArtProvider::GetBitmap(ART_ALIGN_TOP, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_TOP, _("Align element top") );
  tb->AddTool( ID_BTN_ALIGN_MIDDLE, _("Align element middle"), wxArtProvider::GetBitmap(ART_ALIGN_MIDDLE, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_MIDDLE, _("Align element middle") );
  tb->AddTool( ID_BTN_ALIGN_BOTTOM, _("Align element bottom"), wxArtProvider::GetBitmap(ART_ALIGN_BOTTOM, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_BOTTOM, _("Align element bottom") );
  tb->Realize();
}

void VisibilityPropertiesCtrl::ExpandAll( bool expand /*=true*/ )
{
  Expand(wxT("cat-rect"), expand);
}

void VisibilityPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
  if( !p )
  {
    wxLogDebug(wxT("VisibilityPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }

  CPMAElement *el = static_cast<CPMAElement*>(p->curel());
  wxString name = ev.GetPropertyName();
  wxVariant val = ev.GetPropertyValue();
  if( name == wxT("overwrite-rect") )
  {
    el->add_has( E_HAS_RECT, val.GetBool() );
    wxRect r = el->iget_rect();
    SetPropertyValue( wxT("X"), r.GetX() );
    SetPropertyValue( wxT("Y"), r.GetY() );
    SetPropertyValue( wxT("width"), r.GetWidth() );
    SetPropertyValue( wxT("height"), r.GetHeight() );
  }
  else if( name == wxT("X") || name == wxT("Y") || name == wxT("height") || name == wxT("width") )
  {
    if( !(el->m_has & E_HAS_RECT) )
    { // user was starting to edit while seeing the inherited values, copy them over
      el->m_rect = el->iget_rect();
      //SetPropertyValue( wxT("overwrite-rect"), true );
      el->m_has |= E_HAS_RECT;
    }
    if( name == wxT("X") )
      el->m_rect.x = ev.GetPropertyValueAsInt();
    else if( name == wxT("Y") )
      el->m_rect.y = ev.GetPropertyValueAsInt();
    else if( name == wxT("width") )
      el->m_rect.width = ev.GetPropertyValueAsInt();
    else if( name == wxT("height") )
      el->m_rect.height = ev.GetPropertyValueAsInt();
  }
  else if( name == wxT("time") )
  {
    el->add_has( E_HAS_TIME, !ev.GetProperty()->IsValueUnspecified() );
    if( !ev.GetProperty()->IsValueUnspecified() )
      el->set_time(val.GetInteger());
    SetPropertyValue( wxT("time"), el->iget_time() );
  }
  else
    return; // nothing changed

  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}
void VisibilityPropertiesCtrl::from_element( const ElementBase *el )
{
  const CPMAElement *cel = static_cast<const CPMAElement*>(el);

  SetPropertyValue( wxT("overwrite-rect"), el->has() & E_HAS_RECT );

  wxToolBar *tb = GetToolBar();
  tb->ToggleTool( (el->is_enabled() ? ID_BTN_ELEMENT_ENABLE : ID_BTN_ELEMENT_DISABLE), true );
  tb->EnableTool( ID_BTN_ELEMENT_ENABLE,  (el->flags() & E_ENABLEALWAYS)==0 );
  tb->EnableTool( ID_BTN_ELEMENT_DISABLE,  (el->flags() & E_ENABLEALWAYS)==0 );
    
  wxRect r = el->iget_rect();
  SetPropertyValue( wxT("X"), r.GetX() );
  SetPropertyValue( wxT("Y"), r.GetY() );
  SetPropertyValue( wxT("width"), r.GetWidth() );
  SetPropertyValue( wxT("height"), r.GetHeight() );

  SetPropertyValue( wxT("time"), cel->iget_time() );

  update_layout();
}

void VisibilityPropertiesCtrl::update_layout()
{
  PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
  if( !p )
  {
    wxLogDebug(wxT("VisibilityPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();

  property_defines( wxT("X"), (el->has() & E_HAS_RECT)!=0 );
  property_defines( wxT("Y"), (el->has() & E_HAS_RECT)!=0 );
  property_defines( wxT("width"), (el->has() & E_HAS_RECT)!=0 );
  property_defines( wxT("height"), (el->has() & E_HAS_RECT)!=0 );

  property_defines(wxT("time"), (el->has() & E_HAS_TIME) != 0 );
}

void VisibilityPropertiesCtrl::OnElementVisibility( wxCommandEvent& ev )
{
  PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
  if( !p )
  {
    wxLogDebug(wxT("VisibilityPropertiesCtrl::OnElementVisibility() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();
  el->set_enabled( ev.GetId() == ID_BTN_ELEMENT_ENABLE );

  // -- update elementicon in list
  wxGetApp().mainframe()->elementsctrl()->update_item(el);

  wxGetApp().mainframe()->OnPropertiesChanged();
}

void VisibilityPropertiesCtrl::OnAlign( wxCommandEvent& ev )
{
  CPMAElement *el = current_element();
  el->add_has( E_HAS_RECT );
  wxRect r = el->rect();
  switch( ev.GetId() )
  {
  case ID_BTN_ALIGN_LEFT: r.x = 0; break;
  case ID_BTN_ALIGN_CENTER: r.x = (640-r.width)/2; break;
  case ID_BTN_ALIGN_RIGHT: r.x = 640-r.width; break;
  case ID_BTN_ALIGN_TOP: r.y = 0; break;
  case ID_BTN_ALIGN_MIDDLE: r.y = (480-r.height)/2; break;
  case ID_BTN_ALIGN_BOTTOM: r.y = 480-r.height; break;
  default:
    return;
  }
  el->set_rect(r);

  // -- propagate
  from_element(el); // update propertygrid
  wxGetApp().mainframe()->OnPropertiesChanged();
}

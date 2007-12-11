#include "colorpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "propertiesctrlbase.h"
#include "pgfontsizeproperty.h"

#include "cpma/element.h"

#include <wx/propgrid/advprops.h>
#include <wx/propgrid/propgrid.h>

BEGIN_EVENT_TABLE(ColorPropertiesCtrl, wxPropertyGridManager)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, ColorPropertiesCtrl::OnItemChanged)
END_EVENT_TABLE()

ColorPropertiesCtrl::ColorPropertiesCtrl( wxWindow *parent ) :
  wxPropertyGridManager( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Color"));

  Append( wxPropertyCategory(_("Foreground")) );
  Append( wxBoolProperty( _("Use"), wxT("color-use"), false) );
  SetPropertyAttribute(wxT("color-use"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( wxColourProperty(_("Color"), wxT("color")) );
  SetPropertyHelpString( wxT("color"), _("Sets the foreground color for the element.") );
  Append( wxUIntProperty(_("Opaqueness"), wxT("color-alpha")) );
//  SetPropertyValidator( wxT("color-alpha"), alpha_validator );
  

  Append( wxPropertyCategory(_("Background")) );
  Append( wxBoolProperty( _("Use"), wxT("bgcolor-use"), false) );
  SetPropertyAttribute(wxT("bgcolor-use"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( wxColourProperty(_("Color"), wxT("bgcolor")) );
  SetPropertyHelpString( wxT("bgcolor"), _("Sets the background color for the element. The element must have a width and height.") );
  Append( wxUIntProperty(_("Opaqueness"), wxT("bgcolor-alpha")) );
  Append( wxBoolProperty( _("Fill"), wxT("fill"), false) );
  SetPropertyAttribute(wxT("fill"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("fill"), _("If the element has a background color, this fills the area it occupies with that color. The element must have a width and height.") );

  Append( wxPropertyCategory(_("Fade")) );
  Append( wxBoolProperty( _("Use"), wxT("fade-use"), false) );
  SetPropertyAttribute(wxT("fade-use"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( wxColourProperty(_("Color"), wxT("fade")) );
  Append( wxUIntProperty(_("Opaqueness"), wxT("fade-alpha")) );

  SetTargetPage(0);
}


void ColorPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("PositionPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  CPMAElement *el = static_cast<CPMAElement*>(p->curel());
  wxString name = ev.GetPropertyName();
  wxVariant val = ev.GetPropertyValue();

  if( name == wxT("color-use") )
  {
    el->add_has( E_HAS_COLOR, val.GetBool() );
    Color4 c = el->iget_color();
    SetPropertyValue( wxT("color"), c.to_wxColour() );
    SetPropertyValue( wxT("color-alpha"), c.a100() );
  }
  else if( name == wxT("color") || name == wxT("color-alpha") )
  {
    if( !(el->has() & E_HAS_COLOR) )
    { // copy parentinfo
      el->set_color( el->iget_color() );
      el->add_has( E_HAS_COLOR );
      SetPropertyValue( wxT("color-use"), true );
    }
    if( name == wxT("color") )
    {
      wxColour& col = *wxDynamicCast(ev.GetPropertyValueAsWxObjectPtr(),wxColour);
      el->set_color(col);
    }
    else if( name == wxT("color-alpha") )
      el->set_color_a100( val.GetInteger() );

  }
  else
    return; // nothing changed

  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void ColorPropertiesCtrl::from_element( ElementBase *el )
{
  CPMAElement *cel = static_cast<CPMAElement*>(el);
  SetPropertyValue( wxT("color-use"), cel->has() & E_HAS_COLOR );
  SetPropertyValue( wxT("color"), cel->iget_color().to_wxColour() );
  SetPropertyValue( wxT("color-alpha"), cel->iget_color().a100() );

  SetPropertyValue( wxT("bgcolor-use"), cel->has() & E_HAS_BGCOLOR );
  SetPropertyValue( wxT("bgcolor"), cel->iget_bgcolor().to_wxColour() );
  SetPropertyValue( wxT("bgcolor-alpha"), cel->iget_bgcolor().a100() );

  SetPropertyValue( wxT("fade-use"), cel->has() & E_HAS_FADE );
  SetPropertyValue( wxT("fade"), cel->iget_fade().to_wxColour() );
  SetPropertyValue( wxT("fade-alpha"), cel->iget_fade().a100() );
  
  //SetPropertyValue( wxT("monospace"), cel->monospace() );
  update_layout();
}


void ColorPropertiesCtrl::update_layout()
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("PositionPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  CPMAElement *el = static_cast<CPMAElement*>(p->curel());

  if( el->has() & E_HAS_COLOR )
  {
    SetPropertyTextColour( wxT("color"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("color"), PROPS_BGCOLOR_NORMAL );
    SetPropertyTextColour( wxT("color-alpha"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("color-alpha"), PROPS_BGCOLOR_NORMAL );
  }
  else
  {
    SetPropertyTextColour( wxT("color"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("color"), PROPS_BGCOLOR_INHERITED );
    SetPropertyTextColour( wxT("color-alpha"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("color-alpha"), PROPS_BGCOLOR_INHERITED );
  }
  // -- font
  /*
  if( el->has() & E_HAS_FONT )
  {
    SetPropertyTextColour( wxT("font"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("font"), PROPS_BGCOLOR_NORMAL );
  }
  else
  { // inherit
    SetPropertyTextColour( wxT("font"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("font"), PROPS_BGCOLOR_INHERITED );
  }
  SetPropertyValue( wxT("font"), el->iget_font() );
  */
}


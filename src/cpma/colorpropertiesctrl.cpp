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
#include "colorpropertiesctrl.h"

#include "../common.h"
#include "../mainframe.h"
#include "../hudfilebase.h"
#include "../propertiesnotebookbase.h"
#include "../prefs.h"

#include "element.h"

#include <wx/propgrid/advprops.h>
#include <wx/propgrid/propgrid.h>

BEGIN_EVENT_TABLE(ColorPropertiesCtrl, CPMAPropertyGridBase)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, ColorPropertiesCtrl::OnItemChanging)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, ColorPropertiesCtrl::OnItemChanged)
END_EVENT_TABLE()

ColorPropertiesCtrl::ColorPropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGridBase( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Color"));

  Append( new wxPropertyCategory(_("Foreground"), wxT("cat-color")) );
  Append( new wxBoolProperty( _("Use"), wxT("color-use"), false) );
  SetPropertyAttribute(wxT("color-use"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  //Append( new wxColourProperty(_("Color"), wxT("color")) );
  m_color = new wxColourProperty(_("Color"), wxT("color"), *wxRED);
  Append( m_color );
  SetPropertyHelpString( wxT("color"), _("Sets the foreground color for the element.") );
  Append( new wxIntProperty(_("Opacity"), wxT("color-alpha"), 0) );
  SetPropertyEditor(wxT("color-alpha"),wxPG_EDITOR(SpinCtrl));
#ifdef WIN32
  // this does stupid things in wxGTK (not allowing to keep buttondown pressed.. stops randomly (?) somewhere)
  SetPropertyAttribute( wxT("color-alpha"), wxT("Min"), (long)0 );
  SetPropertyAttribute( wxT("color-alpha"), wxT("Max"), (long)100 );
#endif
  static const wxChar* color_special[] = { _("No"), _("Team color (red/blue)"), _("Enemey color (red/blue)"), (const wxChar*)0};
  Append( new wxEnumProperty(_("Special?"), wxT("color-special"), color_special) );


  Append( new wxPropertyCategory(_("Background"), wxT("cat-bgcolor")) );
  Append( new wxBoolProperty( _("Use"), wxT("bgcolor-use"), false) );
  SetPropertyAttribute(wxT("bgcolor-use"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( new wxColourProperty(_("Color"), wxT("bgcolor")) );
  SetPropertyHelpString( wxT("bgcolor"), _("Sets the background color for the element. The element must have a width and height.") );
  Append( new wxIntProperty(_("Opacity"), wxT("bgcolor-alpha"), 0) );
  SetPropertyEditor(wxT("bgcolor-alpha"),wxPG_EDITOR(SpinCtrl));
#ifdef WIN32
  SetPropertyAttribute( wxT("bgcolor-alpha"), wxT("Min"), (long)0 );
  SetPropertyAttribute( wxT("bgcolor-alpha"), wxT("Max"), (long)100 );
#endif
  Append( new wxBoolProperty( _("Fill"), wxT("fill"), false) );
  SetPropertyHelpString( wxT("fill"), _("If the element has a background color, this fills the area it occupies with that color. The element must have a width and height.") );

  Append( new wxPropertyCategory(_("Fade"), wxT("cat-fade")) );
  Append( new wxBoolProperty( _("Use"), wxT("fade-use"), false) );
  SetPropertyAttribute(wxT("fade-use"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( new wxColourProperty(_("Color"), wxT("fade")) );
  Append( new wxIntProperty(_("Opacity"), wxT("fade-alpha"), -1) );
  SetPropertyEditor(wxT("fade-alpha"),wxPG_EDITOR(SpinCtrl));
#ifdef WIN32
  SetPropertyAttribute( wxT("fade-alpha"), wxT("Min"), (long)0 );
  SetPropertyAttribute( wxT("fade-alpha"), wxT("Max"), (long)100 );
#endif

  //SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);
}

void ColorPropertiesCtrl::ExpandAll( bool expand /*= true*/ )
{
  Expand(wxT("cat-color"), expand);
  Expand(wxT("cat-bgcolor"), expand);
  Expand(wxT("cat-fade"), expand);
}

void ColorPropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  const CPMAElement* el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();

  // if user is trying to disable this but a parent has it enabled, tell him
  if( name == wxT("fill") && !ev.GetValue().GetBool() && el->iget_fill() && !el->fill() )
  {
    wxMessageBox(CANTDISABLEPROPERTY_MSG);
    ev.Veto();
  }
}

void ColorPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();
  wxVariant val = ev.GetPropertyValue();

  if( name == wxT("color-use") )
  {
    el->add_has( E_HAS_COLOR, val.GetBool() );
    // use own color (not parental)
    Color4 c = el->iget_color();
    SetPropertyValue( wxT("color"), she::colour2variant(c.to_wxColour()) );
    SetPropertyValue( wxT("color-alpha"), c.a100() );
  }
  else if( name == wxT("color") || name == wxT("color-alpha") || name == wxT("color-special") )
  {
    if( !(el->has() & E_HAS_COLOR) )
    { // copy parentinfo
      el->set_color( el->iget_color() );
      el->add_has( E_HAS_COLOR );
      SetPropertyValue( wxT("color-use"), true );
    }
    if( name == wxT("color") )
    {
      wxColour col;
      col << val;
      el->set_color(col);
    }
    else if( name == wxT("color-alpha") )
      el->set_color_a100( val.GetInteger() );
    else if( name == wxT("color-special") )
    {
      Color4 col = el->iget_color();
      switch( val.GetInteger() )
      {
        case 1:
          col.set_type(COLOR_T);
          break;
        case 2:
          col.set_type(COLOR_E);
          break;
        case 0:
        default:
          col.set_type(COLOR_RGBA);
          break;
      }
      el->set_color(col);
      wxLogDebug(col.to_string());
    }
  }
// -----------------------------------------------------
  else if( name == wxT("bgcolor-use") )
  {
    el->add_has( E_HAS_BGCOLOR, val.GetBool() );
    // use own color (not parental)
    Color4 c = el->iget_bgcolor();
    // normally if you enable bgcolor you want something to see... so just 
    // fix alpha if it's 0 (default bgcolor (if not specified) has alpha 0, obviously. see cpma/element.h:E_BGCOLOR_DEFAULT)
    if( val.GetBool() && c.a100() == 0 )
      c.set_a100(100);
    SetPropertyValue( wxT("bgcolor"), she::colour2variant(c.to_wxColour()) );
    SetPropertyValue( wxT("bgcolor-alpha"), c.a100() );
  }
  else if( name == wxT("bgcolor") || name == wxT("bgcolor-alpha") )
  {
    if( !(el->has() & E_HAS_BGCOLOR) )
    { // copy parentinfo
      el->set_bgcolor( el->iget_bgcolor() );
      el->add_has( E_HAS_BGCOLOR );
      SetPropertyValue( wxT("bgcolor-use"), true );
    }
    if( name == wxT("bgcolor") )
    {
      wxColour col;
      col << val;
      el->set_bgcolor(col);
    }
    else if( name == wxT("bgcolor-alpha") )
      el->set_bgcolor_a100( val.GetInteger() );
  }
// -----------------------------------------------------
  else if( name == wxT("fade-use") )
  {
    el->add_has( E_HAS_FADE, val.GetBool() );
    // use own color (not parental)
    Color4 c = el->iget_fade();
    SetPropertyValue( wxT("fade"), she::colour2variant(c.to_wxColour()) );
    SetPropertyValue( wxT("fade-alpha"), c.a100() );
  }
  else if( name == wxT("fade") || name == wxT("fade-alpha") )
  {
    if( !(el->has() & E_HAS_FADE) )
    { // copy parentinfo
      el->set_fade( el->iget_fade() );
      el->add_has( E_HAS_FADE );
      SetPropertyValue( wxT("fade-use"), true );
    }
    if( name == wxT("fade") )
    {
      wxColour col;
      col << val;
      el->set_fade(col);
    }
    else if( name == wxT("fade-alpha") )
      el->set_fade_a100( val.GetInteger() );
  }
// -----------------------------------------------------
  else if( name == wxT("fill") )
  {
    if( el->flags() & E_PARENT && val.GetBool() )
      wxMessageBox( CHECKBOXWARNING_MSG );
    // first update current element value
    el->set_fill(val.GetBool());
    // if we are disabling, we still want a parental value to be active (i.e. this only changes cell color)
    if( !val.GetBool() )
      SetPropertyValue( wxT("fill"), el->iget_fill() );
  }
  else
    return; // nothing changed

  wxGetApp().hudfile()->set_modified();
  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void ColorPropertiesCtrl::from_element( const ElementBase* el )
{
  const CPMAElement* cel = static_cast<const CPMAElement*>(el);

  SetPropertyValue( wxT("color-use"), (cel->has() & E_HAS_COLOR) != 0 );
  SetPropertyValue( wxT("color"), she::colour2variant(cel->iget_color().to_wxColour()) );
  SetPropertyValue( wxT("color-alpha"), cel->iget_color().a100() );
  switch( cel->iget_color().get_type() )
  {
    case COLOR_T:
      SetPropertyValue( wxT("color-special"), 1);
      break;
    case COLOR_E:
      SetPropertyValue( wxT("color-special"), 2);
      break;
    case COLOR_RGBA:
    default:
      SetPropertyValue( wxT("color-special"), 0);
      break;
  }

  SetPropertyValue( wxT("bgcolor-use"), (cel->has() & E_HAS_BGCOLOR) != 0 );
  SetPropertyValue( wxT("bgcolor"), she::colour2variant(cel->iget_bgcolor().to_wxColour()) );
  SetPropertyValue( wxT("bgcolor-alpha"), cel->iget_bgcolor().a100() );
  SetPropertyValue( wxT("fill"), cel->iget_fill() );

  SetPropertyValue( wxT("fade-use"), (cel->has() & E_HAS_FADE) != 0 );
  SetPropertyValue( wxT("fade"), she::colour2variant(cel->iget_fade().to_wxColour()) );
  SetPropertyValue( wxT("fade-alpha"), cel->iget_fade().a100() );


  if( cel->type() == E_T_WEAPONLIST )
    SetPropertyLabel(wxT("fill"), _("Show weapons w/o ammo"));
  else
    SetPropertyLabel(wxT("fill"), _("Fill"));
  
  update_layout();

  if( !Prefs::get().var(wxT("props_neverdisable")).bval() )
  {
    EnableProperty(wxT("bgcolor-use"), cel->type() != E_T_BAR);
    EnableProperty(wxT("bgcolor"), cel->type() != E_T_BAR);
    EnableProperty(wxT("bgcolor-alpha"), cel->type() != E_T_BAR);
  }
  
  /*
  // FIXME this does remove categories.. one by one while selecting new items? wtf? bug in wxpropgrid?
  if( cel->type() == E_T_BAR )
    Expand(wxT("cat-bgcolor"));
  else
    Collapse(wxT("cat-bgcolor"));
    */
}


void ColorPropertiesCtrl::update_layout()
{
  const CPMAElement* const el = current_element();

  property_defines(wxT("color"), (el->has() & E_HAS_COLOR) != 0);
  property_defines(wxT("color-alpha"), (el->has() & E_HAS_COLOR) != 0);
  EnableProperty(wxT("color"), !el->iget_color().is_special() );
  EnableProperty(wxT("color-alpha"), !el->iget_color().is_special() );
  property_defines(wxT("color-special"), (el->has() & E_HAS_COLOR) != 0);

  property_defines(wxT("bgcolor"), (el->has() & E_HAS_BGCOLOR) != 0);
  property_defines(wxT("bgcolor-alpha"), (el->has() & E_HAS_BGCOLOR) != 0);
  property_defines(wxT("fill"), el->fill() );

  property_defines(wxT("fade"), (el->has() & E_HAS_FADE) != 0);
  property_defines(wxT("fade-alpha"), (el->has() & E_HAS_FADE) != 0);
  
}


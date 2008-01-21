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
#include "imagepropertiesctrl.h"

#include "../common.h"
#include "../mainframe.h"
#include "../hudfilebase.h"
#include "../elementbase.h"
#include "../propertiesnotebookbase.h"
#include "../pakfileproperty.h"
#include "../prefs.h"

#include "element.h"

BEGIN_EVENT_TABLE(ImagePropertiesCtrl, CPMAPropertyGrid)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, ImagePropertiesCtrl::OnItemChanged)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, ImagePropertiesCtrl::OnItemChanging)
END_EVENT_TABLE()

ImagePropertiesCtrl::ImagePropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGrid( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Image"));

  Append( new wxBoolProperty(_("3D Model?"), wxT("use-model"), false) );
  SetPropertyHelpString(wxT("use-model"), _("Can be any model in the game. Note that most Q3 models do not work properly if r_vertexlight is on.\n\nModels have file extension .md3"));

  append_picture_properties();
  append_model_properties();
}




void ImagePropertiesCtrl::ExpandAll( bool expand /*=true*/ )
{
  wxPGId id = GetPropertyByName(wxT("cat-image"));
  if(id) Expand(id, expand);
  id = GetPropertyByName(wxT("cat-model"));
  if(id) Expand(id, expand);
  id = GetPropertyByName(wxT("cat-angles"));
  if(id) Expand(id, expand);
  id = GetPropertyByName(wxT("cat-offset"));
  if(id) Expand(id, expand);
}

void ImagePropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  const CPMAElement * el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();

  // TODO if a parent has model specified, can we undo it?
}

void ImagePropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();
  wxVariant val = prop->GetValue();

  if( name == wxT("image") )
  {
    el->set_image( val.GetString() );
    el->add_has( E_HAS_IMAGE, !val.GetString().empty() );
  }
  else if( name == wxT("model") )
  {
    el->set_model( val.GetString() );
    el->add_has( E_HAS_MODEL, !val.GetString().empty() );
  }
  else if( name == wxT("skin") )
  {
    el->set_skin( val.GetString() );
    el->add_has( E_HAS_SKIN, !val.GetString().empty() );
  }
  else if( name == wxT("use-model") )
  {
    el->set_usemodel( val.GetBool() );
  }
  else if( name == wxT("use-angles") )
  {
    el->add_has( E_HAS_ANGLES, val.GetBool() );
  }
  else if( name == wxT("yaw") || name == wxT("roll") || name == wxT("pitch") || name == wxT("pan") )
  {
    if( !(el->has() & E_HAS_ANGLES) )
    { // user was starting to edit while seeing the inherited values, copy them over
      el->set_angle(E_ANGLE_YAW, el->iget_angle(E_ANGLE_YAW));
      el->set_angle(E_ANGLE_ROLL, el->iget_angle(E_ANGLE_ROLL));
      el->set_angle(E_ANGLE_PITCH, el->iget_angle(E_ANGLE_PITCH));
      el->set_angle(E_ANGLE_PAN, el->iget_angle(E_ANGLE_PAN));
      el->add_has( E_HAS_ANGLES );
    }
    if( name == wxT("yaw") )
      el->set_angle(E_ANGLE_YAW, val.GetInteger() );
    else if( name == wxT("pitch") )
      el->set_angle(E_ANGLE_PITCH, val.GetInteger() );
    else if( name == wxT("roll") )
      el->set_angle(E_ANGLE_ROLL, val.GetInteger() );
    else if( name == wxT("pan") )
      el->set_angle(E_ANGLE_PAN, val.GetInteger() );
  }
  else if( name == wxT("use-offset") )
  {
    el->add_has( E_HAS_OFFSET, val.GetBool() );
  }
  else if( name == wxT("x") || name == wxT("y") || name == wxT("z") )
  {
    if( !(el->has() & E_HAS_OFFSET) )
    { // user was starting to edit while seeing the inherited values, copy them over
      el->set_offset(E_OFFSET_X, el->iget_offset(E_OFFSET_X));
      el->set_offset(E_OFFSET_Y, el->iget_offset(E_OFFSET_Y));
      el->set_offset(E_OFFSET_Z, el->iget_offset(E_OFFSET_Z));
      el->add_has( E_HAS_OFFSET );
    }
    if( name == wxT("x") )
      el->set_offset(E_OFFSET_X, val.GetInteger() );
    else if( name == wxT("y") )
      el->set_offset(E_OFFSET_Y, val.GetInteger() );
    else if( name == wxT("z") )
      el->set_offset(E_OFFSET_Z, val.GetInteger() );
  }
  else
    return; // nothing changed

  wxGetApp().hudfile()->set_modified();
  update_layout( name == wxT("use-model") );

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void ImagePropertiesCtrl::from_element( const ElementBase *el )
{
  const CPMAElement *cel = static_cast<const CPMAElement*>(el);

  SetPropertyValue( wxT("use-model"), cel->usemodel() );

  update_layout();

  if( !Prefs::get().var(wxT("props_neverdisable")).bval() )
  {
    Enable( cel->type() == E_T_USERICON || cel->type() == E_T_UNKNOWN );
    ExpandAll( cel->type() == E_T_USERICON || cel->type() == E_T_UNKNOWN );
  }
}



void ImagePropertiesCtrl::append_picture_properties( const CPMAElement *el /*=0*/ )
{
  Append( new wxPropertyCategory(_("Picture"), wxT("cat-image")) );

  Append( new PakFileProperty(_("File"), wxT("image"), el != 0 ? el->iget_image() : wxT(""), PFP_IMAGE) );
}

void ImagePropertiesCtrl::append_model_properties( const CPMAElement *el /*=0*/ )
{
  Append( new wxPropertyCategory(_("Model"), wxT("cat-model")) );

  Append( new wxPropertyCategory(_("Model"), wxT("cat-model")) );
  // if E_T_ICON then model is fix! why not write it readonly? :)
  Append( new wxStringProperty(_("File"), wxT("model"), el ? el->iget_model() : wxT("")) );
  Append( new wxStringProperty(_("Skin"), wxT("skin"), el ? el->iget_skin() : wxT("")) );

  Append( new wxPropertyCategory(_("Rotation"), wxT("cat-angles")) );
  SetPropertyHelpString( wxT("cat-angles"), _("Alters the display of MODEL. Note that most Q3 models do not work properly if r_vertexlight is on.") );
  Append( new wxBoolProperty(_("Use"), wxT("use-angles"), el ? (el->has() & E_HAS_ANGLES) != 0 : false) );
  SetPropertyAttribute(wxT("use-angles"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( new wxIntProperty(_("Pitch"), wxT("pitch"), el ? el->iget_angle(E_ANGLE_PITCH) : 0) );
  SetPropertyHelpString( wxT("pitch"), _("Rotation around axis pointing right (horizontal, X).") ); 
  SetPropertyEditor(wxT("pitch"),wxPG_EDITOR(SpinCtrl));
  Append( new wxIntProperty(_("Yaw"), wxT("yaw"), el ? el->iget_angle(E_ANGLE_YAW) : 0) );
  SetPropertyHelpString( wxT("yaw"), _("Rotation around axis pointing up axis (vertical, Y).") ); 
  SetPropertyEditor(wxT("yaw"),wxPG_EDITOR(SpinCtrl));
  Append( new wxIntProperty(_("Roll"), wxT("roll"), el ? el->iget_angle(E_ANGLE_ROLL) : 0) );
  SetPropertyHelpString( wxT("roll"), _("Rotation around axis pointing ahead (Z).") ); 
  SetPropertyEditor(wxT("roll"),wxPG_EDITOR(SpinCtrl));
  Append( new wxIntProperty(_("Pan+/Rotate-"), wxT("pan"), el ? el->iget_angle(E_ANGLE_PAN) : 0) );
  SetPropertyHelpString( wxT("pan"), _("Use a positive value for left/right panning, a negative value for continuous rotation.") ); 
  SetPropertyEditor(wxT("pan"),wxPG_EDITOR(SpinCtrl));

  Append( new wxPropertyCategory(_("Offset"), wxT("cat-offset")) );
  SetPropertyHelpString( wxT("cat-offset"), _("Moves the model along the X/Y/Z axis.") );
  Append( new wxBoolProperty(_("Use"), wxT("use-offset"), el ? (el->has() & E_HAS_OFFSET) != 0 : false) );
  SetPropertyAttribute(wxT("use-offset"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  Append( new wxIntProperty(wxT("X"), wxT("x"), el ? (int)el->iget_offset(E_OFFSET_X) : 0) );
  SetPropertyEditor(wxT("x"),wxPG_EDITOR(SpinCtrl));
  Append( new wxIntProperty(wxT("y"), wxT("y"), el ? (int)el->iget_offset(E_OFFSET_Y) : 0) );
  SetPropertyEditor(wxT("y"),wxPG_EDITOR(SpinCtrl));
  Append( new wxIntProperty(wxT("z"), wxT("z"), el ? (int)el->iget_offset(E_OFFSET_Z) : 0) );
  SetPropertyEditor(wxT("z"),wxPG_EDITOR(SpinCtrl));
}

void ImagePropertiesCtrl::update_layout( bool reset /*=true*/)
{
  const CPMAElement *el = current_element();
  

  // remove propertyrows and re-add those we are looking for
  if( reset )
  {
    wxPGId id = GetPropertyByName(wxT("cat-image"));
    if( id ) DeleteProperty(wxT("cat-image"));
    id = GetPropertyByName(wxT("image"));
    if( id ) DeleteProperty(wxT("image"));

    id = GetPropertyByName(wxT("cat-model"));
    if( id ) DeleteProperty(wxT("cat-model"));
    id = GetPropertyByName(wxT("model"));
    if( id ) DeleteProperty(wxT("model"));
    id = GetPropertyByName(wxT("skin"));
    if( id ) DeleteProperty(wxT("skin"));
    id = GetPropertyByName(wxT("cat-angles"));
    if( id ) DeleteProperty(wxT("cat-angles"));
    id = GetPropertyByName(wxT("use-angles"));
    if( id ) DeleteProperty(wxT("use-angles"));
    id = GetPropertyByName(wxT("pitch"));
    if( id ) DeleteProperty(wxT("pitch"));
    id = GetPropertyByName(wxT("yaw"));
    if( id ) DeleteProperty(wxT("yaw"));
    id = GetPropertyByName(wxT("roll"));
    if( id ) DeleteProperty(wxT("roll"));
    id = GetPropertyByName(wxT("pan"));
    if( id ) DeleteProperty(wxT("pan"));
    id = GetPropertyByName(wxT("cat-offset"));
    if( id ) DeleteProperty(wxT("cat-offset"));
    id = GetPropertyByName(wxT("use-offset"));
    if( id ) DeleteProperty(wxT("use-offset"));
    id = GetPropertyByName(wxT("x"));
    if( id ) DeleteProperty(wxT("x"));
    id = GetPropertyByName(wxT("y"));
    if( id ) DeleteProperty(wxT("y"));
    id = GetPropertyByName(wxT("z"));
    if( id ) DeleteProperty(wxT("z"));

    if( el->usemodel() )
      append_model_properties(el);
    else
      append_picture_properties(el);
  }
  if( el->usemodel() )
  {
    property_defines( wxT("model"), (el->has() & E_HAS_MODEL) != 0 );
    property_defines( wxT("skin"), (el->has() & E_HAS_SKIN) != 0 );
    property_defines( wxT("yaw"), (el->has() & E_HAS_ANGLES) != 0 );
    property_defines( wxT("roll"), (el->has() & E_HAS_ANGLES) != 0 );
    property_defines( wxT("pitch"), (el->has() & E_HAS_ANGLES) != 0 );
    property_defines( wxT("pan"), (el->has() & E_HAS_ANGLES) != 0 );
    property_defines( wxT("x"), (el->has() & E_HAS_OFFSET) != 0 );
    property_defines( wxT("y"), (el->has() & E_HAS_OFFSET) != 0 );
    property_defines( wxT("z"), (el->has() & E_HAS_OFFSET) != 0 );

  }
  else
    property_defines( wxT("image"), (el->has() & E_HAS_IMAGE) != 0 );
}


#include "imagepropertiesctrl.h"

#include "../common.h"
#include "../mainframe.h"
#include "../hudfilebase.h"
#include "../elementbase.h"
#include "../propertiesnotebookbase.h"

#include "element.h"

BEGIN_EVENT_TABLE(ImagePropertiesCtrl, CPMAPropertyGridBase)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, ImagePropertiesCtrl::OnItemChanged)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, ImagePropertiesCtrl::OnItemChanging)
END_EVENT_TABLE()

ImagePropertiesCtrl::ImagePropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGridBase( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Image"));

  Append( new wxBoolProperty(_("3D Model?"), wxT("use-model"), false) );
  SetPropertyHelpString(wxT("use-model"), _("Can be any model in the game. Note that most Q3 models do not work properly if r_vertexlight is on.\n\nModels have file extension .md3"));

  append_picture_properties();
  append_model_properties();

  /*
  Append( new wxPropertyCategory(_("Model"), wxT("cat-model")) );
  Append( new wxStringProperty(_("File"), wxT("model")) );
  Append( new wxStringProperty(_("Skin"), wxT("skin")) );
  Append( new wxPropertyCategory(_("Rotation"), wxT("cat-rotation")) );
  SetPropertyHelpString( wxT("cat-rotation"), _("Alters the display of MODEL. Note that most Q3 models do not work properly if r_vertexlight is on.") );
  Append( new wxBoolProperty(_("Use"), wxT("use-rotation"), false) );
  Append( new wxIntProperty(_("Pitch"), wxT("pitch"), 0) );
  SetPropertyHelpString( wxT("pitch"), _("Rotation around axis pointing right (horizontal, X).") ); 
  Append( new wxIntProperty(_("Yaw"), wxT("yaw"), 0) );
  SetPropertyHelpString( wxT("yaw"), _("Rotation around axis pointing up axis (vertical, Y).") ); 
  Append( new wxIntProperty(_("Roll"), wxT("roll"), 0) );
  SetPropertyHelpString( wxT("roll"), _("Rotation around axis pointing ahead (Z).") ); 
  Append( new wxIntProperty(_("Pan+/Rotate-"), wxT("pan"), 0) );
  SetPropertyHelpString( wxT("pan"), _("Use a positive value for left/right panning, a negative value for continuous rotation.") ); 
  Append( new wxPropertyCategory(_("Offset"), wxT("cat-offset")) );
  SetPropertyHelpString( wxT("cat-offset"), _("Moves the model along the X/Y/Z axis.") );
  Append( new wxIntProperty(wxT("X"), wxT("x"), 0) );
  Append( new wxIntProperty(wxT("y"), wxT("y"), 0) );
  Append( new wxIntProperty(wxT("z"), wxT("z"), 0) );

  /*
  static const wxChar* font_type[] = {wxEmptyString, wxT("cpma"), wxT("id"), wxT("idblock"), wxT("threewave"), (const wxChar*)0};
  Append( new wxEnumProperty(_("Type"), wxT("font"), font_type) );
  //SetPropertyEditor(wxT("font"), wxPG_EDITOR(Choice));

  static const wxChar* align[] = {wxEmptyString, wxT("left"), wxT("center"), wxT("right"), (const wxChar*)0};
  Append( new wxEnumProperty(_("Alignment"), wxT("textalign"), align) );
  //SetPropertyEditor(wxT("textalign"), wxPG_EDITOR(Choice));

  Append( new wxBoolProperty( _("Monospace"), wxT("monospace"), false) );
  SetPropertyHelpString( wxT("monospace"), _("Displays all characters of the font with the same width") );

  Append( new wxPropertyCategory( _("Style") ) );
  // valid combos: no|no (not defined), no|yes (textstyle 1), yes|no (textstyle 0)
  Append( new wxBoolProperty(_("Force none"), wxT("style-none"), false) );
  Append( new wxBoolProperty(_("Shadow"), wxT("style-shadow"), false) );
  SetPropertyHelpString( wxT("style-none"), _("By default there is no `textstyle' set but if a parent item defines one you can reset the style here") );
  SetPropertyHelpString( wxT("style-shadow"), _("Dropshadowed text") );

  Append( new wxPropertyCategory( _("Size") ) );

  wxArrayString size_type;
  size_type.Add(wxEmptyString);
  size_type.Add(fontsizetype_element_to_ui(E_FST_POINT));
  size_type.Add(fontsizetype_element_to_ui(E_FST_COORD));
//  static const wxChar* size_type[] = {fontsizetype_element_to_ui(E_FST_POINT), fontsizetype_element_to_ui(E_FST_COORD), (const wxChar*)0};
  Append( new wxEnumProperty(_("Type"), wxT("fontsizetype"), size_type) );
  */
}





void ImagePropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  CPMAElement *el = current_element();
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
  else
    return; // nothing changed

  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void ImagePropertiesCtrl::from_element( ElementBase *el )
{
  CPMAElement *cel = static_cast<CPMAElement*>(el);

  SetPropertyValue( wxT("use-model"), cel->usemodel() );
  update_layout();
}

void ImagePropertiesCtrl::append_picture_properties( CPMAElement *el /*=0*/ )
{
  Append( new wxPropertyCategory(_("Picture"), wxT("cat-picture")) );

  Append( new wxStringProperty(_("File"), wxT("image"), el ? el->iget_image() : wxEmptyString) );
}

void ImagePropertiesCtrl::append_model_properties( CPMAElement *el /*=0*/ )
{
  Append( new wxPropertyCategory(_("Model"), wxT("cat-model")) );

  Append( new wxPropertyCategory(_("Model"), wxT("cat-model")) );
  // if E_T_ICON then model is fix! why not write it readonly? :)
  Append( new wxStringProperty(_("File"), wxT("model"), el ? el->iget_model() : wxEmptyString) );
  Append( new wxStringProperty(_("Skin"), wxT("skin"), el ? el->iget_skin() : wxEmptyString) );
  Append( new wxPropertyCategory(_("Rotation"), wxT("cat-rotation")) );
  SetPropertyHelpString( wxT("cat-rotation"), _("Alters the display of MODEL. Note that most Q3 models do not work properly if r_vertexlight is on.") );
  Append( new wxBoolProperty(_("Use"), wxT("use-rotation"), el ? (el->has() & E_HAS_ANGLES) != 0 : false) );
  Append( new wxIntProperty(_("Pitch"), wxT("pitch"), el ? el->) );
  SetPropertyHelpString( wxT("pitch"), _("Rotation around axis pointing right (horizontal, X).") ); 
  Append( new wxIntProperty(_("Yaw"), wxT("yaw"), 0) );
  SetPropertyHelpString( wxT("yaw"), _("Rotation around axis pointing up axis (vertical, Y).") ); 
  Append( new wxIntProperty(_("Roll"), wxT("roll"), 0) );
  SetPropertyHelpString( wxT("roll"), _("Rotation around axis pointing ahead (Z).") ); 
  Append( new wxIntProperty(_("Pan+/Rotate-"), wxT("pan"), 0) );
  SetPropertyHelpString( wxT("pan"), _("Use a positive value for left/right panning, a negative value for continuous rotation.") ); 
  Append( new wxPropertyCategory(_("Offset"), wxT("cat-offset")) );
  SetPropertyHelpString( wxT("cat-offset"), _("Moves the model along the X/Y/Z axis.") );
  Append( new wxBoolProperty(_("Use"), wxT("use-offset"), false) );
  Append( new wxIntProperty(wxT("X"), wxT("x"), 0) );
  Append( new wxIntProperty(wxT("y"), wxT("y"), 0) );
  Append( new wxIntProperty(wxT("z"), wxT("z"), 0) );
}

void ImagePropertiesCtrl::update_layout()
{
  CPMAElement *el = current_element();
  

  // remove propertyrows and re-add those we are looking for
  wxPGId id = GetPropertyByName(wxT("cat-picture"));
  if( id ) DeleteProperty(wxT("cat-picture"));
  id = GetPropertyByName(wxT("image"));
  if( id ) DeleteProperty(wxT("image"));

  id = GetPropertyByName(wxT("cat-model"));
  if( id ) DeleteProperty(wxT("cat-model"));
  id = GetPropertyByName(wxT("model"));
  if( id ) DeleteProperty(wxT("model"));
  id = GetPropertyByName(wxT("skin"));
  if( id ) DeleteProperty(wxT("skin"));
  id = GetPropertyByName(wxT("cat-rotation"));
  if( id ) DeleteProperty(wxT("cat-rotation"));
  id = GetPropertyByName(wxT("use-rotation"));
  if( id ) DeleteProperty(wxT("use-rotation"));
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
  id = GetPropertyByName(wxT("x"));
  if( id ) DeleteProperty(wxT("x"));
  id = GetPropertyByName(wxT("y"));
  if( id ) DeleteProperty(wxT("y"));
  id = GetPropertyByName(wxT("z"));
  if( id ) DeleteProperty(wxT("z"));

  if( el->usemodel() )
  {
    
    append_model_properties(el);
    property_defines( wxT("model"), (el->has() & E_HAS_MODEL) != 0 );
    property_defines( wxT("skin"), (el->has() & E_HAS_SKIN) != 0 );
  }
  else
  {
    append_picture_properties(el);
    property_defines( wxT("image"), (el->has() & E_HAS_IMAGE) != 0 );
  }
  /*
  property_defines( wxT("textalign"), (el->has() & E_HAS_TEXTALIGN) != 0);
  property_defines(wxT("monospace"), el->monospace() );
  property_defines( wxT("style-shadow"), (el->has() & E_HAS_TEXTSTYLE) != 0 );

  property_defines( wxT("fontsizetype"), (el->has() & E_HAS_FONTSIZE) != 0 );

  int type = el->iget_fontsizetype();
  SetPropertyValue( wxT("fontsizetype"), fontsizetype_element_to_ui(type) );
  // remove propertyrows and re-add those we are looking for
  wxPGId id = GetPropertyByName(wxT("fontsize_pt"));
  if( id ) DeleteProperty(wxT("fontsize_pt"));
  id = GetPropertyByName(wxT("fontsize_x"));
  if( id ) DeleteProperty(wxT("fontsize_x"));
  id = GetPropertyByName(wxT("fontsize_y"));
  if( id ) DeleteProperty(wxT("fontsize_y"));
  if( type == E_FST_POINT )
  {
    Append( new wxIntProperty(_("Size"), wxT("fontsize_pt"), el->iget_fontsizept()) );
//    HideProperty(wxT("fontsize_pt"));
    property_defines( wxT("fontsize_pt"), (el->has() & E_HAS_FONTSIZE) != 0 );
  }
  else if( type == E_FST_COORD )
  {
    Append( new wxIntProperty(_("X Size"), wxT("fontsize_x"), el->iget_fontsizex()) );
    Append( new wxIntProperty(_("Y Size"), wxT("fontsize_y"), el->iget_fontsizey()) );
    property_defines( wxT("fontsize_x"), (el->has() & E_HAS_FONTSIZE) != 0 );
    property_defines( wxT("fontsize_y"), (el->has() & E_HAS_FONTSIZE) != 0 );
  }

  
  */
}

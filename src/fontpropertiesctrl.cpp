#include "fontpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "propertiesctrlbase.h"
#include "pgfontsizeproperty.h"

#include "cpma/element.h"

BEGIN_EVENT_TABLE(FontPropertiesCtrl, wxPropertyGridManager)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, FontPropertiesCtrl::OnItemChanged)
END_EVENT_TABLE()

FontPropertiesCtrl::FontPropertiesCtrl( wxWindow *parent ) :
  wxPropertyGridManager( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Font"));

  Append( wxPropertyCategory( _("Font")) );
  
  static const wxChar* font_type[] = {wxEmptyString, wxT("cpma"), wxT("id"), wxT("idblock"), wxT("threewave"), (const wxChar*)0};
  Append( wxEnumProperty(_("Type"), wxT("font"), font_type) );
  SetPropertyEditor(wxT("font"), wxPG_EDITOR(Choice));
  wxPGCheckBoxEditor

  static const wxChar* align[] = {wxEmptyString, wxT("left"), wxT("center"), wxT("right"), (const wxChar*)0};
  Append( wxEnumProperty(_("Alignment"), wxT("textalign"), align) );
  SetPropertyEditor(wxT("textalign"), wxPG_EDITOR(Choice));

  Append( wxBoolProperty( _("Monospace"), wxT("monospace"), false) );
//  SetPropertyAttribute(wxT("monospace"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("monospace"), _("Displays all characters of the font with the same width") );

  Append( wxPropertyCategory( _("Style") ) );
  Append( wxBoolProperty(_("None"), wxT("style-none"), false) );
  Append( wxBoolProperty(_("Shadow"), wxT("style-shadow"), false) );
  SetPropertyAttribute(wxT("style-none"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("style-none"), _("By default there is no `textstyle' set but if a parent item defines one you can reset the style here") );
  SetPropertyAttribute(wxT("style-shadow"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("style-shadow"), _("Dropshadowed text") );

  Append( wxPropertyCategory( _("Size") ) );

  wxArrayString size_type;
  size_type.Add(wxEmptyString);
  size_type.Add(fontsizetype_element_to_ui(E_FST_POINT));
  size_type.Add(fontsizetype_element_to_ui(E_FST_COORD));
//  static const wxChar* size_type[] = {fontsizetype_element_to_ui(E_FST_POINT), fontsizetype_element_to_ui(E_FST_COORD), (const wxChar*)0};
  Append( wxEnumProperty(_("Type"), wxT("fontsizetype"), size_type) );

  //Append( PGFontSizeProperty(_("Size"), wxT("fontsize")) );
  //Append( wxStringProperty(
  //static const wxChar* size_type[] = {wxT("Pointsize"), wxT("Pixelsize"),  (const wxChar*)0};
  //Append( wxEnumProperty(_("Sizetype"), wxPG_LABEL, size_type) );
  

  SetTargetPage(0);
}


void FontPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("FontPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  CPMAElement *el = static_cast<CPMAElement*>(p->curel());
  wxString name = ev.GetPropertyName();
  wxVariant val = ev.GetPropertyValue();

  if( name == wxT("font") )
  {
    wxString font = ev.GetPropertyValueAsString();
    if( font.empty() )
    { // go back to inheritance
      el->remove_has( E_HAS_FONT );
    }
    else
    {
      el->set_font( font );
      el->add_has( E_HAS_FONT );
    }
  }
  else if( name == wxT("textalign") )
  {
    wxString textalign = ev.GetPropertyValueAsString();
    if( textalign.empty() )
    { // inheritance
      el->remove_has( E_HAS_TEXTALIGN );
    }
    else
    {
      wxString ta = ev.GetPropertyValueAsString();
      el->set_textalign( toupper(ta[0]) );
      el->add_has( E_HAS_TEXTALIGN );
    }
  }
  else if( name == wxT("monospace") )
  {
    bool ms = val.GetBool();
    if( el->flags() & E_PARENT && ms )
      wxMessageBox( _("Be aware that the `Monospace' you just ticked cannot be disabled on subsequent elements!") );
    el->set_monospace(ms);
    el->add_has( E_HAS_MONOSPACE, ms );
  }
  else if( name == wxT("fontsizetype") )
  {
    int type = fontsizetype_ui_to_element(ev.GetPropertyValueAsString());
    if( type == E_FST_NONE)
    { // inherit
      el->remove_has( E_HAS_FONTSIZE );
    }
    else
    {
      el->add_has( E_HAS_FONTSIZE );
      el->set_fontsizetype( type );
    }
  }
  else if( name == wxT("fontsize_pt") || name == wxT("fontsize_x") || name == wxT("fontsize_y") )
  {
    if( !(el->has() && E_HAS_FONTSIZE) )
    { // copy it over
      el->set_fontsizetype( el->iget_fontsizetype() );
      el->set_fontsizept( el->iget_fontsizept() );
      el->set_fontsizex( el->iget_fontsizex() );
      el->set_fontsizey( el->iget_fontsizey() );
      el->add_has( E_HAS_FONTSIZE );
    }
    if( name == wxT("fontsize_pt") )
      el->set_fontsizept( ev.GetPropertyValueAsInt() );
    else if( name == wxT("fontsize_x") )
      el->set_fontsizex( ev.GetPropertyValueAsInt() );
    else if( name == wxT("fontsize_y") )
      el->set_fontsizey( ev.GetPropertyValueAsInt() );

  }
  else if( name == wxT("style-none") )
  {
    if( val.GetBool() )
    {
      el->add_has(E_HAS_TEXTSTYLE);
      el->set_textstyle(E_TEXTSTYLE_NONE);
    }
    else
    {
      if( el->textstyle() == E_TEXTSTYLE_NONE )
      { // shadow is not set, so remove stuff
        el->remove_has(E_HAS_TEXTSTYLE);
      }
    }
    SetPropertyValue( wxT("style-none"), el->iget_textstyle() == E_TEXTSTYLE_NONE );
    SetPropertyValue( wxT("style-shadow"), el->iget_textstyle() == E_TEXTSTYLE_SHADOW );
  }
  else if( name == wxT("style-shadow") )
  {
    if( val.GetBool() )
    {
      el->add_has(E_HAS_TEXTSTYLE);
      el->set_textstyle(E_TEXTSTYLE_SHADOW);
    }
    else
    {
      el->remove_has(E_HAS_TEXTSTYLE);
      el->set_textstyle(E_TEXTSTYLE_NONE);
    }
    SetPropertyValue( wxT("style-shadow"), el->iget_textstyle() == E_TEXTSTYLE_SHADOW );
    SetPropertyValue( wxT("style-none"), el->iget_textstyle() == E_TEXTSTYLE_NONE );
  }
  else
    return; // nothing changed

  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void FontPropertiesCtrl::from_element( ElementBase *el )
{
  CPMAElement *cel = static_cast<CPMAElement*>(el);
  SetPropertyValue( wxT("monospace"), cel->iget_monospace() );
  SetPropertyValue( wxT("fontsizetype"), fontsizetype_element_to_ui(cel->fontsizetype()) );
  SetPropertyValue( wxT("style-none"), cel->iget_textstyle() == E_TEXTSTYLE_NONE );
  SetPropertyValue( wxT("style-shadow"), cel->iget_textstyle() == E_TEXTSTYLE_SHADOW );

  //SetPropertyValue( wxT("style-none"), cel->has() & E_HAS_TEXTSTYLE && cel->textstyle() == E_TEXTSTYLE_NONE );
  //SetPropertyValue( wxT("style-shadow"), cel->has() & E_HAS_TEXTSTYLE && cel->textstyle() & E_TEXTSTYLE_SHADOW );
  update_layout();
}

int FontPropertiesCtrl::fontsizetype_ui_to_element( const wxString& fs )
{
  if( fs == wxT("points") )
    return E_FST_POINT;
  else if( fs == wxT("pixels") )
    return E_FST_COORD;
  return E_FST_NONE;
}

wxString FontPropertiesCtrl::fontsizetype_element_to_ui( int fst )
{
  switch( fst )
  {
    case E_FST_COORD: return wxT("pixels");
    case E_FST_POINT: return wxT("points");
    default:
      break;
  }
  return wxT("");
}

wxString FontPropertiesCtrl::textalign_element_to_ui( const wxChar& ta )
{
  if( ta == 'L' ) return wxT("left");
  else if( ta == 'C' ) return wxT("center");
  else if( ta == 'R' ) return wxT("right");
  return wxT("");
}


void FontPropertiesCtrl::update_layout()
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("FontPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  CPMAElement *el = static_cast<CPMAElement*>(p->curel());

  // -- font
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

  // -- textalign
  if( el->has() & E_HAS_TEXTALIGN )
  {
    SetPropertyTextColour( wxT("textalign"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("textalign"), PROPS_BGCOLOR_NORMAL );
  }
  else
  { // inherit
    SetPropertyTextColour( wxT("textalign"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("textalign"), PROPS_BGCOLOR_INHERITED );
  }
  SetPropertyValue( wxT("textalign"), textalign_element_to_ui(el->iget_textalign()) );

  // -- textstyle
  if( el->has() & E_HAS_TEXTSTYLE )
  {
    SetPropertyTextColour( wxT("style-none"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("style-none"), PROPS_BGCOLOR_NORMAL );
    SetPropertyTextColour( wxT("style-shadow"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("style-shadow"), PROPS_BGCOLOR_NORMAL );
  }
  else
  {
    SetPropertyTextColour( wxT("style-none"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("style-none"), PROPS_BGCOLOR_INHERITED );
    SetPropertyTextColour( wxT("style-shadow"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("style-shadow"), PROPS_BGCOLOR_INHERITED );
  }

  // -- fontsize
  int type = el->fontsizetype();
  int fspt = el->fontsizept();
  int fsx = el->fontsizex();
  int fsy = el->fontsizey();
  if( !(el->has() & E_HAS_FONTSIZE) )
  {
    type = el->iget_fontsizetype();
    fspt = el->iget_fontsizept();
    fsx = el->iget_fontsizex();
    fsy = el->iget_fontsizey();
    SetPropertyTextColour( wxT("fontsizetype"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("fontsizetype"), PROPS_BGCOLOR_INHERITED );
  }
  else
  {
    SetPropertyTextColour( wxT("fontsizetype"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("fontsizetype"), PROPS_BGCOLOR_NORMAL );
  }
  SetPropertyValue( wxT("fontsizetype"), fontsizetype_element_to_ui(type) );
  wxPGId id = GetPropertyByName(wxT("fontsize_pt"));
  if( id.IsOk() ) Delete(wxT("fontsize_pt"));
  id = GetPropertyByName(wxT("fontsize_x"));
  if( id.IsOk() ) Delete(wxT("fontsize_x"));
  id = GetPropertyByName(wxT("fontsize_y"));
  if( id.IsOk() ) Delete(wxT("fontsize_y"));
  if( type == E_FST_POINT )
  {
    Append( wxIntProperty(_("Size"), wxT("fontsize_pt"), fspt) );
    if( el->has() & E_HAS_FONTSIZE )
    {
      SetPropertyTextColour( wxT("fontsize_pt"), PROPS_COLOR_NORMAL );
      SetPropertyColour( wxT("fontsize_pt"), PROPS_BGCOLOR_NORMAL );
    }
    else
    {
      SetPropertyTextColour( wxT("fontsize_pt"), PROPS_COLOR_INHERITED );
      SetPropertyColour( wxT("fontsize_pt"), PROPS_BGCOLOR_INHERITED );
    }
  }
  else if( type == E_FST_COORD )
  {
    Append( wxIntProperty(_("X Size"), wxT("fontsize_x"), fsx) );
    Append( wxIntProperty(_("Y Size"), wxT("fontsize_y"), fsy) );
    if( el->has() & E_HAS_FONTSIZE )
    {
      SetPropertyTextColour( wxT("fontsize_x"), PROPS_COLOR_NORMAL );
      SetPropertyColour( wxT("fontsize_x"), PROPS_BGCOLOR_NORMAL );
      SetPropertyTextColour( wxT("fontsize_y"), PROPS_COLOR_NORMAL );
      SetPropertyColour( wxT("fontsize_y"), PROPS_BGCOLOR_NORMAL );
    }
    else
    {
      SetPropertyTextColour( wxT("fontsize_x"), PROPS_COLOR_INHERITED );
      SetPropertyColour( wxT("fontsize_x"), PROPS_BGCOLOR_INHERITED );
      SetPropertyTextColour( wxT("fontsize_y"), PROPS_COLOR_INHERITED );
      SetPropertyColour( wxT("fontsize_y"), PROPS_BGCOLOR_INHERITED );
    }
  }
  bool ms = el->iget_monospace();
  if( !(el->has() & E_HAS_MONOSPACE) && ms )
  { // we inherit true.. so no way to turn it off :o
    SetPropertyReadOnly( wxT("monospace"), true );
    SetPropertyTextColour( wxT("monospace"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("monospace"), PROPS_BGCOLOR_INHERITED );
  }
  else
  {
    SetPropertyReadOnly( wxT("monospace"), false );
    SetPropertyTextColour( wxT("monospace"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("monospace"), PROPS_BGCOLOR_NORMAL );
  }
}


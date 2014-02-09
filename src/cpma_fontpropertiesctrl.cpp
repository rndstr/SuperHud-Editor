#include "cpma_fontpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "propertiesnotebookbase.h"
#include "displayctrlbase.h"
#include "prefs.h"

#include "cpma_element.h"

BEGIN_EVENT_TABLE(FontPropertiesCtrl, CPMAPropertyGrid)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, FontPropertiesCtrl::OnItemChanged)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, FontPropertiesCtrl::OnItemChanging)
END_EVENT_TABLE()

FontPropertiesCtrl::FontPropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGrid( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Font"));

  Append( new wxPropertyCategory( _("Font"), wxT("cat-font")) );
  
  static const wxChar* font_type[] = {wxEmptyString, wxT("cpma"), wxT("id"), wxT("threewave"), wxT("idblock"), wxT("sansman"), (const wxChar*)0};
  /*
  wxArrayString font_type;
  fonts_type fonts = wxGetApp().mainframe()->displayctrl()->fonts();
  for( fonts_type::const_iterator cit = fonts.begin(); cit != fonts.end(); ++cit )
    font_type.Add(cit->first);
  */

  Append( new wxEnumProperty(_("Type"), wxT("font"), font_type) );
  //SetPropertyEditor(wxT("font"), wxPG_EDITOR(Choice));

  static const wxChar* align[] = {wxEmptyString, wxT("left"), wxT("center"), wxT("right"), (const wxChar*)0};
  Append( new wxEnumProperty(_("Alignment"), wxT("textalign"), align) );
  //SetPropertyEditor(wxT("textalign"), wxPG_EDITOR(Choice));

  Append( new wxBoolProperty( _("Monospace"), wxT("monospace"), false) );
  SetPropertyHelpString( wxT("monospace"), _("Displays all characters of the font with the same width") );

  Append( new wxPropertyCategory( _("Style"), wxT("cat-textstyle") ) );
  // valid combos: no|no (not defined), no|yes (textstyle 1), yes|no (textstyle 0)
  Append( new wxBoolProperty(_("Force none"), wxT("style-none"), false) );
  Append( new wxBoolProperty(_("Shadow"), wxT("style-shadow"), false) );
  SetPropertyHelpString( wxT("style-none"), _("By default there is no `textstyle' set but if a parent item defines one you can reset the style here") );
  SetPropertyHelpString( wxT("style-shadow"), _("Dropshadowed text") );

  

  Append( new wxPropertyCategory( _("Size"), wxT("cat-fontsize") ) );

  wxArrayString size_type;
  size_type.Add(wxEmptyString);
  size_type.Add(fontsizetype_element_to_ui(CPMA_E_FST_POINT));
  size_type.Add(fontsizetype_element_to_ui(CPMA_E_FST_COORD));
  Append( new wxEnumProperty(_("Type"), wxT("fontsizetype"), size_type) );
}

void FontPropertiesCtrl::ExpandAll( bool expand /*=true*/ )
{
  Expand(wxT("cat-font"), expand);
  Expand(wxT("cat-textstyle"), expand);
  Expand(wxT("cat-fontsize"), expand);
}

void FontPropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  const CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();

  // if user is trying to disable this but a parent has it enabled, tell him
  if( name == wxT("monospace") && !ev.GetValue().GetBool() && el->iget_monospace() && !el->monospace() )
  {
    wxMessageBox(CANTDISABLEPROPERTY_MSG);
    ev.Veto();
  }
  else if( name == wxT("style-shadow") && !ev.GetValue().GetBool() && !(el->has() & CPMA_E_HAS_TEXTSTYLE) && el->iget_textstyle() == CPMA_E_TEXTSTYLE_SHADOW )
  {
    wxMessageBox(_("There is no way to disable this property on that element as a parent element specifies it.\nYou should enable `Enforce none'."));
    ev.Veto();
  }
}

void FontPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();
  wxVariant val = prop->GetValue();

  if( name == wxT("monospace") )
  {
    if( el->flags() & E_PARENT && val.GetBool() )
      wxMessageBox( CHECKBOXWARNING_MSG );
    // first update current element value
    el->set_monospace(val.GetBool());
    // if we are disabling, we still want a parental value to be active (i.e. this only changes cell color)
    if( !val.GetBool() )
      SetPropertyValue( wxT("monospace"), el->iget_monospace() );
  }
  else if( name == wxT("font") )
  { // dropdown
    wxString font = ev.GetPropertyValueAsString();
    el->add_has( CPMA_E_HAS_FONT, !font.empty() );
    el->set_font( font );
    SetPropertyValue( wxT("font"), el->iget_font() );
  }
  else if( name == wxT("textalign") )
  {
    wxString ta = ev.GetPropertyValueAsString();
    el->add_has( CPMA_E_HAS_TEXTALIGN, !ta.empty() );
    if( !ta.empty() )
      el->set_textalign( FontPropertiesCtrl::textalign_ui_to_element(ta) );
    SetPropertyValue( wxT("textalign"), FontPropertiesCtrl::textalign_element_to_ui(el->iget_textalign()) );
  }
  else if( name == wxT("style-none") )
  {
    if( val.GetBool() )
    {
      el->add_has(CPMA_E_HAS_TEXTSTYLE);
      el->set_textstyle(CPMA_E_TEXTSTYLE_NONE);
    }
    else
    {
      if( el->textstyle() == CPMA_E_TEXTSTYLE_NONE )
      { // shadow is not set, so remove stuff
        el->remove_has(CPMA_E_HAS_TEXTSTYLE);
      }
    }
    SetPropertyValue( wxT("style-none"), el->has() & CPMA_E_HAS_TEXTSTYLE && el->iget_textstyle() == CPMA_E_TEXTSTYLE_NONE );
    SetPropertyValue( wxT("style-shadow"), el->iget_textstyle() == CPMA_E_TEXTSTYLE_SHADOW );
  }
  else if( name == wxT("style-shadow") )
  {
    el->add_has(CPMA_E_HAS_TEXTSTYLE, val.GetBool());
    el->set_textstyle( (val.GetBool() ? CPMA_E_TEXTSTYLE_SHADOW : CPMA_E_TEXTSTYLE_NONE) );
    SetPropertyValue( wxT("style-shadow"), el->iget_textstyle() == CPMA_E_TEXTSTYLE_SHADOW );
    SetPropertyValue( wxT("style-none"), el->has() & CPMA_E_HAS_TEXTSTYLE && el->iget_textstyle() == CPMA_E_TEXTSTYLE_NONE );
  }
  else if( name == wxT("fontsizetype") )
  {
    int type = FontPropertiesCtrl::fontsizetype_ui_to_element(ev.GetPropertyValueAsString());
    el->add_has( CPMA_E_HAS_FONTSIZE, type != CPMA_E_FST_NONE );
    if( type != CPMA_E_FST_NONE)
      el->set_fontsizetype( type );
    
  }
  else if( name == wxT("fontsize_pt") || name == wxT("fontsize_x") || name == wxT("fontsize_y") )
  {
    if( !(el->has() & CPMA_E_HAS_FONTSIZE) )
    { // copy it over
      el->set_fontsizetype( el->iget_fontsizetype() );
      el->set_fontsizept( el->iget_fontsizept() );
      el->set_fontsizex( el->iget_fontsizex() );
      el->set_fontsizey( el->iget_fontsizey() );
      el->add_has( CPMA_E_HAS_FONTSIZE );
    }
    if( name == wxT("fontsize_pt") )
      el->set_fontsizept( ev.GetPropertyValueAsInt() );
    else if( name == wxT("fontsize_x") )
      el->set_fontsizex( ev.GetPropertyValueAsInt() );
    else if( name == wxT("fontsize_y") )
      el->set_fontsizey( ev.GetPropertyValueAsInt() );

  }
  else
    return; // nothing changed

  wxGetApp().hudfile()->set_modified();
  update_layout( name == wxT("fontsizetype") );

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void FontPropertiesCtrl::from_element( const ElementBase *el )
{
  const CPMAElement *cel = static_cast<const CPMAElement*>(el);

  SetPropertyValue( wxT("font"), cel->iget_font() );
  SetPropertyValue( wxT("textalign"), textalign_element_to_ui(cel->iget_textalign()) );
  SetPropertyValue( wxT("monospace"), cel->iget_monospace() );
  SetPropertyValue( wxT("style-none"), (cel->has() & CPMA_E_HAS_TEXTSTYLE)!=0 && cel->iget_textstyle() == CPMA_E_TEXTSTYLE_NONE );
  SetPropertyValue( wxT("style-shadow"), cel->iget_textstyle() == CPMA_E_TEXTSTYLE_SHADOW );
  SetPropertyValue( wxT("fontsizetype"), fontsizetype_element_to_ui(cel->fontsizetype()) );
  
  update_layout();

  if( !Prefs::get().var(wxT("props_neverdisable")).bval() )
  {
    ExpandAll( cel->type() == E_T_TEXT || cel->type() == E_T_WEAPONLIST || cel->type() == E_T_UNKNOWN);
    Enable( cel->type() == E_T_TEXT || cel->type() == E_T_WEAPONLIST || cel->type() == E_T_UNKNOWN );
  }
}

int FontPropertiesCtrl::fontsizetype_ui_to_element( const wxString& fs )
{
  if( fs == wxT("points") )
    return CPMA_E_FST_POINT;
  else if( fs == wxT("pixels") )
    return CPMA_E_FST_COORD;
  return CPMA_E_FST_NONE;
}

wxString FontPropertiesCtrl::fontsizetype_element_to_ui( int fst )
{
  switch( fst )
  {
    case CPMA_E_FST_COORD: return wxT("pixels");
    case CPMA_E_FST_POINT: return wxT("points");
    default:
      break;
  }
  return wxT("");
}

wxString FontPropertiesCtrl::textalign_element_to_ui( const wxChar& ta )
{
  if( toupper(ta) == 'L' ) return wxT("left");
  else if( toupper(ta) == 'C' ) return wxT("center");
  else if( toupper(ta) == 'R' ) return wxT("right");
  wxLogDebug(wxT("FontPropertiesCtrl::textalign_element_to_ui - unknown textalign `%c'"), ta);
  return wxT("");
}

wxChar FontPropertiesCtrl::textalign_ui_to_element( const wxString& ta )
{
  if( ta == wxT("left") )
    return 'L';
  else if( ta == wxT("center") )
    return 'C';
  else if( ta == wxT("right") )
    return 'R';

  wxLogDebug(wxT("FontPropertiesCtrl::textalign_ui_to_element - unknown textalign `%s'"), ta.c_str());
  return 'L';
}

void FontPropertiesCtrl::update_layout( bool reset /*=true*/ )
{
  CPMAElement *el = current_element();
  

  property_defines( wxT("font"), (el->has() & CPMA_E_HAS_FONT) != 0);
  property_defines( wxT("textalign"), (el->has() & CPMA_E_HAS_TEXTALIGN) != 0);
  property_defines(wxT("monospace"), el->monospace() );
  property_defines( wxT("style-shadow"), (el->has() & CPMA_E_HAS_TEXTSTYLE) != 0 );

  property_defines( wxT("fontsizetype"), (el->has() & CPMA_E_HAS_FONTSIZE) != 0 );

  int type = el->iget_fontsizetype();
  SetPropertyValue( wxT("fontsizetype"), FontPropertiesCtrl::fontsizetype_element_to_ui(type) );
    // remove propertyrows and re-add those we are looking for
  if( reset )
  {
    wxPGId id = GetPropertyByName(wxT("fontsize_pt"));
    if( id ) DeleteProperty(wxT("fontsize_pt"));
    id = GetPropertyByName(wxT("fontsize_x"));
    if( id ) DeleteProperty(wxT("fontsize_x"));
    id = GetPropertyByName(wxT("fontsize_y"));
    if( id ) DeleteProperty(wxT("fontsize_y"));
    if( type == CPMA_E_FST_POINT )
    {
      Append( new wxIntProperty(_("Size"), wxT("fontsize_pt"), el->iget_fontsizept()) );
      SetPropertyEditor(wxT("fontsize_pt"),wxPG_EDITOR(SpinCtrl));
    }
    else if( type == CPMA_E_FST_COORD )
    {
      Append( new wxIntProperty(_("Width"), wxT("fontsize_x"), el->iget_fontsizex()) );
      SetPropertyEditor(wxT("fontsize_x"),wxPG_EDITOR(SpinCtrl));
      Append( new wxIntProperty(_("Height"), wxT("fontsize_y"), el->iget_fontsizey()) );
      SetPropertyEditor(wxT("fontsize_y"),wxPG_EDITOR(SpinCtrl));
    }
  }
  if( type == CPMA_E_FST_POINT )
  {
    property_defines( wxT("fontsize_pt"), (el->has() & CPMA_E_HAS_FONTSIZE) != 0 );
  }
  else if( type == CPMA_E_FST_COORD )
  {
    property_defines( wxT("fontsize_x"), (el->has() & CPMA_E_HAS_FONTSIZE) != 0 );
    property_defines( wxT("fontsize_y"), (el->has() & CPMA_E_HAS_FONTSIZE) != 0 );
  }

/*
  // -- fontsize
  int type = el->fontsizetype();
  int fspt = el->fontsizept();
  int fsx = el->fontsizex();
  int fsy = el->fontsizey();
  property_defines( wxT("fontsizetype"), (el->has() & CPMA_E_HAS_FONTSIZE) != 0) );
  if( !(el->has() & CPMA_E_HAS_FONTSIZE) )
  {
    type = el->iget_fontsizetype();
    fspt = el->iget_fontsizept();
    fsx = el->iget_fontsizex();
    fsy = el->iget_fontsizey();
  }
  
  SetPropertyValue( wxT("fontsizetype"), fontsizetype_element_to_ui(type) );
  wxPGId id = GetPropertyByName(wxT("fontsize_pt"));
  if( id ) DeleteProperty(wxT("fontsize_pt"));
  id = GetPropertyByName(wxT("fontsize_x"));
  if( id ) DeleteProperty(wxT("fontsize_x"));
  id = GetPropertyByName(wxT("fontsize_y"));
  if( id ) DeleteProperty(wxT("fontsize_y"));
  if( type == CPMA_E_FST_POINT )
  {
    Append( new wxIntProperty(_("Size"), wxT("fontsize_pt"), fspt) );
    if( el->has() & CPMA_E_HAS_FONTSIZE )
    {
      SetPropertyTextColour( wxT("fontsize_pt"), PROPS_COLOR_NORMAL );
      SetPropertyBackgroundColour( wxT("fontsize_pt"), PROPS_BGCOLOR_NORMAL );
    }
    else
    {
      SetPropertyTextColour( wxT("fontsize_pt"), PROPS_COLOR_INHERITED );
      SetPropertyBackgroundColour( wxT("fontsize_pt"), PROPS_BGCOLOR_INHERITED );
    }
  }
  else if( type == CPMA_E_FST_COORD )
  {
    Append( new wxIntProperty(_("X Size"), wxT("fontsize_x"), fsx) );
    Append( new wxIntProperty(_("Y Size"), wxT("fontsize_y"), fsy) );
    if( el->has() & CPMA_E_HAS_FONTSIZE )
    {
      SetPropertyTextColour( wxT("fontsize_x"), PROPS_COLOR_NORMAL );
      SetPropertyBackgroundColour( wxT("fontsize_x"), PROPS_BGCOLOR_NORMAL );
      SetPropertyTextColour( wxT("fontsize_y"), PROPS_COLOR_NORMAL );
      SetPropertyBackgroundColour( wxT("fontsize_y"), PROPS_BGCOLOR_NORMAL );
    }
    else
    {
      SetPropertyTextColour( wxT("fontsize_x"), PROPS_COLOR_INHERITED );
      SetPropertyBackgroundColour( wxT("fontsize_x"), PROPS_BGCOLOR_INHERITED );
      SetPropertyTextColour( wxT("fontsize_y"), PROPS_COLOR_INHERITED );
      SetPropertyBackgroundColour( wxT("fontsize_y"), PROPS_BGCOLOR_INHERITED );
    }
  }

  
  */
}

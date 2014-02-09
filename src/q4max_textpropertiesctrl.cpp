#include "q4max_textpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "propertiesnotebookbase.h"
#include "displayctrlbase.h"
#include "prefs.h"

#include "q4max_element.h"

BEGIN_EVENT_TABLE(Q4MAXTextPropertiesCtrl, Q4MAXPropertyGrid)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, Q4MAXTextPropertiesCtrl::OnItemChanged)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, Q4MAXTextPropertiesCtrl::OnItemChanging)
END_EVENT_TABLE()

Q4MAXTextPropertiesCtrl::Q4MAXTextPropertiesCtrl( wxWindow *parent ) :
  Q4MAXPropertyGrid( parent, ID_NOTEBOOK_PROPERTIES )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Text"));

  Append( new wxPropertyCategory( _("Font"), wxT("cat-font")) );
  
  static const wxChar* font_type[] = {wxEmptyString, wxT("Chain"), wxT("Marine"), wxT("Lowpixel"), wxT("Profont"), (const wxChar*)0};

  Append( new wxEnumProperty(_("Type"), wxT("font"), font_type) );
  //SetPropertyEditor(wxT("font"), wxPG_EDITOR(Choice));

  static const wxChar* align[] = {wxEmptyString, wxT("left"), wxT("center"), wxT("right"), (const wxChar*)0};
  Append( new wxEnumProperty(_("Alignment"), wxT("textalign"), align) );
  //SetPropertyEditor(wxT("textalign"), wxPG_EDITOR(Choice));

  Append( new wxFloatProperty(_("Scale"), wxT("textscale")) );
  SetPropertyHelpString( wxT("textscale"), _("Multiplies the default font width with the given value") );

  Append( new wxBoolProperty( _("Monospaced"), wxT("monospaced"), false) );
  SetPropertyHelpString( wxT("monospaced"), _("Displays all characters of the font with the same width") );

  /*
  Append( new wxPropertyCategory( _("Style"), wxT("cat-textstyle") ) );
  // valid combos: no|no (not defined), no|yes (textstyle 1), yes|no (textstyle 0)
  Append( new wxBoolProperty(_("Force none"), wxT("style-none"), false) );
  Append( new wxBoolProperty(_("Shadow"), wxT("style-shadow"), false) );
  SetPropertyHelpString( wxT("style-none"), _("By default there is no `textstyle' set but if a parent item defines one you can reset the style here") );
  SetPropertyHelpString( wxT("style-shadow"), _("Dropshadowed text") );
*/
  


}

void Q4MAXTextPropertiesCtrl::ExpandAll( bool expand /*=true*/ )
{
  Expand(wxT("cat-font"), expand);
//  Expand(wxT("cat-textstyle"), expand);
}

void Q4MAXTextPropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  /*
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  const MAXElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();
  */
}

void Q4MAXTextPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  MAXElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();
  wxVariant val = prop->GetValue();

  if( name == wxT("monospaced") )
  {
    if( el->flags() & E_PARENT && val.GetBool() )
      wxMessageBox( CHECKBOXWARNING_MSG );
    // first update current element value
    el->set_bval(wxT("monospaced"), val.GetBool());
  }
  else if( name == wxT("font") )
  { // dropdown
    int font = ev.GetPropertyValueAsInt() - 1;
    el->add_has( MAX_E_HAS_FONT, font >= 0 );
    el->set_ival( wxT("font"), font );
    SetPropertyValue( wxT("font"), el->iget_ival(wxT("font"))+1 );
  }
  else if( name == wxT("textscale") )
  {
    el->add_has( MAX_E_HAS_TEXTSCALE, val.GetBool());
  }
  else if( name == wxT("textalign") )
  {
    wxString ta = ev.GetPropertyValueAsString();
    el->add_has( MAX_E_HAS_TEXTALIGN, !ta.empty() );
    if( !ta.empty() )
      el->set_val( wxT("textalign"), Q4MAXTextPropertiesCtrl::textalign_ui_to_element(ta) );
    SetPropertyValue( wxT("textalign"), Q4MAXTextPropertiesCtrl::textalign_element_to_ui(el->iget_val(wxT("textalign"))) );
  }
  /*
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
  */
  
  else
    return; // nothing changed

  wxGetApp().hudfile()->set_modified();
  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void Q4MAXTextPropertiesCtrl::from_element( const ElementBase *el )
{
  const MAXElement *cel = static_cast<const MAXElement*>(el);

  SetPropertyValue( wxT("font"), cel->iget_ival(wxT("font"))+1 );
  SetPropertyValue( wxT("textalign"), textalign_element_to_ui(cel->iget_val(wxT("textalign"))) );
//  SetPropertyValue( wxT("textscale"), cel->iget_dval(wxT("")) );
  SetPropertyValue( wxT("monospaced"), cel->iget_bval(wxT("monospaced")) );
  //SetPropertyValue( wxT("style-none"), (cel->has() & CPMA_E_HAS_TEXTSTYLE)!=0 && cel->iget_textstyle() == CPMA_E_TEXTSTYLE_NONE );
  //SetPropertyValue( wxT("style-shadow"), cel->iget_textstyle() == CPMA_E_TEXTSTYLE_SHADOW );
  
  update_layout();

  /*
  if( !Prefs::get().var(wxT("props_neverdisable")).bval() )
  {
    ExpandAll( cel->type() == E_T_TEXT || cel->type() == E_T_WEAPONLIST || cel->type() == E_T_UNKNOWN);
    Enable( cel->type() == E_T_TEXT || cel->type() == E_T_WEAPONLIST || cel->type() == E_T_UNKNOWN );
  }
  */
}


wxString Q4MAXTextPropertiesCtrl::textalign_element_to_ui( const wxString& ta )
{
  if( ta.Upper().at(0) == 'L' ) return wxT("left");
  else if( ta.Upper().at(0) == 'C' ) return wxT("center");
  else if( ta.Upper().at(0) == 'R' ) return wxT("right");
  wxLogDebug(wxT("Q4MAXTextPropertiesCtrl::textalign_element_to_ui - unknown textalign `%s'"), ta.c_str());
  return wxT("");
}

wxString Q4MAXTextPropertiesCtrl::textalign_ui_to_element( const wxString& ta )
{
  if( ta == wxT("left") )
    return wxT("L");
  else if( ta == wxT("center") )
    return wxT("C");
  else if( ta == wxT("right") )
    return wxT("R");

  wxLogDebug(wxT("Q4MAXTextPropertiesCtrl::textalign_ui_to_element - unknown textalign `%s'"), ta.c_str());
  return wxT("L");
}

void Q4MAXTextPropertiesCtrl::update_layout( bool reset /*=true*/ )
{
  MAXElement *el = current_element();
  

  property_defines( wxT("font"), (el->has() & MAX_E_HAS_FONT) != 0);
  property_defines( wxT("textscale"), (el->has() & MAX_E_HAS_TEXTSCALE) != 0);
  property_defines( wxT("textalign"), (el->has() & MAX_E_HAS_TEXTALIGN) != 0);
  property_defines(wxT("monospaced"), (el->has() & MAX_E_HAS_MONOSPACED) != 0 );
  //property_defines( wxT("style-shadow"), (el->has() & CPMA_E_HAS_TEXTSTYLE) != 0 );

  
}

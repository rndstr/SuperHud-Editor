#include "miscpropertiesctrl.h"

#include "../common.h"
#include "../mainframe.h"
#include "../hudfilebase.h"
#include "../elementbase.h"
#include "../propertiesnotebookbase.h"

#include "element.h"

BEGIN_EVENT_TABLE(MiscPropertiesCtrl, CPMAPropertyGridBase)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, MiscPropertiesCtrl::OnItemChanged)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, MiscPropertiesCtrl::OnItemChanging)
END_EVENT_TABLE()

MiscPropertiesCtrl::MiscPropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGridBase( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES); // needed for `time' to revert back to inherital value
  AddPage(_("Misc"));
  
  Append( new wxBoolProperty( _("Double line bar"), wxT("doublebar"), false) );
  SetPropertyHelpString( wxT("doublebar"), _("Makes bars two lines. Gets a little funky if RECT Height is set below 6 as the space between the two bars is 4 pixels and each bar requires 1 pixel (4+1+1=6).") );

  Append( new wxBoolProperty( _("Draw 3D model"), wxT("draw3d"), false) );
  SetPropertyHelpString( wxT("draw3d"), _("If both a model and an icon are present, use the model (irrelevant for most stuff).") );


  Append( new wxIntProperty( _("Duration"), wxT("time"), 0) );
  SetPropertyHelpString( wxT("time"), _("How long the element will be displayed for if it doesn't update again. Generally used for item pickups, frag messages, chat, etc.\n\nClear to disable.") );

  // the CheckBox editor does not obey the SetPropertyReadOnly? event vetoing doesn't work, neither. so stick to ComboBox.
  //SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);
}

void MiscPropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();

  // if user is trying to disable this but a parent has it enabled, tell him
  if( name == wxT("doublebar") && !ev.GetValue().GetBool() && el->iget_doublebar() && !el->doublebar() )
  {
    wxMessageBox(CANTDISABLE_MSG);
    ev.Veto();
  }
  else // if user is trying to disable this but a parent has it enabled, tell him
  if( name == wxT("draw3d") && !ev.GetValue().GetBool() && el->iget_draw3d() && !el->draw3d() )
  {
    wxMessageBox(CANTDISABLE_MSG);
    ev.Veto();
  }
}

void MiscPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();
  wxVariant val = prop->GetValue();

  if( name == wxT("doublebar") )
  {
    if( el->flags() & E_PARENT && val.GetBool() )
      wxMessageBox( _("Be aware that the `DOUBLEBAR' you just ticked cannot be disabled on subsequent elements!") );
    // first update current element value
    el->set_doublebar(val.GetBool());
    // if we are disabling, we still want a parental value to be active (i.e. this only changes cell color)
    if( !val.GetBool() )
      SetPropertyValue( wxT("doublebar"), el->iget_doublebar() );
  }
  else if( name == wxT("draw3d") )
  {
    if( el->flags() & E_PARENT && val.GetBool() )
      wxMessageBox( _("Be aware that the `DRAW3D' you just ticked cannot be disabled on subsequent elements!") );
    // first update current element value
    el->set_draw3d(val.GetBool());
    // if we are disabling, we still want a parental value to be active (i.e. this only changes cell color)
    if( !val.GetBool() )
      SetPropertyValue( wxT("draw3d"), el->iget_draw3d() );
  }
  else if( name == wxT("time") )
  {
    wxLogDebug(wxT("`%s' %d"), val.GetString().c_str(), prop->IsValueUnspecified());
    el->add_has( E_HAS_TIME, !prop->IsValueUnspecified() );
    if( !prop->IsValueUnspecified() )
      el->set_time(val.GetInteger());
    SetPropertyValue( wxT("time"), el->iget_time() );
  }
  
  else
    return; // nothing changed

  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void MiscPropertiesCtrl::from_element( ElementBase *el )
{
  CPMAElement *cel = static_cast<CPMAElement*>(el);

  SetPropertyValue( wxT("draw3d"), cel->iget_draw3d() );
  SetPropertyValue( wxT("doublebar"), cel->iget_doublebar() );
  SetPropertyValue( wxT("time"), cel->iget_time() );

  update_layout();
}


void MiscPropertiesCtrl::update_layout()
{
  CPMAElement *el = current_element();

  property_defines(wxT("doublebar"), el->doublebar() );
  property_defines(wxT("draw3d"), el->draw3d() );
  property_defines(wxT("time"), (el->has() & E_HAS_TIME) != 0 );
}


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
#include "miscpropertiesctrl.h"

#include "../common.h"
#include "../mainframe.h"
#include "../hudfilebase.h"
#include "../elementbase.h"
#include "../propertiesnotebookbase.h"
#include "../prefs.h"

#include "element.h"

BEGIN_EVENT_TABLE(MiscPropertiesCtrl, CPMAPropertyGridBase)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, MiscPropertiesCtrl::OnItemChanged)
  EVT_PG_CHANGING(ID_NOTEBOOK_PROPERTIES, MiscPropertiesCtrl::OnItemChanging)
END_EVENT_TABLE()

MiscPropertiesCtrl::MiscPropertiesCtrl( wxWindow *parent ) :
  CPMAPropertyGridBase( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  // needed for `time' to give the user possibility to revert back to inherital value
  // TODO we could also add a [x] button that goes back to inherited!
  // -> NOTE this is now in visbilityproperties
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES); 

  AddPage(_("Misc"));
  
  Append( new wxBoolProperty( _("Double line bar"), wxT("doublebar"), false) );
  SetPropertyHelpString( wxT("doublebar"), _("Makes bars two lines. Gets a little funky if RECT Height is set below 6 as the space between the two bars is 4 pixels and each bar requires 1 pixel (4+1+1=6).") );

  Append( new wxBoolProperty( _("Draw 3D model"), wxT("draw3d"), false) );
  SetPropertyHelpString( wxT("draw3d"), _("If both a model and an icon are present, use the model (irrelevant for most stuff).") );

  Append( new wxPropertyCategory(_("Element description (click!)"), wxT("help")) );

  //SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);

  // a value of 0 (=disable) isn't the same as unspecified (=inherit)
  //Append( new wxIntProperty( _("Duration"), wxT("time"), 0) );
  //SetPropertyHelpString( wxT("time"), _("How long the element will be displayed for if it doesn't update again. Generally used for item pickups, frag messages, chat, etc.\n\nClear to disable.") );
}

void MiscPropertiesCtrl::OnItemChanging( wxPropertyGridEvent& ev )
{
  wxPGProperty *prop = ev.GetProperty();
  if( !prop ) return;

  const CPMAElement *el = current_element();
  if( !el ) return;

  wxString name = prop->GetName();

  // if user is trying to disable this but a parent has it enabled, tell him
  if( name == wxT("doublebar") && !ev.GetValue().GetBool() && el->iget_doublebar() && !el->doublebar() )
  {
    wxMessageBox(CANTDISABLEPROPERTY_MSG);
    ev.Veto();
  }
  else if( name == wxT("draw3d") && !ev.GetValue().GetBool() && el->iget_draw3d() && !el->draw3d() )
  {
    wxMessageBox(CANTDISABLEPROPERTY_MSG);
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
      wxMessageBox( CHECKBOXWARNING_MSG );
    // first update current element value
    el->set_doublebar(val.GetBool());
    // if we are disabling, we still want a parental value to be active (i.e. this only changes cell color)
    if( !val.GetBool() )
      SetPropertyValue( wxT("doublebar"), el->iget_doublebar() );
  }
  else if( name == wxT("draw3d") )
  {
    if( el->flags() & E_PARENT && val.GetBool() )
      wxMessageBox( CHECKBOXWARNING_MSG );
    // first update current element value
    el->set_draw3d(val.GetBool());
    // if we are disabling, we still want a parental value to be active (i.e. this only changes cell color)
    if( !val.GetBool() )
      SetPropertyValue( wxT("draw3d"), el->iget_draw3d() );
  }
  /*
  else if( name == wxT("time") )
  {
    el->add_has( E_HAS_TIME, !prop->IsValueUnspecified() );
    if( !prop->IsValueUnspecified() )
      el->set_time(val.GetInteger());
    SetPropertyValue( wxT("time"), el->iget_time() );
  }
  */
  
  else
    return; // nothing changed

  wxGetApp().hudfile()->set_modified();
  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void MiscPropertiesCtrl::from_element( const ElementBase *el )
{
  const CPMAElement *cel = static_cast<const CPMAElement*>(el);

  SetPropertyValue( wxT("draw3d"), cel->iget_draw3d() );
  SetPropertyValue( wxT("doublebar"), cel->iget_doublebar() );
  //SetPropertyValue( wxT("time"), cel->iget_time() );
#ifndef NDEBUG
  SetPropertyHelpString( wxT("help"), wxString::Format(wxT("[%s] %s"), CPMAElement::type2string(cel->type()).c_str(), el->desc().c_str()) );
#else
  SetPropertyHelpString( wxT("help"), el->desc() );
#endif

  update_layout();

  if( !Prefs::get().var(wxT("props_neverdisable")).bval() )
  {
    EnableProperty(wxT("draw3d"), cel->type() == E_T_ICON || cel->type() == E_T_USERICON || cel->type() == E_T_UNKNOWN );
    EnableProperty(wxT("doublebar"), cel->type() == E_T_BAR || cel->type() == E_T_UNKNOWN );
  }
}


void MiscPropertiesCtrl::update_layout()
{
  const CPMAElement *el = current_element();
  property_defines(wxT("doublebar"), el->doublebar() );
  property_defines(wxT("draw3d"), el->draw3d() );
  //property_defines(wxT("time"), (el->has() & E_HAS_TIME) != 0 );
}


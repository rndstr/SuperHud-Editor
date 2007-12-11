#include "miscpropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementbase.h"
#include "propertiesctrlbase.h"
#include "pgfontsizeproperty.h"

#include "cpma/element.h"

BEGIN_EVENT_TABLE(MiscPropertiesCtrl, wxPropertyGridManager)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, MiscPropertiesCtrl::OnItemChanged)
END_EVENT_TABLE()

MiscPropertiesCtrl::MiscPropertiesCtrl( wxWindow *parent ) :
  wxPropertyGridManager( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE )
{
  //SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
  AddPage(_("Misc"));

  
  Append( wxBoolProperty( _("Double line bar"), wxT("doublebar"), false) );
  SetPropertyAttribute(wxT("doublebar"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("doublebar"), _("Makes bars two lines. Gets a little funky if RECT Height is set below 6 as the space between the two bars is 4 pixels and each bar requires 1 pixel (4+1+1=6).") );

  Append( wxBoolProperty( _("Draw 3D model"), wxT("draw3d"), false) );
  SetPropertyAttribute(wxT("draw3d"),wxPG_BOOL_USE_CHECKBOX,(long)1,wxPG_RECURSE);
  SetPropertyHelpString( wxT("draw3d"), _("If both a model and an icon are present, use the model (irrelevant for most stuff).") );


  Append( wxIntProperty( _("Duration"), wxT("time"), 0) );
  SetPropertyHelpString( wxT("time"), _("How long the element will be displayed for if it doesn't update again. Generally used for item pickups, frag messages, chat, etc.\n\nSet to -1 to disable.") );

  
  SetTargetPage(0);
}


void MiscPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("MiscPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  CPMAElement *el = static_cast<CPMAElement*>(p->curel());
  wxString name = ev.GetPropertyName();
  wxVariant val = ev.GetPropertyValue();

  if( name == wxT("doublebar") )
  {
    bool db = val.GetBool();
    if( el->flags() & E_PARENT && db )
      wxMessageBox( _("Be aware that the `Doublebar' you just ticked cannot be disabled on subsequent elements!") );
    el->set_doublebar(db);
    el->add_has( E_HAS_DOUBLEBAR, db );
  }
  else if( name == wxT("draw3d") )
  {
    bool d3 = val.GetBool();
    if( el->flags() & E_PARENT && d3 )
      wxMessageBox( _("Be aware that the `Draw3d' you just ticked cannot be disabled on subsequent elements!") );
    el->set_draw3d(d3);
    el->add_has( E_HAS_DRAW3D, d3 );
  }
  else if( name == wxT("time") )
  {
    wxLogDebug(wxT("`%s' %d"), val.GetString().c_str(), val.GetInteger());
    el->add_has( E_HAS_TIME, (val.GetInteger() != -1) );
    if( val.GetInteger() != -1 )
      el->set_time(val.GetInteger());
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
  PropertiesCtrlBase *p = wxGetApp().mainframe()->propsctrl();
  if( !p )
  {
    wxLogDebug(wxT("MiscPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  CPMAElement *el = static_cast<CPMAElement*>(p->curel());

  if( el->has() & E_HAS_TIME )
  {
    SetPropertyTextColour( wxT("time"), PROPS_COLOR_NORMAL );
    SetPropertyColour( wxT("time"), PROPS_BGCOLOR_NORMAL );
  }
  else
  {
    SetPropertyTextColour( wxT("time"), PROPS_COLOR_INHERITED );
    SetPropertyColour( wxT("time"), PROPS_BGCOLOR_INHERITED );
  }

  
}


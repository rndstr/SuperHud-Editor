#include "pakfileproperty.h"  

#include "pakfiledialog.h"
#include "common.h"

// FLAGS can be wxPG_NO_ESCAPE if escape sequences shall not be expanded.
//WX_PG_IMPLEMENT_STRING_PROPERTY(PakFileProperty, wxPG_NO_ESCAPE)

WX_PG_IMPLEMENT_DERIVED_PROPERTY_CLASS(PakFileProperty,wxLongStringProperty,const wxString&)
PakFileProperty::PakFileProperty( const wxString& name, const wxString& label, const wxString& value, ePFPFileType type )
  : wxLongStringProperty(name,label,value), m_type(type)
{
    m_flags |= wxPG_NO_ESCAPE;
}
PakFileProperty::~PakFileProperty() { }

wxValidator* wxPG_PROPCLASS(PakFileProperty)::DoGetValidator () const \
{ return (wxValidator*) NULL; }


bool PakFileProperty::OnButtonClick( wxPropertyGrid* propgrid, wxString& value )
{
  PakFileDialog dlg(wxGetApp().mainframe(), wxID_ANY, _("Pak File Browser"));
  switch(m_type)
  {
    case PFP_IMAGE:
      dlg.AddExtImages();
      break;
    case PFP_MODEL:
      dlg.AddExt(wxT("md3"));
      break;
    case PFP_SKIN:
      dlg.AddExt(wxT("skin"));
      break;
    case PFP_ALL:
    default:
      break;
  }

  if( wxID_OK == dlg.ShowModal() )
  {
    value = dlg.GetPakPath();
    return true;
  }
    //
    // TODO: Show dialog, read initial string from value. If changed,
    //   store new string to value and return TRUE.
    //
  return false;
}

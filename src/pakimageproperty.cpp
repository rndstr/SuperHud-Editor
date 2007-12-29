#include "pakimageproperty.h"  

#include "pakfiledialog.h"
#include "common.h"

// FLAGS can be wxPG_NO_ESCAPE if escape sequences shall not be expanded.
WX_PG_IMPLEMENT_STRING_PROPERTY(PakImageProperty, wxPG_NO_ESCAPE)

bool PakImageProperty::OnButtonClick( wxPropertyGrid* propgrid, wxString& value )
{
  PakFileDialog dlg(wxGetApp().mainframe(), wxID_ANY, _("Pak File Browser"));
  dlg.AddExtImages();
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
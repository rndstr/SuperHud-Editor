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

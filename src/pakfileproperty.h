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
#ifndef PAKFILEPRROPERTY_H
#define PAKFILEPPROPERTY_H

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>


typedef enum
{
  PFP_ALL,
  PFP_IMAGE,
  PFP_MODEL,
  PFP_SKIN
} ePFPFileType;

class PakFileProperty : public wxLongStringProperty
{
    WX_PG_DECLARE_DERIVED_PROPERTY_CLASS(PakFileProperty)
  public:
    PakFileProperty( const wxString& name = wxPG_LABEL, const wxString& label = wxPG_LABEL, const wxString& value = wxEmptyString, ePFPFileType type = PFP_ALL);
    virtual ~PakFileProperty();
    virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );
    WX_PG_DECLARE_VALIDATOR_METHODS()
  private:
    ePFPFileType m_type;
};


#endif

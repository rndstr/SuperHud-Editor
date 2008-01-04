#ifndef PAKFILEPROPERTY_H
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

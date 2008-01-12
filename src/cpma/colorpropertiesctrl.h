#ifndef __CPMA_COLORPROPERTIESCTRL_H__
#define __CPMA_COLORPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>

#include "propertygridbase.h"

class ElementBase;

class ColorPropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    ColorPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

  private:
    wxColourProperty *m_color;

    void OnItemChanging( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );

    static wxString textalign_element_to_ui( const wxChar& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif // __FONTPROPERTIESCTRL_H__

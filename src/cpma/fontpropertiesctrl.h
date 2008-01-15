#ifndef __CPMA_FONTPROPERTIESCTRL_H__
#define __CPMA_FONTPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

#include "propertygridbase.h"

class ElementBase;

class FontPropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    FontPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    /// @arg reset Whether to reset the items (delete&append)
    void update_layout( bool reset = true );

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    static wxString textalign_element_to_ui( const wxChar& ta );
    static wxChar textalign_ui_to_element( const wxString& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif // __FONTPROPERTIESCTRL_H__

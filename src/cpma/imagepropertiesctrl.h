#ifndef __CPMA_IMAGEPROPERTIESCTRL_H__
#define __CPMA_IMAGEPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

#include "propertygridbase.h"

class ElementBase;

class ImagePropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    ImagePropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    static bool use_model( CPMAElement *el );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    static wxString textalign_element_to_ui( const wxChar& ta );
    static wxChar textalign_ui_to_element( const wxString& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif // __FONTPROPERTIESCTRL_H__

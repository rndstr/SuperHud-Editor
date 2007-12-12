#ifndef __COLORPROPERTIESCTRL_H__
#define __COLORPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>


class ElementBase;

class ColorPropertiesCtrl : public wxPropertyGridManager
{
  public:
    ColorPropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    static wxString textalign_element_to_ui( const wxChar& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif // __FONTPROPERTIESCTRL_H__
#ifndef __VISIBILITYPROPERTIESCTRL_H__
#define __VISIBILITYPROPERTIESCTRL_H__


#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>


class ElementBase;

class VisibilityPropertiesCtrl : public wxPropertyGridManager
{
  public:
    VisibilityPropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    void OnRightClick( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnElementVisibility( wxCommandEvent& ev );


    DECLARE_EVENT_TABLE()

};


#endif // __VISIBILITYPROPERTIESCTRL_H__

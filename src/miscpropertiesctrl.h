#ifndef __MISCPROPERTIESCTRL_H__
#define __MISCPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>


class ElementBase;

class MiscPropertiesCtrl : public wxPropertyGridManager
{
  public:
    MiscPropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    

    DECLARE_EVENT_TABLE()
};

#endif // __MISCPROPERTIESCTRL_H__

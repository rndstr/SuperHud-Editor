#ifndef __POSITIONPROPERTIESCTRL_H__
#define __POSITIONPROPERTIESCTRL_H__


#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>


class ElementBase;

class PositionPropertiesCtrl : public wxPropertyGridManager
{
  public:
    PositionPropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    void OnRightClick( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );


    DECLARE_EVENT_TABLE()

};


#endif // __POSITIONPROPERTIESCTRL_H__

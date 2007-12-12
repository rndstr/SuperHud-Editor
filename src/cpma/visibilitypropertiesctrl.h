#ifndef __CPMA_VISIBILITYPROPERTIESCTRL_H__
#define __CPMA_VISIBILITYPROPERTIESCTRL_H__


#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

#include "propertygridbase.h"

class ElementBase;

class VisibilityPropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    VisibilityPropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    void OnRightClick( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnElementVisibility( wxCommandEvent& ev );
    void OnAlign( wxCommandEvent& ev );
 



    DECLARE_EVENT_TABLE()

};


#endif // __VISIBILITYPROPERTIESCTRL_H__

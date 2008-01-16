#ifndef __CPMA_VISIBILITYPROPERTIESCTRL_H__
#define __CPMA_VISIBILITYPROPERTIESCTRL_H__


#include "propertygridbase.h"

class ElementBase;

class VisibilityPropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    VisibilityPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

    void ExpandAll( bool expand = true );

  private:
    void OnRightClick( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnElementVisibility( wxCommandEvent& ev );
    void OnAlign( wxCommandEvent& ev );
 



    DECLARE_EVENT_TABLE()

};


#endif // __VISIBILITYPROPERTIESCTRL_H__

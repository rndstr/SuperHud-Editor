#ifndef __CPMA_MISCPROPERTIESCTRL_H__
#define __CPMA_MISCPROPERTIESCTRL_H__

#include "propertygridbase.h"

class ElementBase;

class MiscPropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    MiscPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    

    DECLARE_EVENT_TABLE()
};

#endif // __MISCPROPERTIESCTRL_H__

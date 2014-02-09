#ifndef CPMA_MISCPROPERTIESCTRL_H
#define CPMA_MISCPROPERTIESCTRL_H

#include "cpma_propertygrid.h"

class ElementBase;

class MiscPropertiesCtrl : public CPMAPropertyGrid
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

#endif

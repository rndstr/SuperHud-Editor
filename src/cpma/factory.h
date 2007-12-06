#ifndef __CPMA_FACTORY_H__
#define __CPMA_FACTORY_H__

#include "../factorybase.h"

class CPMAFactory : public FactoryBase
{
  public:
    bool              init();
    ElementsCtrlBase* create_elementsctrl( wxWindow *parent );
    DisplayCtrlBase*  create_displayctrl( wxWindow *parent );
    HudFileBase*      create_hudfile();

};

#endif // __CPMA_FACTORY_H__

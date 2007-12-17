#ifndef __CPMA_FACTORY_H__
#define __CPMA_FACTORY_H__

#include "../factorybase.h"

class CPMAFactory : public FactoryBase
{
  public:
    bool                init();
    ElementsCtrlBase*   create_elementsctrl( wxWindow *parent ) const;
    PropertiesNotebookBase* create_propertiesnotebook( wxWindow *parent ) const;
    DisplayCtrlBase*    create_displayctrl( wxWindow *parent ) const;
    HudFileBase*        create_hudfile() const;
    PakManagerBase*     create_pakmanager() const;

};

#endif // __CPMA_FACTORY_H__

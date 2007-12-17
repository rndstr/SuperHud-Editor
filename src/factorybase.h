#ifndef __FACTORYBASE_H__
#define __FACTORYBASE_H__

class ElementsCtrlBase;
class PropertiesNotebookBase;
class DisplayCtrlBase;
class HudFileBase;
class wxWindow;
class PakManagerBase;

class FactoryBase
{
  public:

    /// this can be used to initialize game specific stuff
    virtual bool init() { return true; }
    /// this can be used to shut down game specific stuff
    virtual void shutdown() {  }
    virtual ElementsCtrlBase*   create_elementsctrl( wxWindow *parent ) const = 0;
    virtual PropertiesNotebookBase* create_propertiesnotebook( wxWindow *parent ) const = 0;
    virtual DisplayCtrlBase*    create_displayctrl( wxWindow *parent ) const = 0;
    virtual HudFileBase*        create_hudfile() const = 0;
    virtual PakManagerBase*     create_pakmanager() const = 0;
};

#endif // __FACTORYBASE_H__


#include "factory.h"

#include "hudspecs.h"
#include "elementsctrl.h"
#include "propertiesctrl.h"
#include "displayctrl.h"
#include "hudfile.h"


bool CPMAFactory::init()
{
  return CPMAHudSpecs::get().load();
}

ElementsCtrlBase* CPMAFactory::create_elementsctrl(wxWindow *parent)
{
  return new CPMAElementsCtrl(parent);
}

PropertiesCtrlBase* CPMAFactory::create_propertiesctrl(wxWindow *parent)
{
  return new CPMAPropertiesCtrl(parent);
}

DisplayCtrlBase* CPMAFactory::create_displayctrl(wxWindow *parent)
{
  return new CPMADisplayCtrl(parent);
}

HudFileBase* CPMAFactory::create_hudfile()
{
  return new CPMAHudFile;
}

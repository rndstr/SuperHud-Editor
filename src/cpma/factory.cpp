#include "factory.h"

#include "hudspecs.h"
#include "elementsctrl.h"
#include "displayctrl.h"
#include "hudfile.h"


bool CPMAFactory::init()
{
  CPMAHudSpecs::get().load();
  return true;
}

ElementsCtrlBase* CPMAFactory::create_elementsctrl(wxWindow *parent)
{
  return new CPMAElementsCtrl(parent);
}

DisplayCtrlBase* CPMAFactory::create_displayctrl(wxWindow *parent)
{
  return new CPMADisplayCtrl(parent);
}

HudFileBase* CPMAFactory::create_hudfile()
{
  return new CPMAHudFile;
}

#include "factory.h"

#include "hudspecs.h"
#include "elementsctrl.h"
#include "propertiesnotebook.h"
#include "displayctrl.h"
#include "hudfile.h"


bool CPMAFactory::init()
{
  return CPMAHudSpecs::get().load();
}

ElementsCtrlBase* CPMAFactory::create_elementsctrl(wxWindow *parent) const
{
  return new CPMAElementsCtrl(parent);
}

PropertiesNotebookBase* CPMAFactory::create_propertiesnotebook(wxWindow *parent) const
{
  return new CPMAPropertiesNotebook(parent);
}

DisplayCtrlBase* CPMAFactory::create_displayctrl(wxWindow *parent) const
{
  return new CPMADisplayCtrl(parent);
}

HudFileBase* CPMAFactory::create_hudfile() const
{
  return new CPMAHudFile;
}


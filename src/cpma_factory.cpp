#include "cpma_factory.h"

#include "cpma_elementsctrl.h"
#include "cpma_propertiesnotebook.h"
#include "cpma_displayctrl.h"
#include "cpma_hudfile.h"


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


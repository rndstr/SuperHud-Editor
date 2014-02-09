#include "q4max_factory.h"

#include "q4max_elementsctrl.h"
#include "q4max_propertiesnotebook.h"
#include "q4max_displayctrl.h"
#include "q4max_hudfile.h"



ElementsCtrlBase* Q4MAXFactory::create_elementsctrl(wxWindow *parent) const
{
  return new Q4MAXElementsCtrl(parent);
}

PropertiesNotebookBase* Q4MAXFactory::create_propertiesnotebook(wxWindow *parent) const
{
  return new Q4MAXPropertiesNotebook(parent);
}

DisplayCtrlBase* Q4MAXFactory::create_displayctrl(wxWindow *parent) const
{
  return new Q4MAXDisplayCtrl(parent);
}

HudFileBase* Q4MAXFactory::create_hudfile() const
{
  return new Q4MAXHudFile;
}


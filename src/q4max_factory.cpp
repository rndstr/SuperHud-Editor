// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#include "q4max_factory.h"

#include "q4max_elementsctrl.h"
#include "q4max_propertiesnotebook.h"
#include "q4max_displayctrl.h"
#include "q4max_hudfile.h"



ElementsCtrlBase* Q4MAXFactory::create_elementsctrl(wxWindow *parent) const
{
  return new MAXElementsCtrl(parent);
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


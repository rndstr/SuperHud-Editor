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


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
#include "command.h"

#include "superhudeditor.h"
#include "elementsctrlbase.h"

void ReorderCommand::takepre()
{
  m_undoels = wxGetApp().hudfile()->elements();
}

void ReorderCommand::takepost()
{
  m_doels = wxGetApp().hudfile()->elements();
}

bool ReorderCommand::Do()
{
  wxGetApp().hudfile()->m_els = m_doels;
  wxGetApp().elementsctrl()->list_refresh(wxGetApp().hudfile()->elements());
  // refreshing the hud changes selection but does not pull a (de)select event
  wxGetApp().mainframe()->elementsctrl()->OnSelectionChanged();
  return true;
}

bool ReorderCommand::Undo()
{
  wxGetApp().hudfile()->m_els = m_undoels;
  wxGetApp().elementsctrl()->list_refresh(wxGetApp().hudfile()->elements());
  // refreshing the hud changes selection but does not pull a (de)select event
  wxGetApp().mainframe()->elementsctrl()->OnSelectionChanged();
  return true;
}

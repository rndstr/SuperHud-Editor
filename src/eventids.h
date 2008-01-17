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
#ifndef EVENTIDS_H
#define EVENTIDS_H

enum
{
	ID_BTN_Q4MAX = wxID_HIGHEST+1,
	ID_BTN_CPMA,

  ID_TIMER_ANIM,

  ID_BTN_INSERT,
  ID_PASTE_WITHRECT,

  ID_BTN_ELEMENT_ENABLE,
  ID_BTN_ELEMENT_DISABLE,

  ID_BTN_ALIGN_LEFT,
  ID_BTN_ALIGN_CENTER,
  ID_BTN_ALIGN_RIGHT,
  ID_BTN_ALIGN_TOP,
  ID_BTN_ALIGN_MIDDLE,
  ID_BTN_ALIGN_BOTTOM,

  ID_BTN_SELECTGAMEDIR,

  ID_MENU_TOOLS_SWITCHGAME,
  ID_MENU_TOOLS_CONVERT,
  ID_MENU_TOOLS_SNAPELEMENTS,
  ID_MENU_TOOLS_SNAPGRID,
  ID_MENU_TOOLS_PREFERENCES,

  ID_MENU_VIEW_DEFAULTPERSPECTIVE,
  ID_MENU_VIEW_GRID,
  ID_MENU_VIEW_HELPER,
  /// all the panes that can be switched on/off
  /// keep first and last in order as we add evthandler with range
  /// @{
  ID_MENU_VIEW_CONFIGPREVIEW,
  ID_MENU_VIEW_TOOLBAR_FILE,
  /// @}
  ID_MENU_VIEW_FOCUSELEMENTS,
  ID_MENU_VIEW_FOCUSHUD,
  ID_MENU_VIEW_FOCUSPROPERTIES,

  ID_MENU_HELP_UPDATE,
  ID_MENU_HELP_TIP,


  ID_NOTEBOOK_PROPERTIES,

  ID_LISTCTRL_ELEMENTS,
  ID_TEXTCTRL_CONFIGPREVIEW,

  ID_LISTCTRL_PAKFILEDIALOG,
  ID_BTN_PAKFILEDLG_GODIRUP,

  // prefs
  ID_BTN_Q3_GAMEDIR_BROWSE,
  ID_BTN_Q3_BACKGROUND_BROWSE,
  ID_BTN_RESETDIALOGS,
  ID_BTN_GRIDCOLOR,
  ID_BTN_FILLSEL,
  ID_BTN_OUTLINESEL,
  ID_BTN_FILL,
  ID_BTN_OUTLINE,
  ID_CHOICE_ASPECTRATIO,
  ID_CHECK_Q3_BACKGROUND_USEDEFAULT,


  ID_INSERT_NOTUNIQ,
  ID_INSERT_NOTUNIQ_END = ID_INSERT_NOTUNIQ+9
};

#endif //  EVENTIDS_H


// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef ELEMENTSLISTCTRL_H
#define ELEMENTSLISTCTRL_H

#include "common.h"

#include "elementbase.h"

#include <wx/listctrl.h>
#include <wx/imaglist.h>

enum
{
  E_LIST_IMG_DISABLED = 0,
  E_LIST_IMG_ENABLED = 1,
  E_LIST_IMG_COLLTITLE = 2,
  E_LIST_IMG_COLLITEM = 3
};








/// adapted wxListCtrl that adds behaviour 
/// for use on hud elements list
class ElementsListCtrl : public wxListCtrl
{
  public:
    ElementsListCtrl( wxWindow *parent );
    virtual ~ElementsListCtrl() {}

    


  private:
    
    wxImageList   m_imglist;

  DECLARE_EVENT_TABLE()
};


#endif // ELEMENTSLISTCTRL_H


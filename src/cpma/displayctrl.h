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
#ifndef CPMADISPLAYCTRL_H
#define CPMADISPLAYCTRL_H

#include "../displayctrlbase.h"

class CPMADisplayCtrl : public DisplayCtrlBase
{
    

    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    
  public:
    CPMADisplayCtrl( wxWindow *parent );
    ~CPMADisplayCtrl();

    void init();

  protected:
  
    int width() const { return CPMADisplayCtrl::WIDTH; }
    int height() const { return CPMADisplayCtrl::HEIGHT; }

};

#endif


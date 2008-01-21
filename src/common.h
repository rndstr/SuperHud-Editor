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

/// includes some common headers
#ifndef COMMON_H
#define COMMON_H

#define ENABLE_Q4MAX 1
#define ENABLE_CPMA 1

#define HAS_CMDPROC 0

#ifdef WIN32
  #define HAS_WEBUPDATER 1
#else
  #define HAS_WEBUPDATER 0
#endif

#if ENABLE_Q4MAX && ENABLE_CPMA
  #define HAS_MULTIPLE_GAMES 1
#else
  #define HAS_MULTIPLE_GAMES 0
#endif

#if ENABLE_CPMA
  #define CPMA_BACKWARD_COMPATIBILITY_142 1
#endif

#include <wx/wx.h>

#include "superhudeditor.h"
#include "eventids.h"
#include "mainframe.h"

#include "exceptions.h"
#include "misc.h"


//#include "factorybase.h"


#endif // COMMON_H


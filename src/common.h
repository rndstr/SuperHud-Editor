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


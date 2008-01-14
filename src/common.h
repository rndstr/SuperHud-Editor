/// includes some common headers
#ifndef COMMON_H
#define COMMON_H

#define ENABLE_Q4MAX 0
#define ENABLE_CPMA 1


#if ENABLE_Q4MAX && ENABLE_CPMA
  #define HAS_MULTIPLE_GAMES 1
#else
  #define HAS_MULTIPLE_GAMES 0
#endif

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/log.h>



#include "superhudeditor.h"
#include "factorybase.h"
#include "mainframe.h"
#include "eventids.h"
#include "artprovider.h"
#include "exceptions.h"
#include "misc.h"



// some general messages
const wxString BUG_MSG = _("Bug detected, please contact the author and tell him the error message.\n\nError:\n");
const wxString CANTDISABLEPROPERTY_MSG = _("There is no way to disable this property on that element as a parent element specifies it.\nYou should remove the property on the parent `!DEFAULT' element.");
const wxString INVALIDGAME_MSG = _("No valid game selected?");
const wxString CHECKBOXWARNING_MSG = _("Be aware that you won't be able to disable that property on subsequents element!");

#include <wx/dir.h>
#include <wx/filename.h>

/// get hidden and files (and don't recurse into dirs)
const int GETALLFILES_FLAGS = wxDIR_FILES|wxDIR_HIDDEN;
const wxString PATH_SEP = wxFileName::GetPathSeparator();


#endif // COMMON_H


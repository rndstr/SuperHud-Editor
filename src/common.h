/// includes some common headers
#ifndef __COMMON_H__
#define __COMMON_H__

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/log.h>

#ifdef __WXMAC__
  #include "OpenGL/gl.h"
  #include "OpenGL/glu.h" // checkcheck
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include "superhudeditor.h"
#include "mainframe.h"
#include "eventids.h"
#include "artprovider.h"
#include "exceptions.h"
#include "prefs.h"
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


#endif // __COMMON_H__


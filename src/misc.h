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
#ifndef MISC_H
#define MISC_H

#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/variant.h>
#include <string>

// some general messages
const wxString BUG_MSG = _("Bug detected, please contact the author and tell him the error message.\n\nError:\n");
const wxString CANTDISABLEPROPERTY_MSG = _("There is no way to disable this property on that element as a parent element specifies it.\nYou should remove the property on the parent `!DEFAULT' element.");
const wxString INVALIDGAME_MSG = _("No valid game selected?");
const wxString CHECKBOXWARNING_MSG = _("Be aware that you won't be able to disable that property on subsequents element!");


/// get hidden and files (and don't recurse into dirs)
const int GETALLFILES_FLAGS = wxDIR_FILES|wxDIR_HIDDEN;
const wxString PATH_SEP = wxFileName::GetPathSeparator();

namespace she
{
  
  template<class T> 
  void ltrim(std::basic_string<T> &s, const std::basic_string<T> &trimset=" \n\r\t") 
  { 
    s.erase(0, s.find_first_not_of(trimset)); 
  } 

  template<class T> 
  void rtrim(std::basic_string<T> &s, const std::basic_string<T> &trimset=" \n\r\t") 
  { 
    s.resize(s.find_last_not_of(trimset) + 1); // (many group discussions on whether this is legit) 
  } 

  template<class T> 
  void trim(std::basic_string<T> &s, const std::basic_string<T> &trimset=" \n\r\t") 
  { 
    rtrim(s, trimset); 
    ltrim(s, trimset); 
  }

  void wxLTrim( wxStringBase& s, const wxStringBase& trimset=wxT(" \n\r\t"));
  void wxRTrim( wxStringBase& s, const wxStringBase& trimset=wxT(" \n\r\t"));
  void wxTrim( wxStringBase& s, const wxStringBase& trimset=wxT(" \n\r\t"));
  wxString pretty_print_float(float f, int aftercomma = 3);

  void draw_rect( const wxRect& r, bool texcoords =false );
  wxString safe_filename( const wxString& filename );

  bool exists_in_zip( const wxString& filepath, const wxString& entrypath );
  bool load_from_zip( char **buf, const wxString& filepath, const wxString& entrypath, size_t *size=0 );

  wxString file_ext( const wxString& str );
  bool is_image_ext( const wxString& ext );

  long bitmap_type_by_ext( const wxString& ext );

  int common_start( const wxString& lhs, const wxString& rhs );

  /// convert a ratiovalue (a:b) to a real double
  /// @returns True if successfull otherwise false
  bool ratio_string2double( const wxString& ratio, double *val );

  /// extracts the width and height from `w:h'
  /// @returns True if successfull otherwise false
  bool ratio_string2long( const wxString& ratio, long *w, long *h );

  wxVariant colour2variant( const wxColour& col );

  template < typename T >
  void clamp( T& val, const T& min, const T& max )
  {
    val = std::min<T>(max, val);
    val = std::max<T>(min, val);
  }


  /// Checks if the latest version is newer than our.
  /// @returns <0 if our<latest, =0 if our=latest, >0 if our>latest.
  int versioncheck( int major, int minor, int release, const wxString& type );
};

#endif

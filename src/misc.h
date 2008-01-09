// Copyright (C) 2006 Roland Schilter <rolansch@student.ethz.ch>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#ifndef MISC_H
#define MISC_H

#include <wx/string.h>
#include <wx/gdicmn.h>
#include <string>

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


  /// Checks if the latest version is newer than our.
  /// @return <0 if our<latest, =0 if our=latest, >0 if our>latest.
  //int versioncheck( int major, int minor, int release, const wxString& type );
};

#endif

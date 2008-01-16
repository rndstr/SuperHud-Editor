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

#include "misc.h"
#include "common.h"

#include <wx/mstream.h>
//#include <wx/wfstream.h>
#include <wx/fs_zip.h>
#include <wx/tokenzr.h>
#include <wx/log.h>



#ifdef __WXMAC__
  #include "OpenGL/gl.h"
  #include "OpenGL/glu.h" // checkcheck
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

namespace she
{

void wxLTrim( wxStringBase& s, const wxStringBase& trimset /*=" \n\r\t"*/)
{
  s.erase(0, s.find_first_not_of(trimset)); 
}

void wxRTrim( wxStringBase& s, const wxStringBase& trimset /*=" \n\r\t"*/)
{
  s.resize( s.find_last_not_of(trimset)+1 );
}

void wxTrim( wxStringBase& s, const wxStringBase& trimset /*=" \n\r\t"*/)
{
  she::wxRTrim(s, trimset);
  she::wxLTrim(s, trimset);
}

wxString pretty_print_float(float f, int aftercomma /*= 3*/)
{
  wxString sac = wxString::Format( wxT("%d"), aftercomma);
  wxString sf = wxString::Format(wxT("%.") + sac + wxT("f"), f );
  size_t pos = sf.find_last_not_of(wxT("0"));
  if( pos != wxString::npos )
    sf = sf.substr(0, pos+1);
  if( sf.Last() == '.' )
    sf = sf.substr(0, sf.length()-1);
  return sf;
}

void draw_rect( const wxRect& r, bool texcoords /*=false*/ )
{
  // was: bot+1, ri+1
glBegin( GL_QUADS );
  if( texcoords) glTexCoord2i(0,1);
  glVertex2i( r.GetLeft(), r.GetBottom() );
  if( texcoords) glTexCoord2i(1,1);
  glVertex2i( r.GetRight(), r.GetBottom() );
  if( texcoords) glTexCoord2i(1,0);
  glVertex2i( r.GetRight(), r.GetTop() );
  if( texcoords) glTexCoord2i(0,0);
  glVertex2i( r.GetLeft(), r.GetTop()   );
glEnd();
}


wxString safe_filename( const wxString& filename )
{
  wxString ret;
  for( size_t i=0; i < filename.length(); ++i )
  {
    if( 
        filename[i] >= '0' && filename[i] <= '9' || 
        filename[i] >= 'a' && filename[i] <= 'z' ||
        filename[i] == '_'
      ) 
    {
      ret += wxString(filename[i], 1);
    }
    else if( filename[i] == '\\' || filename[i] == '/' )
      ret += wxT("-");
  }
  return ret;
}

bool exists_in_zip( const wxString& filepath, const wxString& entrypath )
{
  wxFileSystem fs;
  wxFSFile *file = 0;
  {
    wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
    file = fs.OpenFile( filepath + wxT("#zip:") + entrypath );
  }
  
  if( 0 == file )
    return false;
  delete file;
  wxLogDebug( wxT("yes") );
  return true;
}

bool load_from_zip( char **buf, const wxString& filepath, const wxString& entrypath, size_t *psize )
{
  wxFileSystem fs;
  wxFSFile *file = 0;
  {
    wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
    file = fs.OpenFile( filepath + wxT("#zip:") + entrypath );
  }
  if( 0 == file )
    return false;
  
  wxStreamBuffer streambuf( *file->GetStream(), wxStreamBuffer::read ); 
 
  size_t size = file->GetStream()->GetSize(); 
  if( psize )
    *psize = size;
  if (!size ) 
    return false; 
 
  *buf = new char[ size ];                             
  streambuf.Read( *buf, size );
  delete file;

  return true;
}


wxString file_ext( const wxString& fp )
{
  size_t pos = fp.find_last_of( wxT("/\\.") );
  if( fp[pos] != '.' )
    return wxT(""); // no extension
  return (pos == wxString::npos ? 
      wxT("") 
      :
      fp.substr( pos+1, fp.length()-pos-1 ));
}

bool is_image_ext( const wxString& ext )
{
  return (wxT("tga") == ext || wxT("jpg") == ext || wxT("png") == ext ||
    wxT("bmp") == ext || wxT("gif") == ext || wxT("xpm") == ext || wxT("ico") == ext ||
    wxT("cur") == ext);
}

int common_start( const wxString& lhs, const wxString& rhs )
{
  size_t i=0;
  while( lhs[i] == rhs[i] && i < lhs.size() && i < rhs.size() ) 
    ++i;
  return i;
}

long bitmap_type_by_ext( const wxString& ext )
{
  // for some reason we need the bitmap type specified in wxImage::LoadFile
  // for tgas.. won't find it otherwise
  if( ext == wxT("tga") )
    return wxBITMAP_TYPE_TGA;
  return wxBITMAP_TYPE_ANY; // auto
}

bool ratio_string2double( const wxString& ratio, double *val )
{
  wxASSERT(val);
  if( ratio.ToDouble(val) )
    return true;
  // maybe it's a:b ?
  size_t pos = ratio.Find(wxT(":"));
  if( pos != wxString::npos )
  {   
    long lhs, rhs;
    if( !ratio.Left(pos).ToLong(&lhs) || !ratio.Right(ratio.length() - pos - 1).ToLong(&rhs) )
      return false;
    *val = static_cast<double>(lhs)/static_cast<double>(rhs);
    return true;
  }
  return false;
}

wxVariant colour2variant( const wxColour& col )
{
  wxVariant var;
  var << col;
  return var;
}

/// Checks if the latest version is newer than our.
/// @return <0 if our<latest, =0 if our=latest, >0 if our>latest.
int versioncheck( int major, int minor, int release, const wxString& type ) 
{
#define vccmp(our,latest) if(our>latest) return 1; else if(our<latest) return -1;

  vccmp(APP_MAJOR_VERSION, major);
  vccmp(APP_MINOR_VERSION, minor);
  vccmp(APP_RELEASE_NUMBER, release);
  if( !APP_RELEASE_TYPE.empty() && type.empty() )
    return -1;
  if( APP_RELEASE_TYPE.empty() && !type.empty() )
    return 1;

  return APP_RELEASE_TYPE.CmpNoCase(type);
}

};

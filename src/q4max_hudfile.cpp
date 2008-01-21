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

#include "q4max_hudfile.h"
#include "common.h"
#include "elementsctrlbase.h"
#include "pakmanager.h"
#include "hudspecs.h"

#include "q4max_element.h"

#include <wx/txtstrm.h>
#include <wx/datetime.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/tokenzr.h>


//#include <wx/stdpaths.h>
//#include <wx/dir.h>
//#include <algorithm>


Q4MAXHudFile::Q4MAXHudFile() : 
  HudFileBase()
{
}


/*
bool Q4MAXHudFile::load( const wxString& filename )
{
  wxLogDebug(wxT("Loading HUD: ") + filename);

  wxString content;
  char *buf;
  size_t size;

  if( !PakManager::get().load( &buf, filename, PM_SEARCH_EVERYWHERE, &size ) )
  {
    wxLogError(_("Couldn't find/load HUD: %s"), filename.c_str());
    return false;
  }


  wxMemoryInputStream mis( buf, size );

  m_load_prevel = 0;

  load_default_elements();
  wxTextInputStream tis( mis );
  wxString line;
  size_t pos;
  // default opts

  // read file line by line and remove `#' comments
  while(!mis.Eof())
  {
    line = tis.ReadLine();
    she::wxTrim(line);
    if( 0 == line.length() || line[0] == '#' )
    { // check for options
      if( (pos = line.find(wxT("="))) != wxString::npos )
      {
        wxString optname = line.Mid(1, pos-1);
        wxString optval = line.Mid(pos+1);
        she::wxTrim(optname);
        she::wxTrim(optval);
        wxLogDebug(wxT("Q4MAXHudFile::load - have found option: ") + optname + wxT(" = ") + optval);
        if( optname == wxT("version") )
          m_opt_version = optval;
        else if( optname == wxT("view_aspectratio") )
          m_opt_aspectratio = optval;
        else
          wxLogDebug(wxT("Q4MAXHudFile::load - WARNING: invalid option ") + optname);
      }
      continue;
    }
    if( (pos = line.find( wxT("#") )) != wxString::npos )
      line = line.substr( 0, pos );
    content += line;
    content += wxT("\n");
  }

  she::wxTrim( content ); 
  try
  {
    wxStringTokenizer tok( content, wxT("}") );
    while(tok.HasMoreTokens())
      parse_item( tok.GetNextToken() );
  }
  catch( hudfile_parse_error& err )
  {
    wxString str = wxString::Format(_("ERROR while parsing `%s'"), filename.c_str());
    str += wxString(err.what(), wxConvUTF8);
    wxLogError( str );
    load_default_elements();
    return false;
  }
  // removeo all non-unique elements that aren't enabled
  for( it_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( ((*it)->flags() & E_NOTUNIQ) && !(*it)->is_enabled() )
      m_els.erase( it );
  }

  // remove E_DRAWNEVER and E_PARENT from end
  ElementBase *el;
  for( int i = m_els.size()-1; i >= 0; --i )
  {
    el = m_els[i];
    if( el->flags() & E_DRAWNEVER && el->flags() & E_PARENT )
    {
      m_els.erase(m_els.begin() + i);
      delete el;
    }
    else
      break;
  }
  
  m_filename = filename;
  wxGetApp().mainframe()->update_elementsctrl();
  m_modified = false;
  return true;
}
*/


bool Q4MAXHudFile::read_properties( ElementBase *hi, const wxString& props )
{
  wxString p;
  bool quoting = false;
  for( wxString::const_iterator cit = props.begin(); cit != props.end(); ++cit )
  {
    if( *cit == '"' )
    {
      if( quoting )
      { // close
        p += *cit;
        p += ';';
        quoting = false;
        continue;
      }
      else // !quoting
        quoting = true;
    }
    p += *cit;
  }
  return HudFileBase::read_properties( hi, p );
}


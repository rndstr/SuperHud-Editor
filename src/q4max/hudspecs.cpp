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
#include "hudspecs.h"

#include "element.h"
#include "../prefs.h"
#include "../common.h"
#include "../pakmanager.h"

#include <wx/arrstr.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/sstream.h>

bool CPMAHudSpecs::load()
{
  wxString hudspecs = Prefs::get().var(wxT("q3_hudspecs")).sval();
  
  m_items.clear();

  char *buf;
  size_t size;
  if( !PakManager::get().load( &buf, hudspecs, PM_SEARCH_APPFILE, &size ) )
  {
    wxLogError(_("Couldn't find/load hudspecs file: %s"), hudspecs.c_str());
    return false;
  }
  wxStringInputStream sis( wxString(buf, wxConvUTF8, size) );
  
  
  wxTextInputStream tis(sis);
  wxString line;
  size_t pos;
  wxString name; // name
  wxString value; // whole options line
  wxString key; // cmd of option
  wxString arg; // optionally an argument (if a '=' is found)
  CPMAElement *hi = 0;
  wxString desc = wxT(""); // desc which is written as _desc=... on a single line
  while(!sis.Eof())
  {
    line = tis.ReadLine();
    she::wxTrim(line);
    if( 0 == line.length() || line[0] == '#' )
      continue;

    if( (pos = line.find( wxT("=") )) == wxString::npos )
    { // chcek if already one is in there with same name (and has HIF_NOTUNIQ), if so copy flags (of nearest previous one).
      she::wxTrim(line);
      cit_hsitems from = m_items.end();
      for( cit_hsitems cit = m_items.begin(); cit != m_items.end(); ++cit )
        if( cit->name.CmpNoCase( line ) == 0 && (cit->flags & E_NOTUNIQ) )
          from = cit;
    
      if( from != m_items.end() )
      {
        m_items.push_back( hsitem_s( *from ) );
        //if( (flags & HIF_MULTDEPENDALIGN) || (flags & HIF_MULTWIDTH) || (flags & HIF_MULTHEIGHT) )   
        // ...
      }
      else
      {
        wxLogError( wxT("Found invalid element in hudspecs file: no previous item found that specifies the options for `%s', ignoring."), name.c_str());
      }
      continue;
    }
    name = line.substr(0, pos);
    she::wxTrim(name);
    value = line.substr(pos+1, line.length() - pos - 1);
    if( name.find(HS_IGNOREPREFIX) == 0 )
    {
      if( name.Cmp(HS_DESCNAME) == 0 )
        desc = value;
      continue;
    }
    she::wxTrim(value, wxT(" \"'"));

    wxStringTokenizer options( value, wxT(":") );
    int flags = E_NONE;
    int overwrites = E_PROPERTIES_DEFAULT;
    bool enable = false;
    int type = E_T_UNKNOWN;
    wxString text;
    wxString icon;
    while( options.HasMoreTokens() )
    {
      key = options.GetNextToken();
      arg = wxT("");
      pos = key.find( wxT("=") );
      if( pos != wxString::npos )
      {
        arg = key.substr(pos+1, key.length() - pos - 1);
        key = key.substr(0, pos);
      }
      she::wxTrim(key);
      she::wxTrim(arg);
      if( key.empty() )
        continue;
      
      if( key.CmpNoCase(wxT("short"))==0 ) flags |= E_SHORT;
      else if( key.CmpNoCase(wxT("enable"))==0 ) enable = true;
      else if( key.CmpNoCase(wxT("enablealways"))==0 ) flags |= E_ENABLEALWAYS;
      else if( key.CmpNoCase(wxT("notuniq"))==0 ) { flags |= E_NOTUNIQ; m_notuniqs.push_back( name ); }
      else if( key.CmpNoCase(wxT("nodefault"))==0 ) { flags |= E_NODEFAULT; }
      else if( key.CmpNoCase(wxT("drawnever"))==0 ) flags |= E_DRAWNEVER;
      else if( key.CmpNoCase(wxT("drawback"))==0 ) flags |= E_DRAWBACK;
      else if( key.CmpNoCase(wxT("drawfront"))==0 ) flags |= E_DRAWFRONT;
      else if( key.CmpNoCase(wxT("drawalways"))==0 ) flags |= E_DRAWALWAYS;
      else if( key.CmpNoCase(wxT("parent"))==0 ) flags |= E_PARENT;
      else if( key.CmpNoCase(wxT("noinherit"))==0 ) flags |= E_NOINHERIT;
      else if( key.CmpNoCase(wxT("overwrites"))==0 ) sscanf( arg.mb_str(), "%i", &overwrites );
      else if( key.CmpNoCase(wxT("type"))==0 )
      {
        if( arg.CmpNoCase(wxT("icon"))==0 )
          type = E_T_ICON;
        else if( arg.CmpNoCase(wxT("usericon"))==0 )
          type = E_T_USERICON;
        else if( arg.CmpNoCase(wxT("text"))==0 )
          type = E_T_TEXT;
        else if( arg.CmpNoCase(wxT("bar"))==0 )
          type = E_T_BAR;
        else if( arg.CmpNoCase(wxT("weaponlist"))==0 )
          type = E_T_WEAPONLIST;
      }
      else if( key.CmpNoCase(wxT("text"))==0 )
      {
        text = arg;
        text.Replace(wxT(";"), wxT(":"), true);
      }
      else if( key.CmpNoCase(wxT("icon"))==0 ) icon = arg;
      else
        wxLogError( wxT("Found unknown option `") + key + wxT("' for element `") + name + wxT("' in hudspec file, was ignored.") );
    }
    // NOTE: that items with HIF_NODEFAULT will still be stored here. but we won't add them upon File->New
    wxLogDebug( wxT("Adding: ") + name );
    
    // some sanitizing...
    if( type == E_T_ICON && icon.empty() )
      wxLogDebug( wxT("WARNING: missing `icon' in hudspecs file: ") + name  );
    if( type == E_T_TEXT && text.empty() )
      wxLogDebug( wxT("WARNING: missing `text' in hudspecs file: ") + name  );
    m_items.push_back( hsitem_s(name, desc, flags, overwrites, enable, type, text, icon) );
  }

  return true;
}

const hsitem_s* CPMAHudSpecs::find_item( const wxString& name ) const
{
  for( cit_hsitems cit = m_items.begin(); cit != m_items.end(); ++cit )
  {
    if( cit->name.CmpNoCase(name) == 0 )
      return &(*cit);
  }
  return 0;
}

CPMAHudSpecs& CPMAHudSpecs::get() 
{
  static CPMAHudSpecs pinstance;
  return pinstance;
}


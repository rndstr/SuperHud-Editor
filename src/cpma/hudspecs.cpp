#include "hudspecs.h"

#include "element.h"
#include "../prefs.h"
#include "../common.h"

#include <wx/arrstr.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>

bool CPMAHudSpecs::load()
{
  wxString hudspecs = Prefs::get().hudspecs;
  wxString fpath; // full hudspecs filepath
  m_items.clear();

  if( hudspecs.empty() )
  { // enumerate one
    wxArrayString files;
    size_t count = wxDir::GetAllFiles(wxStandardPaths::Get().GetDataDir() + wxT("/data/cpma/hudspecs/"), &files, wxT("*.dat"), wxDIR_FILES);
    if( wxDir::Exists( wxStandardPaths::Get().GetUserDataDir() + wxT("/data/cpma/hudspecs/") ) )
      count += wxDir::GetAllFiles(wxStandardPaths::Get().GetUserDataDir() + wxT("/data/cpma/hudspecs/"), &files, wxT("*.dat"), wxDIR_FILES);
    if( count == 1 )
      fpath = files[0];
    else if( count > 1 )
    {
      fpath = wxT("");
      wxString fname;
      wxString curmax = wxT("0");
      size_t pos;
      for( unsigned int i=0; i < files.size(); ++i )
      {
        pos = files[i].find_last_of(wxT("\\/"));
        fname = files[i].substr(pos+1, files[i].length() - pos - 1);
        if( curmax.CmpNoCase(fname) < 0 )
        {
          curmax = fname;
          fpath = files[i];
        }
      }
    }
    if( fpath.empty() )
    {
      wxLogError( _("Cannot find any hudspecs file. Try re-installing and if this doesn't fix it contact the author.") );
      return false;
    }
  }
  else 
  {
    fpath = wxStandardPaths::Get().GetDataDir() + wxT("/data/cpma/hudspecs/") + hudspecs;
    
    if( !wxFile::Exists( fpath.c_str() ) )
    {
      fpath = wxStandardPaths::Get().GetUserDataDir() + wxT("/data/cpma/hudspecs/") + hudspecs;
      if( !wxFile::Exists( fpath.c_str() ) )
      {
        wxLogError( _("Cannot find `%s' which you probably set manually in your configfile (hudspecs). Make sure it only contains the filename (e.g. '143.dat')."), fpath.c_str() );
        return false;
      }
    }
  }
  
  wxLogDebug( wxT("Loading hudspecs: ") + fpath );
  wxFileInputStream fis( fpath );
  
  if (!fis.Ok())
    return false;
  wxTextInputStream tis(fis);
  wxString line;
  size_t pos;
  wxString name; // name
  wxString value; // whole options line
  wxString key; // cmd of option
  wxString arg; // optionally an argument (if a '=' is found)
  CPMAElement *hi = 0;
  wxString desc = wxT(""); // desc which is written as _desc=... on a single line
  while(!fis.Eof())
  {
    line = tis.ReadLine();
    wxTrim(line);
    if( 0 == line.length() || line[0] == '#' )
      continue;

    if( (pos = line.find( wxT("=") )) == wxString::npos )
    { // chcek if already one is in there with same name (and has HIF_NOTUNIQ), if so copy flags (of nearest previous one).
      wxTrim(line);
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
        wxLogWarning( _("Found invalid element in hudspecs file (`%s'): no previous item found that specifies the options for `%s', ignoring."), fpath.c_str(), line.c_str());
      }
      continue;
    }
    name = line.substr(0, pos);
    wxTrim(name);
    value = line.substr(pos+1, line.length() - pos - 1);
    if( name.find(HS_IGNOREPREFIX) == 0 )
    {
      if( name.Cmp(HS_DESCNAME) == 0 )
        desc = value;
      continue;
    }
    wxTrim(value, wxT(" \"'"));

    wxStringTokenizer options( value, wxT(":") );
    int flags = E_NONE;
    int overwrites = E_PROPERTIES_DEFAULT;
    int multheight = 1;
    int multwidth = 1;
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
      wxTrim(key);
      wxTrim(arg);
      if( key.empty() )
        continue;
      
      if( key.CmpNoCase(wxT("short"))==0 ) flags |= E_SHORT;
      else if( key.CmpNoCase(wxT("enable"))==0 ) enable = true;
      else if( key.CmpNoCase(wxT("enablealways"))==0 ) flags |= E_ENABLEALWAYS;
      else if( key.CmpNoCase(wxT("notuniq"))==0 ) { flags |= E_NOTUNIQ; /*m_special_items.push_back( name );*/ }
      else if( key.CmpNoCase(wxT("nodefault"))==0 ) { flags |= E_NODEFAULT; }
      else if( key.CmpNoCase(wxT("drawnever"))==0 ) flags |= E_DRAWNEVER;
      else if( key.CmpNoCase(wxT("drawalways"))==0 ) flags |= E_DRAWALWAYS;
      else if( key.CmpNoCase(wxT("parent"))==0 ) flags |= E_PARENT;
      else if( key.CmpNoCase(wxT("noinherit"))==0 ) flags |= E_NOINHERIT;
      else if( key.CmpNoCase(wxT("multheight"))==0 ) { sscanf( arg.mb_str(), "%i", &multheight ); flags |= E_MULTHEIGHT; }
      else if( key.CmpNoCase(wxT("multwidth"))==0 ) { sscanf( arg.mb_str(), "%i", &multwidth ); flags |= E_MULTWIDTH; }
      else if( key.CmpNoCase(wxT("multdependalign"))==0 ) flags |= E_MULTDEPENDALIGN;
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
        wxLogError( wxT("Found unknown option `") + key + wxT("' for element `") + name + wxT("' in hudspec file (`") + fpath + wxT("'), was ignored.") );
    }
    // NOTE: that items with HIF_NODEFAULT will still be stored here. but we won't add them upon File->New
    wxLogDebug( wxT("adding: ") + name );
    if( (flags & E_MULTDEPENDALIGN) || (flags & E_MULTWIDTH) || (flags & E_MULTHEIGHT) )
    {
      m_items.push_back( hsitem_s(name, desc, flags, overwrites, enable, type, text, icon, multwidth, multheight) );
    }
    else
    {
      // some sanitizing...
      if( type == E_T_ICON && icon.empty() )
        wxLogDebug( wxT("WARNING: missing `icon' in hudspecs file: ") + name  );
      if( type == E_T_TEXT && text.empty() )
        wxLogDebug( wxT("WARNING: missing `text' in hudspecs file: ") + name  );
      m_items.push_back( hsitem_s(name, desc, flags, overwrites, enable, type, text, icon) );
    }
  }

  return true;
}

const hsitem_s* CPMAHudSpecs::find_item( const wxString& name )
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


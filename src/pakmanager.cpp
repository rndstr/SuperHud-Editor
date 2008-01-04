#include "pakmanager.h"

#include "common.h"

#include <algorithm>

#include <wx/filesys.h>
#include <wx/file.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/arrstr.h>
#include <iostream>


PakManager& PakManager::get()
{
  static PakManager pinstance;
  return pinstance;
}


void PakManager::init()
{
  PakManager::enumerate_game_pakfiles(&m_pakfiles);

  wxDir::GetAllFiles( wxStandardPaths::Get().GetDataDir() + PATH_SEP + wxT("data"), &m_apppakfiles, PM_APPPAK_FILES, GETALLFILES_FLAGS );
  m_apppakfiles.Sort();
}


void PakManager::cleanup()
{
  for( pakcontent_t::iterator it = m_pakcontent.begin(); it != m_pakcontent.end(); ++it )
  {
    if( it->second.buffer != 0 )
    {
      wxLogDebug(wxT("PakManager::cleanup - Deleting cache data: %s"), it->first.c_str());
      delete [] it->second.buffer;
      it->second.buffer = 0;
      it->second.bufsize = 0;
    }
  }
  m_pakcontent.clear();
}

void PakManager::cleanup_lastloaded()
{
  if( m_lastloaded != m_pakcontent.end() )
  {
    delete [] m_lastloaded->second.buffer;
    m_lastloaded->second.buffer = 0;
    m_lastloaded = m_pakcontent.end();
  }
}

wxString PakManager::get_location( const wxString& fpath, int search_where /*=PM_SEARCH_ALL*/, ePakManagerSearchWhere *pfound_where /*=0*/ ) const
{
  wxString loc;
  wxFileSystem fs;
  ePakManagerSearchWhere found = PM_SEARCH_NOWHERE; // 0

  // absolute
  // TODO check if wxFileSystem::FindFirst also finds absolute paths and replace the wxFile::Exists with it
  // but what's the current basedir? this could also find relative paths and label it as absolute!
  // is there a function that determines if a path is relative/absolute?
  // current way works correct though :p
  if( search_where & PM_SEARCH_ABSOLUTE )
  {
    if( wxFile::Exists( fpath ) )
    {
      found = PM_SEARCH_ABSOLUTE;
      loc = fpath;
    }
  }

  if( !found && search_where & PM_SEARCH_GAMERELATIVE )
  {
    // TODO does this find something if the fpath is absolute dir but not below cpma?
    // what about `..' hacks?

    // mod dir
    if( fs.FindFileInPath(&loc, wxGetApp().factory()->dir_game() + PATH_SEP + wxGetApp().factory()->dirname_moddata() + PATH_SEP, fpath) )
      found = PM_SEARCH_GAMERELATIVE;
    // game data dir
    else if( fs.FindFileInPath(&loc, wxGetApp().factory()->dir_game() + PATH_SEP + wxGetApp().factory()->dirname_gamedata() + PATH_SEP, fpath) )
      found = PM_SEARCH_GAMERELATIVE;
  }
  if( !found && search_where & PM_SEARCH_GAMEPAK )
  { // search pak files
    wxLogDebug(fpath);
    for( int i=m_pakfiles.Count()-1; i >= 0; --i )
    {
      fs.ChangePathTo(m_pakfiles[i] + wxT("#zip:"));
      if( !fs.FindFirst(fpath, wxFILE).empty() )
      {
        loc = m_pakfiles[i] + wxT("#zip:") + fpath;
        found = PM_SEARCH_GAMEPAK;
        break;
      }
        wxLogDebug(wxT("FALSE"));
    }
  }
  if( !found && search_where & PM_SEARCH_APPRELATIVE )
  {
    if( fs.FindFileInPath(&loc, wxStandardPaths::Get().GetDataDir() + PATH_SEP + wxT("data") + PATH_SEP, fpath) )
      found = PM_SEARCH_APPRELATIVE;
  }
  if( !found && search_where & PM_SEARCH_APPPAK )
  { // find all files in datadir *.pke
    for( int i=m_apppakfiles.Count()-1; i >= 0; --i )
    {
      fs.ChangePathTo(m_apppakfiles[i] + wxT("#zip:"));
      if( !fs.FindFirst(fpath, wxFILE).empty() )
      {
        loc = m_apppakfiles[i] + wxT("#zip:") + fpath;
        found = PM_SEARCH_APPPAK;
        break;
      }
    }
  }
  if( pfound_where ) *pfound_where = found;
  return loc;
}



bool PakManager::load_from_location( char **buf, const wxString& location, size_t *psize /*=0*/ )
{
  wxLogDebug(wxT("PakManager::load_from_location - ") + location);

  wxFileSystem fs;
  wxFSFile *file = 0;
  {
#ifdef NDEBUG
    wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
#endif
    file = fs.OpenFile( location, wxFS_READ );
  }
  if( 0 == file )
  {
    wxLogDebug(wxT("PakManager::load_from_location - Failed opening file: ") + location );
    return false;
  }

  wxStreamBuffer streambuf( *file->GetStream(), wxStreamBuffer::read );

  size_t size = file->GetStream()->GetSize();
  if( psize )
    *psize = size;
  if (!size )
  {
    wxLogDebug(wxT("PakManager::load_from_location - file of size 0"));
    delete file;
    return false;
  }

  *buf = new char[ size ];
  wxLogDebug(wxT("%d"), size);
  if( size != streambuf.Read(*buf, size) )
  {
//    wxLogDebug(wxT("%d - %d %d %d %d"), streambuf.Stream()->GetLastError(), wxSTREAM_NO_ERROR, wxSTREAM_EOF, wxSTREAM_WRITE_ERROR, wxSTREAM_READ_ERROR);
    delete file;
    return false;
  }
  delete file;
  return true;
}

wxString PakManager::searchwhere2string( ePakManagerSearchWhere wher )
{
  switch(wher)
  {
  case PM_SEARCH_NOWHERE: return wxT("NOWHERE");
  case PM_SEARCH_ABSOLUTE: return wxT("ABSOLUTE");
  case PM_SEARCH_GAMERELATIVE : return wxT("GAMERELATIVE");
  case PM_SEARCH_GAMEPAK : return wxT("GAMEPAK");
  case PM_SEARCH_APPRELATIVE : return wxT("APPRELATIVE");
  case PM_SEARCH_APPPAK: return wxT("APPPAK");
  default:
    break;
  }
  return wxT("UNKNOWN");
}

bool PakManager::load( char **buf, const wxString& fpath, int search_where, size_t *psize /*= 0*/)
{
  wxASSERT(buf != 0);

  // FIXME if the file doesn't belong to a valid game data file we shouldn't add it to cache! (pakcontent)
  wxFSFile *file = 0;
  // maybe we don't even need pak access?

  ePakManagerSearchWhere found;
  wxString location = get_location(fpath, search_where, &found); // final location uri to be used with wxFileSystem

  if( found == PM_SEARCH_NOWHERE )
  {
    wxLogDebug(wxT("PakManager::load - Cannot find file: %s"), fpath.c_str());
    return false;
  }
  wxLogDebug(wxT("PakManager::load - Found file: %s [%s]"), fpath.c_str(), PakManager::searchwhere2string(found).c_str());
  wxLogDebug(wxT("PakManager::load -  > ") + location);

  pakcontent_t::iterator cont = m_pakcontent.find( fpath );
  if( cont == m_pakcontent.end() || cont->second.buffer == 0 )
  { // not found or cache not available
    // load
    wxLogDebug(wxT("PakManager::load - Outdated or non-existant cache entry: %s"), location.c_str());
    size_t size;
    if( !PakManager::load_from_location(buf, location, &size) )
      return false;
    if( cont == m_pakcontent.end() )
    { // no entry, add it but only if
      wxLogDebug(wxT("PakManager::load -  insert item [%d Bytes]: %s"), size, location.c_str());
      std::pair<pakcontent_t::iterator, bool> mofo = m_pakcontent.insert( std::make_pair(location, pakcontentnode_t( *buf, size)) );
      cont = mofo.first;
    }
    else
    { // we only refreshed cache
      wxLogDebug(wxT("PakManager::load -  update ite [%d Bytes]: %s"), size, location.c_str());
      cont->second.buffer = *buf;
      cont->second.bufsize = size;
    }
    // cont is now a valid iterator to the just loaded content
    if( psize )
      *psize = size;
  }
  else
  { // cached and found!
    wxLogDebug(wxT("PakManager::load - Cached entry found [%d Bytes]: %s"), cont->second.bufsize, location.c_str());
    *buf = cont->second.buffer;
    if( psize )
      *psize = cont->second.bufsize;
  }
  m_lastloaded = cont;
  return true;
}


size_t PakManager::enumerate_game_pakfiles( wxArrayString *files )
{
  wxLogDebug( wxT("PakManager::enumerate_game_pakfiles") );
  size_t count=0;

  wxStringTokenizer tok(wxGetApp().factory()->pakfiles(), wxT(";"));
  wxString token;
  size_t pos;
#ifndef WIN32
  wxArrayString homefiles; // files in homedir (~/.q3a/), they overwrite quake3_data_dir.
  wxString homedir = wxStandardPaths::Get().GetUserConfigDir(); // /home/veal
#endif
  while( tok.HasMoreTokens() )
  {
    token = tok.GetNextToken();
    wxTrim( token );
    // get basename :o
    pos = token.find_last_of( wxT("\\/") );
    if( pos != wxString::npos )
    { // there is a subdir
      wxLogDebug( wxT(" > in ") + wxGetApp().factory()->dir_game() + PATH_SEP + token.substr(0, pos+1) + wxT(" for ") + token.substr(pos+1, token.length()-pos-1) );
      count += wxDir::GetAllFiles( wxGetApp().factory()->dir_game() + PATH_SEP + token.substr(0, pos+1), files, token.substr(pos+1, token.length()-pos-1), GETALLFILES_FLAGS);

#ifndef WIN32
      // NB this is not entirely correct.. we need to figure out how to get $HOME the correct way
      // for e.g. Mac builds, i don't know where they store the userfiles.
      wxLogDebug( wxT(" > in ") + homedir + PATH_SEP + wxGetApp().factory()->unixdirname_userdata() + PATH_SEP + token.substr(0, pos+1) + wxT(" for ") + token.substr(pos+1, token.length()-pos-1) );
      count += wxDir::GetAllFiles( homedir + PATH_SEP + wxGetApp().factory()->unixdirname_userdata() + + PATH_SEP + token.substr(0, pos+1), &homefiles, token.substr(pos+1, token.length()-pos-1), GETALLFILES_FLAGS);
#endif
    }
    else
    { // no subdir, NOTE that this doesn't occur normally... as all gamepakfiles are in subdirs
      wxLogDebug( wxT(" > ") + wxGetApp().factory()->dir_game() + wxT("#") + token );
      count += wxDir::GetAllFiles( wxGetApp().factory()->dir_game(), files, token, GETALLFILES_FLAGS);
#ifndef WIN32
      wxLogDebug( wxT(" > ") + homedir + PATH_SEP + wxGetApp().factory()->unixdirname_userdata() + wxT("#") + token );
      count += wxDir::GetAllFiles( homedir + PATH_SEP + wxGetApp().factory()->unixdirname_userdata(), &homefiles, token, GETALLFILES_FLAGS);
#endif
    }
  }
  files->Sort();
#ifndef WIN32
  homefiles.Sort();
  // we append them as they are more important than previous (we start looking through from the back!)
  for( wxArrayString::const_iterator cit = homefiles.begin(); cit != homefiles.end(); ++cit )
    files->Add( *cit );
#endif
  return count;
}

void PakManager::enumerate_pakdircontents( const wxString& pakpath, pakbrowser_dirs_type *dirs, pakbrowser_files_type *files, wxGauge *gauge /*=0*/) const
{
  wxLogDebug( wxT("PakManager::enumerate_pakdircontents") );
#ifdef NDEBUG
  wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
#endif
  wxFileSystem fs;
  wxString fn; // finding
  wxString rel; // relative to pak
  wxString name; // last part of path only
  size_t pos;
  wxStatusBar *sb = (wxGetApp().mainframe() ? wxGetApp().mainframe()->statusbar() : 0);

  int g = 0;
  if( gauge )
  {
    gauge->SetRange( m_pakfiles.Count() );
    gauge->SetValue(0);
  }
  for( int i=m_pakfiles.Count()-1; i >= 0; --i )
  {
    if(sb) sb->PushStatusText( m_pakfiles[i], SB_MSG );
    fs.ChangePathTo( m_pakfiles[i] + wxT("#zip:") + pakpath, true );
    // dirs first
    fn = fs.FindFirst( wxT("*"), wxDIR );
    while( !fn.empty() )
    {
      if( (pos = fn.find(wxT("#zip:"))) == wxString::npos )
        continue;
      rel = fn.substr(pos+5, fn.length() - pos - 5);
      pos = rel.find_last_of(wxT("/"));
      name = rel.substr(pos+1, rel.length() - pos - 1);
      wxLogDebug(wxT(" DIR > %s"), name.c_str());
      dirs->push_back(name);
      fn = fs.FindNext();
    }
    // files now
    fn = fs.FindFirst( wxT("*"), wxFILE );
    while( !fn.empty() )
    {
      if( (pos = fn.find(wxT("#zip:"))) == wxString::npos )
        continue;
      rel = fn.substr(pos+5, fn.length() - pos - 5);
      pos = rel.find_last_of(wxT("/"));
      name = rel.substr(pos+1, rel.length() - pos - 1);
      wxLogDebug(wxT(" FIL > %s, %s"), name.c_str(), m_pakfiles[i].c_str());
      files->insert( std::make_pair(name, m_pakfiles[i]) );
      fn = fs.FindNext();
    }
    if( gauge )
      gauge->SetValue(++g);

    if(sb) sb->PopStatusText(SB_MSG);
  }
  dirs->sort();
  dirs->unique();
}


void PakManager::debug() const
{
  wxLogDebug(wxT("PakManager::debug - Game pak files (most important first)"));
  for( int i=m_pakfiles.Count()-1; i >= 0; --i )
    wxLogDebug( wxT(" > ") + m_pakfiles[i]);
  wxLogDebug(wxT("PakManager::debug - Application pak files"));
  for( int i=m_apppakfiles.Count()-1; i >= 0; --i )
    wxLogDebug( wxT(" > ") + m_apppakfiles[i]);
  wxLogDebug(wxT("PakManager::debug - Pak contents"));
  for( pakcontent_t::const_iterator cit = m_pakcontent.begin(); cit != m_pakcontent.end(); ++cit )
    wxLogDebug( wxT(" > 0x%x [%d Bytes] %s"), cit->second.buffer, cit->second.bufsize, cit->first.c_str());
}


#include "pakmanager.h"

#include "common.h"

#include <algorithm>

#include <wx/filesys.h>
#include <wx/file.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/arrstr.h>


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
      wxLogDebug(wxT("Deleting cache data: %s"), it->first.c_str());
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
    for( int i=m_pakfiles.Count()-1; i >= 0; --i )
    {
      if( fs.FindFileInPath(&loc, m_pakfiles[i] + wxT("#zip:"), fpath) )
      {
        found = PM_SEARCH_GAMEPAK;
        break;
      }
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
      if( fs.FindFileInPath(&loc, m_apppakfiles[i] + wxT("#zip:"), fpath) )
      {
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
  wxFileSystem fs;
  wxFSFile *file = 0;
  {
#ifndef NDEBUG
    wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
#endif
    file = fs.OpenFile( location, wxFS_READ );
  }
  if( 0 == file )
  {
    wxLogDebug(wxT("Failed opening file"));
    return false;
  }
  
  wxStreamBuffer streambuf( *file->GetStream(), wxStreamBuffer::read ); 
 
  size_t size = file->GetStream()->GetSize(); 
  if( psize )
    *psize = size;
  if (!size ) 
  {
    delete file;
    return false; 
  }
 
  *buf = new char[ size ];                             
  streambuf.Read( *buf, size );
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

  // absolute filepath
  // although not really allowed
  ePakManagerSearchWhere found;
  wxString location = get_location(fpath, search_where, &found); // final location uri to be used with wxFileSystem
  wxLogDebug(wxT("Found file `%s' at %s (%s)"), fpath.c_str(), location.c_str(), PakManager::searchwhere2string(found).c_str());

  pakcontent_t::iterator cont = m_pakcontent.find( fpath );
  if( cont == m_pakcontent.end() || cont->second.buffer == 0 )
  { // not found or cache not available
    // load
    wxLogDebug(wxT("Outdated or non-existant cache entry: %s"), location.c_str());
    size_t size;
    if( !PakManager::load_from_location(buf, location, &size) )
      return false;
    if( cont == m_pakcontent.end() )
    { // no entry, add it but only if 
      wxLogDebug(wxT(" insert item: %s"), location.c_str());
      std::pair<pakcontent_t::iterator, bool> mofo = m_pakcontent.insert( std::make_pair(location, pakcontentnode_t( *buf, size)) );
      cont = mofo.first;
    }
    else
    { // we only refreshed cache
      wxLogDebug(wxT(" update item: %s"), location.c_str());
      cont->second.buffer = *buf;
      cont->second.bufsize = size;
    }
    // cont is now a valid iterator to the just loaded content
    if( psize )
      *psize = size;
  }
  else
  { // cached and found!
    wxLogDebug(wxT("Cached entry found: %s"), location.c_str());
    *buf = cont->second.buffer;
    if( psize )
      *psize = cont->second.bufsize;
  }
  m_lastloaded = cont;
  return true;
}


size_t PakManager::enumerate_game_pakfiles( wxArrayString *files )
{
  size_t count=0;
  wxStringTokenizer tok(wxGetApp().factory()->pakfiles(), wxT(";"));
  wxString token;
#ifndef WIN32
  wxArrayString homefiles; // files in homedir (~/.q3a/), they overwrite quake3_data_dir.
  wxString homedir = wxStandardPaths::Get().GetUserConfigDir(); // /home/veal
#endif
  while( tok.HasMoreTokens() )
  {
    token = tok.GetNextToken();
    wxTrim( token );
    // TODO the problem here is, if token contains directorynames (e.g. baseq3/*.pk3) and that
    // directory does not exist, an error box pops up complaining 'the system cannot find the path specified'
    // but with reference to the dirgame, which _does_ exist.
    count += wxDir::GetAllFiles( wxGetApp().factory()->dir_game(), files, token, GETALLFILES_FLAGS );
#ifndef WIN32
    count += wxDir::GetAllFiles( homedir + PATH_SEP + wxGetApp().factory()->unixdirname_userdata(), &homefiles, token, GETALLFILES_FLAGS );
#endif
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


void PakManager::debug() const
{
  wxLogDebug(wxT(" PAKFILES"));
  for( int i=m_pakfiles.Count()-1; i >= 0; --i )
  {
    wxLogDebug(m_pakfiles[i]);
  }
  wxLogDebug(wxT(" PAKCONTENTS"));
  for( pakcontent_t::const_iterator cit = m_pakcontent.begin(); cit != m_pakcontent.end(); ++cit )
  {
    wxLogDebug(cit->first);
    wxLogDebug(wxT("buf: 0x%x bufsize: %d"), cit->second.buffer, cit->second.bufsize);
  }

}
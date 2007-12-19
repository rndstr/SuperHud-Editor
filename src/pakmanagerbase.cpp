#include "pakmanagerbase.h"

#include "common.h"

#include <algorithm>

#include <wx/filesys.h>
#include <wx/file.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/arrstr.h>


void PakManagerBase::init()
{
  PakManagerBase::enumerate_pak_files(&m_pakfiles);
}


void PakManagerBase::cleanup()
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

void PakManagerBase::cleanup_lastloaded()
{
  if( m_lastloaded != m_pakcontent.end() )
  {
    delete [] m_lastloaded->second.buffer;
    m_lastloaded->second.buffer = 0;
    m_lastloaded = m_pakcontent.end();
  }
}

wxString PakManagerBase::get_location( const wxString& fpath, int search_where /*=PM_SEARCH_ALL*/, ePakManagerSearchWhere *pfound_where /*=0*/ ) const
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

    // FIXME should in specialized class
    // mod dir
    if( fs.FindFileInPath(&loc, Prefs::get().q3_gamedir + PATH_SEP + wxT("cpma") + PATH_SEP, fpath) )
      found = PM_SEARCH_GAMERELATIVE;
    // game data dir
    else if( fs.FindFileInPath(&loc, Prefs::get().q3_gamedir + PATH_SEP + wxT("baseq3") + PATH_SEP, fpath) )
      found = PM_SEARCH_GAMERELATIVE;
  }
  if( !found && search_where & PM_SEARCH_GAMEPAK )
  { // search pak files
    for( wxArrayString::const_reverse_iterator it = m_pakfiles.rbegin(); it != m_pakfiles.rend(); ++it )
    {
      if( fs.FindFileInPath(&loc, *it + wxT("#zip:"), fpath) )
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
    wxArrayString apppaks;
    wxDir::GetAllFiles( wxStandardPaths::Get().GetDataDir(), &apppaks, PM_APPPAK_FILES, GETALLFILES_FLAGS);
    apppaks.Sort();
    for( wxArrayString::const_reverse_iterator it = apppaks.rbegin(); it != apppaks.rend(); ++it )
    {
      if( fs.FindFileInPath(&loc, *it + wxT("#zip:"), fpath) )
      {
        found = PM_SEARCH_APPPAK;
        break;
      }
    }
  }
  if( pfound_where ) *pfound_where = found;
  return loc;
}



bool PakManagerBase::load_from_location( char **buf, const wxString& location, size_t *psize /*=0*/ )
{
  wxFileSystem fs;
  wxFSFile *file = 0;
  {
    //wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
    file = fs.OpenFile( location, wxFS_READ );
  }
  if( 0 == file )
  {
    wxLogDebug(wxT("Failed openingfile"));
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

wxString PakManagerBase::searchwhere2string( ePakManagerSearchWhere wher )
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

bool PakManagerBase::load( char **buf, const wxString& fpath, int search_where, size_t *psize /*= 0*/)
{
  wxASSERT(buf != 0);
  // FIXME if the file doesn't belong to a valid game data file we shouldn't add it to cache! (pakcontent)
  wxFSFile *file = 0;
  // maybe we don't even need pak access?

  // absolute filepath
  // although not really allowed
  ePakManagerSearchWhere found;
  wxString location = get_location(fpath, search_where, &found); // final location uri to be used with wxFileSystem
  wxLogDebug(wxT("Found file `%s' at %s (%s)"), fpath.c_str(), location.c_str(), PakManagerBase::searchwhere2string(found).c_str());

  pakcontent_t::iterator cont = m_pakcontent.find( fpath );
  if( cont == m_pakcontent.end() || cont->second.buffer == 0 )
  { // not found or cache not available
    // load
    wxLogDebug(wxT("Outdated or non-existant cache entry: %s"), location.c_str());
    size_t size;
    if( !PakManagerBase::load_from_location(buf, location, &size) )
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


// FIXME should in specialized class
size_t PakManagerBase::enumerate_pak_files( wxArrayString *files )
{
  size_t count=0;
  wxStringTokenizer tok(Prefs::get().q3_pakfiles, wxT(";"));
  wxString token;
#ifndef WIN32
  wxArrayString homefiles; // files in homedir (~/.q3a/), they overwrite quake3_data_dir.
  wxString homedir = wxStandardPaths::Get().GetUserConfigDir(); // /home/veal
#endif
  while( tok.HasMoreTokens() )
  {
    token = tok.GetNextToken();
    wxTrim( token );
    count += wxDir::GetAllFiles( Prefs::get().q3_gamedir, files, token, GETALLFILES_FLAGS );
#ifndef WIN32
    count += wxDir::GetAllFiles( homedir + PATH_SEP + Prefs::get().q3_homedirname, &homefiles, token, GETALLFILES_FLAGS );
#endif

    // get basename :o
    //pos = token.find_last_of( wxT("\\/") );
    /*
    wxString basename;
    wxString dirname;
    wxFileName::SplitPath( token, &dirname, &basename, 0, 0 );
    if( pos != wxString::npos )
    {
      //wxLogDebug( Prefs::get()->quake3_data_dir + wxT("/") + token.substr(0, pos+1) + wxT(" # ") + token.substr(pos+1, token.length()-pos-1) );
      count += wxDir::GetAllFiles( Prefs::get().q3_gamedir + PATH_SEP + wxString(token.substr(0, pos+1)), files, token.substr(pos+1, token.length()-pos-1), GETALLFILES_FLAGS);

#ifndef WIN32
      // NOTE this is not entirely correct.. we need to figure out how to get $HOME the correct way 
      // for e.g. Mac builds, i don't know where they store the userfiles.
      count += wxDir::GetAllFiles( homedir + PATH_SEP + wxT(".q3a") + PATH_SEP + token.substr(0, pos+1), &homefiles, token.substr(pos+1, token.length()-pos-1), GETALLFILES_FLAGS);
#endif
    }
    else
    {
      //wxLogDebug( Prefs::get()->quake3_data_dir + wxT(" # ") + token );
      count += wxDir::GetAllFiles( Prefs::get().q3_gamedir, files, token, GETALLFILES_FLAGS);
#ifndef WIN32
      count += wxDir::GetAllFiles( homedir + PATH_SEP + wxT(".q3a") + PATH_SEP + token.substr(0, pos+1), &homefiles, token.substr(pos+1, token.length()-pos-1), GETALLFILES_FLAGS);
#endif
    }
    */
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

void PakManagerBase::debug() const
{
  wxLogDebug(wxT(" PAKFILES"));
  for( wxArrayString::const_reverse_iterator it = m_pakfiles.rbegin(); it != m_pakfiles.rend(); ++it )
  {
    wxLogDebug(*it);
  }
  wxLogDebug(wxT(" PAKCONTENTS"));
  for( pakcontent_t::const_iterator cit = m_pakcontent.begin(); cit != m_pakcontent.end(); ++cit )
  {
    wxLogDebug(cit->first);
    wxLogDebug(wxT("0x%x"), cit->second.buffer);
    wxLogDebug(wxT("%d"), cit->second.bufsize);
  }

}
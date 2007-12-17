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
  enumerate_pak_files(&m_pakfiles);
}


void PakManagerBase::cleanup()
{
  for( pakcontent_t::iterator it = m_pakcontent.begin(); it != m_pakcontent.end(); ++it )
  {
    if( it->second.buffer != 0 )
    {
      delete [] it->second.buffer;
      it->second.buffer = 0;
      it->second.bufsize = 0;
    }
  }
  m_pakcontent.clear();
}

wxString PakManagerBase::get_location( const wxString& fpath, bool allow_absolute /*=false*/) const
{
  wxString loc;
  // absolute
  // TODO check if wxFileSystem::FindFirst also finds absolute paths
  if( allow_absolute && wxFile::Exists( fpath ) )
    loc = fpath;
  else
  { 
    wxString str;
    wxFileSystem fs;
    // TODO does this find something if the fpath is absolute dir but not below cpma?
    // what about `..' hacks?
    if( fs.FindFileInPath(&str, Prefs::get().q3_gamedir + wxT("/cpma/"), fpath) )
    { // mod dir
      loc = str;
    }
    else if( fs.FindFileInPath(&str, Prefs::get().q3_gamedir + wxT("/baseq3/"), fpath) )
    { // game data dir
      loc = str;
    }
    else
    { // search pak files
      for( wxArrayString::const_reverse_iterator it = m_pakfiles.rbegin(); it != m_pakfiles.rend(); ++it )
      {
        if( fs.FindFileInPath(&str, *it + wxT("#zip:"), fpath) )
        {
          loc = str;
          break;
        }
      }
    }
  }
  return loc;
}



bool PakManagerBase::load_from_location( char **buf, const wxString& location, size_t *psize /*=0*/ )
{
  wxFileSystem fs;
  wxFSFile *file = 0;
  {
    //wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
    file = fs.OpenFile( location );
  }
  if( 0 == file )
    return false;
  
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



bool PakManagerBase::load( char **buf, const wxString& fpath, size_t *psize /*= 0*/ )
{
  wxASSERT(buf != 0);
  // FIXME if the file doesn't belong to a valid game data file we shouldn't add it to cache! (pakcontent)
  wxFSFile *file = 0;
  // maybe we don't even need pak access?

  // absolute filepath
  // although not really allowed
  wxString location = get_location(fpath);; // final location uri to be used with wxFileSystem

  pakcontent_t::iterator cont = m_pakcontent.find( fpath );
  if( cont == m_pakcontent.end() || cont->second.buffer == 0 )
  { // not found or cache not available
    // load
    size_t size;
    if( !load_from_location(buf, location, &size) )
      return false;
    if( cont == m_pakcontent.end() )
    { // no entry, add it
      std::pair<pakcontent_t::iterator, bool> mofo = m_pakcontent.insert( std::make_pair(fpath, pakcontentnode_t( fpath, *buf, size)) );
    }
    if( psize )
      *psize = size;
  }
  return true;
}

size_t PakManagerBase::enumerate_pak_files( wxArrayString *files )
{
  size_t count=0;
  wxStringTokenizer tok(Prefs::get().q3_pakfiles, wxT(";"));
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
    {
      //wxLogDebug( Prefs::get()->quake3_data_dir + wxT("/") + token.substr(0, pos+1) + wxT(" # ") + token.substr(pos+1, token.length()-pos-1) );
      count += wxDir::GetAllFiles( Prefs::get().q3_gamedir + wxT("/") + wxString(token.substr(0, pos+1)), files, token.substr(pos+1, token.length()-pos-1), wxDIR_FILES);

#ifndef WIN32
      // NOTE this is not entirely correct.. we need to figure out how to get $HOME the correct way 
      // for e.g. Mac builds, i don't know where they store the userfiles.
      count += wxDir::GetAllFiles( homedir + wxT("/.q3a/") + token.substr(0, pos+1), &homefiles, token.substr(pos+1, token.length()-pos-1), wxDIR_FILES);
#endif
    }
    else
    {
      //wxLogDebug( Prefs::get()->quake3_data_dir + wxT(" # ") + token );
      count += wxDir::GetAllFiles( Prefs::get().q3_gamedir, files, token, wxDIR_FILES);
#ifndef WIN32
      count += wxDir::GetAllFiles( homedir + wxT("/.q3a/") + token.substr(0, pos+1), &homefiles, token.substr(pos+1, token.length()-pos-1), wxDIR_FILES);
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
  // fix slashes
  std::for_each( files->begin(), files->end(), dir_to_forward_slash );
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
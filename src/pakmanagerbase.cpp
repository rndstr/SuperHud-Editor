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
}

wxString PakManagerBase::get_location( const wxString& fpath, bool allow_absolute /*=false*/) const
{
  wxLogDebug(wxT("looking for %s"), fpath);
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

bool PakManagerBase::load( char **buf, const wxString& fpath, size_t *size /*= 0*/ )
{
  wxFSFile *file = 0;
  // maybe we don't even need pak access?

  // absolute filepath
  // although not really allowed
  wxString location = get_location(fpath);; // final location uri to be used with wxFileSystem

  wxLogDebug(location);
  
  return false;

/*

  pakcontent_t::iterator cont = m_pakcontent.find( pakpath );
  if( cont == m_pakcontent.end() )
  { // not found.
    //if( !prefs_cache_enable )
    if( false )
    { // that's okay, look in all pk3s.
      for( wxArrayString::reverse_iterator it = m_pakfiles.rbegin(); it != m_pakfiles.rend(); ++it )
      {
        if( load_from_zip( buf, *it, pakpath, size ) )
        {
          std::pair<pk3content_t::iterator, bool> mofo = m_pk3content.insert( std::make_pair(pk3path, pk3contentnode_t( &(*it), *buf, *size )) );
          cont = mofo.first;
          m_justloaded = cont;
          return true;
        }
      }
      return false; // cannot find file in pk3s
    }
    else
      return false; // cannot find file in pk3 cache
  }
  else
  {
    if( cont->second.buffer == 0 )
    {
      if( !load_from_zip( buf, *cont->second.pakfile, pakpath, size ) )
      {
        wxLogError( wxT("Failed loading Pak content: ") + pakpath + wxT("\nfrom file: ") + *cont->second.pakfile);
        return false;
      }
      cont->second.buffer = *buf;
      cont->second.bufsize = *size;
    }
    else
    {
      *buf = cont->second.buffer;
      *size = cont->second.bufsize;
    }
    
  }
  //m_justloaded = cont;
  return true;
*/
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

bool PakManagerBase::load_from_zip( char **buf, const wxString& filepath, const wxString& entrypath, size_t *psize )
{
  wxFileSystem fs;
  wxFSFile *file = 0;
  {
    wxLogNull unfortunately_this_gives_us_invalidzipfile_without_any_information_which_one_so_just_shut_the_fuck_up;
    file = fs.OpenFile( filepath + wxT("#zip:") + entrypath );
  }
  if( 0 == file )
    return false;
  
  wxStreamBuffer streambuf(   *file->GetStream(), wxStreamBuffer::read ); 
 
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

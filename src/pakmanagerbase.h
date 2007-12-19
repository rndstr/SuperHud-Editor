#ifndef __PAKMANAGERBASE_H__
#define __PAKMANAGERBASE_H__

#include <wx/wx.h>
#include <wx/arrstr.h>
#include <map>

typedef enum
{
  PM_SEARCH_NOWHERE = 0,
  PM_SEARCH_ABSOLUTE = 1<<0, ///< allow absolute files to be found
  PM_SEARCH_GAMERELATIVE = 1<<1, ///< relative to gamedir
  PM_SEARCH_GAMEPAK = 1<<2, ///< in archives (pk3/pk4)
  PM_SEARCH_APPRELATIVE = 1<<3, ///< relative to our application data
  PM_SEARCH_APPPAK = 1<<4, ///< in application archive NOTYETIMPLEMENTED
  PM_SEARCH_HUDFILE = PM_SEARCH_GAMERELATIVE | PM_SEARCH_GAMEPAK,
  PM_SEARCH_APPFILE = PM_SEARCH_ABSOLUTE | PM_SEARCH_APPRELATIVE | PM_SEARCH_APPPAK,
} ePakManagerSearchWhere;

const wxString PM_APPPAK_FILES = wxT("*.pke");

class PakManagerBase
{
   struct pakcontentnode_t
   {
     pakcontentnode_t() : buffer(0), bufsize(0) {}
     pakcontentnode_t( char *buf, size_t size ) : buffer(buf), bufsize(size) {}
     char *buffer; ///< file contents, probably 0
     size_t bufsize; ///< size of the buffer
   };
   /// maps a location to pakcontent
   /// NOTE that we use the location (and not the relative seach path which could lead to
   /// an actual file on hdd _or_ inside an archive file) as with the current way we always 
   /// search for the file on each access/loadrequest (can change file structure while app
   /// is loaded, no need to refresh cache structure)
   typedef std::map<wxString, pakcontentnode_t> pakcontent_t;

  public:
    PakManagerBase() {}
    virtual ~PakManagerBase() {}

    virtual void init();
    virtual void shutdown() { cleanup(); }
    void cleanup();
    void cleanup_lastloaded();

     
    /// Loads a file
    ///
    /// @arg buf The buffer where the loaded file content is stored NOTE that this class cleans this up
    /// @arg fpath Several possibilites: 1) absolute fs path 2) relative to gamedir fs path 3) path inside some pakfile
    /// @arg psize Used to return the size the buffer had (if not interested submit 0)
    /// @arg search_type Where we look for the file (ePakManagerSearchWhere bitmask)
    /// @return True if file was found and successfully loaded, otherwise false
    bool load( char **buf, const wxString& fpath, int search_where /*= PM_SEARCH_ALL*/, size_t *psize = 0 ); 

    /// @arg buf Pointer to a buffer where the filecontents should be stored
    /// @arg location Is a valid filepath to be used with wxFileSystem::OpenFile
    /// @arg psize Used to return the size the buffer had (if not interested submit 0)
    static bool load_from_location( char **buf, const wxString& location, size_t *psize =0 );

    /// Translates a filepath to an absolute location (that can be used within
    /// wxFileSystem). Checks absolute availability, mod dirs, base game data dir, 
    /// any available pak files
    ///
    /// @arg search_type Where we look for the file (ePakManagerSearchWhere bitmask)
    /// @arg found_where Where we found the file (if not interested supply 0)
    /// @return The full location path to be used in wxFileSystem, only has forward dir slashes (needed for the cache to be consistent..)
    wxString get_location( const wxString& fpath, int search_where /*= PM_SEARCH_ALL*/, ePakManagerSearchWhere *pfound_where = 0 ) const;

    static size_t enumerate_pak_files( wxArrayString *files );

    static wxString searchwhere2string( ePakManagerSearchWhere wher );

    virtual void debug() const;


  protected:
    /// contains all pk3 filepaths with length.
    /// this gets cached to a file. (ORLY?)
    /// at next startup we only check if filesizes
    /// have changed and update the contents unless filesize unchanged.
    wxArrayString m_pakfiles;

    /// has internal pk3 paths to actual content of the 
    /// file referenced by this path.
    /// char* is default 0 and only set if a user request it.
    pakcontent_t m_pakcontent;

    /// we keep track so user can just call cleanup_lastloaded()
    pakcontent_t::iterator m_lastloaded;
};

#endif // __PAKMANAGERBASE_H__

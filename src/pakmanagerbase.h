#ifndef __PAKMANAGERBASE_H__
#define __PAKMANAGERBASE_H__

#include <wx/wx.h>
#include <wx/arrstr.h>
#include <map>

class PakManagerBase
{
   struct pakcontentnode_t
   {
     pakcontentnode_t() : pakfilepath(), buffer(0), bufsize(0) {}
     pakcontentnode_t( wxString filepath ) : pakfilepath(filepath), buffer(0), bufsize(0) {}
     pakcontentnode_t( wxString filepath, char *buf, size_t size ) : pakfilepath(filepath), buffer(buf), bufsize(size) {}
     wxString pakfilepath; ///< filepath of the pak file
     char *buffer; ///< file contents, probably 0
     size_t bufsize; ///< size of the buffer
   };
   typedef std::map<wxString, pakcontentnode_t> pakcontent_t;

  public:
    PakManagerBase() {}
    virtual ~PakManagerBase() {}

    virtual void init();
    virtual void shutdown() { cleanup(); }
    void cleanup();

     
    /// Loads a file (either from 
    /// @arg fpath Several possibilites: 1) absolute fs path 2) relative to gamedir fs path 3) path inside some pakfile
    /// @arg buf The buffer where the loaded file content is stored NOTE that this class cleans this up
    /// @return True if file was found and successfully loaded, otherwise false
    bool load( char **buf, const wxString& fpath, size_t *psize = 0 ); 
    static bool load_from_location( char **buf, const wxString& location, size_t *psize =0 );

    /// Translates a filepath to an absolute location (that can be used within
    /// wxFileSystem). Checks absolute availability, mod dirs, base game data dir, 
    /// any available pak files
    wxString get_location( const wxString& fpath, bool allow_absolute =false) const;

    static size_t enumerate_pak_files( wxArrayString *files );

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
};

#endif // __PAKMANAGERBASE_H__

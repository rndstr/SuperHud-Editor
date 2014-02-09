#ifndef PREFS_H
#define PREFS_H

#include "common.h"

#include "varcontainer.h"
#include "color4.h"

#include <wx/string.h>


class Prefs : public varcont_type
{
    
  public:
    
    /// initializes the config object
    /// @returns bool True if the configfile didn't yet exist aka "first start" otherwise false
    bool init();
    void cleanup();

    void load();
    /// stores all preferences in config file
    void save();
    void read_var( var_type& var );
    void write_var( const var_type& var ) const;


    /// returns the initial path to be used in a filedialog for the current game
    wxString filedialog_path() const;

    const wxString& oldappversion() const { return m_oldappversion; }

  protected:
    wxString m_oldappversion; ///< the one when we read it...

  // singleton
  public:
    static Prefs& get();
  protected:
    Prefs() {}
    Prefs(const Prefs&);
    Prefs& operator= (const Prefs&);
    virtual ~Prefs() {}
};

#endif // PREFS_H


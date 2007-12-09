#ifndef __CPMAHUDFILE_H__
#define __CPMAHUDFILE_H__

#include  "../hudfilebase.h"

#define BACKWARD_COMPATIBILITY_142

/// Used for algos trying to iterate through all PostDecorate elements.
const wxString HF_POSTDECORATE_PREFIX = wxT("PostDecorate");
/// Used for algos trying to iterate through all PreDecorate elements.
const wxString HF_PREDECORATE_PREFIX = wxT("PreDecorate");
/// maximuum number of pre and postdecorates
const int HF_MAX_PREPOSTDECORATE = 64;

// -- parsing


/// possible property(command) delimiters: newline or semicolon.
const wxString HF_PROPERTY_DELIM = wxT("\n;");
/// what we trim for.
const wxString HF_PROPERTY_TRIM = wxT(" \n\t\r");

#ifdef BACKWARD_COMPATIBILITY_142
  const wxString HF_POSTDECORATE_PREFIX_142 = wxT("_PostDecorate");
  const wxString HF_PREDECORATE_PREFIX_142 = wxT("!PreDecorate");
#endif

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile();


    void load_default_elements();
    bool load( const wxString& filename );
    bool save( const wxString& filename );

  private:
    bool parse_item( wxString s );
    bool read_properties( ElementBase *hi, const wxString& props );

    ElementBase *m_load_prevel;
};


#endif // __CPMAHUDFILE_H__


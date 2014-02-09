#ifndef Q4MAXHUDFILE_H
#define Q4MAXHUDFILE_H

#include "hudfilebase.h"
#include "q4max_element.h"

// -- parsing

#ifdef BACKWARD_COMPATIBILITY_142
  const wxString HF_POSTDECORATE_PREFIX_142 = wxT("_PostDecorate");
  const wxString HF_PREDECORATE_PREFIX_142 = wxT("!PreDecorate");
#endif

class Q4MAXHudFile : public HudFileBase
{
  public:
    Q4MAXHudFile();
    virtual ~Q4MAXHudFile() {}

    wxString default_hudfilename() const { return wxT("hud/hud.cfg"); }

  protected:
    bool          read_properties( ElementBase *hi, const wxString& props );
    ElementBase*  create_element( const wxString& name ) const { return new Q4MAXElement(name); }
    ElementBase*  create_element( const hsitem_s& defit ) const { return new Q4MAXElement(defit); }
};


#endif

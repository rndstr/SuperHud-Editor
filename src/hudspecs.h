#ifndef HUDSPECS_H
#define HUDSPECS_H


#include <wx/string.h>
#include <list>
#include <vector>

/// items whose names are starting with this are ignored.
const wxString HS_IGNOREPREFIX = wxT("_");
/// place this string with '=....blahblah' on a single line and all consecutive
/// items will have this description
const wxString HS_DESCNAME = wxT("_desc");

struct hsitem_s
{
  hsitem_s(const wxString& n, const wxString& d=wxT(""), int f=0, int h=0, bool e=false, int ty=0, const wxString& t=wxT(""),
    wxString i=wxT("")) :
    name(n),
    desc(d),
    flags(f),
    has(h),
    enable(e),
    type(ty),
    text(t),
    icon(i)
    {
    }

  wxString name;
  wxString desc;
  int flags;
  int has; // overwrites
  bool enable;
  int type;
  wxString text;
  wxString icon;
};

typedef std::vector<wxString>               notuniqs_type;
typedef notuniqs_type::const_iterator       cit_notuniqs;

class HudSpecs
{
  public:
    typedef std::list<hsitem_s> hsitems_type;
    typedef hsitems_type::iterator it_hsitems;
    typedef hsitems_type::const_iterator cit_hsitems;
    bool load();

    const hsitems_type&   items() const { return m_items; }
    hsitems_type&         items() { return m_items; }

    const hsitem_s*       find_item( const wxString& name ) const;
    const notuniqs_type& notuniqs() const { return m_notuniqs; }

  protected:
    hsitems_type          m_items;
    notuniqs_type         m_notuniqs;

  // singleton
  public:
    static HudSpecs& get();
  protected:
    HudSpecs() {}
    HudSpecs(const HudSpecs&);
    HudSpecs& operator= (const HudSpecs&);
};


#endif


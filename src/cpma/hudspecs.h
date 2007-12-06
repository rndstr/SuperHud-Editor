#ifndef __CPMA_HUDSPECS_H__
#define __CPMA_HUDSPECS_H__

#include <wx/string.h>
#include <list>

/// items whose names are starting with this are ignored.
const wxString HS_IGNOREPREFIX = wxT("_");
/// place this string with '=....blahblah' on a single line and all consecutive
/// items will have this description
const wxString HS_DESCNAME = wxT("_desc");

struct hsitem_s
{
  hsitem_s(const wxString& n, const wxString& d=wxT(""), int f=0, int h=0, bool e=false, int ty=0, const wxString& t=wxT(""),
    wxString i=wxT(""), int mw=1, int mh=1) :
    name(n),
    desc(d),
    flags(f),
    has(h),
    enable(e),
    type(ty),
    text(t),
    icon(i),
    multwidth(mw),
    multheight(mh)
    {}

  wxString name;
  wxString desc;
  int flags;
  int has; // overwrites
  bool enable;
  int type;
  wxString text;
  wxString icon;
  int multwidth;
  int multheight;
};

class CPMAHudSpecs
{
  public:
    typedef std::list<hsitem_s> hsitems_type;
    typedef hsitems_type::iterator it_hsitems;
    typedef hsitems_type::const_iterator cit_hsitems;
    bool load();

    const hsitems_type&  items() const { return m_default_items; }
    hsitems_type&        items() { return m_default_items; }

  protected:
    hsitems_type  m_default_items;

  // singleton
  public:
    static CPMAHudSpecs& get();
  protected:
    CPMAHudSpecs() {}
    CPMAHudSpecs(const CPMAHudSpecs&);
    CPMAHudSpecs& operator= (const CPMAHudSpecs&);
};


#endif

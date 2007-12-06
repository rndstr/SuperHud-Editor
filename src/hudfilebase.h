#ifndef __HUDFILEBASE_H__
#define __HUDFILEBASE_H__

#include "common.h"
#include "elementbase.h"
#include <vector>

/// represents a hudfile (document)
class HudFileBase
{
  public:
    typedef std::vector<ElementBase*>           elements_type;
    typedef elements_type::iterator             it_elements;
    typedef elements_type::const_iterator       cit_elements;
  public:
    HudFileBase();
    virtual ~HudFileBase() { clear(); }

    void set_modified( bool modified = true ) { m_modified = modified; }
   


    /// sets up the document after user clicked File->New
    virtual void on_new() { clear(); }

    virtual void clear(); 
    /// adds an element
    virtual void append( ElementBase *el );

    /// Loads a hudfile
    /// 
    /// @arg wxString filename The full filename
    virtual bool load( const wxString& filename ) = 0;

  protected:
    elements_type       m_els;
    bool                m_modified;
};


#endif // __HUDFILEBASE_H__


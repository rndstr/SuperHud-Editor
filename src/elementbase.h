#ifndef __ELEMENTBASE_H__
#define __ELEMENTBASE_H__

#include <wx/string.h>

class ElementBase
{
  public:
    ElementBase( const wxString& name ) :
      m_name(name)
    {}
    virtual ~ElementBase() {}


    // get&set
    const wxString& name() const { return m_name; }

  protected:
    wxString m_name; ///< is not unique


};

#endif // __ELEMENTBASE_H__

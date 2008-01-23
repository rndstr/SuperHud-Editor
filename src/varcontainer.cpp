#include "varcontainer.h"

Var::Var( const wxString& name, const wxString& def, int type /*= VART_ANY*/, int flags /*= VARF_NONE*/ ) :
      m_name(name),
      m_value(),
      m_def(def),
      m_type(type),
      m_flags(flags),
      m_isset(false)
    {
    }

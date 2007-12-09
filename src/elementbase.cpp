#include "elementbase.h"

#include <wx/tokenzr.h>


bool ElementBase::parse_property( const wxString& cmd, wxString args )
{
  if (cmd.CmpNoCase(wxT("rect")) == 0)
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.x );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.y );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.width );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.height );
    m_has |= E_HAS_RECT;
  }
  else
  {
    return false;
  }
  return true;
}


void ElementBase::write_properties( wxTextOutputStream& stream ) const
{
  if( m_has & E_HAS_RECT ) 
  {
    if( m_flags & E_SHORT )
      stream << wxString::Format(wxT("rect %i %i %i %i; "), m_rect.x, m_rect.y, m_rect.width, m_rect.height );
    else
      stream << wxString::Format(wxT("\n  rect %i %i %i %i"), m_rect.x, m_rect.y, m_rect.width, m_rect.height );
  }
}

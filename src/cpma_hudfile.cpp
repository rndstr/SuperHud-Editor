#include "cpma_hudfile.h"
#include "common.h"
#include "elementsctrlbase.h"
#include "pakmanager.h"


#include "cpma_element.h"

#include <wx/txtstrm.h>
#include <wx/datetime.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/tokenzr.h>


//#include <wx/stdpaths.h>
//#include <wx/dir.h>
//#include <algorithm>


CPMAHudFile::CPMAHudFile() : 
  HudFileBase()
{
}


bool CPMAHudFile::save( const wxString& filename )
{
  int decoratecount = 0;
  for( cit_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( (*it)->name() == HF_POSTDECORATE_PREFIX || (*it)->name() == HF_PREDECORATE_PREFIX )
      ++decoratecount;
  }
  if( decoratecount > CPMA_HF_MAX_PREPOSTDECORATE )
    wxLogWarning( _("You have more than %d combined PreDecorate/PostDecorate elements which CPMA does not support"), CPMA_HF_MAX_PREPOSTDECORATE );
  return HudFileBase::save(filename);
}

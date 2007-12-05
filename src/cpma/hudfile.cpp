
#include "hudfile.h"
#include "../common.h"

#include <wx/wfstream.h>


bool CPMAHudFile::load( const wxString& filename )
{
  wxString content;
  wxFileInputStream fis( filename );

  if( !fis.Ok() )
    return false;
  return true;
}


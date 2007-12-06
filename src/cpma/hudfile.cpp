
#include "hudfile.h"
#include "../common.h"

#include "hudspecs.h"
#include "element.h"

#include <wx/wfstream.h>


CPMAHudFile::CPMAHudFile() : 
  HudFileBase()
{
}

void CPMAHudFile::on_new()
{
  clear();
  ElementBase *el = 0;
  const CPMAHudSpecs::hsitems_type& items = CPMAHudSpecs::get().items();
  for( CPMAHudSpecs::cit_hsitems cit = items.begin(); cit != items.end(); ++cit )
  {
    if( cit->flags & E_NODEFAULT )
      continue;
    el = new CPMAElement(*cit);// cit->name, cit->desc, cit->enable, cit->text, cit->icon, 0, E_RECT_DEFAULT, cit->type, cit->flags, cit->has );
    //hi = Hud::create_item( *cit );
    add( el );

    //if( force_disable ) 
      //hi->set_enable( false );
  }
}

bool CPMAHudFile::load( const wxString& filename )
{
  wxString content;
  wxFileInputStream fis( filename );

  if( !fis.Ok() )
    return false;
  return true;
}


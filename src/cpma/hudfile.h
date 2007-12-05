#ifndef __CPMAHUDFILE_H__
#define __CPMAHUDFILE_H__

#include  "../hudfilebase.h"

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile() : HudFileBase() {}

    bool load( const wxString& filename );

};


#endif // __CPMAHUDFILE_H__


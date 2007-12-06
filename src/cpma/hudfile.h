#ifndef __CPMAHUDFILE_H__
#define __CPMAHUDFILE_H__

#include  "../hudfilebase.h"

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile();

    bool load( const wxString& filename );

  private:
    void reset(); ///< loads default items


};


#endif // __CPMAHUDFILE_H__


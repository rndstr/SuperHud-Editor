#ifndef __CPMAHUDFILE_H__
#define __CPMAHUDFILE_H__

#include  "../hudfilebase.h"

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile();


    void on_new();
    bool load( const wxString& filename );

  private:
    


};


#endif // __CPMAHUDFILE_H__


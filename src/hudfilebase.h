#ifndef __HUDFILEBASE_H__
#define __HUDFILEBASE_H__

#include "common.h"
class HudFileBase
{
  public:
    HudFileBase() {}


    /// Loads a hudfile
    /// 
    /// @arg wxString filename The full filename
    virtual bool load( const wxString& filename ) = 0;


};


#endif // __HUDFILEBASE_H__


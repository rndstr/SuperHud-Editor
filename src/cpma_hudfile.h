#ifndef CPMAHUDFILE_H
#define CPMAHUDFILE_H

#include "hudfilebase.h"
#include "cpma_element.h"



/// maximuum number of pre and postdecorates
const int CPMA_HF_MAX_PREPOSTDECORATE = 64;

class CPMAHudFile : public HudFileBase
{
  public:
    CPMAHudFile();
    virtual ~CPMAHudFile() {}

    bool save( const wxString& filename );
    wxString default_hudfilename() const { return wxT("hud/hud.cfg"); }

  protected:
    ElementBase*  create_element( const wxString& name ) const { return new CPMAElement(name); }
    ElementBase*  create_element( const hsitem_s& defit ) const { return new CPMAElement(defit); }
};


#endif

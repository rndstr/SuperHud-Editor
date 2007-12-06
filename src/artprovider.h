#ifndef __ARTPROVIDER_H__
#define __ARTPROVIDER_H__

#include <wx/wx.h>
#include <wx/artprov.h>
#include "xpm/icons/cpma16.xpm"
#include "xpm/icons/q4max16.xpm"

const wxArtID ART_CPMA = _T("ART_CPMA");
const wxArtID ART_Q4MAX = _T("ART_Q4MAX");


class ArtProvider : public wxArtProvider
{
  protected:
    virtual wxBitmap CreateBitmap(const wxArtID& id, const wxArtClient& client, const wxSize& size)
    { 
      if( id == ART_CPMA )
        return wxBitmap(cpma16_xpm);
      else if( id == ART_Q4MAX )
        return wxBitmap(q4max16_xpm);
      
      return wxNullBitmap;
    }



};

#endif // __ARTPROVIDER_H__
#ifndef ARTPROVIDER_H
#define ARTPROVIDER_H

#include <wx/wx.h>
#include <wx/artprov.h>

#include "xpm/icons/cpma16.xpm"
#include "xpm/icons/q4max16.xpm"
#include "xpm/icons/cpma48.xpm"
#include "xpm/icons/q4max48.xpm"

#include "xpm/icons/align_left.xpm"
#include "xpm/icons/align_center.xpm"
#include "xpm/icons/align_right.xpm"
#include "xpm/icons/align_top.xpm"
#include "xpm/icons/align_middle.xpm"
#include "xpm/icons/align_bottom.xpm"

#include "xpm/icons/element_enabled.xpm"
#include "xpm/icons/element_disabled.xpm"
#include "xpm/icons/elements_collection_title.xpm"
#include "xpm/icons/elements_collection_item.xpm"

#include "xpm/bitmaps/setupwizard.xpm"

const wxArtID ART_CPMA = _T("ART_CPMA");
const wxArtID ART_Q4MAX = _T("ART_Q4MAX");
const wxArtID ART_ALIGN_LEFT = _T("ART_ALIGN_LEFT");
const wxArtID ART_ALIGN_CENTER = _T("ART_ALIGN_CENTER");
const wxArtID ART_ALIGN_RIGHT = _T("ART_ALIGN_RIGHT");
const wxArtID ART_ALIGN_TOP = _T("ART_ALIGN_TOP");
const wxArtID ART_ALIGN_MIDDLE = _T("ART_ALIGN_MIDDLE");
const wxArtID ART_ALIGN_BOTTOM = _T("ART_ALIGN_BOTTOM");

const wxArtID ART_ELEMENT_ENABLED = _T("ART_ELEMENT_ENABLED");
const wxArtID ART_ELEMENT_DISABLED = _T("ART_ELEMENT_DISABLED");
const wxArtID ART_ELEMENTS_COLLECTION_TITLE = _T("ART_ELEMENTS_COLLECTION_TITLE");
const wxArtID ART_ELEMENTS_COLLECTION_ITEM = _T("ART_ELEMENTS_COLLECTION_ITEM");

const wxArtID ART_BMP_SETUPWIZARD = _T("ART_BMP_SETUPWIZARD");


class ArtProvider : public wxArtProvider
{
  protected:
    virtual wxBitmap CreateBitmap(const wxArtID& id, const wxArtClient& client, const wxSize& size)
    { 
      if( id == ART_CPMA )
        return (size == wxSize(48,48) ? wxBitmap(cpma48_xpm) : wxBitmap(cpma16_xpm));
      else if( id == ART_Q4MAX )
        return (size == wxSize(48,48) ? wxBitmap(q4max48_xpm) : wxBitmap(q4max16_xpm));

      else if( id == ART_ALIGN_LEFT )
        return wxBitmap(align_left_xpm);
      else if( id == ART_ALIGN_CENTER)
        return wxBitmap(align_center_xpm);
      else if( id == ART_ALIGN_RIGHT)
        return wxBitmap(align_right_xpm);
      else if( id == ART_ALIGN_TOP)
        return wxBitmap(align_top_xpm);
      else if( id == ART_ALIGN_MIDDLE)
        return wxBitmap(align_middle_xpm);
      else if( id == ART_ALIGN_BOTTOM)
        return wxBitmap(align_bottom_xpm);
      
      else if( id == ART_ELEMENT_ENABLED )
        return wxBitmap(element_enabled_xpm);
      else if( id == ART_ELEMENT_DISABLED )
        return wxBitmap(element_disabled_xpm);
      else if( id == ART_ELEMENTS_COLLECTION_TITLE )
        return wxBitmap(elements_collection_title_xpm);
      else if( id == ART_ELEMENTS_COLLECTION_ITEM )
        return wxBitmap(elements_collection_item_xpm);

      else if( id == ART_BMP_SETUPWIZARD )
        return wxBitmap(setupwizard_xpm);
      
      return wxNullBitmap;
    }



};

#endif // ARTPROVIDER_H


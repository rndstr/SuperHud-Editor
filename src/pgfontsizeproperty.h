#ifndef __PGFONTSIZEPROPERTY_H__
#define __PGFONTSIZEPROPERTY_H__

#include <wx/propgrid/propdev.h>

struct FontSize
{
  FontSize() : type(0), pt(12), x(12), y(12) {}
  int   type; ///< 0 = pointsize, 1 = pixelsize
  int   x, y, pt;
};

#define wxPG_EXTRAS_DECL
WX_PG_DECLARE_VALUE_TYPE_VOIDP_WITH_DECL(FontSize,wxPG_EXTRAS_DECL)
WX_PG_DECLARE_PROPERTY_WITH_DECL(PGFontSizeProperty,const FontSize&,FontSize(),wxPG_EXTRAS_DECL)


#endif

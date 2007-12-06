
#include "element.h"

CPMAElement::CPMAElement( const wxString& name, const wxString& desc /*=""*/, int type /*=E_T_UNKNOWN*/, 
  bool enable /*=false*/, int flags /*= E_NONE*/,
  int has /*= E_HAS_NONE*/,
  const wxString& text /*=""*/, 
  const wxString& icon /*=""*/, 
  const wxRect& rect /*= E_RECT_DEFAULT*/) :
ElementBase(),
    m_name(name),
    m_desc(desc),
    m_type(type),
    m_enabled(enable),
    m_flags(flags),
    m_has(has),

    m_rect(E_RECT_DEFAULT),
    m_font(E_FONT_DEFAULT),
    m_fontsize_type(E_FST_POINT),
    m_fontsize_pt(E_FONTSIZE_DEFAULT_POINT), m_fontsize_x(E_FONTSIZE_DEFAULT_COORDX), m_fontsize_y(E_FONTSIZE_DEFAULT_COORDY),
    m_textalign(E_TEXTALIGN_DEFAULT),
    m_time(-1),
    m_textstyle(0),
    m_monospace(E_MONOSPACE_DEFAULT),
    m_color(E_COLOR_DEFAULT),
    m_bgcolor(E_BGCOLOR_DEFAULT),
    m_fill(E_FILL_DEFAULT),
    m_fadecolor(E_COLOR_DEFAULT), // ?
    m_image(wxT("")),
    m_model(wxT("")),
    m_offset_z(0.f), m_offset_x(0.f), m_offset_y(0.f),
    m_angles_pitch(0), m_angles_yaw(0), m_angles_roll(0), m_angles_panrot(0),
    m_doublebar(E_DOUBLEBAR_DEFAULT),
    m_draw3d(E_DRAW3D_DEFAULT),
    m_text(text),
    m_icon(icon),
    //m_texid(HI_IMG_NOTLOADED),
    m_modelfound(false),
    m_skinfound(false)
{
}

CPMAElement::CPMAElement( const hsitem_s& def ) :
    ElementBase(),
    m_name(def.name),
    m_desc(def.desc),
    m_type(def.type),
    m_enabled(def.enable),
    m_flags(def.flags),
    m_has(def.has),

    m_rect(E_RECT_DEFAULT),
    m_font(E_FONT_DEFAULT),
    m_fontsize_type(E_FST_POINT),
    m_fontsize_pt(E_FONTSIZE_DEFAULT_POINT), m_fontsize_x(E_FONTSIZE_DEFAULT_COORDX), m_fontsize_y(E_FONTSIZE_DEFAULT_COORDY),
    m_textalign(E_TEXTALIGN_DEFAULT),
    m_time(-1),
    m_textstyle(0),
    m_monospace(E_MONOSPACE_DEFAULT),
    m_color(E_COLOR_DEFAULT),
    m_bgcolor(E_BGCOLOR_DEFAULT),
    m_fill(E_FILL_DEFAULT),
    m_fadecolor(E_COLOR_DEFAULT), // ?
    m_image(wxT("")),
    m_model(wxT("")),
    m_offset_z(0.f), m_offset_x(0.f), m_offset_y(0.f),
    m_angles_pitch(0), m_angles_yaw(0), m_angles_roll(0), m_angles_panrot(0),
    m_doublebar(E_DOUBLEBAR_DEFAULT),
    m_draw3d(E_DRAW3D_DEFAULT),
    m_text(def.text),
    m_icon(def.icon),
    //m_texid(HI_IMG_NOTLOADED),
    m_modelfound(false),
    m_skinfound(false)
{
}


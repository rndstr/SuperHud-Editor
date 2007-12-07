#ifndef __CPMA_ELEMENT_H__
#define __CPMA_ELEMENT_H__

#include "../common.h"
#include "../elementbase.h"
#include "../color4.h"
#include "hudspecs.h"

typedef enum
{
  E_T_UNKNOWN=0,
  E_T_TEXT, ///< the game will give us text to render.
  E_T_ICON, ///< the game will override the IMAGE.
  E_T_USERICON, ///< more or less a general element where we can specify everything.
  E_T_BAR, ///< ammo bar, health bar, armor bar.
  E_T_WEAPONLIST
} eElementType;

typedef enum
{
  E_FST_POINT,
  E_FST_COORD
} eElementFontSizeType;



const int E_PROPERTIES_DEFAULT = E_HAS_NONE;


/// Defaults
/// @{

/// default fontsize for type E_FST_POINT
const int E_FONTSIZE_DEFAULT_POINT = 12; // verified 1.35
/// default fontsize for type E_FST_COORD
const int E_FONTSIZE_DEFAULT_COORDX = 12; // verified 1.35
const int E_FONTSIZE_DEFAULT_COORDY = 12; // verified 1.35

const Color4 E_BGCOLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 0.f ); // (alpha==0) verified
const Color4 E_COLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 1.f ); // verified
const wxString E_FONT_DEFAULT = wxT(""); // should be `cpma' or otherwise previews are drawn?
const bool E_FILL_DEFAULT = false;
const bool E_MONOSPACE_DEFAULT = false;
const bool E_DOUBLEBAR_DEFAULT = false;
const bool E_DRAW3D_DEFAULT = false;
const char E_TEXTALIGN_DEFAULT = 'L';
/// @}


class CPMAElement : public ElementBase
{
  public:
    //CPMAElement();
    CPMAElement( const hsitem_s& def );
    CPMAElement( const wxString& name, const wxString& desc =wxT(""), int type =E_T_UNKNOWN, 
      bool enable =false, int flags =E_NONE, int has =E_HAS_NONE, const wxString& text =wxT(""), 
      const wxString& icon =wxT(""), const wxRect& rect = E_RECT_DEFAULT);

    virtual void write_properties( wxTextOutputStream& stream ) const;

  protected:
    wxString      m_desc;
    int           m_type ; ///< eElementType    
    int           m_has; ///< which eElementProperties it overwrites
    

    /// properties
    /// @{
    

    wxString    m_font;
    // either pointsize or coordsize.
    int         m_fontsize_type; ///< eElementFontSizeType
    int         m_fontsize_pt;
    int         m_fontsize_x;
    int         m_fontsize_y;

    wxChar      m_textalign; // L|C|R
    int         m_time; ///< milliseconds.

    int         m_textstyle; // 1 = dropshadow
    bool        m_monospace;

    Color4      m_color;

    Color4      m_bgcolor;
    bool        m_fill;
    
    Color4      m_fadecolor; ///< only used if m_time bigger than 0.

    wxString    m_image;

    /// the model to display, only if type == HIT_ICON|HIT_USERICON|HIT_TEXT
    wxString    m_model; ///< if this is set, m_image contains skinpath!

    float       m_offset_z;
    float       m_offset_x;
    float       m_offset_y;

    int         m_angles_pitch;
    int         m_angles_yaw;
    int         m_angles_roll;
    int         m_angles_panrot;

    bool        m_doublebar;
    bool        m_draw3d;
    /// @}

    wxString    m_text; ///< text for preview
    wxString    m_icon;

    bool        m_modelfound;
    bool        m_skinfound;
};

#endif // __CPMA_ELEMENT_H__

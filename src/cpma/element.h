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

typedef enum {
  E_NONE = 0,
  E_NOTUNIQ  = 1<<0,  ///< the name is allowed more than once.
  E_NODEFAULT  = 1<<1, ///< upon File->New this item is not added to the item list.
  E_PARENT = 1<<2, ///< is an inheriter.
  E_NOINHERIT = 1<<3, ///< this item does not inherit from previous !DEFAULT items.
  E_DRAWNEVER = 1<<4, ///< never draw the item no matter if it's enabled or not.
  E_DRAWALWAYS = 1<<5, ///< NOTINUSE
  E_SHORT = 1<<6, ///< while saving writes a oneliner instead of each property on one line.

  /// cannot be disabled.
  E_ENABLEALWAYS = 1<<7,

  /// multiply height with m_multheight to get screen_height
  E_MULTHEIGHT = 1<<8,

  /// multiply width with m_multwidth to get screen_width
  E_MULTWIDTH = 1<<9, 

  /// depends on textalign property, if 'C' then we use only multwidth otherwise only multheight.
  E_MULTDEPENDALIGN = 1<<10
} eElementFlags;

typedef enum
{
  E_FST_POINT,
  E_FST_COORD
} eElementFontSizeType;

/// whether that element has the property enabled (specified, enabled)
typedef enum {
  E_HAS_NONE = 0,
  E_HAS_RECT = 1<<0,
  E_HAS_TIME = 1<<1,
  E_HAS_FONT = 1<<2,
  E_HAS_FONTSIZE = 1<<3,
  E_HAS_TEXTSTYLE = 1<<4,
  E_HAS_TEXTALIGN = 1<<5,
  E_HAS_COLOR = 1<<6,
  E_HAS_BGCOLOR = 1<<7,
  E_HAS_FADE = 1<<8,
  E_HAS_IMAGE = 1<<9,
  E_HAS_MODEL = 1<<10,
  E_HAS_SKIN = 1<<11,
  E_HAS_OFFSET = 1<<12,
  E_HAS_ANGLES = 1<<13,
  // NOTE: for those now we don't have actually a overwrite checkbox.
  // but we still include it here. Those are set if the attributes are true.
  // So we can still search with Hud::get_inheriter
  E_HAS_MONOSPACE = 1<<14,
  E_HAS_FILL = 1<<15,
  E_HAS_DOUBLEBAR = 1<<16,
  E_HAS_DRAW3D = 1<<17
//  HIO_ALL = (1<<10)-1,
} eElementProperties;

const int E_PROPERTIES_DEFAULT = E_HAS_NONE;


/// default fontsize for type E_FST_POINT
const int E_FONTSIZE_DEFAULT_POINT = 12; // verified 1.35
/// default fontsize for type E_FST_COORD
const int E_FONTSIZE_DEFAULT_COORDX = 12; // verified 1.35
const int E_FONTSIZE_DEFAULT_COORDY = 12; // verified 1.35
const wxRect E_RECT_DEFAULT = wxRect( 0, 0, 64, 32 ); // (0,0) verified 1.35
const Color4 E_BGCOLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 0.f ); // (alpha==0) verified
const Color4 E_COLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 1.f ); // verified
const wxString E_FONT_DEFAULT = wxT(""); // should be `cpma' or otherwise previews are drawn?
const bool E_FILL_DEFAULT = false;
const bool E_MONOSPACE_DEFAULT = false;
const bool E_DOUBLEBAR_DEFAULT = false;
const bool E_DRAW3D_DEFAULT = false;
const char E_TEXTALIGN_DEFAULT = 'L';


class CPMAElement : public ElementBase
{
  public:
    //CPMAElement();
    CPMAElement( const hsitem_s& def );
    CPMAElement( const wxString& name, const wxString& desc =wxT(""), int type =E_T_UNKNOWN, 
      bool enable =false, int flags =E_NONE, int has =E_HAS_NONE, const wxString& text =wxT(""), 
      const wxString& icon =wxT(""), const wxRect& rect = E_RECT_DEFAULT);

  protected:
    wxString      m_desc;
    int           m_type ; ///< eElementType
    bool          m_enabled;
    int           m_flags;
    int           m_has; ///< which eElementProperties it overwrites
    

    /// properties
    /// @{
    wxRect      m_rect;

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

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
  E_FST_NONE,
  E_FST_POINT,
  E_FST_COORD
} eElementFontSizeType;

typedef enum
{
  E_TEXTSTYLE_NONE = 0,
  E_TEXTSTYLE_SHADOW = 1
} eElementTextStyle;;


const int E_PROPERTIES_DEFAULT = E_HAS_NONE;


/// Defaults
/// @{

const int E_FONTSIZE_NONE = -1;
/// default fontsize for type E_FST_POINT
const int E_FONTSIZE_DEFAULT_POINT = 12; // verified 1.35
/// default fontsize for type E_FST_COORD
const int E_FONTSIZE_DEFAULT_COORDX = 12; // verified 1.35
const int E_FONTSIZE_DEFAULT_COORDY = 12; // verified 1.35

const Color4 E_BGCOLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 0.f ); // (alpha==0) verified
const Color4 E_COLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 1.f ); // verified
const Color4 E_FADE_DEFAULT = E_COLOR_DEFAULT; // NOTverified

const wxString E_FONT_DEFAULT = wxT("cpma"); // should be `cpma' or otherwise previews are drawn? <-- oO what is that supposed to mean?
const char E_TEXTALIGN_DEFAULT = 'L';
const int E_TEXTSTYLE_DEFAULT = E_TEXTSTYLE_NONE;
const int E_TIME_DEFAULT = 0;
/// @}


class CPMAElement : public ElementBase
{
  public:
    //CPMAElement();
    CPMAElement( const hsitem_s& def );
    CPMAElement( const wxString& name, const wxString& desc =wxT(""), int type =E_T_UNKNOWN, 
      bool enable =false, int flags =E_NONE, int has =E_HAS_NONE, const wxString& text =wxT(""), 
      const wxString& icon =wxT(""), const wxRect& rect = E_RECT_DEFAULT);

    virtual void  write_properties( wxTextOutputStream& stream ) const;
    virtual bool  parse_property( const wxString& cmd, wxString args );

    int         iget_time() const;
    void        set_time( int time ) { m_time = time; }

    void        set_font( const wxString& font ) { m_font = font; }
    wxString    font() const { return m_font; }
    wxString    iget_font() const;

    void        set_textalign( const wxChar& ta ) { m_textalign = ta; }
    wxChar      iget_textalign() const;

    void        set_monospace( bool monospace = true ) { add_has( E_HAS_MONOSPACE, monospace ); }
    bool        monospace() const { return (m_has & E_HAS_MONOSPACE) != 0; }
    bool        iget_monospace() const;

    void        set_draw3d( bool draw3d = true ) { add_has( E_HAS_DRAW3D, draw3d ); }
    bool        draw3d() const { return (m_has & E_HAS_DRAW3D) != 0; }
    bool        iget_draw3d() const;

    void        set_doublebar( bool doublebar = true ) { add_has( E_HAS_DOUBLEBAR, doublebar ); }
    bool        doublebar() const { return (m_has & E_HAS_DOUBLEBAR) != 0; }
    bool        iget_doublebar() const;

    void        set_fontsizetype( int fst ) { m_fontsize_type = fst; }
    int         fontsizetype() const { return m_fontsize_type; }
    int         iget_fontsizetype() const;
    void        set_fontsizept( int pt ) { m_fontsize_pt = pt; }
    int         fontsizept() const { return m_fontsize_pt; }
    int         iget_fontsizept() const;
    void        set_fontsizex( int x ) { m_fontsize_x = x; }
    int         fontsizex() const { return m_fontsize_x; }
    int         iget_fontsizex() const;
    void        set_fontsizey( int y ) { m_fontsize_y = y; }
    int         fontsizey() const { return m_fontsize_y; }
    int         iget_fontsizey() const;

    void        set_textstyle( int textstyle ) { m_textstyle = textstyle; }
    int         iget_textstyle() const;
    int         textstyle() const { return m_textstyle; }

    void        set_color( const wxColour& c ) { m_color.set(c); }
    void        set_color( const Color4& c ) { m_color.set(c); }
    void        set_color_a100( int a100 ) { m_color.set_a100(a100); }
    Color4      iget_color() const;
    void        set_bgcolor( const wxColour& c ) { m_bgcolor.set(c); }
    Color4      iget_bgcolor() const;
    void        set_fade( const wxColour& c ) { m_fade.set(c); }
    Color4      iget_fade() const;
    void        set_fill( bool fill = true ) { add_has( E_HAS_FILL, fill ); }
    bool        fill() const { return (m_has & E_HAS_FILL) != 0; }
    bool        iget_fill() const;

  protected:
    wxString      m_desc;
    int           m_type ; ///< eElementType    
    

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
    

    Color4      m_color;

    Color4      m_bgcolor;
    
    
    Color4      m_fade; ///< only used if m_time bigger than 0.

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

    // no longer stored, we just use E_HAS_name
    //bool        m_doublebar;
    //bool        m_draw3d;
    //bool        m_monospace;
    //bool        m_fill;
    /// @}

    wxString    m_text; ///< text for preview
    wxString    m_icon;

    bool        m_modelfound;
    bool        m_skinfound;
};

#endif // __CPMA_ELEMENT_H__

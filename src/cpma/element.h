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


enum
{
  E_ANGLE_PITCH,
  E_ANGLE_YAW,
  E_ANGLE_ROLL,
  E_ANGLE_PAN
};
enum
{
  E_OFFSET_X,
  E_OFFSET_Y,
  E_OFFSET_Z
};


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


class Texture;

class CPMAElement : public ElementBase
{
  public:
    struct Properties
    {
      Properties();

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

      wxString    m_skin; ///< will finally go to m_image

      bool        m_usemodel; ///< for prefs dialog whether use model is selected

      float       m_offset[3];

      int         m_angle_pitch;
      int         m_angle_yaw;
      int         m_angle_roll;
      int         m_angle_pan;
    };

  public:
    //CPMAElement();
    CPMAElement( const hsitem_s& def );
    CPMAElement( const wxString& name, const wxString& desc =wxT(""), int type =E_T_UNKNOWN, 
      bool enable =false, int flags =E_NONE, int has =E_HAS_NONE, const wxString& text =wxT(""), 
      const wxString& icon =wxT(""), const wxRect& rect = E_RECT_DEFAULT);
    virtual ~CPMAElement();

    virtual void  write_properties( wxTextOutputStream& stream ) const;
    virtual bool  parse_property( const wxString& cmd, wxString args );
    virtual void  prerender();
    virtual void  render() const;
    virtual void copy_from( const ElementBase * const el );
    virtual void reset();

    const Properties&  properties() const { return m_props; }

    bool        iget_has(int what) const;

    int         iget_time() const;
    void        set_time( int time ) { m_props.m_time = time; }

    void        set_font( const wxString& font ) { m_props.m_font = font; }
    wxString    font() const { return m_props.m_font; }
    wxString    iget_font() const;

    void        set_textalign( const wxChar& ta ) { m_props.m_textalign = ta; }
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

    void        set_angle( int which, int val );
    int         angle( int which ) const;
    int         iget_angle( int which ) const;

    float       iget_offset(int which) const;
    void        set_offset(int which, float val);

    void        set_fontsizetype( int fst ) { m_props.m_fontsize_type = fst; }
    int         fontsizetype() const { return m_props.m_fontsize_type; }
    int         iget_fontsizetype() const;
    void        set_fontsizept( int pt ) { m_props.m_fontsize_pt = pt; }
    int         fontsizept() const { return m_props.m_fontsize_pt; }
    int         iget_fontsizept() const;
    void        set_fontsizex( int x ) { m_props.m_fontsize_x = x; }
    int         fontsizex() const { return m_props.m_fontsize_x; }
    int         iget_fontsizex() const;
    void        set_fontsizey( int y ) { m_props.m_fontsize_y = y; }
    int         fontsizey() const { return m_props.m_fontsize_y; }
    int         iget_fontsizey() const;

    void        set_textstyle( int textstyle ) { m_props.m_textstyle = textstyle; }
    int         iget_textstyle() const;
    int         textstyle() const { return m_props.m_textstyle; }

    void        set_color( const wxColour& c ) { m_props.m_color.set(c); }
    void        set_color( const Color4& c ) { m_props.m_color.set(c); }
    void        set_color_a100( int a100 ) { m_props.m_color.set_a100(a100); }
    Color4      iget_color() const;
    void        set_bgcolor( const wxColour& c ) { m_props.m_bgcolor.set(c); }
    Color4      iget_bgcolor() const;
    void        set_fade( const wxColour& c ) { m_props.m_fade.set(c); }
    Color4      iget_fade() const;
    void        set_fill( bool fill = true ) { add_has( E_HAS_FILL, fill ); }
    bool        fill() const { return (m_has & E_HAS_FILL) != 0; }
    bool        iget_fill() const;

    wxString    iget_image() const;
    void        set_image( const wxString& image ) { m_props.m_image = image; }
    wxString    image() const { return m_props.m_image; }
    wxString    iget_model() const;
    void        set_model( const wxString& model ) { m_props.m_model = model; }
    wxString    model() const { return m_props.m_model; }
    wxString    iget_skin() const;
    void        set_skin( const wxString& skin ) { m_props.m_skin = skin; }
    wxString    skin() const { return m_props.m_skin; }

    bool        usemodel() const { return m_props.m_usemodel; }
    void        set_usemodel( bool um = true ) { m_props.m_usemodel = um; }

  protected:
    int           m_type ; ///< eElementType    
    Properties    m_props;

     

    wxString    m_text; ///< text for preview
    wxString    m_icon;

    // the picture to display; can be either the fixed one supplied by
    // the game or a dynamic entered by the user but never at the same time both.
    // so we use this for all the images
    Texture     *m_ptex;

    //bool        m_modelfound;
    //bool        m_skinfound;
};

#endif // __CPMA_ELEMENT_H__

// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef CPMAELEMENT_H
#define CPMAELEMENT_H

#include "common.h"
#include "elementbase.h"
#include "color4.h"
#include "hudspecs.h"

typedef enum
{
  CPMA_E_FST_NONE,
  CPMA_E_FST_POINT,
  CPMA_E_FST_COORD
} eCPMAElementFontSizeType;

typedef enum
{
  CPMA_E_TEXTSTYLE_NONE = 0,
  CPMA_E_TEXTSTYLE_SHADOW = 1
} eCPMAElementTextStyle;;


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

/// whether that element has the property enabled (specified)
/// aka the element overwrites that property
typedef enum {
  CPMA_E_HAS_TIME = 1<<2,
  CPMA_E_HAS_FONT = 1<<3,
  CPMA_E_HAS_FONTSIZE = 1<<4,
  CPMA_E_HAS_TEXTSTYLE = 1<<5,
  CPMA_E_HAS_TEXTALIGN = 1<<6,
  CPMA_E_HAS_COLOR = 1<<7,
  CPMA_E_HAS_BGCOLOR = 1<<8,
  CPMA_E_HAS_FADE = 1<<9,
  CPMA_E_HAS_IMAGE = 1<<10,
  CPMA_E_HAS_MODEL = 1<<11,
  CPMA_E_HAS_SKIN = 1<<12,
  CPMA_E_HAS_OFFSET = 1<<13,
  CPMA_E_HAS_ANGLES = 1<<14,
  // NOTE: for those now we don't have actually a overwrite checkbox.
  // but we still include it here. Those are set if the attributes are true.
  // So we can still search with Hud::get_inheriter
  CPMA_E_HAS_MONOSPACE = 1<<15,
  CPMA_E_HAS_FILL = 1<<16,
  CPMA_E_HAS_DOUBLEBAR = 1<<17,
  CPMA_E_HAS_DRAW3D = 1<<18
//  HIO_ALL = (1<<10)-1,
} eCPMAElementProperties;


const int WEAPONLIST_ITEMCOUNT = 9; ///, how many items there are
const int WEAPONLIST_SPACE = 4; ///< how many pixels between items
const int WEAPONLIST_SELECTEDIDX = 6; ///< which item do we highlight? should be same as ammo icon! (6=rail)


/// Defaults
/// @{

//const int CPMA_E_FONTSIZE_NONE = -1;
/// default fontsize for type CPMA_E_FST_POINT
const int CPMA_E_FONTSIZE_DEFAULT_POINT = 12; // verified 1.35
/// default fontsize for type CPMA_E_FST_COORD
const int CPMA_E_FONTSIZE_DEFAULT_COORDX = 12; // verified 1.35
const int CPMA_E_FONTSIZE_DEFAULT_COORDY = 12; // verified 1.35

const Color4 CPMA_E_BGCOLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 0 ); // (alpha==0) verified
const Color4 CPMA_E_COLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 100 ); // verified
const Color4 CPMA_E_FADE_DEFAULT = CPMA_E_COLOR_DEFAULT; // NOTverified

const wxString CPMA_E_FONT_DEFAULT = wxT("cpma"); // should be `cpma' or otherwise previews are drawn? <-- oO what is that supposed to mean?
const char CPMA_E_TEXTALIGN_DEFAULT = 'L';
const int CPMA_E_TEXTSTYLE_DEFAULT = CPMA_E_TEXTSTYLE_NONE;
const int CPMA_E_TIME_DEFAULT = 0;
/// @}


class Texture;

class CPMAElement : public ElementBase
{
  public:
    struct Properties
    {
      Properties();

      wxString    font;
      // either pointsize or coordsize.
      int         fontsize_type; ///< eElementFontSizeType
      int         fontsize_pt;
      int         fontsize_x;
      int         fontsize_y;

      wxChar      textalign; // L|C|R
      int         time; ///< milliseconds.

      int         textstyle; // 1 = dropshadow
      

      Color4      color;

      Color4      bgcolor;
      
      
      Color4      fade; ///< only used if m_time bigger than 0.

      wxString    image;

      /// the model to display, only if type == HIT_ICON|HIT_USERICON|HIT_TEXT
      wxString    model; ///< if this is set, m_image contains skinpath!

      wxString    skin; ///< will finally go to m_image

      bool        usemodel; ///< for prefs dialog whether use model is selected

      float       offset[3];

      int         angle_pitch;
      int         angle_yaw;
      int         angle_roll;
      int         angle_pan;
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
    virtual void  postparse();
    virtual void  prerender();
    virtual void  render() const;
    virtual void copy_from( const ElementBase * const el );
    virtual void reset();
    virtual void convert( double from, double to, bool size, bool stretchposition, bool fontsize);
    virtual bool is_removable() const { return ElementBase::is_removable() || (m_type == E_T_UNKNOWN); }

    const Properties&  properties() const { return m_props; }

    int         type() const { return m_type; }

    bool        iget_has(int what) const;

    virtual wxRect  iget_hudrect() const;

    int         iget_time() const;
    void        set_time( int time ) { m_props.time = time; }

    void        set_font( const wxString& font ) { m_props.font = font; }
    wxString    font() const { return m_props.font; }
    wxString    iget_font() const;

    void        set_textalign( const wxChar& ta ) { m_props.textalign = ta; }
    wxChar      iget_textalign() const;

    void        set_monospace( bool monospace = true ) { add_has( CPMA_E_HAS_MONOSPACE, monospace ); }
    bool        monospace() const { return (m_has & CPMA_E_HAS_MONOSPACE) != 0; }
    bool        iget_monospace() const;

    void        set_draw3d( bool draw3d = true ) { add_has( CPMA_E_HAS_DRAW3D, draw3d ); }
    bool        draw3d() const { return (m_has & CPMA_E_HAS_DRAW3D) != 0; }
    bool        iget_draw3d() const;

    void        set_doublebar( bool doublebar = true ) { add_has( CPMA_E_HAS_DOUBLEBAR, doublebar ); }
    bool        doublebar() const { return (m_has & CPMA_E_HAS_DOUBLEBAR) != 0; }
    bool        iget_doublebar() const;

    void        set_angle( int which, int val );
    int         angle( int which ) const;
    int         iget_angle( int which ) const;

    float       iget_offset(int which) const;
    void        set_offset(int which, float val);

    void        set_fontsizetype( int fst ) { m_props.fontsize_type = fst; }
    int         fontsizetype() const { return m_props.fontsize_type; }
    int         iget_fontsizetype() const;
    void        set_fontsizept( int pt ) { m_props.fontsize_pt = pt; }
    int         fontsizept() const { return m_props.fontsize_pt; }
    int         iget_fontsizept() const;
    void        set_fontsizex( int x ) { m_props.fontsize_x = x; }
    int         fontsizex() const { return m_props.fontsize_x; }
    int         iget_fontsizex() const;
    void        set_fontsizey( int y ) { m_props.fontsize_y = y; }
    int         fontsizey() const { return m_props.fontsize_y; }
    int         iget_fontsizey() const;

    void        set_textstyle( int textstyle ) { m_props.textstyle = textstyle; }
    int         iget_textstyle() const;
    int         textstyle() const { return m_props.textstyle; }

    void        set_color( const wxColour& c ) { m_props.color.set(c); }
    void        set_color( const Color4& c ) { m_props.color = c; wxLogDebug(wxT("set C4"));}
    void        set_color_a100( int a100 ) { m_props.color.set_a100(a100); }
    Color4      iget_color() const;

    void        set_bgcolor( const wxColour& c ) { m_props.bgcolor.set(c); }
    void        set_bgcolor( const Color4& c ) { m_props.bgcolor = c; }
    Color4      iget_bgcolor() const;
    void        set_bgcolor_a100( int a100 ) { m_props.bgcolor.set_a100(a100); }

    void        set_fade( const wxColour& c ) { m_props.fade.set(c); }
    void        set_fade( const Color4& c ) { m_props.fade = c; }
    Color4      iget_fade() const;
    void        set_fade_a100( int a100 ) { m_props.fade.set_a100(a100); }

    void        set_fill( bool fill = true ) { add_has( CPMA_E_HAS_FILL, fill ); }
    bool        fill() const { return (m_has & CPMA_E_HAS_FILL) != 0; }
    bool        iget_fill() const;

    wxString    iget_image() const;
    void        set_image( const wxString& image ) { m_props.image = image; }
    wxString    image() const { return m_props.image; }
    wxString    iget_model() const;
    void        set_model( const wxString& model ) { m_props.model = model; }
    wxString    model() const { return m_props.model; }
    wxString    iget_skin() const;
    void        set_skin( const wxString& skin ) { m_props.skin = skin; }
    wxString    skin() const { return m_props.skin; }

    bool        usemodel() const { return m_props.usemodel; }
    void        set_usemodel( bool um = true ) { m_props.usemodel = um; }


  protected:
    void        cleanup();

    int           m_type ; ///< eCPMAElementType    
    Properties    m_props;

     

    wxString    m_text; ///< text for preview
    wxString    m_icon;

    // the picture to display; can be either the fixed one supplied by
    // the game or a dynamic entered by the user but never at the same time both.
    // so we use this for all the images
    Texture     *m_ptex;

    /// all the textures needed by the WEAPONLIST item
    std::vector<Texture*> m_weaponlist_tex;
};

#endif

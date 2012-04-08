// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef Q4MAXELEMENT_H
#define Q4MAXELEMENT_H

#include "common.h"
#include "elementbase.h"
#include "color4.h"
#include "hudspecs.h"
#include "varcontainer.h"



/*
const int WEAPONLIST_ITEMCOUNT = 9; ///, how many items there are
const int WEAPONLIST_SPACE = 4; ///< how many pixels between items
const int WEAPONLIST_SELECTEDIDX = 6; ///< which item do we highlight? should be same as ammo icon! (6=rail)
*/

typedef enum 
{
  E_FT_CHAIN = 0,
  E_FT_MARINE = 1,
  E_FT_LOWPIXEL = 2,
  E_FT_PROFONT = 3,

  // use as binary or operator
  E_FT_OUTLINE = 0x4
} eQ4MAXElementFont;

/// whether that element has the property enabled (specified)
/// aka the element overwrites that property
/// NOTE transform this to wxInt64 constants if you add more... the functions handling
/// those are supposed to already use wxInt64
typedef enum
{
  /*
  E_HAS_NONE = 0,
  E_HAS_POS = 1<<0,
  E_HAS_DIM = 1<<1,
  E_HAS_RECT = E_HAS_POS | E_HAS_DIM
  */
  MAX_E_HAS_COLOR = 1<<2,
  MAX_E_HAS_COLORED = 1<<3,
  MAX_E_HAS_COLORBG = 1<<4,
  MAX_E_HAS_COLORHIGHLIGHT = 1<<5,
  MAX_E_HAS_COLORHIGH = 1<<6,
  MAX_E_HAS_COLORMED = 1<<7,
  MAX_E_HAS_COLORLOW = 1<<8,
  MAX_E_HAS_DIMENSIONS = 1<<9,
  MAX_E_HAS_FONT = 1<<10,
  MAX_E_HAS_HIGHWATERMARK = 1<<11,
  MAX_E_HAS_HORIZONTAL = 1<<12,
  MAX_E_HAS_ICONDIMENSIONS = 1<<13,
  
  MAX_E_HAS_ICONOFFSET = 1<<14,
  MAX_E_HAS_IMAGE = 1<<15,
  MAX_E_HAS_ITEMSPACER = 1<<16,
  MAX_E_HAS_LISTALIGN = 1<<17,
  MAX_E_HAS_MARINECOLOR = 1<<18,
  MAX_E_HAS_MEDWATERMARK = 1<<19,
  MAX_E_HAS_MONOSPACED = 1<<20,
  MAX_E_HAS_STROGGCOLOR = 1<<21,
  MAX_E_HAS_TABSTOPS = 1<<22,
  MAX_E_HAS_TEXT = 1<<23,
  MAX_E_HAS_TEXTALIGN = 1<<24,
  MAX_E_HAS_TEXTOFFSET = 1<<25,
  MAX_E_HAS_TEXTSCALE = 1<<26,
  MAX_E_HAS_TEXTSTYLE = 1<<27,
  MAX_E_HAS_TIME = 1<<28,
  MAX_E_HAS_VISIBLE = 1<<29,
  MAX_E_HAS_VISIBLEFORFREEFLOAT = 1<<30,
  MAX_E_HAS_WRAP = 1<<31
} eQ4MAXElementProperties;

typedef enum
{
  MAX_E_VIS_HIDDEN = 0,
  MAX_E_VIS_DUEL = 1,
  MAX_E_VIS_TDM = 2,
  MAX_E_VIS_CTF = 4,
  MAX_E_VIS_ALL = 7

} eQ4MAXElementVisible;

/// Defaults
/// @{

const Color4 MAX_E_COLORBG_DEFAULT = Color4( 1.f, 1.f, 1.f, 0 );
const Color4 MAX_E_COLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 100 );
const bool MAX_E_COLORED_DEFAULT = true;
const Color4 MAX_E_COLORHIGHLIGHT_DEFAULT;
const wxString MAX_E_COLORHIGH_DEFAULT = wxT("-1"); // use same as Color
const wxString MAX_E_COLORMED_DEFAULT = wxT("-1");
const wxString MAX_E_COLORLOW_DEFAULT = wxT("-1");

const int MAX_E_FONT_DEFAULT = E_FT_CHAIN;
const int MAX_E_DIMENSIONSX_DEFAULT = 0;
const int MAX_E_DIMENSIONSY_DEFAULT = 0;
const int MAX_E_HIGHWATERMARK_DEFAULT = 100;
const int MAX_E_MEDWATERMARK_DEFAULT = 100;
const int MAX_E_HORIZONTAL_DEFAULT = 0;
const Color4 MAX_E_MARINECOLOR_DEFAULT = Color4(1.f, 1.f, 1.f, 100); // FIXME
const Color4 MAX_E_STROGGCOLOR_DEFAULT = Color4(1.f, 1.f, 1.f, 100); // FIXME
const bool MAX_E_MONOSPACED_DEFAULT = false;
const wxString MAX_E_TABSTOPS_DEFAULT = wxT("30,90,112,120,145,160"); // FIXME
const wxString MAX_E_LISTALIGN_DEFAULT = wxT("L");
const wxString MAX_E_TEXTALIGN_DEFAULT = wxT("L");
const int MAX_E_WRAP_DEFAULT = 600; // FIXME
const int MAX_E_TIME_DEFAULT = 0;
const int MAX_E_VISIBLE_DEFAULT = MAX_E_VIS_ALL;
/// @}


class Texture;

class Q4MAXElement : public ElementBase
{
  public:
    class Property : public Var
    {
      public:
        Property( const wxString& name, const wxString& def, int type, int flags ) :
          Var(name, def, type, flags), m_defines(E_HAS_NONE) { }
        
        Property& defines( wxInt64 def ) { m_defines = def; return *this; }
        wxInt64 defines() const { return m_defines; }

      protected:
        wxInt64 m_defines; ///< according to Element::m_has what does this property specify
    };



    class Properties : public VarContainer<Property>
    {
      public:
        Properties();
        bool init();
        
        Properties& operator= (const Properties& p)
        {
          if (this == &p) return *this;

          // copy all 
          const VarContainer<Property>::variables_type& rhs = p.vars();
          for( VarContainer<Property>::variables_type::const_iterator cit = rhs.begin(); cit != rhs.end(); ++cit )
          {
            wxASSERT_MSG( exists(cit->first), wxT("cannot copy property that does not exist in this element") );
#ifdef NDEBUG
            if( exists(cit->first) )
#endif
              set( cit->first, cit->second.sval() );
          }
          return *this;
        }
    };

  public:
    //Q4MAXElement();
    Q4MAXElement( const hsitem_s& def );
    Q4MAXElement( const wxString& name, const wxString& desc =wxT(""), int type =E_T_UNKNOWN, 
      bool enable =false, int flags =E_NONE, int has =E_HAS_NONE, const wxString& text =wxT(""), 
      const wxString& icon =wxT(""), const wxRect& rect = E_RECT_DEFAULT);
    virtual ~Q4MAXElement();

    

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

    bool        iget_has(wxInt64 what) const;

    //virtual wxRect  iget_hudrect() const;


    Vec2        iget_v2val( const wxString& name ) const;

    int         iget_ival( const wxString& name ) const;
    bool        set_ival( const wxString& name, int val );
    /*
    int         iget_time() const;
    void        set_time( int time ) { m_props.time = time; }

    void        set_font( const wxString& font ) { m_props.font = font; }
    wxString    font() const { return m_props.font; }
    wxString    iget_font() const;

    void        set_textalign( const wxChar& ta ) { m_props.textalign = ta; }
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

    void        set_fill( bool fill = true ) { add_has( E_HAS_FILL, fill ); }
    bool        fill() const { return (m_has & E_HAS_FILL) != 0; }
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
    */

    bool            is_rendered() const;

    static wxString type2string( int type );
  protected:
    void        cleanup();

    int           m_type ; ///< eElementType    
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


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
typedef enum {
  //E_HAS_NONE = 0,
  //E_HAS_RECT = 1<<0,
  Q4MAX_E_HAS_COLOR = 1<<2,
  Q4MAX_E_HAS_COLORED = 1<<3,
  Q4MAX_E_HAS_COLORBG = 1<<4,
  Q4MAX_E_HAS_COLORHIGHLIGHT = 1<<5,
  Q4MAX_E_HAS_COLORHIGH = 1<<6,
  Q4MAX_E_HAS_COLORMED = 1<<7,
  Q4MAX_E_HAS_COLORLOW = 1<<8,
  Q4MAX_E_HAS_DIMENSIONS = 1<<9,
  Q4MAX_E_HAS_FONT = 1<<10,
  Q4MAX_E_HAS_HIGHWATERMARK = 1<<11,
  Q4MAX_E_HAS_HORIZONTAL = 1<<12
  /*
  Q4MAX_E_HAS_TIME = 1<<1,
  Q4MAX_E_HAS_FONT = 1<<2,
  Q4MAX_E_HAS_FONTSIZE = 1<<3,
  Q4MAX_E_HAS_TEXTSTYLE = 1<<4,
  Q4MAX_E_HAS_TEXTALIGN = 1<<5,

  

  Q4MAX_E_HAS_BGCOLOR = 1<<7,
  Q4MAX_E_HAS_FADE = 1<<8,
  Q4MAX_E_HAS_IMAGE = 1<<9,
  Q4MAX_E_HAS_MODEL = 1<<10,
  Q4MAX_E_HAS_SKIN = 1<<11,
  Q4MAX_E_HAS_OFFSET = 1<<12,
  Q4MAX_E_HAS_ANGLES = 1<<13,
  // NOTE: for those now we don't have actually a overwrite checkbox.
  // but we still include it here. Those are set if the attributes are true.
  // So we can still search with Hud::get_inheriter
  Q4MAX_E_HAS_MONOSPACE = 1<<14,
  Q4MAX_E_HAS_FILL = 1<<15,
  Q4MAX_E_HAS_DOUBLEBAR = 1<<16,
  Q4MAX_E_HAS_DRAW3D = 1<<17
  */
} eQ4MAXElementProperties;

/// Defaults
/// @{

const Color4 Q4MAX_E_COLORBG_DEFAULT = Color4( 1.f, 1.f, 1.f, 0 );
const Color4 Q4MAX_E_COLOR_DEFAULT = Color4( 1.f, 1.f, 1.f, 100 );
const bool Q4MAX_E_COLORED_DEFAULT = true;
const Color4 Q4MAX_E_COLORHIGHLIGHT_DEFAULT;
const wxString Q4MAX_E_COLORHIGH_DEFAULT = wxT("-1"); // use same as Color
const wxString Q4MAX_E_COLORMED_DEFAULT = wxT("-1");
const wxString Q4MAX_E_COLORLOW_DEFAULT = wxT("-1");

const int Q4MAX_E_FONT_DEFAULT = E_FT_CHAIN;
const int Q4MAX_E_DIMENSIONSX_DEFAULT = 0;
const int Q4MAX_E_DIMENSIONSY_DEFAULT = 0;
const char Q4MAX_E_TEXTALIGN_DEFAULT = 'L';
const int Q4MAX_E_HIGHWATERMARK_DEFAULT = 100;
const int Q4MAX_E_HORIZONTAL_DEFAULT = 0;
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
        
        Property& defines( int def ) { m_defines = def; return *this; }
        int defines() const { return m_defines; }

      protected:
        int m_defines; ///< according to Element::m_has what does this property specify
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

    bool        iget_has(int what) const;

    //virtual wxRect  iget_hudrect() const;

    Vec2        iget_v2val( const wxString& name ) const;
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


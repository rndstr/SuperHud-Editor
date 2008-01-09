#ifndef ELEMENTBASE_H
#define ELEMENTBASE_H

#include <wx/string.h>
#include <wx/txtstrm.h>
#include <wx/gdicmn.h>

/// whether that element has the property enabled (specified)
/// aka the element overwrites that property
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

typedef enum {
  E_NONE = 0,
  E_NOTUNIQ  = 1<<0,  ///< the name is allowed more than once.
  E_NODEFAULT  = 1<<1, ///< upon File->New this item is not added to the item list.
  E_PARENT = 1<<2, ///< is an inheriter.
  E_NOINHERIT = 1<<3, ///< this item does not inherit from previous !DEFAULT items.
  E_DRAWNEVER = 1<<4, ///< never draw the item no matter if it's enabled or not.
  E_DRAWBACK = 1<<5,
  E_DRAWFRONT = 1<<6,
  E_DRAWALWAYS = 1<<7, ///< NOTINUSE
  E_SHORT = 1<<8, ///< while saving writes a oneliner instead of each property on one line.

  /// cannot be disabled.
  E_ENABLEALWAYS = 1<<9,

  /// multiply height with m_multheight to get screen_height
  E_MULTHEIGHT = 1<<10,

  /// multiply width with m_multwidth to get screen_width
  E_MULTWIDTH = 1<<11, 

  /// depends on textalign property, if 'C' then we use only multwidth otherwise only multheight.
  E_MULTDEPENDALIGN = 1<<12
} eElementFlags;

/// Defaults
/// @{
const wxRect E_RECT_DEFAULT = wxRect( 0, 0, 64, 32 ); // (0,0) verified 1.35
/// @}

/// posible argument delimiters found in the hudfile.
/// NOTE: this meaning here changed... we 'fix' the delimiters 
/// (which can be ' ' or ',' previously) so while parsing we only
/// have single-space as delimiter.
const wxString HF_PROPERTY_ARG_DELIM = wxT(" ");

class ElementBase
{
   friend class CPMAHudFile; // for proper detection if item has already been read (through m_enabled as is_enabled() is lying)
   friend class CPMAPropertiesCtrl; // TODO still needed?
   friend class VisibilityPropertiesCtrl; // TODO still needed?
  public:
    ElementBase( const wxString& name, const wxString& desc = wxT(""), int flags = E_NONE, int has = E_HAS_NONE, bool enabled = false, 
      const wxRect& rect = E_RECT_DEFAULT ) :
      m_name(name), m_desc(desc), m_flags(flags), m_has(has), m_enabled(enabled), m_rect(rect)
    {}
    virtual ~ElementBase() {}

    /// while reading the hudfile we pass single properties here.
    /// @arg cmd The name of the property.
    /// @arg args The rest of the propertyline, the arguments.
    /// @return True if the property was parsed otherwise false.
    virtual bool    parse_property( const wxString& cmd, wxString args );
    virtual void    write_properties( wxTextOutputStream& stream ) const;

    /// is called after parsing finished for this element
    virtual void    postparse() {}

    /// is called everytime before rendering
    virtual void prerender() {}

    /// draw on view
    virtual void render() const {}

    /// copy properties from foreign element
    virtual void copy_from( const ElementBase * const el );

    /// resets all properties to default
    virtual void reset();

    /// @arg from     Current aspectratio X/Y
    /// @arg to       New aspectratio X/Y
    /// @arg size     Whether to adapt size(width) -> makes squares stay squares
    /// @arg stretchposition Whether to stretch positions to make corner elements stay in the corner
    virtual void convert( double from, double to, bool size, bool stretchposition, bool fontsize);
 
    void            move_to( const wxPoint& p ) { m_rect.SetPosition(p); }
    /// moving by offset
    void            move( const wxPoint& p ) { m_rect.x += p.x; m_rect.y += p.y; }

    // get&set
    const wxString& name() const { return m_name; }
    int             flags() const { return m_flags; }
    int             has() const { return m_has; }
    /// adds a value (bitmask) to what this element overwrite
    /// @arg bool add If false we actually remove it.
    void            add_has( int has, int add = true ) { if( !add) remove_has(has); else m_has |= has; }
    void            remove_has( int has ) { m_has &= ~has; }
    bool            is_enabled() const { return (m_flags & E_ENABLEALWAYS ? true : m_enabled); }
    void            set_enabled(bool en = true) { m_enabled = en; }
    bool            is_selected() const;
    wxRect          rect() const { return m_rect; }
    void            set_rect( const wxRect& r ) { m_rect = r; }

    // get&set properties (with regard to `i'nheritance)
    wxRect          iget_rect() const;
    bool            is_rendered() const;
    bool            is_removable() const { return (flags() & E_NOTUNIQ) != 0; }

  protected:
    wxString  m_name; ///< is not unique
    wxString  m_desc;
    int       m_flags;
    int       m_has;

    // props for all items
    bool     m_enabled;
    wxRect   m_rect;
};

#endif // ELEMENTBASE_H

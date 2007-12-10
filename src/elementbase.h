#ifndef __ELEMENTBASE_H__
#define __ELEMENTBASE_H__

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
   friend class CPMAPropertiesCtrl;
   friend class PositionPropertiesCtrl;
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

    // get&set
    const wxString& name() const { return m_name; }
    int             flags() const { return m_flags; }
    int             has() const { return m_has; }
    bool            is_enabled() const { return m_enabled; }
    void            set_enabled(bool en = true) { m_enabled = en; }
    wxRect          rect() const { return m_rect; }

    // get&set properties (with regard to `i'nheritance)
    wxRect          iget_rect() const;

  protected:
    wxString  m_name; ///< is not unique
    wxString  m_desc;
    int       m_flags;
    int       m_has;
    

    // global props
    bool     m_enabled;
    wxRect   m_rect;


};

#endif // __ELEMENTBASE_H__

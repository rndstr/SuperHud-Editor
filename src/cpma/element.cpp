
#include "element.h"
#include "../hudfilebase.h"

#include <wx/tokenzr.h>

#include <list>
using namespace std;

CPMAElement::CPMAElement( const wxString& name, const wxString& desc /*=""*/, int type /*=E_T_UNKNOWN*/, 
  bool enable /*=false*/, int flags /*= E_NONE*/,
  int has /*= E_HAS_NONE*/,
  const wxString& text /*=""*/, 
  const wxString& icon /*=""*/, 
  const wxRect& rect /*= E_RECT_DEFAULT*/) :
ElementBase(name, desc, flags, has, enable),
    m_type(type),

    m_font(E_FONT_DEFAULT),
    m_fontsize_type(E_FST_POINT),
    m_fontsize_pt(E_FONTSIZE_DEFAULT_POINT), m_fontsize_x(E_FONTSIZE_DEFAULT_COORDX), m_fontsize_y(E_FONTSIZE_DEFAULT_COORDY),
    m_textalign(E_TEXTALIGN_DEFAULT),
    m_time(-1),
    m_textstyle(E_TEXTSTYLE_DEFAULT),
    m_monospace(E_MONOSPACE_DEFAULT),
    m_color(E_COLOR_DEFAULT),
    m_bgcolor(E_BGCOLOR_DEFAULT),
    m_fill(E_FILL_DEFAULT),
    m_fade(E_COLOR_DEFAULT), // ?
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
    ElementBase(def.name, def.desc, def.flags, def.has, def.enable),
    m_type(def.type),

    m_font(E_FONT_DEFAULT),
    m_fontsize_type(E_FST_POINT),
    m_fontsize_pt(E_FONTSIZE_DEFAULT_POINT), m_fontsize_x(E_FONTSIZE_DEFAULT_COORDX), m_fontsize_y(E_FONTSIZE_DEFAULT_COORDY),
    m_textalign(E_TEXTALIGN_DEFAULT),
    m_time(-1),
    m_textstyle(E_TEXTSTYLE_DEFAULT),
    m_monospace(E_MONOSPACE_DEFAULT),
    m_color(E_COLOR_DEFAULT),
    m_bgcolor(E_BGCOLOR_DEFAULT),
    m_fill(E_FILL_DEFAULT),
    m_fade(E_COLOR_DEFAULT), // ?
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



bool CPMAElement::parse_property( const wxString& cmd, wxString args )
{ 
  // let parent check on the property
  if (ElementBase::parse_property( cmd, args ))
    return true;

  //m_has = E_PROPERTIES_DEFAULT;

  if( cmd.CmpNoCase(wxT("doublebar")) == 0 )
  {
    if( m_type != E_T_BAR )
      wxLogWarning( _T("Found command `doublebar' which the element `%s' does not support."), m_name.c_str() );
    else
    {
      m_doublebar = true;
      m_has |= E_HAS_DOUBLEBAR;
    }
  }
  else if( cmd.CmpNoCase(wxT("font")) == 0 )
  {
    m_font = args;
    m_has |= E_HAS_FONT;
  }
  else if( cmd.CmpNoCase(wxT("fontsize")) == 0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    switch( targ.CountTokens() )
    {
    case 1:
      if( 1 != sscanf( targ.GetNextToken().mb_str(), "%i", &m_fontsize_pt ) )
      {
        wxLogWarning( _("Unknown `fontsize' argument in element `%s', should be one (pointsize) or two (xy-size) numbers. (defaultvalues set)"), m_name.c_str() );
        m_fontsize_pt = E_FONTSIZE_DEFAULT_POINT;
      }
      m_fontsize_x = E_FONTSIZE_DEFAULT_COORDX;
      m_fontsize_y = E_FONTSIZE_DEFAULT_COORDY;
      m_fontsize_type = E_FST_POINT;
      m_has |= E_HAS_FONTSIZE;
      break;

    case 2:
      if( 1 != sscanf( targ.GetNextToken().mb_str(), "%i", &m_fontsize_x ) ||
        1 !=sscanf( targ.GetNextToken().mb_str(), "%i", &m_fontsize_y ) )
      {
        wxLogWarning( _("Unknown `fontsize' argument in element `%s', should be one (pointsize) or two (xy-size) numbers. (defaultvalues set)"), m_name.c_str() );
        m_fontsize_x = E_FONTSIZE_DEFAULT_COORDX;
        m_fontsize_y = E_FONTSIZE_DEFAULT_COORDY;
      }
      m_fontsize_pt = E_FONTSIZE_DEFAULT_POINT;
      m_fontsize_type = E_FST_COORD;
      m_has |= E_HAS_FONTSIZE;
      break;

    default:
      wxLogWarning( _("Unknown `fontsize' argument in element `%s', should be one (pointsize) or two (xy-size) numbers. (ignored)"), m_name.c_str() );
      break;
    }  
  }
  else if( cmd.CmpNoCase(wxT("color"))==0 )
  {
    if( !m_color.from_string( args ) )
      wxLogWarning( _("Unknown `%s' argument: %s"), "color", args.c_str() );
    m_has |= E_HAS_COLOR;
  }
  else if( cmd.CmpNoCase(wxT("bgcolor"))==0 )
  {
    if( !m_bgcolor.from_string( args ) )
      wxLogWarning( _("Unknown `%s' argument: %s"), "bgcolor", args.c_str() );
    m_has |= E_HAS_BGCOLOR;
  }
  else if( cmd.CmpNoCase(wxT("fade"))==0 )
  {
    if( !m_fade.from_string( args ) )
      wxLogWarning( _("Unknown `%s' argument: %s"), "fade", args.c_str() );
    m_has |= E_HAS_FADE;
  }
  else if( cmd.CmpNoCase(wxT("time"))==0 )
  {
    sscanf( args.mb_str(), "%i", &m_time );
    m_has |= E_HAS_TIME;
  }
  else if( cmd.CmpNoCase(wxT("textalign"))==0 )
  {
    sscanf( args.mb_str(), "%c", &m_textalign );
    m_textalign = toupper( m_textalign );
    if( m_textalign != 'C' && m_textalign != 'L' && m_textalign != 'R')
    {
      wxLogWarning( _("Unknown `%c' argument: %s"), "textalign", m_textalign );
      m_textalign = 'L';
    }
    m_has |= E_HAS_TEXTALIGN;
  }
  else if( cmd.CmpNoCase(wxT("textstyle"))==0 )
  {
    sscanf( args.mb_str(), "%i", &m_textstyle );
    m_has |= E_HAS_TEXTSTYLE;
  }
  else if( cmd.CmpNoCase(wxT("monospace"))==0 )
  {
    m_monospace = true;
    m_has |= E_HAS_MONOSPACE;
  }
  else if( cmd.CmpNoCase(wxT("fill"))==0 )
  {
    m_fill = true;
    m_has |= E_HAS_FILL;
  }
  else if( cmd.CmpNoCase(wxT("draw3d"))==0 )
  {
    m_draw3d = true;
    m_has |= E_HAS_DRAW3D;
  }
  else if( cmd.CmpNoCase(wxT("image"))==0 )
  {
    m_image = args;
    wxTrim(m_image, wxT("\"'"));
    m_has |= E_HAS_IMAGE;
  }
  else if( cmd.CmpNoCase(wxT("model"))==0 )
  {
    m_model = args;
    if( m_type == E_T_ICON )
    { // model implies draw3d (and there is no model drawn, see hudspecs/README.superhud)
      m_draw3d = true;
      m_has |= E_HAS_DRAW3D;
      m_model = wxT("");
    }
    else
    {
      wxTrim(m_model, wxT("\"'"));
      m_has |= E_HAS_MODEL;
    }
  }
  else if( cmd.CmpNoCase(wxT("offset"))==0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    if( targ.CountTokens() != 3 ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_offset_x) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_offset_y) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_offset_z)
        )
      wxLogWarning( _("Unknown `offset' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    m_has |= E_HAS_OFFSET;
  }
  else if( cmd.CmpNoCase(wxT("angles"))==0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    float pitch = 0.f, yaw = 0.f, roll = 0.f, panrot = 0.f;
    if( targ.CountTokens() != 3 && targ.CountTokens() != 4 )
    {
      wxLogWarning( _("Unknown `angles' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    }
    else if (
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &pitch) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &yaw) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &roll)
        )
    {
      wxLogWarning( _("Unknown `angles' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    }
    else if( targ.CountTokens() == 1 )
      sscanf(targ.GetNextToken().mb_str(), "%f", &panrot); // optional
    m_angles_pitch = static_cast<int>(pitch);
    m_angles_yaw = static_cast<int>(yaw);
    m_angles_roll = static_cast<int>(roll);
    m_angles_panrot = static_cast<int>(panrot);

      
    m_has |= E_HAS_ANGLES;
  }
  else
  {
    return false; // not found
  }

  return true;
}


void CPMAElement::write_properties( wxTextOutputStream& stream ) const
{
  ElementBase::write_properties(stream);

  list<wxString> lines;
  if( (m_has & E_HAS_FONT)  && !m_font.empty() )
    lines.push_back( wxT("font ") + m_font );
  if( (m_has & E_HAS_TIME) && m_time >= 0 )
    lines.push_back(wxString::Format( wxT("time %i"), m_time ));
  if( (m_has & E_HAS_FONTSIZE) && m_fontsize_type == E_FST_POINT )
    lines.push_back(wxString::Format( wxT("fontsize %i"),  m_fontsize_pt));
  if( (m_has & E_HAS_FONTSIZE) && m_fontsize_type == E_FST_COORD )
    lines.push_back(wxString::Format( wxT("fontsize %i %i"),  m_fontsize_x, m_fontsize_y ));
  if( m_monospace )
    lines.push_back(wxT("monospace"));
  if( (m_has & E_HAS_TEXTSTYLE) && m_textstyle >= 0 )
    lines.push_back(wxString::Format( wxT("textstyle %i"), m_textstyle ));
  if( (m_has & E_HAS_TEXTALIGN) && m_textalign != ' ' )
    lines.push_back(wxString::Format( wxT("textalign %c"),  m_textalign));
  if( m_has & E_HAS_COLOR )
    //stream << wxString::Format( "\n  color %i %i %i %i", m_color.r_norm(), m_color.g_norm(), m_color.b_norm(), m_color.a_norm() );
    lines.push_back(wxT("color ") + m_color.to_string());
  if( m_has & E_HAS_BGCOLOR )
    lines.push_back(wxT("bgcolor ") + m_bgcolor.to_string());
  if( m_fill )
    lines.push_back(wxT("fill"));
  if( m_doublebar )
    lines.push_back(wxT("doublebar"));
  if( m_draw3d )
    lines.push_back(wxT("draw3d"));
  if( m_has & E_HAS_FADE )
    lines.push_back(wxT("fade ") + m_fade.to_string());
  /*
  if( m_ismodel )
  {
    if( m_has & E_HAS_MODEL )
      lines.push_back(wxT("model \"") + m_model + wxT("\""));
    if( m_has & E_HAS_SKIN )
      lines.push_back(wxT("image \"") + m_skin + wxT("\""));
    if( m_has & E_HAS_OFFSET )
    {
      wxString offset = wxT("offset ") + pretty_print_float(m_offset_x) + wxT(" ") + 
        pretty_print_float(m_offset_y) + wxT(" ") +
        pretty_print_float(m_offset_z);
      lines.push_back( offset );
    }
    if( m_has & E_HAS_ANGLES )
    {
      wxString angles = wxT("angles ") + pretty_print_float(m_angles_pitch) + wxT(" ") + 
        pretty_print_float(m_angles_yaw) + wxT(" ") +
        pretty_print_float(m_angles_roll);
      if( m_angles_panrot != 0.f )
        angles += wxT(" ") + pretty_print_float(m_angles_panrot);
      lines.push_back( angles );
    }
    
  }
  else
  {
    if( m_has & E_HAS_IMAGE )
      lines.push_back(wxT("image \"") + m_image + wxT("\""));
  }
  */

  if( m_flags & E_SHORT )
  {
    for( list<wxString>::const_iterator cit = lines.begin(); cit != lines.end(); ++cit )
      stream << *cit << wxT("; ");
  }
  else
  {
    for( list<wxString>::const_iterator cit = lines.begin(); cit != lines.end(); ++cit )
      stream << wxT("\n  ") << *cit;
  }   
}


wxString CPMAElement::iget_font() const
{
  wxString f = m_font;
  if( !(m_has & E_HAS_FONT) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FONT ));
    if( parent == 0 ) f = E_FONT_DEFAULT;
    else f = parent->iget_font();
  }
  return f; 
}
wxChar CPMAElement::iget_textalign() const
{
  wxChar ta = m_textalign;
  if( !(m_has & E_HAS_TEXTALIGN) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_TEXTALIGN ));
    if( parent == 0 ) ta = E_TEXTALIGN_DEFAULT;
    else ta = parent->iget_textalign();
  }
  return ta; 
}
bool CPMAElement::iget_monospace() const
{
  if( m_monospace )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_MONOSPACE ));

  if( parent == 0 )
    return false;

  return parent->iget_monospace();
}
int CPMAElement::iget_fontsizetype() const
{
  int fst = m_fontsize_type;
  if( !(m_has & E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FONTSIZE ));
    if( parent == 0 ) fst = E_FST_POINT;
    else fst = parent->iget_fontsizetype();
  }
  return fst; 
}
int CPMAElement::iget_fontsizept() const
{
  int s = m_fontsize_pt;
  if( !(m_has & E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FONTSIZE ));
    if( parent == 0 ) s = E_FONTSIZE_DEFAULT_POINT;
    else s = parent->iget_fontsizept();
  }
  return s;
}
int CPMAElement::iget_fontsizex() const
{
  int s = m_fontsize_x;
  if( !(m_has & E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FONTSIZE ));
    if( parent == 0 ) s = E_FONTSIZE_DEFAULT_COORDX;
    else s = parent->iget_fontsizex();
  }
  return s;
}
int CPMAElement::iget_fontsizey() const
{
  int s = m_fontsize_y;
  if( !(m_has & E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FONTSIZE ));
    if( parent == 0 ) s = E_FONTSIZE_DEFAULT_COORDY;
    else s = parent->iget_fontsizey();
  }
  return s;
}
int CPMAElement::iget_textstyle() const
{
  int ts = m_textstyle;
  if( !(m_has & E_HAS_TEXTSTYLE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_TEXTSTYLE ));
    if( parent == 0 ) ts = E_TEXTSTYLE_DEFAULT;
    else ts = parent->iget_textstyle();
  }
  return ts;
}
Color4 CPMAElement::iget_color() const
{
  Color4 c = m_color;
  if( !(m_has & E_HAS_COLOR) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_COLOR ));
    if( parent == 0 ) c = E_COLOR_DEFAULT;
    else c = parent->iget_color();
  }
  return c;
}
Color4 CPMAElement::iget_bgcolor() const
{
  Color4 c = m_bgcolor;
  if( !(m_has & E_HAS_COLOR) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_BGCOLOR ));
    if( parent == 0 ) c = E_BGCOLOR_DEFAULT;
    else c = parent->iget_color();
  }
  return c;
}
Color4 CPMAElement::iget_fade() const
{
  Color4 c = m_fade;
  if( !(m_has & E_HAS_FADE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FADE ));
    if( parent == 0 ) c = E_FADE_DEFAULT;
    else c = parent->iget_fade();
  }
  return c;
}



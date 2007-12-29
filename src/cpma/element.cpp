
#include "element.h"
#include "../hudfilebase.h"
#include "../displayctrlbase.h"
#include "../prefs.h"

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
    m_time(E_TIME_DEFAULT),
    m_textstyle(E_TEXTSTYLE_DEFAULT),
    m_color(E_COLOR_DEFAULT),
    m_bgcolor(E_BGCOLOR_DEFAULT),
    m_fade(E_COLOR_DEFAULT), // ?
    m_image(),
    m_model(),
    m_usemodel(false),
    m_angle_pitch(0), m_angle_yaw(0), m_angle_roll(0), m_angle_pan(0),
    m_text(text),
    m_icon(icon),
    //m_texid(HI_IMG_NOTLOADED),
    m_modelfound(false),
    m_skinfound(false)
{
  m_offset[0] = 0.f; m_offset[1] = 0.f; m_offset[2] = 0.f;
}

CPMAElement::CPMAElement( const hsitem_s& def ) :
    ElementBase(def.name, def.desc, def.flags, def.has, def.enable),
    m_type(def.type),

    m_font(E_FONT_DEFAULT),
    m_fontsize_type(E_FST_POINT),
    m_fontsize_pt(E_FONTSIZE_DEFAULT_POINT), m_fontsize_x(E_FONTSIZE_DEFAULT_COORDX), m_fontsize_y(E_FONTSIZE_DEFAULT_COORDY),
    m_textalign(E_TEXTALIGN_DEFAULT),
    m_time(E_TIME_DEFAULT),
    m_textstyle(E_TEXTSTYLE_DEFAULT),
    m_color(E_COLOR_DEFAULT),
    m_bgcolor(E_BGCOLOR_DEFAULT),
    m_fade(E_COLOR_DEFAULT), // ?
    m_image(),
    m_model(),
    m_usemodel(false),
    m_angle_pitch(0), m_angle_yaw(0), m_angle_roll(0), m_angle_pan(0),
    m_text(def.text),
    m_icon(def.icon),
    //m_texid(HI_IMG_NOTLOADED),
    m_modelfound(false),
    m_skinfound(false)
{
  m_offset[0] = 0.f; m_offset[1] = 0.f; m_offset[2] = 0.f;
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
    m_has |= E_HAS_MONOSPACE;
  }
  else if( cmd.CmpNoCase(wxT("fill"))==0 )
  {
    m_has |= E_HAS_FILL;
  }
  else if( cmd.CmpNoCase(wxT("draw3d"))==0 )
  {
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
    { // model implies draw3d (and there is no model drawn, see data/cpma/docs/README.superhud)
      m_has |= E_HAS_DRAW3D;
      m_model = wxT("");
    }
    else
    {
      wxTrim(m_model, wxT("\"'"));
      m_has |= E_HAS_MODEL;
    }
    set_usemodel();
  }
  else if( cmd.CmpNoCase(wxT("offset"))==0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    if( targ.CountTokens() != 3 ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_offset[E_OFFSET_X]) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_offset[E_OFFSET_Y]) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_offset[E_OFFSET_Z])
        )
      wxLogWarning( _("Unknown `offset' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    m_has |= E_HAS_OFFSET;
  }
  else if( cmd.CmpNoCase(wxT("angles"))==0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    float pitch = 0.f, yaw = 0.f, roll = 0.f, pan = 0.f;
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
      sscanf(targ.GetNextToken().mb_str(), "%f", &pan); // optional
    m_angle_pitch = static_cast<int>(pitch);
    m_angle_yaw = static_cast<int>(yaw);
    m_angle_roll = static_cast<int>(roll);
    m_angle_pan = static_cast<int>(pan);

      
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
  if( monospace() )
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
  if( fill() )
    lines.push_back(wxT("fill"));
  if( doublebar() )
    lines.push_back(wxT("doublebar"));
  if( draw3d() )
    lines.push_back(wxT("draw3d"));
  if( m_has & E_HAS_FADE )
    lines.push_back(wxT("fade ") + m_fade.to_string());
  if( usemodel() ) //m_has & E_HAS_MODEL )
  {
    if( m_has & E_HAS_MODEL )
      lines.push_back(wxT("model \"") + m_model + wxT("\""));
    if( m_has & E_HAS_SKIN )
      lines.push_back(wxT("image \"") + m_skin + wxT("\""));
    if( m_has & E_HAS_ANGLES )
    {
      wxString angles;
      if( m_angle_pan == 0 )
        angles = wxString::Format( wxT("angles %d %d %d"), m_angle_pitch, m_angle_yaw, m_angle_roll );
      else
        angles = wxString::Format( wxT("angles %d %d %d %d"), m_angle_pitch, m_angle_yaw, m_angle_roll, m_angle_pan );
      lines.push_back( angles );
    }
    if( m_has & E_HAS_OFFSET )
    {
      wxString offset = wxT("offset ") + pretty_print_float(m_offset[E_OFFSET_X]) + wxT(" ") + 
        pretty_print_float(m_offset[E_OFFSET_Y]) + wxT(" ") +
        pretty_print_float(m_offset[E_OFFSET_Z]);
      lines.push_back( offset );
    }
  }
  else if( m_has & E_HAS_IMAGE )
    lines.push_back(wxT("image \"") + m_image + wxT("\""));
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
      if( m_angles_pan != 0.f )
        angles += wxT(" ") + pretty_print_float(m_angles_pan);
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

int CPMAElement::iget_time() const
{
  int t = m_time;
  if( !(m_has & E_HAS_TIME) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_TIME ));
    if( parent == 0 ) t = E_TIME_DEFAULT;
    else t = parent->iget_time();
  }
  return t; 
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
  if( m_has & E_HAS_MONOSPACE )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_MONOSPACE ));

  return (parent != 0);
}
bool CPMAElement::iget_doublebar() const
{
  if( m_has & E_HAS_DOUBLEBAR )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_DOUBLEBAR ));

  return (parent != 0);
}
bool CPMAElement::iget_draw3d() const
{
  if( m_has & E_HAS_DRAW3D )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_DRAW3D ));

  return (parent != 0);
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
bool CPMAElement::iget_fill() const
{
  if( m_has & E_HAS_FILL )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_FILL ));

  return (parent != 0);
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

bool CPMAElement::iget_has(int what) const
{
  bool has = (m_has & what) != 0;
  if( !has )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, has ));
    has = (parent != 0);
  }
  return has;
}

wxString CPMAElement::iget_image() const
{
  wxString img = m_image;
  if( !(m_has & E_HAS_IMAGE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_IMAGE ));
    if( parent == 0 ) img = wxEmptyString;
    else img = parent->iget_image();
  }
  return img;
}
wxString CPMAElement::iget_model() const
{
  wxString m = m_model;
  if( !(m_has & E_HAS_MODEL) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_MODEL ));
    if( parent == 0 ) m = wxEmptyString;
    else m = parent->iget_model();
  }
  return m;
}
wxString CPMAElement::iget_skin() const
{
  wxString s = m_skin;
  if( !(m_has & E_HAS_SKIN) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_SKIN ));
    if( parent == 0 ) s = wxEmptyString;
    else s = parent->iget_skin();
  }
  return s;
}
float CPMAElement::iget_offset(int which) const
{
  float d = m_offset[which];
  if( !(m_has & E_HAS_OFFSET) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_OFFSET ));
    if( parent == 0 ) d = 0.f;
    else d = parent->iget_offset(which);
  }
  return d;
}
void CPMAElement::set_offset( int which, float val )
{
  if( which < E_OFFSET_X || which > E_OFFSET_Z )
    return;
  m_offset[which] = val;
}
int CPMAElement::iget_angle(int which) const
{
  int d = angle(which);
  if( !(m_has & E_HAS_ANGLES) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, E_HAS_ANGLES ));
    if( parent == 0 ) d = 0;
    else d = parent->iget_angle(which);
  }
  return d;
}
void CPMAElement::set_angle( int which, int val )
{
  switch(which)
  {
  case E_ANGLE_YAW: m_angle_yaw = val; break;
  case E_ANGLE_PITCH: m_angle_pitch = val; break;
  case E_ANGLE_ROLL: m_angle_roll = val; break;
  case E_ANGLE_PAN: m_angle_pan = val; break;
  }
}
int CPMAElement::angle(int which) const
{
  switch(which)
  {
  case E_ANGLE_YAW: return m_angle_yaw;
  case E_ANGLE_PITCH: return m_angle_pitch;
  case E_ANGLE_ROLL: return m_angle_roll;
  case E_ANGLE_PAN: return m_angle_pan;
  default:
    break;
  }
  return 0;
}
void CPMAElement::render() const
{
  //wxLogDebug(wxT("RENDER ") + m_name);
  if( m_flags & E_DRAWNEVER )
    return;
  bool hasownbg = true;

  switch( m_type )
  {
  case E_T_BAR:
    {
      Color4 color =  iget_color();
      color.glBind();
      if( iget_doublebar() )
      {
        wxRect top(m_rect), bottom(m_rect);
        top.height = top.height/2-2;
        bottom.height = top.height;
        bottom.y += bottom.height+4;
        draw_rect(top);
        draw_rect(top);(bottom);
      }
      else
        draw_rect(m_rect);
    }
    break;

  case E_T_USERICON:
    { // COLOR_T && FILL = border around ...
      /*
      int texid = hi->get_image_texid();
      bool fill = hi->get_fill();
      bool hasimage = (!hi->get_image().empty() && !hi->m_ismodel);
      Color4 bgcolor = hi->get_bgcolor();
      Color4 color = hi->get_color();
      if( color.is_special() )
      {
        bgcolor.set_type( color.get_type() );
        color.set_a1( bgcolor.a1() );


      }

      if( fill && bgcolor.a1() != 0.f )
      {
        bgcolor.glBind();
        render_rect(r);
      }
      if( (!fill || bgcolor.a1() == 0.f) &&
          (!hasimage || color.a1() == 0.f) )
      {
        render_helper_bg( hi, r );
      }
      if( hasimage )
      {
        glEnable( GL_TEXTURE_2D );
        color.glBind();
        glBindTexture( GL_TEXTURE_2D, (texid > 0 ? texid : m_default_texid) );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        render_rect( r, true );
        glDisable( GL_TEXTURE_2D );
      }
      else if( hi->m_ismodel && m_model_texid > 0)
      {
        glEnable( GL_TEXTURE_2D );
        color.glBind();
        glBindTexture( GL_TEXTURE_2D, m_model_texid );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        render_rect( r, true );
        glDisable( GL_TEXTURE_2D );
      }
      */
    }
    break;

  case E_T_ICON:
    {
      /*
      Color4 bgcolor = hi->get_bgcolor();
      Color4 color = hi->get_color();
      if( color.is_special() )
      {
        bgcolor.set_type( color.get_type() );
        color = HI_DEFAULTCOLOR;
      }
      color.set_a1(1.f); // NB for eg StatusBar_ArmorIcon the color would be completely discarded.

      if( hi->get_fill() )
      {
        bgcolor.glBind();
        render_rect(r);
      }
      else
        render_helper_bg( hi, r );
 
      if( !hi->m_icon.empty() && hi->m_texid > 0 )
      {
        glEnable( GL_TEXTURE_2D );
        color.glBind();
        glBindTexture( GL_TEXTURE_2D, hi->m_texid );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        render_rect( r, true );
        glDisable( GL_TEXTURE_2D );
      }
      */
    }
    break;


  case E_T_TEXT:
    { // COLOR_T && FILL = border around ...
      /*
      int sx, sy;
      wxString text = m_text;
      bool monospace = iget_monospace();
      fonts_t::const_iterator fit = m_fonts.find( hi->get_font() );
      wxRect rr(r);
      Color4 bgcolor = hi->get_bgcolor();
      Color4 color = hi->get_color();
      if( color.is_special() )
      {
        bgcolor.set_type( color.get_type() );
        color = HI_DEFAULTCOLOR;
      }

      hi->get_fontsize( &sx, &sy );
      
      if( !hi->get_fill() )
      {
        
        int w = (fit == m_fonts.end() ? 0 : fit->second->get_width(sx, text, monospace));
        switch( hi->get_textalign() )
        {
          case 'C':
            rr.x = rr.x + (rr.width - w)/2 ;
            break;
          case 'R':
            rr.x = rr.x + rr.width - w;
            break;
        }
        rr.width = w;
      }

      if( rr.width && rr.height && bgcolor.a1() != 0.f )
      {
        bgcolor.glBind();
        render_rect(rr);
      }
      else
        render_helper_bg( hi, r );

      if( fit != m_fonts.end() && !text.empty())
      {
        color.glBind();
        switch( hi->get_fontsizetype() )
        {
        case HIFST_POINT:
          fit->second->print( dc, r, sx, text, monospace, hi->get_textalign() );
          break;

        case HIFST_COORD:
          fit->second->print( dc, r, sx, sy, text, monospace, hi->get_textalign() );
          break;
        }
      }
      */
    }
    break;
  }

  glDisable(GL_TEXTURE_2D);
  // -- draw helper outline
  if( Prefs::get().var(wxT("helper")) )
  {
    if( false /*is_selected() && */ )
    {
      Prefs::get().var(wxT("helper_fill_selected")).colorval().glBind();
      draw_rect(m_rect);

      Prefs::get().var(wxT("helper_border_selected")).colorval().glBind();
    glBegin( GL_LINE_LOOP );
      glVertex2i( m_rect.GetLeft(), m_rect.GetBottom()+1 );
      glVertex2i( m_rect.GetRight(), m_rect.GetBottom()+1 );
      glVertex2i( m_rect.GetRight(), m_rect.GetTop()+1 );
      glVertex2i( m_rect.GetLeft(), m_rect.GetTop()+1 );
    glEnd();
    }
    else
    {
      Prefs::get().var(wxT("helper_fill")).colorval().glBind();
      draw_rect(m_rect);

      Prefs::get().var(wxT("helper_border")).colorval().glBind();
    glBegin( GL_LINE_LOOP );
      glVertex2i( m_rect.GetLeft(), m_rect.GetBottom()+1 );
      glVertex2i( m_rect.GetRight(), m_rect.GetBottom()+1 );
      glVertex2i( m_rect.GetRight(), m_rect.GetTop()+1 );
      glVertex2i( m_rect.GetLeft(), m_rect.GetTop()+1 );
    glEnd();
    }
  }
  glEnable(GL_TEXTURE_2D);
}

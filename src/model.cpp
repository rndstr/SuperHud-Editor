#include "model.h"
#include "common.h"
#include "pakmanager.h"

#ifdef __WXMAC__
  #include "OpenGL/gl.h"
  #include "OpenGL/glu.h" // checkcheck
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include <wx/sstream.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/regex.h>

Model::Model()
{

}

Model::~Model()
{
  cleanup();
}

void Model::cleanup()
{
  m_inds.clear();
  m_verts.clear();
}

void Model::render()
{
  if( !m_verts.size() )
    return;
  // triangle list
#if 0
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(m_verts[0].x));
  glDrawElements(GL_TRIANGLES, m_inds.size(), GL_UNSIGNED_SHORT, &m_inds[0]);
  glFlush();
  glDisableClientState( GL_VERTEX_ARRAY );
#else
  glBegin(GL_TRIANGLES);
  for( size_t i=0; i<m_inds.size(); i+=3 )
  {
    glNormal3f(
      m_verts[m_inds[i]].norm.x,
      m_verts[m_inds[i]].norm.y,
      m_verts[m_inds[i]].norm.z);
    glColor3f(
      m_verts[m_inds[i]].col.x,
      m_verts[m_inds[i]].col.y,
      m_verts[m_inds[i]].col.z);
    glVertex3f(
      m_verts[m_inds[i]].pos.x,
      m_verts[m_inds[i]].pos.y,
      m_verts[m_inds[i]].pos.z);

    glNormal3f(
      m_verts[m_inds[i+1]].norm.x,
      m_verts[m_inds[i+1]].norm.y,
      m_verts[m_inds[i+1]].norm.z);
    glColor3f(
      m_verts[m_inds[i+1]].col.x,
      m_verts[m_inds[i+1]].col.y,
      m_verts[m_inds[i+1]].col.z);
    glVertex3f(
      m_verts[m_inds[i+1]].pos.x,
      m_verts[m_inds[i+1]].pos.y,
      m_verts[m_inds[i+1]].pos.z);

    glNormal3f(
      m_verts[m_inds[i+2]].norm.x,
      m_verts[m_inds[i+2]].norm.y,
      m_verts[m_inds[i+2]].norm.z);
    glColor3f(
      m_verts[m_inds[i+2]].col.x,
      m_verts[m_inds[i+2]].col.y,
      m_verts[m_inds[i+2]].col.z);
    glVertex3f(
      m_verts[m_inds[i+2]].pos.x,
      m_verts[m_inds[i+2]].pos.y,
      m_verts[m_inds[i+2]].pos.z);
  }
  glEnd();
#endif
}

// NOTE that we make a few assumptions here
// (e.g. no triangle has different normals for different faces etc.)
bool Model::load_mde( const wxString& fpath, int search_where )
{
  cleanup();
  wxLogDebug(wxT("Loading model: ") + fpath);
  wxGetApp().mainframe()->statusbar()->PushStatusText(_("Loading model ") + fpath);

  char *buf;
  size_t size;
  if( !PakManager::get().load( &buf, fpath, search_where, &size ) )
  {
    wxLogError(_("Couldn't find/load file: %s"), fpath.c_str());
    wxGetApp().mainframe()->statusbar()->PopStatusText();
    return false;
  }
  wxMemoryInputStream mis( buf, size );
  wxTextInputStream contents( mis );


  wxRegEx face_re(wxT("f ([[:digit:]]*)/([[:digit:]]*)/([[:digit:]]*) ([[:digit:]]*)/([[:digit:]]*)/([[:digit:]]*) ([[:digit:]]*)/([[:digit:]]*)/([[:digit:]]*)"));
  wxString line;
  wxString cmd;
  wxString arg;
  float x, y, z;
  unsigned int minidx = 10 , maxidx = 0;
  vec3s_type norms;
  WX_DECLARE_STRING_HASH_MAP( Vec3, Material );
  Material mtls;
  wxString curmtl; // used while reading material and applying to faces
  // read file line by line and remove `#' comments
  while(!mis.Eof())
  {
    line = contents.ReadLine();
    she::wxTrim(line);
    if( 0 == line.length() || line[0] == '#' )
      continue;
    wxStringInputStream sis(line);
    wxTextInputStream lis(sis);
    lis >> cmd;
    if( cmd == wxT("newmtl") )
    {
      lis >> curmtl;
    }
    else if( cmd == wxT("Ka") )
    {
      lis >> x >> y >> z;
      mtls[curmtl] = Vec3(x, y, z);
    }
    else if( cmd == wxT("v") )
    {
      lis >> x >> y >> z;
      m_verts.push_back( Vertex(x, y, z) );
    }
    else if( cmd == wxT("vn") )
    {
      lis >> x >> y >> z;
      norms.push_back( Vec3(x, y, z) );
    }
    else if( cmd == wxT("usemtl") )
    {
      lis >> curmtl;
    }
    else if( cmd == wxT("f") ) // FACE!
    {
      if( face_re.Matches(line) )
      {
        unsigned long a, b, c;
        unsigned long na, nb, nc;
        face_re.GetMatch(line, 1).ToULong(&a);
        face_re.GetMatch(line, 3).ToULong(&na);
        face_re.GetMatch(line, 4).ToULong(&b);
        face_re.GetMatch(line, 6).ToULong(&nb);
        face_re.GetMatch(line, 7).ToULong(&c);
        face_re.GetMatch(line, 9).ToULong(&nc);
        // for now we assume indexing starts at 1
        // we will (TODO) fix it later if we detect that they actually start at 0
        m_inds.push_back(a-1);
        m_inds.push_back(b-1);
        m_inds.push_back(c-1);
        minidx = wxMin(wxMin(wxMin(b,c), a), minidx);
        maxidx = wxMax(wxMax(wxMax(b,c), a), maxidx);
        m_verts[a-1].norm = norms[na-1];
        m_verts[b-1].norm = norms[nb-1];
        m_verts[c-1].norm = norms[nc-1];
        m_verts[a-1].col = mtls[curmtl];
        m_verts[b-1].col = mtls[curmtl];
        m_verts[c-1].col = mtls[curmtl];
      }
      else
        wxLogWarning(wxT("face regex did not match! omgwtf?!"));
    }
  }

  wxLogDebug(wxT(" have read %d vertices and %d faces"), m_verts.size(), m_inds.size()/3);
  wxLogDebug(wxT(" vert indecies ref'ed by face: min = %d max = %d"),minidx, maxidx);
  
  // we no longer need the buffer
  PakManager::get().cleanup_lastloaded();
  if( minidx == 0 )
  {
    wxLogDebug( BUG_MSG + wxT("face references vertices starting at 0") );
    wxGetApp().mainframe()->statusbar()->PopStatusText();
    cleanup();
    return false;
  }

  wxGetApp().mainframe()->statusbar()->PopStatusText();
  return true;
}

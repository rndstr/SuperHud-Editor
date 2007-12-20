#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <wx/string.h>

struct Vec3
{
  Vec3() {}
  Vec3( float _x, float _y, float _z ) : x(_x), y(_y), z(_z) {}
  void set( float _x, float _y, float _z ) { x=_x; y=_y; z=_z; }
  float x, y, z;
};

struct Vertex
{
  Vertex( float x, float y, float z ) : pos(x, y, z) {}
  Vec3 pos;
  Vec3 norm;
  Vec3 col;
};


class Model
{
  protected:
    typedef std::vector<Vertex> vertices_type;
    typedef std::vector<unsigned short> indecies_type;
    typedef std::vector<Vec3> vec3s_type;
  public:
    Model();
    virtual ~Model();

    void cleanup();
    // combined mtl and obj
    bool load_mde( const wxString& fpath, int search_where );

    virtual void render();

  protected:
    vertices_type   m_verts;
    indecies_type   m_inds;
};

#endif // __MODEL_H__

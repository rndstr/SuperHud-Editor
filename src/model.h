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
#ifndef MODEL_H
#define MODEL_H

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
  Vertex( float x, float y, float z ) : pos(x, y, z), col(1.f, 1.f, 1.f) {}
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

#endif // MODEL_H

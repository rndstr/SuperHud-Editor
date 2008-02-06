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
#ifndef Q4MAXPROPERTIESNOTEBOOK_H
#define Q4MAXPROPERTIESNOTEBOOK_H

#include "propertiesnotebookbase.h"


class Q4MAXVisibilityPropertiesCtrl;
class Q4MAXTextPropertiesCtrl;

class Q4MAXPropertiesNotebook : public PropertiesNotebookBase
{
  public:
    Q4MAXPropertiesNotebook( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    Q4MAXVisibilityPropertiesCtrl   *m_vis;
    Q4MAXTextPropertiesCtrl         *m_text;

};

#endif

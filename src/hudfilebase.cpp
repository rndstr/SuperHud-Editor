#include "hudfilebase.h"

HudFileBase::HudFileBase() :
  m_modified(false)
{
}

void HudFileBase::clear()
{
  for( cit_elements cit = m_els.begin(); cit != m_els.end(); ++cit )
    delete (*cit);
  m_els.clear();
}

void HudFileBase::add( ElementBase *el )
{
  set_modified();
  m_els.push_back(el);
}

#include "command.h"

#include "superhudeditor.h"
#include "elementsctrlbase.h"

void ReorderCommand::takepre()
{
  m_undoels = wxGetApp().hudfile()->elements();
}

void ReorderCommand::takepost()
{
  m_doels = wxGetApp().hudfile()->elements();
}

bool ReorderCommand::Do()
{
  wxGetApp().hudfile()->m_els = m_doels;
  wxGetApp().elementsctrl()->list_refresh(wxGetApp().hudfile()->elements());
  // refreshing the hud changes selection but does not pull a (de)select event
  wxGetApp().mainframe()->elementsctrl()->OnSelectionChanged();
  return true;
}

bool ReorderCommand::Undo()
{
  wxGetApp().hudfile()->m_els = m_undoels;
  wxGetApp().elementsctrl()->list_refresh(wxGetApp().hudfile()->elements());
  // refreshing the hud changes selection but does not pull a (de)select event
  wxGetApp().mainframe()->elementsctrl()->OnSelectionChanged();
  return true;
}

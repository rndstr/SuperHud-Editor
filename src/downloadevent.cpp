/**********************************************************************
*  File: DownloadEvent.cpp
*
*  Purpose: Implementation of the download notification event for the
*  wxDownloadFile class.
*
*  Authors: Priyank Bolia <soft@priyank.in> 
*           Roland Schilter <roli@schilter.me>
*
*  Created: 24 December, 2005 
*
*  Modified: 09 August, 2006
*            06 September, 2006
*            added wxDownloadStream
*
*  Copyright (C) 2005-2006 by Priyank Bolia. All rights reserved.
*
*  Licence: wxWindows licence
* 
*  http://www.priyank.in
*********************************************************************/

#include "downloadevent.h"
#include <wx/stream.h>

DEFINE_EVENT_TYPE( wxEVT_DOWNLOAD )
IMPLEMENT_DYNAMIC_CLASS(wxDownloadEvent, wxNotifyEvent)

wxDownloadEvent::wxDownloadEvent(wxEventType commandType, int id)
: wxNotifyEvent(commandType, id)
, m_status(DOWNLOAD_NONE)
, m_url(wxT(""))
, m_localfname(wxT(""))
, m_bytesdownloaded(0)
, m_filesize(0)

{
}

wxDownloadEvent::wxDownloadEvent(const wxDownloadEvent& event)
: wxNotifyEvent(event)
{
	m_status = (wxDownloadEvent::eDownloadStatus)((wxDownloadEvent&)event).status();
	m_filesize = ((wxDownloadEvent&)event).filesize();
	m_bytesdownloaded = ((wxDownloadEvent&)event).bytesdownloaded();
	m_url = ((wxDownloadEvent&)event).url();
	m_localfname = ((wxDownloadEvent&)event).localfname();
  m_text = ((wxDownloadEvent&)event).text();
}

wxDownloadEvent::~wxDownloadEvent(void)
{
}


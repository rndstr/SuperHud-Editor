/**********************************************************************
*  File: DownloadEvent.cpp
*
*  Purpose: Implementation of the download notification event for the
*  wxDownloadFile class.
*
*  Authors: Priyank Bolia <soft@priyank.in> 
*           Roland Schilter <rolansch@ethz.ch>
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
, m_downloadStatus(DOWNLOAD_NONE)
, m_strURL(wxT(""))
, m_strFILE(wxT(""))
, m_strCONTENT(0)
, m_nBytesDownloaded(0)
, m_nFileSize(0)

{
}

wxDownloadEvent::wxDownloadEvent(const wxDownloadEvent& event)
: wxNotifyEvent(event)
{
	m_downloadStatus = (wxDownloadEvent::DownloadSTATUS)((wxDownloadEvent&)event).GetDownLoadStatus();
	m_nFileSize = ((wxDownloadEvent&)event).GetFileSize();
	m_nBytesDownloaded = ((wxDownloadEvent&)event).GetDownLoadedBytesCount();
	m_strURL = ((wxDownloadEvent&)event).GetDownLoadURL();
	m_strFILE = ((wxDownloadEvent&)event).GetDownLoadedFile();
  m_strCONTENT = ((wxDownloadEvent&)event).GetDownLoadedText();
}

wxDownloadEvent::~wxDownloadEvent(void)
{
}

int wxDownloadEvent::GetDownLoadStatus(void)
{
	return (int)m_downloadStatus;
}

void wxDownloadEvent::SetDownLoadStatus(int status)
{
	m_downloadStatus = (DownloadSTATUS)status;
}

wxString wxDownloadEvent::GetDownLoadURL(void)
{
	return m_strURL;
}

void wxDownloadEvent::SetDownLoadURL(wxString strURL)
{
	m_strURL = strURL;
}

wxString wxDownloadEvent::GetDownLoadedFile(void)
{
	return m_strFILE;
}

void wxDownloadEvent::SetDownLoadedFile(wxString strFILE)
{
	m_strFILE = strFILE;
}

wxString* wxDownloadEvent::GetDownLoadedText(void)
{
  return m_strCONTENT;
}


void wxDownloadEvent::SetDownLoadedText( wxString *strCONTENT )
{
  m_strCONTENT = strCONTENT;
}

bool wxDownloadEvent::GetDownloadAutoupdate()
{
  return m_autoupdate;
}

void wxDownloadEvent::SetDownloadAutoupdate( bool au )
{
  m_autoupdate = au;
}

wxInt64 wxDownloadEvent::GetDownLoadedBytesCount(void)
{
	return m_nBytesDownloaded;
}

void wxDownloadEvent::SetDownLoadedBytesCount(wxInt64 nBytes)
{
	m_nBytesDownloaded = nBytes;
}

wxInt64 wxDownloadEvent::GetFileSize(void)
{
	return m_nFileSize;
}

void wxDownloadEvent::SetFileSize(wxInt64 nSize)
{
	m_nFileSize = nSize;
}

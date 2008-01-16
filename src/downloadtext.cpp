/**********************************************************************
*  File: DownloadFile.cpp
*
*  Purpose: Class for downloading a file from a given URL and send back 
*  event notification to the owner, when the download fails or complete.
*
*  Authors: Priyank Bolia <soft@priyank.in> 
*           Roland Schilter <rolansch@student.ethz.ch>
*
*  Created: 24 December, 2005 
*
*  Modified: 12 January, 2006
*            06 September, 2006
*            added wxDownloadText
*
*  Copyright (C) 2005-2006 by Priyank Bolia. All rights reserved.
*
*  Licence: wxWindows licence
* 
*  http://www.priyank.in
*********************************************************************/

#include "downloadtext.h"
//#ifdef  __WXMSW__
//	#include <wx/msw/registry.h>
//#endif

#include <wx/wx.h>
#include <wx/fs_inet.h>
#include <wx/url.h>
#include <wx/file.h>
#include <wx/stream.h>
#include "prefs.h"

wxDownloadText::wxDownloadText(wxWindow *pParent, const wxString& strURL, bool bNotify, wxInt64 nBytes)
: wxThread(wxTHREAD_DETACHED)
, m_parent(pParent)
, m_url(strURL)
, m_downloading(true)
, m_notify(bNotify)
, m_notifybytes(nBytes)
{
	if ( this->Create() != wxTHREAD_NO_ERROR )
	{
		wxLogError(wxT("Can't create download thread!"));
	}
	else
	{
		if ( this->Run() != wxTHREAD_NO_ERROR )
		{
			wxLogError(wxT("Can't start download thread!"));
		}
	}
}

wxDownloadText::~wxDownloadText(void)
{
}

void* wxDownloadText::Entry()
{
    char c = 0;
    int bytesread = 0;
	m_downloading = true;
	wxDownloadEvent event( wxEVT_DOWNLOAD, GetId() );
	event.SetEventObject( (wxObject *)this->This() );
	event.set_url( m_url);
	event.set_status(wxDownloadEvent::DOWNLOAD_RUNNING);
	
#if 0
  // -- according to docs this is the way to go but i couldn't figure out
  // how to set the proxy...
  wxFileSystem fs;
  wxFSFile *file = fs.OpenFile( m_url );
  if( 0 == file )
  {
    event.SetDownLoadStatus(wxDownloadEvent::DOWNLOAD_FAIL);
		if(m_parent)
      m_parent->GetEventHandler()->ProcessEvent( event );
    return 0;
  }
  
  wxStreamBuffer streambuf( *file->GetStream(), wxStreamBuffer::read ); 
 
  size_t size = file->GetStream()->GetSize(); 
  if( !size )
  { // empty file received
    event.SetDownLoadStatus(wxDownloadEvent::DOWNLOAD_COMPLETE);
    if(m_parent)
      m_parent->GetEventHandler()->ProcessEvent( event );
    return 0;
  }
 
  char *buf = new char[ size + 1 ];                             
  size_t haveread = streambuf.Read( buf, size );
  buf[size] = '\0';
  delete file;
  char *ptr = buf;
  while(*ptr)
  {
    wxString add((char)*ptr, (size_t)1);
    m_strContent += add;
    ++ptr;
  }
  delete [] buf;

  event.SetDownLoadStatus(wxDownloadEvent::DOWNLOAD_COMPLETE);
  if(m_parent)
    m_parent->GetEventHandler()->ProcessEvent( event );
  return 0;
#else

	wxURL Url(m_url);
	((wxProtocol &)Url.GetProtocol()).SetTimeout(100);
	if (Url.GetError() == wxURL_NOERR)
	{
		wxInputStream *pIn_Stream = NULL;

    /*
#ifdef __WXMSW__
		wxRegKey *pRegKey = new wxRegKey(wxT("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"));
		if( pRegKey->Exists() && pRegKey->HasValue(wxT("ProxyEnable")))
		{
			long lProxyEnable;
			pRegKey->QueryValue(wxT("ProxyEnable"), &lProxyEnable);
			if(lProxyEnable == 1 && pRegKey->HasValue(wxT("ProxyServer")))
			{
				wxString strProxyAddress;
				pRegKey->QueryValue(wxT("ProxyServer"), strProxyAddress);
				Url.SetProxy(strProxyAddress);
			}
		}
    // FIXME if no proxy it doesn't work? ;)
		delete pRegKey;
#endif
    */


    if( !Prefs::get().var(wxT("net_proxy")).sval().empty() )
      Url.SetProxy( Prefs::get().var(wxT("net_proxy")).sval());
		pIn_Stream = Url.GetInputStream();
    if(!pIn_Stream)
      wxLogDebug(wxT("Cannot get a stream!"));
		if(pIn_Stream)
		{
			if(pIn_Stream->GetSize() != 0xFFFFFFFF)
				event.set_filesize(pIn_Stream->GetSize());
			wxInt64 nCount = 0;
			while ((bytesread = (int)(pIn_Stream->Read(&c, 1)).LastRead()) > 0 && m_downloading && !TestDestroy() )
			{
        wxString add((char)c, (size_t)1);
        event.m_text += add;
        //m_strContent += add;
				nCount += bytesread;
				if (m_notify && (nCount%m_notifybytes) == 0 && nCount>=m_notifybytes) 
				{
					event.set_status(wxDownloadEvent::DOWNLOAD_INPROGRESS);
					event.set_bytesdownloaded(nCount);
          m_parent->AddPendingEvent(event);

					//m_parent->GetEventHandler()->ProcessEvent( event );
				}
			}
			delete pIn_Stream;
			event.set_status(wxDownloadEvent::DOWNLOAD_COMPLETE);
			if(m_parent)
        m_parent->AddPendingEvent(event);
				//m_parent->GetEventHandler()->ProcessEvent( event );
		}
		else
		{
			event.set_status(wxDownloadEvent::DOWNLOAD_FAIL);
			if(m_parent)
        m_parent->AddPendingEvent(event);
				//m_parent->GetEventHandler()->ProcessEvent( event );
		}
	}
	else
	{
		event.set_status(wxDownloadEvent::DOWNLOAD_FAIL);
		if(m_parent)
      m_parent->AddPendingEvent(event);
			//m_parent->GetEventHandler()->ProcessEvent( event );
	}
	return 0;
#endif
}

void wxDownloadText::OnExit()
{
}

void wxDownloadText::CancelDownload(void)
{
	m_downloading = false;
	m_parent = NULL;
}

void wxDownloadText::SetDownloadingNotification(bool bEnable, wxInt64 nBytes)
{
	m_notify = bEnable;
	m_notifybytes = nBytes;
}

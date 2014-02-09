/**********************************************************************
*  File: DownloadFile.h
*
*  Purpose: Class for downloading a file from a given URL and send back 
*  event notification to the owner, when the download fails or complete.
*
*  Authors: Priyank Bolia <soft@priyank.in> 
*           Roland Schilter <roli@schilter.me>
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
*
*********************************************************************/

#ifndef DOWNLOADSTREAM_H
#define DOWNLOADSTREAM_H

//#include <wx/wx.h>
#include <wx/thread.h>
#include "downloadevent.h"

class wxDownloadText : public wxThread
{
private:
	wxWindow *m_parent;		/// parent window pointer
	wxString m_url;			/// URL from where to download the file
	bool m_downloading;			/// boolean value to stop the download, when user cancels the download
	bool m_notify;	/// Enable or disable the downloading in progress notification.
	wxInt64 m_notifybytes;		/// After how many number of bytes downloaded, to send the notification.
  bool m_autoupdate;

public:
	/**
	 * Constructor
	 * \param *pParent parent window pointer
	 * \param strURL URL from where to download the file
   * \param autoupdate If true, we only issue messageboxes if error or new version available.
	 * \param bNotify Enable or disable the downloading in progress notification.
	 * \param nBytes After how many number of bytes downloaded, to send the notification.
	 * \return 
	 */
	wxDownloadText(wxWindow *pParent, const wxString& strURL, bool bNotify = false, wxInt64 nBytes = 1000);
	/**
	 * Virtual destructor
	 * \param void 
	 * \return 
	 */
	virtual ~wxDownloadText();

public:
    /// thread execution starts here
    virtual void *Entry();

	/// called when the thread exits - whether it terminates normally or is
    /// stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
	/**
	 * Function to cancel the download, by the user/program.
	 * \param void 
	 */
	void CancelDownload(void);

	/**
	 * Sends an DOWNLOAD_INPROGRESS event after the specified number of bytes downloaded.
	 * \param bool Enable or disable the notification.
	 * \param wxInt64 After how many bytes downloaded, the notification should be sent.
	 */
	void SetDownloadingNotification(bool bEnable = true, wxInt64 nBytes = 1000);
};

#endif


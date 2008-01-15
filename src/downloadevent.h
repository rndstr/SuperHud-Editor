/**********************************************************************
*  File: DownloadEvent.h
*
*  Purpose: Declaration of the download notification event for the
*  wxDownloadFile class.
*
*  Authors: Priyank Bolia <soft@priyank.in> 
*           Roland Schilter <rolansch@ethz.ch>
*
*  Created: 24 December, 2005 
*
*  Modified: 12 January, 2006
*            06 September, 2006
*            added wxDownloadStream 
*
*  Copyright (C) 2005-2006 by Priyank Bolia. All rights reserved.
*
*  Licence: wxWindows licence
* 
*  http://www.priyank.in
*********************************************************************/

#ifndef DOWNLOADEVENT_H
#define DOWNLOADEVENT_H

#include <wx/event.h>

class wxDownloadFile;
class wxDownloadStream;
class wxOutputStream;

class wxDownloadEvent : public wxNotifyEvent
{
public:
	/// Current status of download
	enum eDownloadStatus
	{
		DOWNLOAD_NONE = -2,				/// No Status, yet to be initialized
		DOWNLOAD_FAIL = -1,				/// Failed to download the file
		DOWNLOAD_RUNNING = 0,			/// Download is currently running
		DOWNLOAD_COMPLETE = 1,			/// Download has completed
		DOWNLOAD_INPROGRESS = 2			/// Number of bytes downloaded notification.
	};

private:
	eDownloadStatus m_status;	/// download current status value from the DownloadSTATUS enum
	wxString m_url;					/// URL from where to download the file
	wxString m_localfname;					/// local file name where the downloaded file would be saved (wxT("") if no file).
	wxInt64 m_bytesdownloaded;			///< Number of bytes downloaded.
	wxInt64 m_filesize;				///< The size of the downloading file.
  wxString m_text;

	friend class wxDownloadFile;
  friend class wxDownloadText;

public:
	/**
	 * Constructor
	 * \param commandType Event Type
	 * \param id Unique id for the event handler to distinguish between multiple instances.
	 * \return 
	 */
	wxDownloadEvent(wxEventType commandType = wxEVT_NULL, int id = 0 );

	/**
	 * Copy Constructor
	 * \param event const wxDownloadEvent
	 * \return 
	 */
	wxDownloadEvent(const wxDownloadEvent& event);

	/**
	 * Virtual Destructor
	 * \param void 
	 * \return 
	 */
	virtual ~wxDownloadEvent(void);

public:
    /**
     * Returns an identical copy of itself. Required for sending with wxPostEvent() as references
	 * which must be copied, it does not make sense to have a reference counting scheme to do this cloning,
	 * because all wxDownloadEvent should have separate data.
     * \return 
     */
    virtual wxEvent *Clone() const
		{ return new wxDownloadEvent(*this); }

	/**
	 * Returns the current download status.
	 * \param void 
	 * \return 
	 */
    int status() const { return m_status; }

	/**
	 * Returns the download URL, from where the file has to be downloaded.
	 * \param void 
	 * \return 
	 */
    wxString url() const { return m_url; }

	/**
	 * Returns the local file name, of the downloaded file.
	 * \param void 
	 * \return 
	 */
    wxString localfname() const { return m_localfname; }


  /**
   * Returns the stream the downloaded file is written to.
   */
    wxString text() const { return m_text; }


	/**
	 * Returns the number of bytes downloaded.
	 * \param void 
	 * \return Number of bytes downloaded.
	 */
    wxInt64 bytesdownloaded() const { return m_bytesdownloaded; }

	/**
	 * Returns the size of the downloading file.
	 * \param void 
	 * \return The size of the downloading file.
	 */
    wxInt64 filesize() const { return m_filesize; }

private:

	/**
	 * Sets the current download status.
	 * \param int DownloadSTATUS enum value
	 */
  void set_status(int status) { m_status = (eDownloadStatus)status; }

	/**
	 * Sets the download URL, from where the file has to be downloaded.
	 * \param wxString Download URL
	 */
  void set_url( const wxString& url ) { m_url = url; }

	/**
	 * Sets the local file name, of the downloaded file.
	 * \param wxString Local file name, where to download the file.
	 */
  void set_localfname( const wxString& lfname ) { m_localfname = lfname; }

  /**
   * Sets the stream we write the downloaded file to.
   */
  void set_text( const wxString& text ) { m_text = text; }

	/**
	 * Sets the number of bytes downloaded; not to be called by the user.
	 * \param wxInt64 Number of bytes downloaded.
	 */
  void set_bytesdownloaded(wxInt64 b) { m_bytesdownloaded = b; }

	/**
	 * Sets the size of the downloading file.
	 * \param nSize Size of the downloading file
	 */
  void set_filesize(wxInt64 fsize) { m_filesize = fsize; }

private:
	/**
	 * The objects of wxDownloadEvent class should be dynamically creatable from run-time type information.
	 */
	DECLARE_DYNAMIC_CLASS(wxDownloadEvent);
};

typedef void (wxEvtHandler::*wxDownloadEventFunction)(wxDownloadEvent&);

/**
 * Declares the event types to be used by the user in their event tables.
 */
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_DOWNLOAD, wxID_EVENT_DOWNLOAD)
END_DECLARE_EVENT_TYPES()

#define EVT_DOWNLOAD(fn) DECLARE_EVENT_TABLE_ENTRY( \
	wxEVT_DOWNLOAD, wxID_ANY, wxID_ANY, (wxObjectEventFunction) (wxEventFunction) \
	(wxDownloadEventFunction) & fn, (wxObject *) NULL ),

#endif // DOWNLOADEVENT_H


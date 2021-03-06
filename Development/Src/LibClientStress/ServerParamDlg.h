#ifndef _SERVER_PARAM_DLG_H__
#define _SERVER_PARAM_DLG_H__

#include <wx\dialog.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class ServerParamDlg : public wxDialog
{
public:
	ServerParamDlg(wxWindow* pParent);
	virtual ~ServerParamDlg();

	DECLARE_EVENT_TABLE()

	void OnConfirm(wxCommandEvent& event);
	void OnNewLine(wxCommandEvent& event);
	void OnDeleteLine(wxCommandEvent& event);

protected:
	void LoadScript();
	void SaveScript();

private:
	wxPropertyGrid* m_pPropGrid;
};

#endif //_SERVER_PARAM_DLG_H__



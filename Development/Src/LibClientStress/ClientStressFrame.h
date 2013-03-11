#ifndef __CLIENT_STRESS_FRAME_INCLUDE__
#define __CLIENT_STRESS_FRAME_INCLUDE__

namespace Atlas
{
	class CStressClient;
	class CStressManager;
	class CStressCase;
	class CmdHistory;
}

class CClientStressFrame;

class CStressFrameView : public wxPanel
{
public:
	CStressFrameView(CClientStressFrame* pFrame, wxWindow* pParent, const wxString& pTabName) : wxPanel(pParent), m_pFrame(pFrame), m_pTabName(pTabName), m_nCurrentClient((_U32)-1)
	{
	}

	const wxString& GetTabName() { return m_pTabName; }
	CClientStressFrame* GetFrame() { return m_pFrame; }

	//
	void SwitchClient(_U32 index)
	{
		if(m_nCurrentClient!=-1) OnClear();
		m_nCurrentClient = index;
		if(m_nCurrentClient!=-1) OnSwitchTo(m_nCurrentClient);
	}

	_U32 GetCurrentClient() { return m_nCurrentClient; }

	virtual void OnSwitchTo(_U32 index) { }
	virtual void OnClear() { }
	virtual void OnNewClient(_U32 index) { }
	virtual void OnNewCase(_U32 index, Atlas::CStressCase* pCase) { }

private:
	CClientStressFrame* m_pFrame;
	wxString m_pTabName;
	_U32 m_nCurrentClient;
};

class CClientStressFrame: public wxFrame, public sigslot::has_slots<>
{
	DECLARE_EVENT_TABLE()

public:
	CClientStressFrame();
	virtual ~CClientStressFrame();

	void InitMenu();
	void InitToolBar();
	void InitClient();

	void OnProtocal(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnDoCmd(wxCommandEvent& event);
	void OnCmdEnter(wxCommandEvent& event);
	void OnAddClient(wxCommandEvent& event);
	void OnSelectAll(wxCommandEvent& event);
	void OnLogin(wxCommandEvent& event);
	void OnLogout(wxCommandEvent& event);
	void OnAddCase(wxCommandEvent& event);
	
	void OnClientSelected(wxCommandEvent& event);
	void OnStressView(wxCommandEvent& event);
	void OnRunScript(wxCommandEvent& event);
	void OnOpenSvrParamDlg(wxCommandEvent& event);

	void OnSize(wxSizeEvent& event);
	void OnShow(wxShowEvent& event);
	void OnTimer(wxTimerEvent& event);

	void UpdateClientList();
	void GetSelectClients(Atlas::Vector<_U32>& clients);

	void AddView(CStressFrameView* pView);
	Atlas::CStressClient* GetStressClient(_U32 id);

	void NotifyClientAdd(_U32 index);
	void NotifyClientAddCase(_U32 index, Atlas::CStressCase* pCase);

protected:
	bool ProcessJsonCommand(const DDLReflect::CLASS_INFO* classinfo, _U16 fid, const Atlas::String& json);

private:
	wxNotebook*				m_pTabView;
	wxSplitterWindow*		m_pSplitter;
	wxListBox*				m_pClientList;
	wxComboBox*				m_pCmdText;
	wxButton*				m_pCmdButton;
	wxTimer					m_Timer;
	
	struct
	{
		int x, y, w, h, m;
	}						m_FrameData;

	CStressFrameView*		m_pViews[100];
	int						m_nViewCount;

	_U32					m_nCurrentIndex;

	Atlas::CmdHistory*				m_pCmdHistory;
};

#endif

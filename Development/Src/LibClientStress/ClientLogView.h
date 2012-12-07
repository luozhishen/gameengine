#ifndef __CLIENT_LOG_VIEW_INCLUDE__
#define __CLIENT_LOG_VIEW_INCLUDE__

#include <string>
#include <map>
#include <vector>

class CClientLogItem
{
public:
	CClientLogItem() : m_Logs(2000)
	{
		m_nStart = 0;
		m_nCount = 0;
		m_bEnable = true;
	}
	void Clear()
	{
		m_nStart = 0;
		m_nCount = 0;
	}
	void Append(const char* msg)
	{
		size_t index;
		if(m_nCount<m_Logs.size())
		{
			index = (m_nStart+m_nCount) % m_Logs.size();
			m_nCount++;
		}
		else
		{
			index = (m_nStart+m_nCount) % m_Logs.size();
			m_nStart++;
		}
		m_Logs[index] = msg;
	}

	std::vector<std::string> m_Logs;
	size_t m_nStart, m_nCount;
	bool m_bEnable;
};

class CClientLogView : public CStressFrameView, public sigslot::has_slots<>
{
	DECLARE_EVENT_TABLE()

public:
	CClientLogView(CClientStressFrame* pFrame, wxWindow* pParent);
	virtual ~CClientLogView();

	void OnClickEnable(wxCommandEvent& event);
	void OnClickClear(wxCommandEvent& event);
	void Append(_U32 index, const char* msg);

	virtual void OnSwitchTo(_U32 index);
	virtual void OnClear();
	virtual void OnNewClient(_U32 index);

	void OnConnected(_U32 index);
	void OnDisconnected(_U32 index);
	void OnData(_U32 index, _U16 iid, _U16 fid, _U32 len, const _U8* data);
	void OnConnectFailed(_U32 index);
	void OnLoginDone(_U32 index);

private:
	wxCheckBox*			m_pEnable;
	wxCheckBox*			m_pAutoReflush;
	wxTextCtrl*			m_pLog;
	std::map<_U32, CClientLogItem> m_Clients;
};

#endif

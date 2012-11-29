#ifndef __CLIENT_STRESS_APP_INCLUDE__
#define __CLIENT_STRESS_APP_INCLUDE__

class CClientStressFrame;

class CClientStressApp : public wxApp
{
public:
	CClientStressApp();
	~CClientStressApp();

	virtual bool OnInit();
	virtual int OnExit();

	virtual void InitClientView(CClientStressFrame* pFrame, wxWindow* pParent);
	virtual void InitCase() = 0;
};

#endif

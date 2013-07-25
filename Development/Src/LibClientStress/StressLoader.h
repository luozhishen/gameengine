#ifndef __STRESS_LOADER_H__
#define __STRESS_LOADER_H__

class CClientStressFrame;

namespace Zion
{

	class CStressCaseTemplate;

	class CStressLoader
	{
	public:
		CStressLoader(CClientStressFrame* pView=NULL);
		~CStressLoader();

		bool LoadTemplate(const char* tpl);
		_U32 CreateClient(const char* tpl_name);
		Zion::Array<Zion::String> GetNames();

	private:
		Zion::Map<Zion::String, CStressCaseTemplate*> m_Tpls;
		CClientStressFrame* m_pView;
	};

}

#endif

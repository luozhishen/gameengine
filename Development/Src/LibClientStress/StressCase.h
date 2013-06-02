#ifndef __ZION_STRESS_CASE__
#define __ZION_STRESS_CASE__

namespace Zion
{

	class CClient;
	class CStressClient;
	class CStressManager;

	class CStressCase : public sigslot::has_slots<>
	{
		friend class CStressClient;
		friend class CStressManager;
	public:
		CStressCase(const char* name);
		virtual ~CStressCase();

		CClient* GetClient();
		CStressClient* GetStressClient() { return m_pClient; }
		const Zion::String& GetName() { return m_strName; }

		virtual const DDLReflect::STRUCT_INFO* GetConfigType() { return NULL; }
		bool GetConfig(void* pConfig, _U32 size);
		bool SetConfig(const void* pConfig, _U32 size);
		const Zion::String GetInfo();

		virtual void OnAttach()	{}
		virtual void OnTick()	{}
		virtual void OnDetach()	{}

	protected:
		virtual CStressCase* Create() = 0;
		void Attach(CStressClient* pClient);
		void Detach(CStressClient* pClient);
		virtual bool _GetConfig(void* pConfig, _U32 size) { return false; }
		virtual bool _SetConfig(const void* pConfig, _U32 size) { return false; }
		virtual void _GetInfo(Zion::String& info) { }

	private:
		Zion::String m_strName;
		CStressClient* m_pClient;
	};

}

#endif 

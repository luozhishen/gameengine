#ifndef __STRESS_CASES__
#define __STRESS_CASES__

namespace Atlas
{

	class CLoginCase : public CStressCase
	{
	public:
		CLoginCase();
		virtual ~CLoginCase();

		virtual void OnAttach();
		virtual void OnTick();
		virtual void OnDetach();

		void OnLoginFailed();
		void OnLoginDone();
		void OnDisconnected();

	protected:
		virtual CStressCase* Create();
		virtual void _GetInfo(std::string& info);

	};
}

#endif  // __STRESS_CASES__

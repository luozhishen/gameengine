#ifndef __ZION_CLIENT_LOGIN__
#define __ZION_CLIENT_LOGIN__

namespace Zion
{

	class CClientLoginMethod
	{
	public:
		CClientLoginMethod(_U32 type);
		_U32 GetType() const;

	private:
		_U32 m_Type;
	};

	class CClientLoginMethodByDevice : public CClientLoginMethod
	{
	public:
		static const _U32 METHOD_TYPE = 1;

		CClientLoginMethodByDevice();
		const Zion::String& GetDeviceID() const;
		void SetDeviceID(const char* device_id);

	private:
		Zion::String m_DeviceID;
	};

	class CClientLoginMethodByToken : public CClientLoginMethod
	{
	public:
		static const _U32 METHOD_TYPE = 2;

		CClientLoginMethodByToken();
		void SetToken(const char* token);
		const Zion::String& GetToken() const;

	private:
		Zion::String m_Token;
	};

}

#endif

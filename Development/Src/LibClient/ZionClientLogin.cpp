#include <ZionBase.h>
#include <mosdk.h>
#include "ZionClientLogin.h"

namespace Zion
{

	CClientLoginMethod::CClientLoginMethod(_U32 type)
	{
		m_Type = type;
	}

	_U32 CClientLoginMethod::GetType() const
	{
		return m_Type;
	}

	CClientLoginMethodByDevice::CClientLoginMethodByDevice() :  CClientLoginMethod(CClientLoginMethodByDevice::METHOD_TYPE)
	{
		m_DeviceID = MOGetDeviceUDID();
	}

	const Zion::String& CClientLoginMethodByDevice::GetDeviceID() const
	{
		return m_DeviceID;
	}

	void CClientLoginMethodByDevice::SetDeviceID(const char* device_id)
	{
		m_DeviceID = device_id;
	}

	CClientLoginMethodByToken::CClientLoginMethodByToken() :  CClientLoginMethod(CClientLoginMethodByToken::METHOD_TYPE)
	{
	}

	void CClientLoginMethodByToken::SetToken(const char* token)
	{
		m_Token = token;
	}

	const Zion::String& CClientLoginMethodByToken::GetToken() const
	{
		return m_Token;
	}

}

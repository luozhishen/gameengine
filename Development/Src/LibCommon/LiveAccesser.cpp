#include <ZionBase.h>
#include "CommonDDL.h"
#include "StubDispatcher.h"
#include "LiveData.h"
#include "LiveAccesser.h"

namespace Zion
{
	namespace LiveData
	{

		CAccesser::CAccesser(CManager* pManager)
		{
			m_LastNum = 0;
			m_pManager = pManager;
		}

		void CAccesser::InitRandom(_U32 seed)
		{
			m_LastNum = seed;
		}

		_F32 CAccesser::Rand()
		{
			String val = CHashMD5::CalcToString(&m_LastNum, sizeof(m_LastNum));
			_U32 m_LastNum = CalcCRC32(val.c_str(), (_U32)val.size());
			return ((_F32)(m_LastNum & 0xffffff)) / 0xffffff;
		}

	}
}

#include <ZionBase.h>
#include "ConfusedNumber.h"

namespace Zion
{

	static _U32 CNUM_MASK = (_U32)-1;

	_U32 CNUM_GET(const A_CONFUSED_U32& num)
	{
		return num.value + num.mask;
	}

	void CNUM_SET(A_CONFUSED_U32& num, _U32 val)
	{
		num.mask = (CNUM_MASK++);
		num.value = val - num.mask;
	}

	_S32 CNUM_GET(const A_CONFUSED_S32& num)
	{
		_U32 val = CNUM_GET(*((const A_CONFUSED_U32*)&num));
		return *((_S32*)&val);
	}

	void CNUM_SET(A_CONFUSED_S32& num, _S32 val)
	{
		CNUM_SET(*((A_CONFUSED_U32*)&num), *((_U32*)&val));
	}

	_F32 CNUM_GET(const A_CONFUSED_F32& num)
	{
		_U32 val = CNUM_GET(*((const A_CONFUSED_U32*)&num));
		return *((_F32*)&val);
	}

	void CNUM_SET(A_CONFUSED_F32& num, _F32 val)
	{
		CNUM_SET(*((A_CONFUSED_U32*)&num), *((_U32*)&val));
	}

	CNUM_U32::CNUM_U32()
	{
	}

	CNUM_U32::CNUM_U32(const A_CONFUSED_U32& num)
	{
		m_num = num;
	}

	CNUM_U32::CNUM_U32(const CNUM_U32& num)
	{
		m_num = num.m_num;
	}

	_U32 CNUM_U32::Get() const
	{
		return CNUM_GET(m_num);
	}

	void CNUM_U32::Set(_U32 val)
	{
		CNUM_SET(m_num, val);
	}

	CNUM_S32::CNUM_S32()
	{
	}

	CNUM_S32::CNUM_S32(const A_CONFUSED_S32& num)
	{
		m_num = num;
	}

	CNUM_S32::CNUM_S32(const CNUM_S32& num)
	{
		m_num = num.m_num;
	}

	_S32 CNUM_S32::Get() const
	{
		return CNUM_GET(*((const A_CONFUSED_S32*)&m_num));
	}

	void CNUM_S32::Set(_S32 val)
	{
		CNUM_SET(*((A_CONFUSED_S32*)&m_num), val);
	}

	CNUM_F32::CNUM_F32()
	{
	}

	CNUM_F32::CNUM_F32(const A_CONFUSED_F32& num)
	{
		m_num = num;
	}

	CNUM_F32::CNUM_F32(const CNUM_F32& num)
	{
		m_num = num.m_num;
	}

	_F32 CNUM_F32::Get() const
	{
		return CNUM_GET(*((const A_CONFUSED_F32*)&m_num));
	}

	void CNUM_F32::Set(_F32 val)
	{
		CNUM_SET(*((A_CONFUSED_F32*)&m_num), val);
	}

}

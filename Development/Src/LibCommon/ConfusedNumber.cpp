#include <ZionBase.h>
#include <ZionCommon.h>
#include "ConfusedNumber.h"

namespace Zion
{

	static _U32 CNUM_MASK = (_U32)-1;

	_U32 CNUM_GET(const A_CONFUSED_NUMBER& num)
	{
		return num.value + num.mask;
	}

	void CNUM_SET(A_CONFUSED_NUMBER& num, _U32 val)
	{
	}

	_S32 CNUM_GET(const A_CONFUSED_INT& num)
	{
		_U32 val = CNUM_GET(*((const A_CONFUSED_NUMBER*)&num));
		return *((_S32*)&val);
	}

	void CNUM_SET(A_CONFUSED_INT& num, _S32 val)
	{
		CNUM_SET(*((A_CONFUSED_NUMBER*)&num), *((_U32*)&val));
	}

	_F32 CNUM_GET(const A_CONFUSED_FLOAT& num)
	{
		_U32 val = CNUM_GET(*((const A_CONFUSED_NUMBER*)&num));
		return *((_F32*)&val);
	}

	void CNUM_SET(A_CONFUSED_FLOAT& num, _F32 val)
	{
		CNUM_SET(*((A_CONFUSED_NUMBER*)&num), *((_U32*)&val));
	}

	CNUM::CNUM()
	{
	}

	CNUM::CNUM(const A_CONFUSED_NUMBER& num)
	{
	}

	CNUM::CNUM(const CNUM& num)
	{
	}

	_U32 CNUM::Get() const
	{
		return CNUM_GET(m_num);
	}

	void CNUM::Set(_U32 val)
	{
		CNUM_SET(m_num, val);
	}

	CNUM_INT::CNUM_INT()
	{
	}

	CNUM_INT::CNUM_INT(const A_CONFUSED_INT& num)
	{
	}

	CNUM_INT::CNUM_INT(const CNUM_INT& num)
	{
	}

	_S32 CNUM_INT::Get() const
	{
		return CNUM_GET(*((const A_CONFUSED_INT*)&m_num));
	}

	void CNUM_INT::Set(_S32 val)
	{
		CNUM_SET(*((A_CONFUSED_INT*)&m_num), val);
	}

	CNUM_FLOAT::CNUM_FLOAT()
	{
	}

	CNUM_FLOAT::CNUM_FLOAT(const A_CONFUSED_FLOAT& num)
	{
	}

	CNUM_FLOAT::CNUM_FLOAT(const CNUM_FLOAT& num)
	{
	}

	_F32 CNUM_FLOAT::Get() const
	{
		return CNUM_GET(*((const A_CONFUSED_FLOAT*)&m_num));
	}

	void CNUM_FLOAT::Set(_F32 val)
	{
		CNUM_SET(*((A_CONFUSED_FLOAT*)&m_num), val);
	}

}

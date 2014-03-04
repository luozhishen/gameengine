#ifndef __ZION_CONFUSED_NUMBER__
#define __ZION_CONFUSED_NUMBER__

#include "ConfusedNumberDDL.h"

namespace Zion
{

	_U32 CNUM_GET(const A_CONFUSED_U32& num);
	void CNUM_SET(A_CONFUSED_U32& num, _U32 val);
	_S32 CNUM_GET(const A_CONFUSED_S32& num);
	void CNUM_SET(A_CONFUSED_S32& num, _S32 val);
	_F32 CNUM_GET(const A_CONFUSED_F32& num);
	void CNUM_SET(A_CONFUSED_F32& num, _F32 val);

	class CNUM_U32
	{
	public:
		CNUM_U32();
		CNUM_U32(const A_CONFUSED_U32& num);
		CNUM_U32(const CNUM_U32& num);
		_U32 Get() const;
		void Set(_U32 val);

	private:
		A_CONFUSED_U32 m_num;
	};

	class CNUM_S32
	{
	public:
		CNUM_S32();
		CNUM_S32(const A_CONFUSED_S32& num);
		CNUM_S32(const CNUM_S32& num);
		_S32 Get() const;
		void Set(_S32 val);

	private:
		A_CONFUSED_S32 m_num;
	};

	class CNUM_F32 : public CNUM_U32
	{
	public:
		CNUM_F32();
		CNUM_F32(const A_CONFUSED_F32& num);
		CNUM_F32(const CNUM_F32& num);
		_F32 Get() const;
		void Set(_F32 val);

	private:
		A_CONFUSED_F32 m_num;
	};

}

#endif

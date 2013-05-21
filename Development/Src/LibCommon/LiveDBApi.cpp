#include <ZionBase.h>

#include "LiveDBApi.h"

#include <string>
#include <map>

namespace Zion
{

	CLiveDBTable::CLiveDBTable(CLiveDBTable::TABLE_TYPE Type, const char* Name)
	{
		m_Type = Type;
		m_Name = Name;
	}

	CLiveDBTable::~CLiveDBTable()
	{
	}

	CLiveDBTableSingle::CLiveDBTableSingle(const char* Name) : CLiveDBTable(CLiveDBTable::TABLE_TYPE_SINGLE, Name)
	{
	}

	CLiveDBTableMulti::CLiveDBTableMulti(const char* Name) : CLiveDBTable(CLiveDBTable::TABLE_TYPE_ARRAY, Name)
	{
	}

	CLiveDBProvider::CLiveDBProvider()
	{
	}

	CLiveDBProvider::~CLiveDBProvider()
	{
	}

	CLiveDBTableSingle* CLiveDBProvider::GetTableSingle(const char* name)
	{
		return NULL;
	}

	CLiveDBTableMulti* CLiveDBProvider::GetTableMulti(const char* name)
	{
		return NULL;
	}

	CLiveDBProviderFactory::CLiveDBProviderFactory(const char* Name)
	{
	}

	CLiveDBProviderFactory::~CLiveDBProviderFactory()
	{
	}

	CLiveDBProvider* OpenContentDBProvider(const char* type, const char* connstr)
	{
		return NULL;
	}

	void CloseContentDBProvider(CLiveDBProvider* pProvider)
	{
	}

	void RegsiterContentDBOProvider(CLiveDBProviderFactory* pFactory)
	{
	}

}

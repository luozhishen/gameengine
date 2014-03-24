#include "ZionDefines.h"
#include "ZionUUID.h"

#include <string.h>

A_UUID::A_UUID()
{
	memset(this, 0, sizeof(A_UUID));
}

A_UUID::A_UUID(const A_UUID& uuid)
{
	*this =	uuid;
}

bool A_UUID::isEmpty() const
{
	A_UUID empty_uuid;
	return (empty_uuid==*this);
}

bool A_UUID::operator<(const A_UUID& n) const
{
	return memcmp(this, &n, sizeof(n))<0;
}

bool A_UUID::operator==(const A_UUID& n) const
{
	return (data1==n.data1)
			&&(data2==n.data2)
			&&(data3==n.data3)
			&&(data4==n.data4)
			&&(data5==n.data5)
			&&(data6==n.data6)
			&&(data7==n.data7)
			&&(data8==n.data8)
			&&(data9==n.data9)
			&&(data10==n.data10)
			&&(data11==n.data11);
}

bool A_UUID::operator != (const A_UUID& n) const
{
	return !(*this==n);
}

#ifdef _WIN32
#include <Rpc.h>
#include <Rpcdce.h>

void AUuidToString(const A_UUID& uuid, char* str)
{
	RPC_CSTR val;
	UuidToStringA((UUID*)&uuid, &val);
	strcpy(str, (const char*)val);
	RpcStringFreeA(&val);
	for(char* i=str; *i!='\0'; i++)
	{
		if(*i>='A' && *i<='Z') *i = (*i - 'A') + 'a';
	}
}

bool AUuidFromString(const char* str, A_UUID& uuid)
{
	if(*str!='\0')
	{
		RPC_STATUS ret = UuidFromStringA((unsigned char*)str, (UUID*)&uuid);
		DWORD dwError = GetLastError();
		return RPC_S_OK==ret;
	}
	else
	{
		memset(&uuid, 0, sizeof(uuid));
		return true;
	}
}

void AUuidGenerate(A_UUID& uuid)
{
	UuidCreate((UUID*)&uuid);
}
#else
#include <uuid/uuid.h>
void AUuidToString(const A_UUID& uuid, char* str)
{
	uuid_unparse(*((uuid_t*)&uuid), str);
	for(char* i=str; *i!='\0'; i++)
	{
		if(*i>='A' && *i<='Z') *i = (*i - 'A') + 'a';
	}
}

bool AUuidFromString(const char* str, A_UUID& uuid)
{
	if(str[0]=='\0')
	{
		memset(&uuid, 0, sizeof(uuid));
	}
	else
	{
		if(uuid_parse((char*)str, *((uuid_t*)&uuid))!=0) return false;
	}
	return true;
}

void AUuidGenerate(A_UUID& uuid)
{
	uuid_generate(*((uuid_t*)&uuid));
}
#endif

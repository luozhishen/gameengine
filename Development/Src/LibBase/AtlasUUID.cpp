#include "AtlasDefines.h"
#include "AtlasUUID.h"

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
}

bool AUuidFromString(const char* str, A_UUID& uuid)
{
	RPC_STATUS ret = UuidFromStringA((unsigned char*)str, (UUID*)&uuid);
	DWORD dwError = GetLastError();
	return RPC_S_OK==ret;
}

void AUuidGenerate(A_UUID& uuid)
{
	UuidCreate((UUID*)&uuid);
}
#else
#ifndef IPHONE
#include <uuid.h>
void AUuidToString(const A_UUID& uuid, char* str)
{
	uint32_t status;
	uuid_to_string(str, (const uuid_t*)&uuid, &status);
}

bool AUuidFromString(const char* str, A_UUID& uuid)
{
	uint32_t status;
	uuid_from_string((uuid_t*)&uuid, &status);
	return status==uuid_s_ok;
}

void AUuidGenerate(A_UUID& uuid)
{
	uint32_t status
	uuid_create((uuid_t*)&uuid, &status);
}
#else
void AUuidToString(const A_UUID& uuid, char* str)
{
}

bool AUuidFromString(const char* str, A_UUID& uuid)
{
	return false;
}

void AUuidGenerate(A_UUID& uuid)
{
}
#endif
#endif
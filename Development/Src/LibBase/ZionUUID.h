#ifndef __ATLAS_UUID_INCLUDE__
#define __ATLAS_UUID_INCLUDE__

struct A_UUID
{
	_U32 data1;
	_U16 data2;
	_U16 data3;
	_U8 data4;
	_U8 data5;
	_U8 data6;
	_U8 data7;
	_U8 data8;
	_U8 data9;
	_U8 data10;
	_U8 data11;
	
	A_UUID();
	A_UUID(const A_UUID& uuid);

	bool isEmpty() const;

	bool operator<(const A_UUID& n) const;
	bool operator==(const A_UUID& n) const;
	bool operator != (const A_UUID& n) const;

};

void AUuidToString(const A_UUID& uuid, char* str);
bool AUuidFromString(const char* str, A_UUID& uuid);
void AUuidGenerate(A_UUID& uuid);

#endif // __ATLAS_UUID_INCLUDE__

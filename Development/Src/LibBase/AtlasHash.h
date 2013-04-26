#ifndef __ATLAS_HASH__
#define __ATLAS_HASH__

namespace Atlas
{

	class CHashMD5
	{
	public:
		static String CalcToString(const void* data, _U32 size);
		static bool CalcToString(const char* filename, String& out);

		static const _U32 DIGEST_LENGTH = 16;
		static const _U32 BLOCK_SIZE = 64;

		CHashMD5();
		~CHashMD5();

		void Init();
		void Update(const void* data, _U32 size);
		void Final(_U8* out);
		void Final(String& out);

	private:
		void transform(const _U8 block[BLOCK_SIZE]);
		_U8 buffer[BLOCK_SIZE];
		_U32 count[2];
		_U32 state[4];
 	};

	class CHashSha1
	{
	public:
		static String CalcToString(const void* data, _U32 size);
		static bool CalcToString(const char* filename, String& out);

		static const _U32 DIGEST_LENGTH = 20;

		CHashSha1();
		~CHashSha1();

		void Init();
		void Update(const void* data, _U32 size);
		void Final(_U8* out);
		void Final(String& out);

	private:
		void process();

		_U32 H0, H1, H2, H3, H4;
		_U8 bytes[64];
		int unprocessedBytes;
		_U32 size;
	};

};

#endif

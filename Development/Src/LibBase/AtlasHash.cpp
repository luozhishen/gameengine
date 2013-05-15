#include "AtlasDefines.h"
#include "AtlasSTL.h"
#include "AtlasHash.h"
#include "AtlasUtils.h"

namespace Atlas
{

	static inline _U32 MD5_F(_U32 x, _U32 y, _U32 z)
	{
		return x&y | ~x&z;
	}

	static inline _U32 MD5_G(_U32 x, _U32 y, _U32 z)
	{
		return x&z | y&~z;
	}

	static inline _U32 MD5_H(_U32 x, _U32 y, _U32 z)
	{
		return x^y^z;
	}

	static inline _U32 MD5_I(_U32 x, _U32 y, _U32 z)
	{
		return y ^ (x | ~z);
	}

	static inline _U32 MD5_rotate_left(_U32 x, int n)
	{
		return (x << n) | (x >> (32-n));
	}

	static inline void MD5_FF(_U32 &a, _U32 b, _U32 c, _U32 d, _U32 x, _U32 s, _U32 ac)
	{
		a = MD5_rotate_left(a+ MD5_F(b,c,d) + x + ac, s) + b;
	}

	static inline void MD5_GG(_U32 &a, _U32 b, _U32 c, _U32 d, _U32 x, _U32 s, _U32 ac)
	{
		a = MD5_rotate_left(a + MD5_G(b,c,d) + x + ac, s) + b;
	}

	static inline void MD5_HH(_U32 &a, _U32 b, _U32 c, _U32 d, _U32 x, _U32 s, _U32 ac)
	{
		a = MD5_rotate_left(a + MD5_H(b,c,d) + x + ac, s) + b;
	}

	static inline void MD5_II(_U32 &a, _U32 b, _U32 c, _U32 d, _U32 x, _U32 s, _U32 ac)
	{
		a = MD5_rotate_left(a + MD5_I(b,c,d) + x + ac, s) + b;
	}

	void MD5_decode(_U32 output[], const _U8 input[], _U32 len)
	{
		for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
			output[i] = ((_U32)input[j]) | (((_U32)input[j+1]) << 8) |
				(((_U32)input[j+2]) << 16) | (((_U32)input[j+3]) << 24);
	}

	void MD5_encode(_U8 output[], const _U32 input[], _U32 len)
	{
		for (_U32 i = 0, j = 0; j < len; i++, j += 4)
		{
			output[j] = input[i] & 0xff;
			output[j+1] = (input[i] >> 8) & 0xff;
			output[j+2] = (input[i] >> 16) & 0xff;
			output[j+3] = (input[i] >> 24) & 0xff;
		}
	}

	String CHashMD5::CalcToString(const void* data, _U32 size)
	{
		CHashMD5 hash;
		hash.Init();
		hash.Update(data, size);
		String md;
		hash.Final(md);
		return md;
	}

	bool CHashMD5::CalcToString(const char* filename, String& out)
	{
		FILE* fp = fopen(filename, "rb");
		if(!fp) return false;
		_U8 buf[1000];
		CHashMD5 hash;
		hash.Init();
		fseek(fp, 0, SEEK_SET);
		while(!feof(fp))
		{
			size_t size = fread(buf, 1, sizeof(buf), fp);
			hash.Update(buf, (_U32)size);
		}
		hash.Final(out);
		fclose(fp);
		return true;
	}

	CHashMD5::CHashMD5()
	{
		Init();
	}

	CHashMD5::~CHashMD5()
	{
	}

	void CHashMD5::Init()
	{
		count[0] = 0;
		count[1] = 0;

		state[0] = 0x67452301;
		state[1] = 0xefcdab89;
		state[2] = 0x98badcfe;
		state[3] = 0x10325476;
	}

	void CHashMD5::Update(const void* data, _U32 length)
	{
		const _U8* input = (const _U8*)data;

		_U32 index = count[0] / 8 % BLOCK_SIZE;

		if((count[0] += (length << 3)) < (length << 3))
			count[1]++;
		count[1] += (length >> 29);
		
		_U32 firstpart = 64 - index;
		_U32 i;
		
		if(length >= firstpart)
		{
			memcpy(&buffer[index], input, firstpart);
			transform(buffer);
			
			for(i = firstpart; i + BLOCK_SIZE <= length; i += BLOCK_SIZE)
				transform(&input[i]);
			
			index = 0;
		}
		else
			i = 0;
		
		memcpy(&buffer[index], &input[i], length-i);
	}

	void CHashMD5::Final(_U8* out)
	{
		static unsigned char padding[64] = {
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		unsigned char bits[8];
		MD5_encode(bits, count, 8);

		_U32 index = count[0] / 8 % 64;
		_U32 padLen = (index < 56) ? (56 - index) : (120 - index);
		Update(padding, padLen);

		Update(bits, 8);

		MD5_encode(out, state, 16);

		memset(buffer, 0, sizeof buffer);
		memset(count, 0, sizeof count);
	}

	void CHashMD5::Final(String& out)
	{
		_U8 md[DIGEST_LENGTH];
		Final(md);
		ToHexString(md, DIGEST_LENGTH, out);
	}

	#define MD5_S11 7
	#define MD5_S12 12
	#define MD5_S13 17
	#define MD5_S14 22
	#define MD5_S21 5
	#define MD5_S22 9
	#define MD5_S23 14
	#define MD5_S24 20
	#define MD5_S31 4
	#define MD5_S32 11
	#define MD5_S33 16
	#define MD5_S34 23
	#define MD5_S41 6
	#define MD5_S42 10
	#define MD5_S43 15
	#define MD5_S44 21

	void CHashMD5::transform(const _U8 block[CHashMD5::BLOCK_SIZE])
	{
		_U32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
		MD5_decode (x, block, BLOCK_SIZE);

		/* Round 1 */
		MD5_FF(a, b, c, d, x[ 0], MD5_S11, 0xd76aa478); /* 1 */
		MD5_FF(d, a, b, c, x[ 1], MD5_S12, 0xe8c7b756); /* 2 */
		MD5_FF(c, d, a, b, x[ 2], MD5_S13, 0x242070db); /* 3 */
		MD5_FF(b, c, d, a, x[ 3], MD5_S14, 0xc1bdceee); /* 4 */
		MD5_FF(a, b, c, d, x[ 4], MD5_S11, 0xf57c0faf); /* 5 */
		MD5_FF(d, a, b, c, x[ 5], MD5_S12, 0x4787c62a); /* 6 */
		MD5_FF(c, d, a, b, x[ 6], MD5_S13, 0xa8304613); /* 7 */
		MD5_FF(b, c, d, a, x[ 7], MD5_S14, 0xfd469501); /* 8 */
		MD5_FF(a, b, c, d, x[ 8], MD5_S11, 0x698098d8); /* 9 */
		MD5_FF(d, a, b, c, x[ 9], MD5_S12, 0x8b44f7af); /* 10 */
		MD5_FF(c, d, a, b, x[10], MD5_S13, 0xffff5bb1); /* 11 */
		MD5_FF(b, c, d, a, x[11], MD5_S14, 0x895cd7be); /* 12 */
		MD5_FF(a, b, c, d, x[12], MD5_S11, 0x6b901122); /* 13 */
		MD5_FF(d, a, b, c, x[13], MD5_S12, 0xfd987193); /* 14 */
		MD5_FF(c, d, a, b, x[14], MD5_S13, 0xa679438e); /* 15 */
		MD5_FF(b, c, d, a, x[15], MD5_S14, 0x49b40821); /* 16 */

		/* Round 2 */
		MD5_GG(a, b, c, d, x[ 1], MD5_S21, 0xf61e2562); /* 17 */
		MD5_GG(d, a, b, c, x[ 6], MD5_S22, 0xc040b340); /* 18 */
		MD5_GG(c, d, a, b, x[11], MD5_S23, 0x265e5a51); /* 19 */
		MD5_GG(b, c, d, a, x[ 0], MD5_S24, 0xe9b6c7aa); /* 20 */
		MD5_GG(a, b, c, d, x[ 5], MD5_S21, 0xd62f105d); /* 21 */
		MD5_GG(d, a, b, c, x[10], MD5_S22,  0x2441453); /* 22 */
		MD5_GG(c, d, a, b, x[15], MD5_S23, 0xd8a1e681); /* 23 */
		MD5_GG(b, c, d, a, x[ 4], MD5_S24, 0xe7d3fbc8); /* 24 */
		MD5_GG(a, b, c, d, x[ 9], MD5_S21, 0x21e1cde6); /* 25 */
		MD5_GG(d, a, b, c, x[14], MD5_S22, 0xc33707d6); /* 26 */
		MD5_GG(c, d, a, b, x[ 3], MD5_S23, 0xf4d50d87); /* 27 */
		MD5_GG(b, c, d, a, x[ 8], MD5_S24, 0x455a14ed); /* 28 */
		MD5_GG(a, b, c, d, x[13], MD5_S21, 0xa9e3e905); /* 29 */
		MD5_GG(d, a, b, c, x[ 2], MD5_S22, 0xfcefa3f8); /* 30 */
		MD5_GG(c, d, a, b, x[ 7], MD5_S23, 0x676f02d9); /* 31 */
		MD5_GG(b, c, d, a, x[12], MD5_S24, 0x8d2a4c8a); /* 32 */

		/* Round 3 */
		MD5_HH(a, b, c, d, x[ 5], MD5_S31, 0xfffa3942); /* 33 */
		MD5_HH(d, a, b, c, x[ 8], MD5_S32, 0x8771f681); /* 34 */
		MD5_HH(c, d, a, b, x[11], MD5_S33, 0x6d9d6122); /* 35 */
		MD5_HH(b, c, d, a, x[14], MD5_S34, 0xfde5380c); /* 36 */
		MD5_HH(a, b, c, d, x[ 1], MD5_S31, 0xa4beea44); /* 37 */
		MD5_HH(d, a, b, c, x[ 4], MD5_S32, 0x4bdecfa9); /* 38 */
		MD5_HH(c, d, a, b, x[ 7], MD5_S33, 0xf6bb4b60); /* 39 */
		MD5_HH(b, c, d, a, x[10], MD5_S34, 0xbebfbc70); /* 40 */
		MD5_HH(a, b, c, d, x[13], MD5_S31, 0x289b7ec6); /* 41 */
		MD5_HH(d, a, b, c, x[ 0], MD5_S32, 0xeaa127fa); /* 42 */
		MD5_HH(c, d, a, b, x[ 3], MD5_S33, 0xd4ef3085); /* 43 */
		MD5_HH(b, c, d, a, x[ 6], MD5_S34,  0x4881d05); /* 44 */
		MD5_HH(a, b, c, d, x[ 9], MD5_S31, 0xd9d4d039); /* 45 */
		MD5_HH(d, a, b, c, x[12], MD5_S32, 0xe6db99e5); /* 46 */
		MD5_HH(c, d, a, b, x[15], MD5_S33, 0x1fa27cf8); /* 47 */
		MD5_HH(b, c, d, a, x[ 2], MD5_S34, 0xc4ac5665); /* 48 */

		/* Round 4 */
		MD5_II(a, b, c, d, x[ 0], MD5_S41, 0xf4292244); /* 49 */
		MD5_II(d, a, b, c, x[ 7], MD5_S42, 0x432aff97); /* 50 */
		MD5_II(c, d, a, b, x[14], MD5_S43, 0xab9423a7); /* 51 */
		MD5_II(b, c, d, a, x[ 5], MD5_S44, 0xfc93a039); /* 52 */
		MD5_II(a, b, c, d, x[12], MD5_S41, 0x655b59c3); /* 53 */
		MD5_II(d, a, b, c, x[ 3], MD5_S42, 0x8f0ccc92); /* 54 */
		MD5_II(c, d, a, b, x[10], MD5_S43, 0xffeff47d); /* 55 */
		MD5_II(b, c, d, a, x[ 1], MD5_S44, 0x85845dd1); /* 56 */
		MD5_II(a, b, c, d, x[ 8], MD5_S41, 0x6fa87e4f); /* 57 */
		MD5_II(d, a, b, c, x[15], MD5_S42, 0xfe2ce6e0); /* 58 */
		MD5_II(c, d, a, b, x[ 6], MD5_S43, 0xa3014314); /* 59 */
		MD5_II(b, c, d, a, x[13], MD5_S44, 0x4e0811a1); /* 60 */
		MD5_II(a, b, c, d, x[ 4], MD5_S41, 0xf7537e82); /* 61 */
		MD5_II(d, a, b, c, x[11], MD5_S42, 0xbd3af235); /* 62 */
		MD5_II(c, d, a, b, x[ 2], MD5_S43, 0x2ad7d2bb); /* 63 */
		MD5_II(b, c, d, a, x[ 9], MD5_S44, 0xeb86d391); /* 64 */

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;

		memset(x, 0, sizeof x);
	}

	_U32 SHA1_lrot(_U32 x, int bits)
	{
		return (x<<bits) | (x>>(32 - bits));
	}

	void SHA1_storeBigEndianUint32(unsigned char* byte, _U32 num)
	{
		byte[0] = (unsigned char)(num>>24);
		byte[1] = (unsigned char)(num>>16);
		byte[2] = (unsigned char)(num>>8);
		byte[3] = (unsigned char)num;
	}

	String CHashSha1::CalcToString(const void* data, _U32 size)
	{
		CHashSha1 hash;
		hash.Init();
		hash.Update(data, size);
		String md;
		hash.Final(md);
		return md;
	}

	bool CHashSha1::CalcToString(const char* filename, String& out)
	{
		FILE* fp = fopen(filename, "rb");
		if(!fp) return false;
		_U8 buf[1000];
		CHashSha1 hash;
		hash.Init();
		while(!feof(fp))
		{
			size_t size = fread(buf, sizeof(buf), 1, fp);
			hash.Update(buf, (_U32)size);
		}
		hash.Final(out);
		fclose(fp);
		return true;
	}

	CHashSha1::CHashSha1()
	{
		Init();
	}

	CHashSha1::~CHashSha1()
	{
	}

	void CHashSha1::Init()
	{
		H0 = 0x67452301;
		H1 = 0xefcdab89;
		H2 = 0x98badcfe;
		H3 = 0x10325476;
		H4 = 0xc3d2e1f0;
		unprocessedBytes = 0;
		size = 0;
	}

	void CHashSha1::Update(const void* mem, _U32 length)
	{
		const _U8* data = (const _U8*)mem;
		int num = (int)length;
		size += num;

		while(num>0)
		{
			int needed = 64 - unprocessedBytes;
			int toCopy = (num < needed) ? num : needed;
			memcpy(bytes + unprocessedBytes, data, toCopy);
			num -= toCopy;
			data += toCopy;
			unprocessedBytes += toCopy;
		
			if(unprocessedBytes==64) process();
		}
	}

	void CHashSha1::Final(_U8* out)
	{
		// save the message size
		_U32 totalBitsL = size << 3;
		_U32 totalBitsH = size >> 29;
		// add 0x80 to the message
		Update("\x80", 1);
	
		static unsigned char footer[64] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		// block has no room for 8-byte filesize, so finish it
		if(unprocessedBytes > 56)
			Update(footer, 64 - unprocessedBytes);

		// how many zeros do we need
		int neededZeros = 56 - unprocessedBytes;
		// store file size (in bits) in big-endian format
		SHA1_storeBigEndianUint32(footer + neededZeros    , totalBitsH);
		SHA1_storeBigEndianUint32(footer + neededZeros + 4, totalBitsL);
		// finish the final block
		Update(footer, neededZeros + 8);
		// copy the digest bytes
		SHA1_storeBigEndianUint32(out, H0);
		SHA1_storeBigEndianUint32(out + 4, H1);
		SHA1_storeBigEndianUint32(out + 8, H2);
		SHA1_storeBigEndianUint32(out + 12, H3);
		SHA1_storeBigEndianUint32(out + 16, H4);
	}

	void CHashSha1::Final(String& out)
	{
		_U8 md[DIGEST_LENGTH];
		Final(md);
		ToHexString(md, DIGEST_LENGTH, out);
	}

	void CHashSha1::process()
	{
		int t;
		_U32 a, b, c, d, e, K, f, W[80];

		a = H0;
		b = H1;
		c = H2;
		d = H3;
		e = H4;

		for(t = 0; t < 16; t++) W[t] = (bytes[t*4] << 24)
										+(bytes[t*4 + 1] << 16)
										+(bytes[t*4 + 2] << 8)
										+ bytes[t*4 + 3];
		for(; t< 80; t++) W[t] = SHA1_lrot(W[t-3]^W[t-8]^W[t-14]^W[t-16], 1);
	
		_U32 temp;
		for(t = 0; t < 80; t++)
		{
			if(t < 20) {
				K = 0x5a827999;
				f = (b & c) | ((b ^ 0xFFFFFFFF) & d);
			} else if(t < 40) {
				K = 0x6ed9eba1;
				f = b ^ c ^ d;
			} else if(t < 60) {
				K = 0x8f1bbcdc;
				f = (b & c) | (b & d) | (c & d);
			} else {
				K = 0xca62c1d6;
				f = b ^ c ^ d;
			}
			temp = SHA1_lrot(a,5) + f + e + W[t] + K;
			e = d;
			d = c;
			c = SHA1_lrot(b,30);
			b = a;
			a = temp;
		}

		H0 += a;
		H1 += b;
		H2 += c;
		H3 += d;
		H4 += e;

		unprocessedBytes = 0;
	}

}

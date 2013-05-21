#ifndef __JPATCH_BUNDLE_INCLUDE__
#define __JPATCH_BUNDLE_INCLUDE__

namespace JPatch
{

	class CSection;
	class CFile;
	class CBundle;

	class CSection
	{
	public:
		Zion::String							_Name;
		_U32									_Offset;
		_U32									_Size;
		Zion::String							_MD5;
	};

	class CFile
	{
	public:
		//
		Zion::String							_Name;
		Zion::Vector<CSection>					_Sections;
		Zion::String							_MD5;
		//
		Zion::Map<Zion::String, CSection*>	_SectionMap;
		CBundle*								_Bundle;

		CSection* GetSectionByMD5(Zion::String& md5);
		CSection* GetSectionByName(Zion::String& name);
	};

	class CBundle
	{
	public:
		bool InitFromBLS(const char* bls, const char* basedir);

		Zion::String							_BaseDir;
		Zion::Map<Zion::String, CFile>		_Files;
	};

}

#endif

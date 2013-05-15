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
		Atlas::String							_Name;
		_U32									_Offset;
		_U32									_Size;
		Atlas::String							_MD5;
	};

	class CFile
	{
	public:
		//
		Atlas::String							_Name;
		Atlas::Vector<CSection>					_Sections;
		Atlas::String							_MD5;
		//
		Atlas::Map<Atlas::String, CSection*>	_SectionMap;
		CBundle*								_Bundle;

		CSection* GetSectionByMD5(Atlas::String& md5);
		CSection* GetSectionByName(Atlas::String& name);
	};

	class CBundle
	{
	public:
		bool InitFromBLS(const char* bls, const char* basedir);

		Atlas::String							_BaseDir;
		Atlas::Map<Atlas::String, CFile>		_Files;
	};

}

#endif

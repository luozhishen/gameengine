#ifndef __JPATCH_INCLUDE__
#define __JPATCH_INCLUDE__

namespace JPatch
{

	class CPatcher
	{
	public:
		CPatcher();
		~CPatcher();

		void setSourceDir(const char* dir);
		void setUserDir(const char* dir);
		void SetTempDir(const char* dir);
		void setUpdateUrl(const char* url);
		void setProgramVersion(const char* ver);

		bool checkVersion();
		bool downloadPatch();
		bool applyPatch();

		void tick();

		enum CHECK_STATE {
			CHECKSTATE_FAILED,
			CHECKSTATE_UNSUPPORT,
			CHECKSTATE_LASTEST,
			CHECKSTATE_SELFUPDATE,
			CHECKSTATE_UPDATE,
		};

		int GetDownloadTotalSize();
		int GetDownloadCurrentSize();

	protected:
		virtual void writeDebugLog(const char* msg) = 0;

		virtual void checkVersion(CHECK_STATE state, const char* ver, const char* url) = 0;

		virtual void beginDownload() = 0;
		virtual void endDownload(bool error) = 0;

		virtual void beginApply() = 0;
		virtual void progressApply(int total_count, int current, const char* filename) = 0;
		virtual void endApply(bool error) = 0;

	private:
		bool getDataVersion(Zion::String& ver);
		bool setDataVersion(Zion::String ver);

	private:
		Zion::String _SourceDir;
		Zion::String _UserDir;
		Zion::String _TempDir;
		Zion::String _UpdateUrl;
		Zion::String _ProgramVersion;

		bool _SelfUpdate;
		Zion::String _PatchUrl;
		Zion::String _PatchVer;
		Zion::String _PatchMD5;
		int _PatchSize;

		MOREQUEST* _checkVersion;
		MOREQUEST* _downloadPatch;
	};

}

#endif

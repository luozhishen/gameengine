#include <ZionBase.h>
#include <MoSDK.h>

#include "Patcher.h"

static bool DeleteDir(const char* dir);
static bool DeleteFile(const char* file);

namespace JPatch
{

	CPatcher::CPatcher()
	{
		_checkVersion = NULL;
		_downloadPatch = NULL;
	}

	CPatcher::~CPatcher()
	{
	}

	void CPatcher::setSourceDir(const char* dir)
	{
		_SourceDir = dir;
	}

	void CPatcher::setUserDir(const char* dir)
	{
		_UserDir = dir;
	}

	void CPatcher::SetTempDir(const char* dir)
	{
		_TempDir = dir;
	}

	void CPatcher::setUpdateUrl(const char* url)
	{
		_UpdateUrl = url;
	}

	void CPatcher::setProgramVersion(const char* ver)
	{
		_ProgramVersion = ver;
	}

	bool CPatcher::checkVersion()
	{
		if(_checkVersion) return false;

		// if userdir!=sourcedir and userdir<sourcedir then delete all file in userdir

		Zion::Map<Zion::String, Zion::String> params;
		params["PROGRAM_VERSION"] = _ProgramVersion;
		params["DATA_VERSION"] = _ProgramVersion;

		_checkVersion = MORequestString(_UpdateUrl.c_str(), params);
		if(!_checkVersion) return false;

		return true;
	}

	bool CPatcher::downloadPatch()
	{
		if(_downloadPatch) return false;
		size_t index = _PatchUrl.find_first_of("/");
		if(index==_PatchUrl.npos) return false;
		_downloadPatch = MODownloadFile(_PatchUrl.c_str(), NULL, _PatchUrl.c_str()+index+1, true);
		if(!_downloadPatch) return false;
		beginDownload();
		return true;
	}

	bool CPatcher::applyPatch()
	{
		return true;
	}

	void CPatcher::tick()
	{
		if(_checkVersion)
		{
			if(MOREQUESTSTATE_FAILED==MORequestStatus(_checkVersion))
			{
				checkVersion(CHECKSTATE_FAILED, NULL, NULL);
				MORequestDestory(_checkVersion);
				_checkVersion = NULL;
			}
			else if(MOREQUESTSTATE_DONE==MORequestStatus(_checkVersion))
			{
				Zion::Vector<Zion::String> params;
				Zion::StringSplit(MORequestGetResult(_checkVersion), ' ', params);
				MORequestDestory(_checkVersion);
				_checkVersion = NULL;
				if(params.size()==1 && params[0]=="LASTEST")
				{
					checkVersion(CHECKSTATE_LASTEST, NULL, NULL);
				}
				else if(params.size()==1 && params[0]=="UNSUPPORT")
				{
					checkVersion(CHECKSTATE_UNSUPPORT, NULL, NULL);
				}
				else if(params.size()==5 && params[0]=="SELFUPDATE")
				{
					_SelfUpdate = true;
					_PatchVer = params[1];
					_PatchUrl = params[2];
					_PatchMD5 = params[3];
					_PatchSize = atoi(params[4].c_str());
					checkVersion(CHECKSTATE_SELFUPDATE, params[1].c_str(), params[2].c_str());
				}
				else if(params.size()==5 && params[0]=="UPDATE")
				{
					_SelfUpdate = false;
					_PatchVer = params[1];
					_PatchUrl = params[2];
					_PatchMD5 = params[3];
					_PatchSize = atoi(params[4].c_str());
					checkVersion(CHECKSTATE_UPDATE, params[1].c_str(), params[2].c_str());
				}
				else
				{
					checkVersion(CHECKSTATE_FAILED, NULL, NULL);
				}
			}
		}

		if(_downloadPatch)
		{
			if(MOREQUESTSTATE_FAILED==MORequestStatus(_downloadPatch))
			{
				endDownload(true);
				MORequestDestory(_downloadPatch);
				_downloadPatch = NULL;
			}
			else if(MOREQUESTSTATE_DONE==MORequestStatus(_downloadPatch))
			{
				// check md5
				if(1)
				{
					endDownload(false);
				}
				else
				{
					// delete patch file
					endDownload(true);
				}
				MORequestDestory(_downloadPatch);
				_downloadPatch = NULL;
			}
		}

	}

	bool CPatcher::getDataVersion(Zion::String& ver)
	{
		return true;
	}

	bool CPatcher::setDataVersion(Zion::String ver)
	{
		return true;
	}

}

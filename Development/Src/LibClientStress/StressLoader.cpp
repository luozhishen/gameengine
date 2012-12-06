#include <AtlasBase.h>
#include <AtlasClient.h>

#include <tinyxml.h>
#include "StressLoader.h"
#include "StressManager.h"
#include "StressClient.h"

namespace Atlas
{
	CStressLoader::CStressLoader()
	{
	}

	CStressLoader::~CStressLoader()
	{
	}

	bool CStressLoader::RunScript(const char* script)
	{
		if(!LoadScript(script))
		{
			return false;
		}
		
		SetSvrParam();
		if(!ScriptImpl())
		{
			return false;
		}
		
		return true;
	}

	bool CStressLoader::LoadScript(const char* szScript)
	{
		m_ServerCases.clear();
		
		TiXmlDocument xmldoc;
		if(!xmldoc.LoadFile(szScript))
			return false;

		TiXmlElement* pNode = xmldoc.RootElement();
		pNode = pNode->FirstChildElement();
		
		TiXmlElement* pParamNode = pNode->FirstChildElement();
		do 
		{
			Server_Param param;
			param.key = pParamNode->Attribute("key");
			param.value = pParamNode->Attribute("value");

			//TiXmlElement* pScriptNode = pParamNode->FirstChildElement();
			m_ServerParams.push_back(param);
			pParamNode = pParamNode->NextSiblingElement();
		} while (pParamNode);


		pNode = pNode->NextSibling()->ToElement();
		pNode = pNode->FirstChildElement();

		do 
		{
			Stress_Case stress;
			int n = 1;
			pNode->Attribute("count", &n);
			stress.count = (unsigned int)n;

			TiXmlElement* pScriptNode = pNode->FirstChildElement();
			do 
			{
				Script_Case script;
				script.case_name += pScriptNode->Attribute("casename");
				script.param1 = pScriptNode->Attribute("param1");
				script.param2 = "";
				script.param3 = "";

				stress.scripts.push_back(script);
				pScriptNode = pScriptNode->NextSiblingElement();
			} while (pScriptNode);

			m_ServerCases.push_back(stress);
			pNode = pNode->NextSiblingElement();
		} while (pNode);

		m_strPath = szScript;
		return true;
	}

	bool CStressLoader::ScriptImpl()
	{
		CStressManager& stressMgr = CStressManager::Get();

		SERVER_CASES::iterator it = m_ServerCases.begin();
		for(; it != m_ServerCases.end(); ++it)
		{
			for(_U32 i = 0; i < (*it).count; ++i)
			{
				_U32 uid = stressMgr.NewClient();
				CStressClient* pClient = stressMgr.GetClient(uid);
				_OnNewClient(uid);

				std::vector<Script_Case>::iterator script_it = (*it).scripts.begin();
				for(; script_it != (*it).scripts.end(); ++script_it)
				{
					CStressCase* pCase = pClient->NewStressCase((*script_it).case_name.c_str());
					if(pCase == NULL)
					{
						return false;
					}

					_OnNewCase(uid, pCase);
				}
			}
		}

		return true;
	}

	void CStressLoader::SetSvrParam()
	{
		SERVER_PARAMS::iterator it = m_ServerParams.begin();
		for(; it != m_ServerParams.end(); ++it)
		{
			CClientApp::GetDefault()->SetParam(it->key.c_str(), it->value.c_str());
		}
	}


	void CStressLoader::SaveSvrParam()
	{
		TiXmlDocument xmldoc;;
		if(m_strPath.empty())
		{
			m_strPath = "svr_case.xml";
		}

		TiXmlElement* rootElem = ATLAS_NEW TiXmlElement("ROOT");
		xmldoc.LinkEndChild(rootElem);

		TiXmlElement* ParamElem = ATLAS_NEW TiXmlElement("SERVER_PARAM");
		rootElem->LinkEndChild(ParamElem);
		
		std::map<std::string, std::string> params = Atlas::CClientApp::GetDefault()->GetParams();
		std::map<std::string, std::string>::iterator it = params.begin();
		for(int i = 0; it != params.end(); ++it, ++i)
		{
			TiXmlElement* elem = ATLAS_NEW TiXmlElement("PARAM");
			elem->SetAttribute("key", it->first.c_str());
			elem->SetAttribute("value", it->second.c_str());
			ParamElem->LinkEndChild(elem);
		}

		TiXmlElement* ScriptElem = ATLAS_NEW TiXmlElement("SCRIPT");
		rootElem->LinkEndChild(ScriptElem);
		
		
		for(SERVER_CASES::iterator it = m_ServerCases.begin(); it != m_ServerCases.end(); ++it)
		{
			TiXmlElement* ClientElem = ATLAS_NEW TiXmlElement("CLIENT");
			ClientElem->SetAttribute("count", (*it).count);
			
			for(_U32 i = 0; i < (*it).scripts.size(); ++i)
			{
				TiXmlElement* CaseElem = ATLAS_NEW TiXmlElement("CASE");
				CaseElem->SetAttribute("casename", (*it).scripts[i].case_name.c_str());
				CaseElem->SetAttribute("param1", (*it).scripts[i].param1.c_str());
				ClientElem->InsertEndChild(*(TiXmlNode*)CaseElem);
			}
			
			ScriptElem->LinkEndChild(ClientElem);
		}
		
		xmldoc.SaveFile(m_strPath.c_str());
	}
}

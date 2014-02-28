#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>

#include <ZionBase.h>
#include <ZionClientApp.h>

#include "StressLoader.h"
#include "StressManager.h"
#include "StressClient.h"
#include "StressCase.h"
#include "ClientStressFrame.h"

#include <fstream>

namespace Zion
{

	class CStressCaseTemplate
	{
	public:
		CStressCaseTemplate()
		{
		}

		~CStressCaseTemplate()
		{
			while(!m_Cases.empty())
			{
				if(m_Cases.begin()->second)
				{
					const DDLReflect::STRUCT_INFO* info = CStressManager::Get().GetCaseConfigType(m_Cases.begin()->first.c_str());
					ZION_ASSERT(info);
					DDLReflect::DestoryObject(info, m_Cases.begin()->second);
				}
				m_Cases.erase(m_Cases.begin());
			}
		}

		bool NewCase(const char* name, const DDLReflect::STRUCT_INFO*& info, void*& data)
		{
			if(m_Cases.find(name)!=m_Cases.end()) return NULL;
			if(!CStressManager::Get().HasCase(name)) return NULL;
			info = CStressManager::Get().GetCaseConfigType(name);
			if(info)
			{
				data = DDLReflect::CreateObject(info);
				if(!data) return false;
			}
			else
			{
				data = NULL;
			}
			m_Cases[name] = data;
			return true;
		}

		CStressClient* CreateClient(CClientStressFrame* pView=NULL)
		{
			_U32 id = CStressManager::Get().NewClient();
			CStressClient* client = CStressManager::Get().GetClient(id);
			if(pView) pView->NotifyClientAdd(id);

			Zion::Map<Zion::String, void*>::iterator i;
			for(i=m_Cases.begin(); i!=m_Cases.end(); i++)
			{
				CStressCase* _case = client->NewStressCase(i->first.c_str());
				const DDLReflect::STRUCT_INFO* info = _case->GetConfigType();
				if(info) _case->SetConfig(i->second, info->size);
			}
			return client;
		}

	private:
		Zion::Map<Zion::String, void*> m_Cases;
	};

	CStressLoader::CStressLoader(CClientStressFrame* pView)
	{
		m_pView = pView;
	}

	CStressLoader::~CStressLoader()
	{
		LoadTemplate(NULL);
	}

	bool CStressLoader::LoadTemplate(const char* tpl)
	{
		while(!m_Tpls.empty())
		{
			ZION_DELETE m_Tpls.begin()->second;
			m_Tpls.erase(m_Tpls.begin());
		}
		if(!tpl) return true;

		Json::Value root;
		std::ifstream f(tpl, std::ifstream::binary);
		if(!f.is_open()) return false;
		Json::Reader reader;
		if(!reader.parse(f, root, false)) return false;

		Json::Value::Members ms = root.getMemberNames();

		Json::Value::Members::iterator i;
		for(i=ms.begin(); i!=ms.end(); i++)
		{
			Json::Value tpl = root.get(*i, Json::Value());
			if(!tpl.isObject()) return false;
			if(m_Tpls.find((*i).c_str())!=m_Tpls.end()) return false;

			CStressCaseTemplate* t = ZION_NEW CStressCaseTemplate();
			m_Tpls[(*i).c_str()] = t;

			Json::Value::Members ns = tpl.getMemberNames();
			Json::Value::Members::iterator j;
			for(j=ns.begin(); j!=ns.end(); j++)
			{
				Json::Value cs = tpl.get(*j, Json::Value());
				if(!cs.isObject()) return false;

				const DDLReflect::STRUCT_INFO* info;
				void* data;
				if(!t->NewCase((*j).c_str(), info, data)) return false;

				if(info && data)
				{
					Json::Value::Members vs = cs.getMemberNames();
					Json::Value::Members::iterator k;
					for(k=vs.begin(); k!=vs.end(); k++)
					{
						Json::Value v = cs.get(*k, Json::Value());
						if(!v.isString()) return false;
						if(!DDLReflect::StructParamFromString(info, (*k).c_str(), data, v.asString().c_str())) return false;
					}
				}
			}
		}

		return true;
	}

	_U32 CStressLoader::CreateClient(const char* tpl_name)
	{
		CStressCaseTemplate* tpl = m_Tpls[tpl_name];
		if(!tpl) return false;
		return tpl->CreateClient(m_pView)->GetIndex();
	}

	Zion::Array<Zion::String> CStressLoader::GetNames()
	{
		Zion::Array<Zion::String> names;

		Zion::Map<Zion::String, CStressCaseTemplate*>::iterator i;
		for(i=m_Tpls.begin(); i!=m_Tpls.end(); i++)
		{
			names.push_back(i->first);
		}

		return names;
	}

}

#include <AtlasBase.h>

#include "ContentObject.h"
#include "CommonDDL.h"

#include <map>
#include <string>
#include <json/reader.h>
#include <json/writer.h>
#include <fstream>

namespace Atlas
{

	const char* AtlasGameDir();

	namespace ContentObject
	{
		static std::map<std::string, std::pair<_U16, const DDLReflect::STRUCT_INFO*>> g_type_map;
		static std::map<_U16, const DDLReflect::STRUCT_INFO*> g_typeid_map;
		static std::map<std::string, std::string> g_typekey_map;
		static _U16 g_typeid_max = 0x1000;

		bool Register(const DDLReflect::STRUCT_INFO* info, const char* keys)
		{
			if(!DDLReflect::IsParent(info, DDLReflect::GetStruct<A_CONTENT_OBJECT>()))
			{
				ATLAS_ASSERT(0);
				return false;
			}

			ATLAS_ASSERT(g_type_map.find(info->name)==g_type_map.end());
			if(g_type_map.find(info->name)!=g_type_map.end()) return false;


			g_type_map[info->name] = std::pair<int, const DDLReflect::STRUCT_INFO*>(g_typeid_max, info);
			g_typeid_map[g_typeid_max++] = info;

			//
			if(keys)
			{
				std::vector<std::string> vkeys;
				StringSplit(keys, '.', vkeys);
				if(vkeys.empty()) ATLAS_ASSERT(0);
				DDLReflect::FIELD_INFO finfo;
				const void* fdata;
				for(size_t i=0; i<vkeys.size(); i++)
				{
					if(!DDLReflect::GetStructFieldInfo(info, vkeys[i].c_str(), (const void*)NULL, finfo, fdata))
					{
						ATLAS_ASSERT(0);
					}
				}
				g_typekey_map[info->name] = keys;
			}
			else
			{
				g_typekey_map[info->name] = "uuid";
			}

			return true;
		}

		void GetTypeList(std::vector<const DDLReflect::STRUCT_INFO*>& list)
		{
			list.clear();
			std::map<_U16, const DDLReflect::STRUCT_INFO*>::const_iterator i;
			for(i=g_typeid_map.begin(); i!=g_typeid_map.end(); i++)
			{
				list.push_back(i->second);
			}
		}

		_U16 GetTypeId(const char* name)
		{
			std::map<std::string, std::pair<_U16, const DDLReflect::STRUCT_INFO*>>::const_iterator i;
			i = g_type_map.find(name);
			if(i==g_type_map.end()) return (_U16)-1;
			return (_U16)i->second.first;
		}

		const DDLReflect::STRUCT_INFO* GetType(const char* name)
		{
			std::map<std::string, std::pair<_U16, const DDLReflect::STRUCT_INFO*>>::const_iterator i;
			i = g_type_map.find(name);
			if(i==g_type_map.end()) return NULL;
			return i->second.second;
		}

		const DDLReflect::STRUCT_INFO* GetType(_U16 id)
		{
			std::map<_U16, const DDLReflect::STRUCT_INFO*>::const_iterator i;
			i = g_typeid_map.find(id);
			if(i==g_typeid_map.end()) return NULL;
			return i->second;
		}

		const char* GetContentTypeKeys(const char* name)
		{
			std::map<std::string, std::string>::iterator i;
			i = g_typekey_map.find(name);
			if(i==g_typekey_map.end()) return NULL;
			return i->second.c_str();
		}

		static std::map<std::string, bool> g_content_file_map;
		static const char* g_default_name = "default";
		static const char* g_default_file = "default.json";

		class CContentObjectManager
		{
		public:
			CContentObjectManager()
			{
			}
			~CContentObjectManager()
			{
				std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
				for(i=m_object_map.begin(); i!=m_object_map.end(); i++)
				{
					DDLReflect::DestoryObject(i->second.second);
				}
				m_object_map.clear();
			}

			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>> m_object_map;
		};
		static CContentObjectManager g_objct_manager;

		A_CONTENT_OBJECT* Create(const DDLReflect::STRUCT_INFO* info, A_UUID& uuid)
		{
			AUuidGenerate(uuid);
			return Alloc(info, uuid);
		}

		A_CONTENT_OBJECT* Alloc(const DDLReflect::STRUCT_INFO* info, const A_UUID& uuid)
		{
			A_CONTENT_OBJECT* object = (A_CONTENT_OBJECT*)DDLReflect::CreateObject(info);
			if(!object) return NULL;
			g_objct_manager.m_object_map[uuid] = std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>(info, object);
			g_content_file_map[QueryContentGroupFile(info)] = true;
			object->uuid = uuid;
			return object;
		}

		void Delete(const A_UUID& uuid)
		{
			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return;
			DDLReflect::DestoryObject(i->second.second);
			g_content_file_map[QueryContentGroupFile(i->second.first)] = true;
			g_objct_manager.m_object_map.erase(i);
		}

		const DDLReflect::STRUCT_INFO* GetType(const A_UUID& uuid)
		{
			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			return i->second.first;
		}

		const A_CONTENT_OBJECT* Query(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info)
		{
			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			if(info!=NULL && i->second.first!=info) return NULL;
			return i->second.second;
		}

		const A_CONTENT_OBJECT* Query(const char* name, const DDLReflect::STRUCT_INFO* info)
		{
			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				if((info!=NULL || i->second.first==info) && strcmp(name, i->second.second->name._Value)==0)
				{
					return i->second.second;					
				}
			}
			return NULL;
		}

		A_CONTENT_OBJECT* Modify(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info)
		{
			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			if(info!=NULL && i->second.first!=info) return NULL;
			g_content_file_map[QueryContentGroupFile(info)] = true;
			return i->second.second;
		}

		bool GetList(const DDLReflect::STRUCT_INFO* info, std::vector<A_UUID>& list, bool bExactMatch)
		{
			list.clear();

			std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				if(i->second.first==info || (!bExactMatch && IsParent(i->second.first, info)))
				{
					list.push_back(i->first);
				}
			}

			return true;
		}

		struct CONTENT_GROUP
		{
			const DDLReflect::STRUCT_INFO* info;
			const char* name;
			const char* file;
		};
		static std::map<const DDLReflect::STRUCT_INFO*, CONTENT_GROUP> g_content_group_map;

		void CreateContentGroup(const DDLReflect::STRUCT_INFO* info, const char* name, const char* file)
		{
			ATLAS_ASSERT(g_content_group_map.find(info)==g_content_group_map.end());
			CONTENT_GROUP group = { info, name, file };
			g_content_group_map[info] = group;
			if(g_content_file_map.find(file)==g_content_file_map.end())
			{
				g_content_file_map[file] = true;
			}
		}

		const char* QueryContentGroupName(const DDLReflect::STRUCT_INFO* info)
		{
			ATLAS_ASSERT(info);
			if(info==DDLReflect::GetStruct<A_CONTENT_OBJECT>()) return g_default_name;
			std::map<const DDLReflect::STRUCT_INFO*, CONTENT_GROUP>::iterator i;
			i = g_content_group_map.find(info);
			if(i!=g_content_group_map.end()) return i->second.name;
			return QueryContentGroupName(info);
		}

		const char* QueryContentGroupFile(const DDLReflect::STRUCT_INFO* info)
		{
			ATLAS_ASSERT(info);
			std::map<const DDLReflect::STRUCT_INFO*, CONTENT_GROUP>::iterator i;
			i = g_content_group_map.find(info);
			if(i!=g_content_group_map.end()) return i->second.file;
			return QueryContentGroupFile(info->parent);
		}

		void GetContentFileList(std::vector<std::string>& list)
		{
			list.clear();
			std::map<std::string, bool>::iterator i;
			for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
			{
				list.push_back(i->first);
			}
		}

		bool LoadContent()
		{
			std::map<std::string, bool>::iterator i;
			for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
			{
				char file[1000];
				sprintf(file, "%s/Content/%s", Atlas::AtlasGameDir(), i->first.c_str());
				if(!LoadContentFromFile(file)) return false;
				g_content_file_map[i->first] = false;
			}

			return true;
		}

		bool LoadContentFromFile(const char* filename)
		{
			Json::Value root;
			std::ifstream f(filename, std::ifstream::binary);
			Json::Reader reader;
			if(!reader.parse(f, root, false))
			{
				return false;
			}

			Json::Value body = root.get("body", Json::Value());
			if(!body.isArray())
			{
				return false;
			}

			for(Json::Value::UInt index=0; index<body.size(); index++)
			{
				Json::Value elem = body.get(index, Json::Value());
				if(!elem.isObject())
				{
					return false;
				}
				Json::Value type = elem.get("type", Json::Value());
				Json::Value data = elem.get("data", Json::Value());
				if(!type.isString() || !data.isObject())
				{
					return false;
				}
				Json::Value vuuid = data.get("uuid", Json::Value());
				if(!vuuid.isString())
				{
					return false;
				}
				A_UUID uuid;
				if(!AUuidFromString(vuuid.asString().c_str(), uuid))
				{
					return false;
				}
				const DDLReflect::STRUCT_INFO* info = GetType(type.asString().c_str());
				if(!info)
				{
					return false;
				}
				A_CONTENT_OBJECT* object = Alloc(info, uuid);
				if(!object)
				{
					return false;
				}
				if(!DDLReflect::Json2Struct(info, data, (_U8*)object))
				{
					return false;
				}
			}
			return true;
		}

		bool SaveContent(const char* file, bool force)
		{
			std::map<std::string, std::ofstream*> vmap;
			std::map<std::string, bool> vmap_a;

			{
				std::map<std::string, bool>::iterator i;
				for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
				{
					if(!i->second && !force) continue;
					if(file && i->first!=file) continue;

					char filepath[1000];
					sprintf(filepath, "%s/Content/%s", Atlas::AtlasGameDir(), i->first.c_str());
					vmap_a[i->first.c_str()] = true;
					vmap[i->first.c_str()] = ATLAS_NEW std::ofstream();
					std::ofstream& f = *(vmap[i->first.c_str()]);
					f.open(filepath, std::ifstream::binary);
					if(!f.is_open())
					{
						std::map<std::string, std::ofstream*>::iterator fi;
						for(fi=vmap.begin(); fi!=vmap.end(); fi++) delete fi->second;
						vmap.clear();
						return false;
					}
					f << "{" << std::endl;
					f << "\t\"head\":\"json\"," << std::endl;
					f << "\t\"body\":" << std::endl;
					f << "\t[" << std::endl;
				}
				if(vmap.size()==0) return true;
			}

			{
				std::map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
				for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
				{
					const char* file = QueryContentGroupFile(i->second.first);
					if(vmap.find(file)==vmap.end()) continue;

					std::ofstream& f = *(vmap[file]);
					std::string va;
					DDLReflect::Struct2Json(i->second.first, (const _U8*)(i->second.second), va);

					if(vmap_a[file])
					{
						vmap_a[file] = false;
					}
					else
					{
					f << "\t\t}," << std::endl;
					}
					f << "\t\t{" << std::endl;
					f << "\t\t\t" << "\"type\":\"" << i->second.first->name << "\"," << std::endl;
					f << "\t\t\t" << "\"data\":" << va;
				}
			}

			{
				std::map<std::string, std::ofstream*>::iterator fi;
				for(fi=vmap.begin(); fi!=vmap.end(); fi++)
				{
					std::ofstream& f = *(fi->second);
					if(!vmap_a[fi->first])
					{
					f << "\t\t}" << std::endl;
					}
					f << "\t]" << std::endl;
					f << "}" << std::endl;

					g_content_file_map[fi->first] = false;
				}
			}
			{
				std::map<std::string, std::ofstream*>::iterator fi;
				for(fi=vmap.begin(); fi!=vmap.end(); fi++) delete fi->second;
				vmap.clear();
			}

			return true;
		}

		bool IsContentDirty()
		{
			std::map<std::string, bool>::iterator i;
			for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
			{
				if(i->second) return true;
			}
			return false;
		}

	}
}

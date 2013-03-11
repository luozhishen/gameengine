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
		struct STRUCT_INTERNAL_INFO
		{
			_U16							type_id;
			const DDLReflect::STRUCT_INFO*	info;
			bool							bExactMatch;
			Atlas::Vector<Atlas::String>	keys;
			Atlas::Map<Atlas::String, A_CONTENT_OBJECT*> key_map;
		};
		// name to typeid
		static Atlas::Map<Atlas::String, _U16>		g_map_byname;
		static Atlas::Vector<STRUCT_INTERNAL_INFO>	g_array;
		const _U16 g_typeid_base = 0x1000;

		bool Register(const DDLReflect::STRUCT_INFO* info, bool bExactMatch, const char* keys)
		{
			if(!DDLReflect::IsParent(info, DDLReflect::GetStruct<A_CONTENT_OBJECT>()))
			{
				ATLAS_ASSERT(0);
				return false;
			}

			Atlas::Vector<Atlas::String> vkeys;
			if(keys)
			{
				StringSplit(keys, ',', vkeys);
				if(vkeys.size()>4 || vkeys.empty())
				{
					ATLAS_ASSERT(0);
					return false;
				}
				DDLReflect::FIELD_INFO finfo;
				const void* fdata;
				for(size_t i=0; i<vkeys.size(); i++)
				{
					if(!DDLReflect::GetStructFieldInfo(info, vkeys[i].c_str(), (const void*)NULL, finfo, fdata))
					{
						ATLAS_ASSERT(0);
					}
				}
			}

			ATLAS_ASSERT(g_map_byname.find(info->name)==g_map_byname.end());
			if(g_map_byname.find(info->name)!=g_map_byname.end()) return false;

			STRUCT_INTERNAL_INFO internal_info;
			internal_info.type_id = g_typeid_base + (_U16)g_array.size();
			internal_info.info = info;
			internal_info.bExactMatch = bExactMatch;
			internal_info.keys = vkeys;

			g_map_byname[info->name] = internal_info.type_id;
			g_array.push_back(internal_info);

			return true;
		}

		void GetTypeList(Atlas::Vector<const DDLReflect::STRUCT_INFO*>& list)
		{
			list.resize(g_array.size());
			for(size_t i=0; i<g_array.size(); i++)
			{
				list[i] = g_array[i].info;
			}
		}

		_U16 GetTypeId(const char* name)
		{
			Atlas::Map<Atlas::String, _U16>::const_iterator i;
			i = g_map_byname.find(name);
			if(i==g_map_byname.end()) return (_U16)-1;
			return i->second;
		}

		const DDLReflect::STRUCT_INFO* GetType(const char* name)
		{
			Atlas::Map<Atlas::String, _U16>::const_iterator i;
			i = g_map_byname.find(name);
			if(i==g_map_byname.end()) return NULL;
			return g_array[i->second-g_typeid_base].info;
		}

		const DDLReflect::STRUCT_INFO* GetType(_U16 id)
		{
			if(id<g_typeid_base) return NULL;
			if(id>=g_typeid_base+(_U16)g_array.size()) return NULL;
			return g_array[id-g_typeid_base].info;
		}

		static Atlas::Map<Atlas::String, bool> g_content_file_map;
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
				Clear();
			}
			void Clear()
			{
				Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
				for(i=m_object_map.begin(); i!=m_object_map.end(); i++)
				{
					DDLReflect::DestoryObject(i->second.second);
				}
				m_object_map.clear();
			}

			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>> m_object_map;
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
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return;
			DDLReflect::DestoryObject(i->second.second);
			g_content_file_map[QueryContentGroupFile(i->second.first)] = true;
			g_objct_manager.m_object_map.erase(i);
		}

		const DDLReflect::STRUCT_INFO* GetType(const A_UUID& uuid)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			return i->second.first;
		}

		A_CONTENT_OBJECT* Modify(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			if(info!=NULL && i->second.first!=info) return NULL;
			g_content_file_map[QueryContentGroupFile(info)] = true;
			return i->second.second;
		}

		const A_CONTENT_OBJECT* QueryByUUID(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			if(info!=NULL && i->second.first!=info) return NULL;
			return i->second.second;
		}

		const A_CONTENT_OBJECT* QueryByName(const char* name, const DDLReflect::STRUCT_INFO* info)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				if((info!=NULL || i->second.first==info) && strcmp(name, i->second.second->name._Value)==0)
				{
					return i->second.second;					
				}
			}
			return NULL;
		}

		const A_CONTENT_OBJECT* QueryByKey(const DDLReflect::STRUCT_INFO* info, const char* v1, const char* v2, const char* v3, const char* v4)
		{
			ATLAS_ASSERT(info && v1);
			if(!info || v1) return NULL;

			_U16 type_id = GetTypeId(info->name);
			if(type_id==(_U16)-1) return NULL;

			STRUCT_INTERNAL_INFO& internal_info = g_array[type_id-g_typeid_base];

			if(internal_info.keys.empty())
			{
				ATLAS_ASSERT(v2==NULL && v3==NULL && v4==NULL);
				if(v2!=NULL || v3!=NULL || v4!=NULL) return NULL;
				A_UUID uuid;
				
				if(!AUuidFromString(v1, uuid)) return NULL;
				return QueryByUUID(uuid, info);
			}

			Atlas::String keys_value;
			size_t keys_count = 1;
			keys_value = v1;
			if(v2)
			{
				keys_value.append(".$$.");
				keys_value.append(v2);
				keys_count = 2;
				if(v3)
				{
					keys_value.append(".$$.");
					keys_value.append(v3);
					keys_count = 3;
					if(v4)
					{
						keys_value.append(".$$.");
						keys_value.append(v4);
						keys_count = 4;
					}
				}
			}

			ATLAS_ASSERT(internal_info.keys.size()==keys_count);
			if(internal_info.keys.size()!=keys_count) return NULL;

			Atlas::Map<Atlas::String, A_CONTENT_OBJECT*>::iterator i;
			i = internal_info.key_map.find(keys_value);
			if(i==internal_info.key_map.end()) return NULL;
			return i->second;
		}

		static Atlas::String g_buildindex_errmsg;

		bool BuildIndex(const DDLReflect::STRUCT_INFO* info)
		{
			if(info==NULL)
			{
				for(size_t i=0; i<g_array.size(); i++)
				{
					if(g_array[i].keys.size()==0) continue;
					if(!BuildIndex(g_array[i].info)) return false;
				}
				return true;
			}

			_U16 type_id = GetTypeId(info->name);
			if(type_id==(_U16)-1)
			{
				g_buildindex_errmsg = StringFormat("invalid struct name : %s", info->name);
				return false;
			}

			STRUCT_INTERNAL_INFO& internal_info = g_array[type_id-g_typeid_base];
			internal_info.key_map.clear();
			if(internal_info.keys.size()==0) return true;

			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				if(info!=i->second.first || (!internal_info.bExactMatch && IsParent(i->second.first, info))) continue;

				Atlas::String keys_value;
				for(size_t f=0; f<internal_info.keys.size(); f++)
				{
					Atlas::String value;
					if(!StructParamToString(info, internal_info.keys[f].c_str(), i->second.second, value))
					{
						g_buildindex_errmsg = StringFormat("error in StructParamToString(%s)", internal_info.keys[f].c_str());
						return false;
					}
					if(f>0) keys_value.append(".$$.");
					keys_value.append(value);
				}
				if(internal_info.key_map.find(keys_value)!=internal_info.key_map.end())
				{
					char o1[60], o2[60];
					AUuidToString(internal_info.key_map[keys_value]->uuid, o1);
					AUuidToString(i->first, o2);
					g_buildindex_errmsg = StringFormat("reduplicate %s vs %s", o1, o2);
					return false;
				}
				internal_info.key_map[keys_value] = i->second.second;
			}

			return true;
		}

		const Atlas::String& BuildIndexGetErrorMsg()
		{
			return g_buildindex_errmsg;
		}

		bool GetList(const DDLReflect::STRUCT_INFO* info, Atlas::Vector<A_UUID>& list, bool bExactMatch)
		{
			list.clear();

			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				if(i->second.first==info || (!bExactMatch && IsParent(i->second.first, info)))
				{
					list.push_back(i->first);
				}
			}

			return true;
		}

		const A_CONTENT_OBJECT* FindFirst(const DDLReflect::STRUCT_INFO* info, bool bExactMatch)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.begin();
			while(i!=g_objct_manager.m_object_map.end())
			{
				if(i->second.first==info || (!bExactMatch && IsParent(i->second.first, info))) return i->second.second;
				i++;
			}
			return NULL;
		};

		const A_CONTENT_OBJECT* FindNext(const DDLReflect::STRUCT_INFO* info, bool bExactMatch, const A_CONTENT_OBJECT* object)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(object->uuid);
			if(i==g_objct_manager.m_object_map.end()) return NULL;
			i++;
			while(i!=g_objct_manager.m_object_map.end())
			{
				if(i->second.first==info || (!bExactMatch && IsParent(i->second.first, info))) return i->second.second;
				i++;
			}
			return NULL;
		}

		struct CONTENT_GROUP
		{
			const DDLReflect::STRUCT_INFO* info;
			const char* name;
			const char* file;
		};
		static Atlas::Map<const DDLReflect::STRUCT_INFO*, CONTENT_GROUP> g_content_group_map;

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
			Atlas::Map<const DDLReflect::STRUCT_INFO*, CONTENT_GROUP>::iterator i;
			i = g_content_group_map.find(info);
			if(i!=g_content_group_map.end()) return i->second.name;
			return QueryContentGroupName(info);
		}

		const char* QueryContentGroupFile(const DDLReflect::STRUCT_INFO* info)
		{
			ATLAS_ASSERT(info);
			Atlas::Map<const DDLReflect::STRUCT_INFO*, CONTENT_GROUP>::iterator i;
			i = g_content_group_map.find(info);
			if(i!=g_content_group_map.end()) return i->second.file;
			return QueryContentGroupFile(info->parent);
		}

		void GetContentFileList(Atlas::Vector<Atlas::String>& list)
		{
			list.clear();
			Atlas::Map<Atlas::String, bool>::iterator i;
			for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
			{
				list.push_back(i->first);
			}
		}

		bool LoadContent(const char* path)
		{
			Atlas::Map<Atlas::String, bool>::iterator i;
			for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
			{
				char file[1000];
				sprintf(file, "%s%s%s", path?path:Atlas::AtlasGameDir(), path?"":"Content/",i->first.c_str());
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
			Atlas::Map<Atlas::String, std::ofstream*> vmap;
			Atlas::Map<Atlas::String, bool> vmap_a;

			{
				Atlas::Map<Atlas::String, bool>::iterator i;
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
						Atlas::Map<Atlas::String, std::ofstream*>::iterator fi;
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
				Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
				for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
				{
					const char* file = QueryContentGroupFile(i->second.first);
					if(vmap.find(file)==vmap.end()) continue;

					std::ofstream& f = *(vmap[file]);
					Atlas::String va;
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
				Atlas::Map<Atlas::String, std::ofstream*>::iterator fi;
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
				Atlas::Map<Atlas::String, std::ofstream*>::iterator fi;
				for(fi=vmap.begin(); fi!=vmap.end(); fi++) delete fi->second;
				vmap.clear();
			}

			return true;
		}

		void ClearContent()
		{
			g_objct_manager.Clear();
			BuildIndex();
		}

		bool IsContentDirty()
		{
			Atlas::Map<Atlas::String, bool>::iterator i;
			for(i=g_content_file_map.begin(); i!=g_content_file_map.end(); i++)
			{
				if(i->second) return true;
			}
			return false;
		}

	}
}

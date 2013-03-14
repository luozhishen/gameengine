#include <AtlasBase.h>

#include "ContentObject.h"
#include "CommonDDL.h"

#include <map>
#include <string>
#include <json/reader.h>
#include <json/writer.h>
#include <fstream>
#include "des64.h"

namespace Atlas
{

	const char* AtlasGameDir();

	namespace ContentObject
	{

		// content group
		class CContentGroup : public IContentGroup
		{
		public:
			CContentGroup()
			{
			}
			CContentGroup(const CContentGroup& group)
			{
				_name = group._name;
				_file = group._file;
				_cook = group._cook;
				_dirty = group._dirty;
			}
			CContentGroup(const char* name, const char* file, bool cook)
			{
				_name = name;
				_file = file;
				_cook = cook;
				_dirty = true;
			}
			virtual IContentGroup* Register(const DDLReflect::STRUCT_INFO* info, bool bExactMatch, const char* keys);
			virtual bool GetCookFlag() { return _cook; }
			virtual void SetCookFlag(bool cook) { _cook = cook; }

			const char* _name;
			const char* _file;
			bool _cook;
			bool _dirty;
		};
		static Atlas::Map<Atlas::String, CContentGroup> g_content_group_map;
		// content object type
		struct STRUCT_INTERNAL_INFO
		{
			_U16							type_id;
			const DDLReflect::STRUCT_INFO*	info;
			bool							bExactMatch;
			Atlas::Vector<Atlas::String>	keys;
			Atlas::Map<Atlas::String, A_CONTENT_OBJECT*> key_map;
			CContentGroup*					group;
		};
		const _U16 g_typeid_base = 0x1000;
		Atlas::Map<Atlas::String, _U16>		g_contentobject_typemap;
		Atlas::Vector<STRUCT_INTERNAL_INFO>	g_contentobject_typearray;
		// content object
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
		static bool LoadContentFromJsonFile(const char* filename);

		IContentGroup* CreateContentGroup(const char* name, const char* file, bool cook)
		{
			Atlas::Map<Atlas::String, CContentGroup>::iterator i;
			for(i=g_content_group_map.begin(); i!=g_content_group_map.end(); i++)
			{
				ATLAS_ASSERT(strcmp(i->second._name, name)!=0);
				ATLAS_ASSERT(strcmp(i->second._file, file)!=0);
			}

			CContentGroup group(name, file, cook);
			g_content_group_map[name] = group;
			return &g_content_group_map[name];
		}

		IContentGroup* GetContentGroup(const char* name)
		{
			Atlas::Map<Atlas::String, CContentGroup>::iterator i;
			i = g_content_group_map.find(name);
			if(i==g_content_group_map.end()) return NULL;
			return &i->second;
		}

		static CContentGroup* QueryContentGroup(const DDLReflect::STRUCT_INFO* info)
		{
			Atlas::Map<Atlas::String, _U16>::iterator i;
			while(info)
			{
				i = g_contentobject_typemap.find(info->name);
				if(i!=g_contentobject_typemap.end())
				{
					return g_contentobject_typearray[i->second-g_typeid_base].group;
				}
				info = info->parent;
			}
			return NULL;
		}

		const char* QueryContentGroupName(const DDLReflect::STRUCT_INFO* info)
		{
			CContentGroup* group = QueryContentGroup(info);
			if(!group) return NULL;
			return group->_name;
		}

		const char* QueryContentGroupFile(const DDLReflect::STRUCT_INFO* info)
		{
			CContentGroup* group = QueryContentGroup(info);
			if(!group) return NULL;
			return group->_file;
		}

		IContentGroup* CContentGroup::Register(const DDLReflect::STRUCT_INFO* info, bool bExactMatch, const char* keys)
		{
			if(!DDLReflect::IsParent(info, DDLReflect::GetStruct<A_CONTENT_OBJECT>()))
			{
				ATLAS_ASSERT(0);
				return NULL;
			}

			Atlas::Vector<Atlas::String> vkeys;
			if(keys)
			{
				StringSplit(keys, ',', vkeys);
				if(vkeys.size()>4 || vkeys.empty())
				{
					ATLAS_ASSERT(0);
					return NULL;
				}
				DDLReflect::FIELD_INFO finfo;
				const void* fdata;
				for(size_t i=0; i<vkeys.size(); i++)
				{
					if(vkeys[i]=="uuid")
					{
						if(vkeys.size()!=1)
						{
							return false;
						}
						else
						{
							vkeys.clear();
							break;
						}
					}
					if(!DDLReflect::GetStructFieldInfo(info, vkeys[i].c_str(), (const void*)NULL, finfo, fdata))
					{
						ATLAS_ASSERT(0);
						return NULL;
					}
				}
			}

			ATLAS_ASSERT(g_contentobject_typemap.find(info->name)==g_contentobject_typemap.end());
			if(g_contentobject_typemap.find(info->name)!=g_contentobject_typemap.end()) return NULL;

			STRUCT_INTERNAL_INFO internal_info;
			internal_info.type_id = g_typeid_base + (_U16)g_contentobject_typearray.size();
			internal_info.info = info;
			internal_info.bExactMatch = bExactMatch;
			internal_info.keys = vkeys;
			internal_info.group = this;

			g_contentobject_typemap[info->name] = internal_info.type_id;
			g_contentobject_typearray.push_back(internal_info);

			return this;
		}

		void GetTypeList(Atlas::Vector<const DDLReflect::STRUCT_INFO*>& list)
		{
			list.resize(g_contentobject_typearray.size());
			for(size_t i=0; i<g_contentobject_typearray.size(); i++)
			{
				list[i] = g_contentobject_typearray[i].info;
			}
		}

		_U16 GetTypeId(const char* name)
		{
			Atlas::Map<Atlas::String, _U16>::const_iterator i;
			i = g_contentobject_typemap.find(name);
			if(i==g_contentobject_typemap.end()) return (_U16)-1;
			return i->second;
		}

		const DDLReflect::STRUCT_INFO* GetType(const char* name)
		{
			Atlas::Map<Atlas::String, _U16>::const_iterator i;
			i = g_contentobject_typemap.find(name);
			if(i==g_contentobject_typemap.end()) return NULL;
			return g_contentobject_typearray[i->second-g_typeid_base].info;
		}

		const DDLReflect::STRUCT_INFO* GetType(_U16 id)
		{
			if(id<g_typeid_base) return NULL;
			if(id>=g_typeid_base+(_U16)g_contentobject_typearray.size()) return NULL;
			return g_contentobject_typearray[id-g_typeid_base].info;
		}

		bool GetTypePrimaryKey(const char* name, Atlas::Set<Atlas::String>& keys)
		{
			keys.clear();
			_U16 id = GetTypeId(name);
			if(id==(_U16)-1) return false;
			Atlas::Vector<Atlas::String>& karray = g_contentobject_typearray[id-g_typeid_base].keys;
			if(karray.empty())
			{
				keys.insert("uuid");
			}
			else
			{
				for(size_t i=0; i<karray.size(); i++)
				{
					keys.insert(karray[i]);
				}
			}
			return true;
		}

		A_CONTENT_OBJECT* Create(const DDLReflect::STRUCT_INFO* info, A_UUID& uuid)
		{
			AUuidGenerate(uuid);
			return Alloc(info, uuid);
		}

		A_CONTENT_OBJECT* Alloc(const DDLReflect::STRUCT_INFO* info, const A_UUID& uuid)
		{
			CContentGroup* group = QueryContentGroup(info);
			A_CONTENT_OBJECT* object = (A_CONTENT_OBJECT*)DDLReflect::CreateObject(info);
			if(!object) return NULL;
			g_objct_manager.m_object_map[uuid] = std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>(info, object);
			group->_dirty = true;
			object->uuid = uuid;
			return object;
		}

		void Delete(const A_UUID& uuid)
		{
			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			i = g_objct_manager.m_object_map.find(uuid);
			if(i==g_objct_manager.m_object_map.end()) return;
			DDLReflect::DestoryObject(i->second.second);
			QueryContentGroup(i->second.first)->_dirty = true;
			g_objct_manager.m_object_map.erase(i);
		}

		const DDLReflect::STRUCT_INFO* GetObjectType(const A_UUID& uuid)
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
			QueryContentGroup(i->second.first)->_dirty = true;
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
			if(!info || !v1) return NULL;

			_U16 type_id = GetTypeId(info->name);
			if(type_id==(_U16)-1) return NULL;

			STRUCT_INTERNAL_INFO& internal_info = g_contentobject_typearray[type_id-g_typeid_base];

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

		const A_CONTENT_OBJECT* QueryByUniqueId(const DDLReflect::STRUCT_INFO* info, const char* value1)
		{
			return NULL;
		}

		bool GenContentObjectUniqueId(_U16 id, const A_CONTENT_OBJECT* obj, Atlas::String& uid)
		{
			if(id<g_typeid_base) return false;
			if(id>=g_typeid_base+(_U16)g_contentobject_typearray.size()) return false;
			STRUCT_INTERNAL_INFO& info = g_contentobject_typearray[id-g_typeid_base];

			uid.clear();
			for(size_t f=0; f<info.keys.size(); f++)
			{
				Atlas::String value;
				if(!DDLReflect::StructParamToString(info.info, info.keys[f].c_str(), obj, value))
				{
					return false;
				}
				if(f>0) uid.append(".$$.");
				uid.append(value);
			}

			return true;
		}

		static Atlas::String g_buildindex_errmsg;

		bool BuildIndex(const DDLReflect::STRUCT_INFO* info)
		{
			if(info==NULL)
			{
				for(size_t i=0; i<g_contentobject_typearray.size(); i++)
				{
					if(g_contentobject_typearray[i].keys.size()==0) continue;
					if(!BuildIndex(g_contentobject_typearray[i].info)) return false;
				}
				return true;
			}

			_U16 type_id = GetTypeId(info->name);
			if(type_id==(_U16)-1)
			{
				g_buildindex_errmsg = StringFormat("invalid struct name : %s", info->name);
				return false;
			}

			STRUCT_INTERNAL_INFO& internal_info = g_contentobject_typearray[type_id-g_typeid_base];
			internal_info.key_map.clear();
			if(internal_info.keys.size()==0) return true;

			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				if(info!=i->second.first || (!internal_info.bExactMatch && IsParent(i->second.first, info))) continue;

				Atlas::String keys_value;
				if(!GenContentObjectUniqueId(type_id, i->second.second, keys_value))
				{
					g_buildindex_errmsg = StringFormat("error in GenContentObjectUniqueId");
					return false;
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

		bool LoadContent(const char* path)
		{
			Atlas::Map<Atlas::String, CContentGroup>::iterator i;
			for(i=g_content_group_map.begin(); i!=g_content_group_map.end(); i++)
			{
				char file[1000];
				sprintf(file, "%s%s%s", path?path:Atlas::AtlasGameDir(), path?"":"Content/Json/", i->second._file);
				if(!LoadContentFromJsonFile(file)) return false;
				i->second._dirty = false;
			}

			return true;
		}

		static bool CreateContentObject(Json::Value elem)
		{
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
			return true;
		}

		bool LoadContentFromJsonFile(const char* filename)
		{
			Json::Value root;
			std::ifstream f(filename, std::ifstream::binary);
			if(!f.is_open()) return true;
			Json::Reader reader;
			if(!reader.parse(f, root, false)) return false;

			Json::Value body = root.get("body", Json::Value());
			if(!body.isArray()) return false;

			for(Json::Value::UInt index=0; index<body.size(); index++)
			{
				if(!CreateContentObject(body.get(index, Json::Value()))) return false;
			}
			return true;
		}

		bool LoadContentFromBinaryFile(const char* filename, const char* deskey)
		{
			DES_KEY key;
			DES_KEY_S keys;
			if(!DES_KeyFromString(key, deskey)) return false;
			DES_SetKey(key, keys);

			FILE* fp = fopen(filename, "rb");
			Atlas::String line;
			for(;;)
			{
				char src[8], buf[9];
				if(fread(src, 1, 8, fp)!=8) break;
				DES_Decrypt(keys, buf, src);
				buf[8] = '\0';
				line.append(buf);
				if(strlen(buf)<8)
				{
					Json::Value elm;
					Json::Reader reader;
					if(!reader.parse(line.c_str(), line.c_str()+line.size(), elm, false)) break;
					if(!CreateContentObject(elm))
					{
						fclose(fp);
						return false;
					}
					line = "";
				}
				if(feof(fp))
				{
					if(line.size()==0)
					{
						fclose(fp);
						return true;
					}
					else
					{
						break;
					}
				}
			}
			fclose(fp);
			return true;
		}

		bool SaveContent(const char* path, bool force)
		{
			Atlas::String realpath;
			if(path)
			{
				realpath = path;
			}
			else
			{
				realpath = StringFormat("%s/Content/Json/", AtlasGameDir());
			}

			Atlas::Map<Atlas::String, std::ofstream*> vmap;
			Atlas::Map<Atlas::String, bool> vmap_a;

			Atlas::Map<Atlas::String, CContentGroup>::iterator gi;
			for(gi=g_content_group_map.begin(); gi!=g_content_group_map.end(); gi++)
			{
				if(!gi->second._dirty && !force) continue;

				char filepath[1000];
				sprintf(filepath, "%s%s", realpath.c_str(), gi->second._file);
				vmap_a[gi->first.c_str()] = true;
				vmap[gi->first.c_str()] = ATLAS_NEW std::ofstream();
				std::ofstream& f = *(vmap[gi->first.c_str()]);
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

			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator oi;
			for(oi=g_objct_manager.m_object_map.begin(); oi!=g_objct_manager.m_object_map.end(); oi++)
			{
				const char* file = QueryContentGroupName(oi->second.first);
				if(vmap.find(file)==vmap.end()) continue;

				std::ofstream& f = *(vmap[file]);
				Atlas::String va;
				DDLReflect::Struct2Json(oi->second.first, (const _U8*)(oi->second.second), va);

				if(vmap_a[file])
				{
					vmap_a[file] = false;
				}
				else
				{
				f << "\t\t}," << std::endl;
				}
				f << "\t\t{" << std::endl;
				f << "\t\t\t" << "\"type\":\"" << oi->second.first->name << "\"," << std::endl;
				f << "\t\t\t" << "\"data\":" << va;
			}

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
				f.close();
				delete fi->second;
				((CContentGroup*)GetContentGroup(fi->first.c_str()))->_dirty = false;
			}
			vmap.clear();

			return true;
		}

		bool SaveContentToBinaryFile(const char* file, const char* deskey)
		{
			DES_KEY key;
			DES_KEY_S keys;
			if(!DES_KeyFromString(key, deskey)) return false;
			DES_SetKey(key, keys);

			FILE* fp = fopen(file, "wb");
			if(!fp) return false;

			Atlas::Map<A_UUID, std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>>::iterator i;
			for(i=g_objct_manager.m_object_map.begin(); i!=g_objct_manager.m_object_map.end(); i++)
			{
				CContentGroup* group = QueryContentGroup(i->second.first);
				if(!group->_cook) continue;
				Atlas::String va;
				DDLReflect::Struct2Json(i->second.first, (const _U8*)(i->second.second), va);
				Atlas::String line = StringFormat("{\"type\":\"%s\",\"data\":%s}", i->second.first->name, va.c_str());
				const char* pos = line.c_str();
				size_t size = line.size() + 1;
				char buf[8];
				while(size>8)
				{
					DES_Encrypt(keys, buf, pos);
					fwrite(buf, 1, 8, fp);
					pos += 8;
					size -= 8;
				}
				char buf1[8];
				memset(buf1, 0, sizeof(buf));
				memcpy(buf1, pos, size);
				DES_Encrypt(keys, buf, buf1);
				fwrite(buf, 1, 8, fp);
			}

			fclose(fp);
			return true;
		}

		void ClearContents()
		{
			g_objct_manager.Clear();
			BuildIndex();
			Atlas::Map<Atlas::String, CContentGroup>::iterator i;
			for(i=g_content_group_map.begin(); i!=g_content_group_map.end(); i++)
			{
				i->second._dirty = true;
			}
		}

		bool IsContentDirty()
		{
			Atlas::Map<Atlas::String, CContentGroup>::iterator i;
			for(i=g_content_group_map.begin(); i!=g_content_group_map.end(); i++)
			{
				if(i->second._dirty) return true;
			}
			return false;
		}

	}
}

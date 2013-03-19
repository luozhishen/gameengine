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
		Atlas::Map<Atlas::String, _U16>		g_typemap;
		Atlas::Vector<STRUCT_INTERNAL_INFO>	g_typearray;
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
				i = g_typemap.find(info->name);
				if(i!=g_typemap.end())
				{
					return g_typearray[i->second-g_typeid_base].group;
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
							ATLAS_ASSERT(0);
							return NULL;
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

			ATLAS_ASSERT(g_typemap.find(info->name)==g_typemap.end());
			if(g_typemap.find(info->name)!=g_typemap.end()) return NULL;

			STRUCT_INTERNAL_INFO internal_info;
			internal_info.type_id = g_typeid_base + (_U16)g_typearray.size();
			internal_info.info = info;
			internal_info.bExactMatch = bExactMatch;
			internal_info.keys = vkeys;
			internal_info.group = this;

			g_typemap[info->name] = internal_info.type_id;
			g_typearray.push_back(internal_info);

			return this;
		}

		void GetTypeList(Atlas::Vector<const DDLReflect::STRUCT_INFO*>& list)
		{
			list.resize(g_typearray.size());
			for(size_t i=0; i<g_typearray.size(); i++)
			{
				list[i] = g_typearray[i].info;
			}
		}

		_U16 GetTypeId(const char* name)
		{
			Atlas::Map<Atlas::String, _U16>::const_iterator i;
			i = g_typemap.find(name);
			if(i==g_typemap.end()) return (_U16)-1;
			return i->second;
		}

		const DDLReflect::STRUCT_INFO* GetType(const char* name)
		{
			Atlas::Map<Atlas::String, _U16>::const_iterator i;
			i = g_typemap.find(name);
			if(i==g_typemap.end()) return NULL;
			return g_typearray[i->second-g_typeid_base].info;
		}

		const DDLReflect::STRUCT_INFO* GetType(_U16 id)
		{
			if(id<g_typeid_base) return NULL;
			if(id>=g_typeid_base+(_U16)g_typearray.size()) return NULL;
			return g_typearray[id-g_typeid_base].info;
		}

		bool GetTypePrimaryKey(const char* name, Atlas::Set<Atlas::String>& keys)
		{
			keys.clear();
			_U16 id = GetTypeId(name);
			if(id==(_U16)-1) return false;
			Atlas::Vector<Atlas::String>& karray = g_typearray[id-g_typeid_base].keys;
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

		A_CONTENT_OBJECT* CreateObject(const DDLReflect::STRUCT_INFO* info, A_UUID& uuid)
		{
			AUuidGenerate(uuid);
			return AllocObject(info, uuid);
		}

		A_CONTENT_OBJECT* AllocObject(const DDLReflect::STRUCT_INFO* info, const A_UUID& uuid)
		{
			CContentGroup* group = QueryContentGroup(info);
			A_CONTENT_OBJECT* object = (A_CONTENT_OBJECT*)DDLReflect::CreateObject(info);
			if(!object) return NULL;
			g_objct_manager.m_object_map[uuid] = std::pair<const DDLReflect::STRUCT_INFO*, A_CONTENT_OBJECT*>(info, object);
			group->_dirty = true;
			object->uuid = uuid;
			return object;
		}

		void DeleteObject(const A_UUID& uuid)
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

			STRUCT_INTERNAL_INFO& internal_info = g_typearray[type_id-g_typeid_base];

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
			ATLAS_ASSERT(info);
			if(!info) return NULL;

			_U16 type_id = GetTypeId(info->name);
			if(type_id==(_U16)-1) return NULL;

			STRUCT_INTERNAL_INFO& internal_info = g_typearray[type_id-g_typeid_base];
			Atlas::Map<Atlas::String, A_CONTENT_OBJECT*>::iterator i;
			i = internal_info.key_map.find(value1);
			if(i==internal_info.key_map.end()) return NULL;
			return i->second;
		}

		bool GenContentObjectUniqueId(_U16 id, const A_CONTENT_OBJECT* obj, Atlas::String& uid)
		{
			if(id<g_typeid_base) return false;
			if(id>=g_typeid_base+(_U16)g_typearray.size()) return false;
			STRUCT_INTERNAL_INFO& info = g_typearray[id-g_typeid_base];

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
				for(size_t i=0; i<g_typearray.size(); i++)
				{
					if(g_typearray[i].keys.size()==0) continue;
					if(!BuildIndex(g_typearray[i].info)) return false;
				}
				return true;
			}

			_U16 type_id = GetTypeId(info->name);
			if(type_id==(_U16)-1)
			{
				g_buildindex_errmsg = StringFormat("invalid struct name : %s", info->name);
				return false;
			}

			STRUCT_INTERNAL_INFO& internal_info = g_typearray[type_id-g_typeid_base];
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

		bool LoadContent(const char* path, bool ignore)
		{
			Atlas::Map<Atlas::String, CContentGroup>::iterator i;
			for(i=g_content_group_map.begin(); i!=g_content_group_map.end(); i++)
			{
				String file = Atlas::StringFormat("%s%s%s", path?path:Atlas::AtlasGameDir(), path?"":"Content/Json/", i->second._file);
				if(LoadContentFromJsonFile(file.c_str()))
				{
					i->second._dirty = false;
				}
				else
				{
					if(!ignore) return false;
				}
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
			A_CONTENT_OBJECT* object = AllocObject(info, uuid);
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
			_U8 rawdata[300*1024];
			for(;;)
			{
				if(feof(fp))
				{
					fclose(fp);
					return true;
				}

				_U16 tid;
				_U32 size, i, count;

				if(fread(&tid, 1, sizeof(tid), fp)!=sizeof(tid)) break;
				if(fread(&size, 1, sizeof(size), fp)!=sizeof(size)) break;
				if(size>sizeof(rawdata)) break;

				const DDLReflect::STRUCT_INFO* info = GetType(tid);
				if(!info) break;

				count = (size==0?0:(size-1)/7+1);
				for(i=0; i<count; i++)
				{
					char buf[8], sbuf[8];
					if(fread(buf, 1, sizeof(buf), fp)!=sizeof(buf)) break;
					DES_Decrypt(keys, sbuf, buf);
					for(size_t x=0; x<7; x++)
					{
						rawdata[i*7+x] = (_U8)(sbuf[x]&0x7f);
						if(sbuf[7]&(1<<x)) rawdata[i*7+x] |= 0x8f;
					}
				}
				if(i!=count) break;

				A_CONTENT_OBJECT* obj = AllocObject(info, *((A_UUID*)rawdata));
				if(!obj) break;

				DDL::MemoryReader reader(rawdata, size);
				if(!info->read_proc(reader, obj)) break;
			}

			fclose(fp);
			return false;
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
				realpath = StringFormat("%sContent/Json/", AtlasGameDir());
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
	
				if(vmap_a[file])
				{
					vmap_a[file] = false;
				}
				else
				{
					f << "\t\t}," << std::endl;
				}
	
				Atlas::String va;
				if(!DDLReflect::Struct2Json(oi->second.first, (const _U8*)(oi->second.second), va))
				{
					ATLAS_ASSERT(0);
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

				_U8 rawdata[300*1024];
				DDL::MemoryWriter writer(rawdata, sizeof(rawdata));
				if(!i->second.first->write_proc(writer, i->second.second))
				{
					fclose(fp);
					return false;
				}

				_U8* src = rawdata;
				char buf[8], sbuf[8];
				_U32 size = writer.GetSize();
				_U16 tid = Atlas::ContentObject::GetTypeId(i->second.first->name);
				fwrite(&tid, 1, sizeof(tid), fp);
				fwrite(&size, 1, sizeof(size), fp);
				while(size>7)
				{
					sbuf[7] = 0;
					for(size_t i=0; i<7; i++)
					{
						sbuf[i] = (char)(src[i]&0x7f);
						if(src[i]&0x80) sbuf[7] |= 1 << i;
					}
					DES_Encrypt(keys, buf, sbuf);
					fwrite(buf, 1, 8, fp);
					src += 7;
					size -= 7;
				}
				if(size>0)
				{
					memset(sbuf, 0, sizeof(sbuf));
					for(size_t i=0; i<size; i++)
					{
						sbuf[i] = (char)(src[i]&0x7f);
						if(src[i]&0x80) sbuf[7] |= 1 << i;
					}
					DES_Encrypt(keys, buf, sbuf);
					fwrite(buf, 1, 8, fp);
				}
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

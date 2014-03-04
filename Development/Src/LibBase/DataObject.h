#ifndef __ZION_DATA_OBJECT_INCLUDE__
#define __ZION_DATA_OBJECT_INCLUDE__

namespace Zion
{
	namespace DataObject
	{

		class IMonitor
		{
		public:
			virtual ~IMonitor() {}

			virtual void SetDirty(const char* name, _U32 offset, _U32 size) = 0;
		};

		class CConstObject;
		class CObject;

		class CConstObject
		{
		public:
			CConstObject();
			CConstObject(const CConstObject* pObject);
			CConstObject(const CObject* pObject);
			CConstObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset);

			CConstObject Ref(const char* name) const;
			const void* GetData();
			const DDLReflect::STRUCT_INFO* GetStructInfo();

			_U8 GetU8(const char* name) const;
			_U16 GetU16(const char* name) const;
			_U32 GetU32(const char* name) const;
			_U64 GetU64(const char* name) const;
			_S8 GetS8(const char* name) const;
			_S16 GetS16(const char* name) const;
			_S32 GetS32(const char* name) const;
			_S64 GetS64(const char* name) const;
			_F32 GetF32(const char* name) const;
			_F64 GetF64(const char* name) const;
			const _STR GetSTR(const char* name) const;
			const A_UUID& GetUUID(const char* name) const;
			const void* GetObjPtr(const char* name, const DDLReflect::STRUCT_INFO* type) const;
			template<typename T>
			const T& GetObj(const char* name) const
			{
				return *((const T*)GetObjPtr(name, DDLReflect::GetStruct<T>()));
			}

			_U8 GetU8(const char* name, _U32 index) const;
			_U16 GetU16(const char* name, _U32 index) const;
			_U32 GetU32(const char* name, _U32 index) const;
			_U64 GetU64(const char* name, _U32 index) const;
			_S8 GetS8(const char* name, _U32 index) const;
			_S16 GetS16(const char* name, _U32 index) const;
			_S32 GetS32(const char* name, _U32 index) const;
			_S64 GetS64(const char* name, _U32 index) const;
			_F32 GetF32(const char* name, _U32 index) const;
			_F64 GetF64(const char* name, _U32 index) const;
			const _STR GetSTR(const char* name, _U32 index) const;
			const A_UUID& GetUUID(const char* name, _U32 index) const;
			const void* GetObjPtr(const char* name, _U32 index, const DDLReflect::STRUCT_INFO* type) const;
			template<typename T>
			const T& GetObj(const char* name, _U32 index) const
			{
				return *((const T*)GetObjPtr(name, index, DDLReflect::GetStruct<T>()));
			}

		protected:
			_U32 m_Offset;
			void* m_pData;
			const DDLReflect::STRUCT_INFO* m_pInfo;
		};

		class CObject : public CConstObject
		{
		public:
			CObject();
			CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const char* name, _U32 offset);
			CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const String& name, _U32 offset);
			CObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset);
			CObject(const CObject* pObject);

			const CObject Ref(const char* name);
			void* GetData();

			bool SetU8(const char* name, _U8 val);
			bool SetU16(const char* name, _U16 val);
			bool SetU32(const char* name, _U32 val);
			bool SetU64(const char* name, _U64 val);
			bool SetS8(const char* name, _S8 val);
			bool SetS16(const char* name, _S16 val);
			bool SetS32(const char* name, _S32 val);
			bool SetS64(const char* name, _S64 val);
			bool SetF32(const char* name, _F32 val);
			bool SetF64(const char* name, _F64 val);
			bool SetSTR(const char* name, const _STR val);
			bool SetUUID(const char* name, const A_UUID& val);
			bool SetObjPtr(const char* name, const DDLReflect::STRUCT_INFO* type, const void* val);
			template<typename T>
			bool SetObj(const char* name, const T& val) const
			{
				return SetObjPtr(name, DDLReflect::GetStruct<T>(), &val);
			}

			bool SetU8(const char* name, _U32 index, _U8 val);
			bool SetU16(const char* name, _U32 index, _U16 val);
			bool SetU32(const char* name, _U32 index, _U32 val);
			bool SetU64(const char* name, _U32 index, _U64 val);
			bool SetS8(const char* name, _U32 index, _S8 val);
			bool SetS16(const char* name, _U32 index, _S16 val);
			bool SetS32(const char* name, _U32 index, _S32 val);
			bool SetS64(const char* name, _U32 index, _S64 val);
			bool SetF32(const char* name, _U32 index, _F32 val);
			bool SetF64(const char* name, _U32 index, _F64 val);
			bool SetSTR(const char* name, _U32 index, const _STR val);
			bool SetUUID(const char* name, _U32 index, const A_UUID& val);
			bool SetObjPtr(const char* name, _U32 index, const DDLReflect::STRUCT_INFO* type, const void* val);
			template<typename T>
			bool SetObj(const char* name, _U32 index, const T& val) const
			{
				return SetObjPtr(name, index, DDLReflect::SetStruct<T>(), &val);
			}

		private:
			IMonitor* m_pMonitor;
			String m_Name;
		};

	}
}

#endif

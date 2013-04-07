#ifndef __SGGAME_CLIENT_TIPS_HELPER__
#define __SGGAME_CLIENT_TIPS_HELPER__
namespace Atlas
{	
	template<typename T>
	class SGClientTipsHelper
	{
	public:
		SGClientTipsHelper(){}

		void AddItem(T t)
		{
			m_data.push_back(t);
		}

		void GetNewList(Atlas::Vector<T>& data_list)
		{
			data_list = m_data;
		}

		bool HasNewItem()
		{	
			return !m_data.empty();
		}

		bool IsNewItem(T id)
		{
			for(size_t i = 0; i < m_data.size(); ++i)
			{
				if(m_data[i] == id)
				{
					return true;
				}
			}

			return false;
		}
		
		void ClearNewList()
		{
			m_data.clear();
		}

		Atlas::Vector<T>& GetData()
		{
			return m_data;
		}

		void Resize(_U32 count)
		{
			m_data.resize( (size_t)count > m_data.size() ? (size_t)count : m_data.size());
		}

	private:
		Atlas::Vector<T> m_data;
	};

}
 
#endif //__SGGAME_CLIENT_TIPS_HELPER__
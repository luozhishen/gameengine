#ifndef __SGGAME_CLIENT_TIPS_HELPER__
#define __SGGAME_CLIENT_TIPS_HELPER__
namespace Zion
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

		void GetNewList(Zion::Array<T>& data_list)
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

		Zion::Array<T>& GetData()
		{
			return m_data;
		}

		void Resize(_U32 count)
		{
			m_data.resize( (size_t)count > m_data.size() ? (size_t)count : m_data.size());
		}

	private:
		Zion::Array<T> m_data;
	};

}
 
#endif //__SGGAME_CLIENT_TIPS_HELPER__
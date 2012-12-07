#ifndef __SGCLIENT_GENREAL_COMPONENT_H__
#define __SGCLIENT_GENREAL_COMPONENT_H__


namespace Atlas
{
	class CSGClientGeneralComponent : public CClientComponent
	{
	public:
		CSGClientGeneralComponent(CSGClient* pClient);
		virtual ~CSGClientGeneralComponent();

		void GetGeneralComponent(std::vector<_U32>& genrels);
		const SG_GENERAL* GetGenerallInfo(_U32 type);

	private:
		std::vector<SG_GENERAL> m_vecGenerals;
	};

}

#endif //__SGCLIENT_GENREAL_COMPONENT_H__
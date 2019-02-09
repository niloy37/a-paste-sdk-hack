#pragma once


namespace ap::sdk
{
	struct c_recv_table;

	class c_client_class
	{
	public:
		void*			m_pCreateFn;
		void*			m_pCreateEventFn;
		char			*m_pNetworkName;
		c_recv_table	*m_pRecvTable;
		c_client_class	*m_pNext;
		int				m_ClassID;
	};
} // namespace ap::sdk
#pragma once

#include "../misc/vector.h"


namespace ap::sdk
{
	struct c_recv_table;
	struct c_recv_prop;

	struct d_variant
	{
		union
		{
			float	m_Float;
			long	m_Int;
			char	*m_pString;
			void	*m_pData;
			vec3f	m_vec3f;
		};
	};

	struct c_recv_table
	{
		c_recv_prop		*m_pProps;
		int				m_nProps;
		void			*m_pDecoder;
		char			*m_pNetTableName;
		bool			m_bInitialized;
		bool			m_bInMainList;
	};
	struct c_recv_prop
	{
		char					*m_pVarName;
		int						m_RecvType;
		int						m_Flags;
		int						m_StringBufferSize;
		bool					m_bInsideArray;
		const void				*m_pExtraData;
		c_recv_prop				*m_pArrayProp;
		void*					m_ArrayLengthProxy;
		void*					m_ProxyFn;
		void*					m_DataTableProxyFn;
		c_recv_table			*m_pDataTable;
		int						m_Offset;
		int						m_ElementStride;
		int						m_nElements;
		const char				*m_pParentArrayPropName;
	};
	class c_recv_proxy_data
	{
	public:
		const c_recv_prop	*m_pRecvProp;
		char _pad[4];
		d_variant			m_Value;
		int					m_iElement;
		int					m_ObjectID;
	};
} // namespace ap::sdk
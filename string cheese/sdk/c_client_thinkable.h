#pragma once

class c_client_unknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class c_client_thinkable
{
public:
	virtual c_client_unknown*		Getc_client_unknown() = 0;
	virtual void				ClientThink() = 0;
	virtual ClientThinkHandle_t	GetThinkHandle() = 0;
	virtual void				SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void				Release() = 0;
};
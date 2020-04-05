#include "UDPServerFactory.h"
#include "UDPServer.h"


UDPServerFactory* UDPServerFactory::m_pInstance = NULL;
void UDPServerFactory::Version()
{
}

UDPServerFactory::UDPServerFactory()
{
	m_pService = NULL;
}

UDPServerFactory* UDPServerFactory::GetInstance()
{
	if (NULL == m_pInstance)
	{
        m_pInstance = new UDPServerFactory();
	}
	return m_pInstance;
}
UDPServerFactory::~UDPServerFactory()
{
	if (NULL != m_pService)
	{
		delete m_pService;
		m_pService = NULL;
	}
}
IStreamServer* UDPServerFactory::CreateProduct()
{
	if (NULL == m_pService)
	{
		m_pService = new CUDPServer();
		return m_pService;
	}
	return m_pService;
}

 
#include "SKYStreamServerFactory.h"
#include "UDPServer.h"


SKYStreamServerFactory* SKYStreamServerFactory::m_pInstance = NULL;
void SKYStreamServerFactory::Version()
{
}

SKYStreamServerFactory::SKYStreamServerFactory()
{
	m_pService = NULL;
}

SKYStreamServerFactory* SKYStreamServerFactory::GetInstance()
{
	if (NULL == m_pInstance)
	{
        m_pInstance = new SKYStreamServerFactory();
	}
	return m_pInstance;
}
SKYStreamServerFactory::~SKYStreamServerFactory()
{
	if (NULL != m_pService)
	{
		delete m_pService;
		m_pService = NULL;
	}
}
ISKYStreamServer* SKYStreamServerFactory::CreateProduct()
{
	if (NULL == m_pService)
	{
        //printf("cm_pUDPBase,(%p)\n", m_pService);
		m_pService = new CUDPServer();
		return m_pService;
	}
	return m_pService;
}

 
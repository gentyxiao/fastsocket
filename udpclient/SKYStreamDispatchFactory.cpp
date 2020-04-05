#include "SKYStreamDispatchFactory.h"

#include "UDPClient.h"

#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

SKYStreamDispatchFactory* SKYStreamDispatchFactory::m_pInstance = NULL;
void SKYStreamDispatchFactory::Version()
{
	//printf("%s:Version:%d.%d.%d\n", PRGNAME,FileLib_MAR_VERSION ,FileLib_MIN_VERSION ,FileLib_RELEASE_VERSION );
}
SKYStreamDispatchFactory::SKYStreamDispatchFactory()
{
	m_pService = NULL;
}
SKYStreamDispatchFactory* SKYStreamDispatchFactory::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new SKYStreamDispatchFactory();
	}
	return m_pInstance;
}
SKYStreamDispatchFactory::~SKYStreamDispatchFactory()
{
	if (NULL != m_pService)
	{ 
		delete m_pService;
		m_pService = NULL;
	}
}
ISKYStreamDispatch* SKYStreamDispatchFactory::CreateProduct()
{
 
	if (NULL == m_pService)
	{
		m_pService = new UDPClient();
	}
	return m_pService;
	 
}


 
 
 
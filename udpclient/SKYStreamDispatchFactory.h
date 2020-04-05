#ifndef SKY_STREAM_DISPATCH_H_
#define SKY_STREAM_DISPATCH_H_


#include "../include/ISKYStreamDispatch.h"

class SKYStreamDispatchFactory
{
protected:
	SKYStreamDispatchFactory();
public:
	~SKYStreamDispatchFactory();

	static SKYStreamDispatchFactory* GetInstance();
	ISKYStreamDispatch* CreateProduct();
	void Version();
	ISKYStreamDispatch * m_pService;
protected:	 
	static SKYStreamDispatchFactory* m_pInstance;
};

#endif
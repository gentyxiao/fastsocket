#ifndef SKY_STREAM_SERVER_FACTORY_H_
#define SKY_STREAM_SERVER_FACTORY_H_
#include "../include/ISKYStreamServer.h"

class SKYStreamServerFactory
{
protected:
    SKYStreamServerFactory();
public:
    ~SKYStreamServerFactory();
    void Version();
    static SKYStreamServerFactory* GetInstance();
    ISKYStreamServer* CreateProduct();
    ISKYStreamServer* m_pService;
protected:
    static SKYStreamServerFactory* m_pInstance;
};
#endif

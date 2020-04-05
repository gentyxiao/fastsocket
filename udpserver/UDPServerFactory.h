#ifndef SKY_STREAM_SERVER_FACTORY_H_
#define SKY_STREAM_SERVER_FACTORY_H_
#include "IStreamServer.h"

class UDPServerFactory
{
protected:
    UDPServerFactory();
public:
    ~UDPServerFactory();
    void Version();
    static UDPServerFactory* GetInstance();
    IStreamServer* CreateProduct();
    IStreamServer* m_pService;
protected:
    static UDPServerFactory* m_pInstance;
};
#endif

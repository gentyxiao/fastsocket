#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "IStreamServer.h"
#include "IStreamSession.h"
#include "UDPServerFactory.h"

class CStreamAction
{
public:
    virtual bool OnRecvData(char* pbuffer, int buffer_size, void * session)
    {
        printf("recv data:%s",pbuffer);
    };
    virtual bool OnTimeOut(void * session){};
};

int main()
{
    CStreamAction * action = new CStreamAction;
    if(!action)
    {
        printf("new action is failed\n");
        return -1;
    }

    IStreamServer* svr = UDPServerFactory::GetInstance()->CreateProduct();
    IStreamSession * session = svr->CreateSession("0.0.0.0",10000);
    if (!session)
    {
        printf("create session failed\n");
        return -2;
    }

    session->SetRequestTimeout(30);
    session->SetSessionAction((IStreamAction*)action);
    int ret = svr->StartSession(session);
    if (ret != 0)
    {
        printf("start svr failed");
        return -3;
    }
    printf("udp start succ, port:%d",10000);
    while (1)
    {
        sleep(10);
    }
    return 0;
}
#ifndef I_SKY_STREAM_SERVER_H_
#define I_SKY_STREAM_SERVER_H_
#include "../../../Global/ErrorInfo.h"
#include "ISKYStreamSession.h"


class ISKYStreamServer
{
public:
    /**
    * @brief init error log
    * @param void *,error log object when
    * @return bool result,false and true.
    */
    virtual bool InitError(void *pError) = 0;
    /**
    * @brief init udpserver,this create more udpserver,wait request touch off the udp server
    * @param pUdpInfo: when init, afferent the callback funtion
    * @return bool result,false and true.
    */
    virtual bool InitServer(int nMaxThread) = 0;
    /**
    * @brief close more udpserver,exit loop
    * @param NULL
    * @return bool result,false and true.
    */
    virtual bool CloseServer() = 0;
    /**
    * @brief create request resource
    * @@param UdpServer ip and port
    * @return UdpSession*, false return nullptr.
    */
    virtual ISKYStreamSession* CreateSession(char* ip, int Port) = 0;
    /**
    * @brief destory request resource,notice different to stopSession
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    virtual int  DestorySession(ISKYStreamSession * session) = 0;
    /**
    * @brief start session,do not create resource,only write a event
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    virtual int  StartSession(ISKYStreamSession * session) = 0;
    /**
    * @brief stop session,notice different to DestorySession,is not release resource
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    virtual int  StopSession(ISKYStreamSession * session) = 0;
};


#endif

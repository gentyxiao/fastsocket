#ifndef I_SKY_STREAM_DISPATCH_H_
#define I_SKY_STREAM_DISPATCH_H_
#include "../../../Global/ErrorInfo.h"
#include "ISKYStreamSession.h"


class ISKYStreamDispatch
{
public:
    /**
    * @brief init error log
    * @param void *,error log object when
    * @return bool result,false and true.
    */
    virtual bool InitError(void *pError) = 0;
    /**
    * @brief init server(adj), notice: server(adj) is not meaning server(n)
    * @param int nMaxThread is threads
    * @return bool result,false and true
    */
    virtual bool InitServer(int nMaxThread) = 0;
    /**
    * @brief Close server(adj)
    * @param int nMaxThread is threads
    * @return bool result,false and true
    */
    virtual bool CloseServer() = 0;
    /**
    * @brief create request resource,eg:socket
    * @@param UdpServer ip and port
    * @return ISKYStreamSession*, false return nullptr.
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
    * @brief user for client do not nothing
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
    /**
    * @brief send data to server side
    * @param stream session info,buffer data addr,buffer length
    * @return int, failure return -1,success return 0.
    */
	virtual int  Dispatch(ISKYStreamSession * session, char *StreamData, int StreamSize) = 0;
};


#endif

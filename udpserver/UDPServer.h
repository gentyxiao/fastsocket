#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_

#include "ISKYStreamServer.h"
#include "UDPServerSession.h"
#include <vector>

class CUDPServer : public ISKYStreamServer
{
public:
    CUDPServer();
    ~CUDPServer();
public:
    /**
    * @brief init error log
    * @param void *,error log object when
    * @return bool result,false and true.
    */
    virtual bool InitError(void *pError);
    /**
    * @brief init udpserver,this create more udpserver,wait request touch off the udp server
    * @param pUdpInfo: when init, afferent the callback funtion
    * @return bool result,false and true.
    */
    virtual bool InitServer(int nMaxThread);
    /**
    * @brief close more udpserver,exit loop
    * @param NULL
    * @return bool result,false and true.
    */
    virtual bool CloseServer();
    /**
    * @brief thread load balancing, choose the EventContext object is just like choosing one thread
    * @param NULL
    * @return EventContext*, false return nullptr.
    */
    EventContext* SelectEventContext();
public:
    /**
    * @brief create request resource
    * @@param UdpServer ip and port
    * @return UdpServerSession*, false return nullptr.
    */
    virtual ISKYStreamSession* CreateSession(char* ip, int Port);
    /**
    * @brief destory request resource,notice different to stopSession
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    virtual int  DestorySession(ISKYStreamSession * session);
    /**
    * @brief start session,do not create resource,only write a event
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    virtual int  StartSession(ISKYStreamSession * session);
    /**
    * @brief stop session,notice different to DestorySession,is not release resource
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    virtual int  StopSession(ISKYStreamSession * session);
    /**
    * @brief after startSession success,call this function read data
    * @param session,client use createSession return value
    * @return int, failure return -1,success return 0.
    */
    bool ReadData(int fd, short event, void *arg);
    /**
    * @brief timeout process
    * @param socket fd,event type,arg is context
    * @return int, failure return -1,success return 0.
    */
    bool TimeOut(int fd, short event, void *arg);
private:
    std::vector<EventContext*> event_context_vector_;
    CErrorInfo *m_errorInfo;
};



#endif

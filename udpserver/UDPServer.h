#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_
#include "IStreamServer.h"
#include "UDPServerSession.h"
#include <vector>

class CUDPServer : public IStreamServer
{
public:
    CUDPServer();
    ~CUDPServer();
public:
    virtual bool InitError(void *pError);
    virtual bool InitServer(int nMaxThread);
    virtual bool CloseServer();
    EventContext* SelectEventContext();
public:
    virtual IStreamSession* CreateSession(char* ip, int Port);
    virtual int  DestorySession(IStreamSession * session);
    virtual int  StartSession(IStreamSession * session);
    virtual int  StopSession(IStreamSession * session);
    bool ReadData(int fd, short event, void *arg);
    bool TimeOut(int fd, short event, void *arg);
private:
    std::vector<EventContext*> event_context_vector_;
};

#endif

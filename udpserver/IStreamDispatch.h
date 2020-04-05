#ifndef I_STREAM_DISPATCH_H_
#define I_STREAM_DISPATCH_H_
#include "IStreamSession.h"
class IStreamDispatch
{
public:

    virtual bool InitError(void *pError) = 0;
    virtual bool InitServer(int nMaxThread) = 0;
    virtual bool CloseServer() = 0;
    virtual IStreamSession* CreateSession(char* ip, int Port) = 0;
    virtual int  DestorySession(IStreamSession * session) = 0;
    virtual int  StartSession(IStreamSession * session) = 0;
    virtual int  StopSession(IStreamSession * session) = 0;
	virtual int  Dispatch(IStreamSession * session, char *StreamData, int StreamSize) = 0;
};
#endif

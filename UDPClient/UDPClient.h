#ifndef UDP_CLIENT_H_
#define UDP_CLIENT_H_

#include "../include/ISKYStreamDispatch.h"

#include "event.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/event.h"
#include <arpa/inet.h>
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h> 
#include <pthread.h>
#define MAXSIZE 1500


class UDPClient :public ISKYStreamDispatch
{
public:
	UDPClient();
	~UDPClient();
public:
	virtual bool InitError(void *pError);

	virtual bool InitServer(int nMaxThread);

	virtual bool CloseServer();

	virtual ISKYStreamSession* CreateSession(char* ip, int Port);

	virtual int  DestorySession(ISKYStreamSession * session);

	virtual int  StartSession(ISKYStreamSession * session);

	virtual int  StopSession(ISKYStreamSession * session);


	virtual int  Dispatch(ISKYStreamSession * session, char *StreamData, int StreamSize);
private:
	CErrorInfo *m_errorInfo;
};

#endif
#ifndef UDP_CLIENT_SESSION_H_
#define UDP_CLIENT_SESSION_H_

#include <atomic>
#include "ISKYStreamSession.h"


class UdpClientSession : public ISKYStreamSession
{
public:
	UdpClientSession();
	~UdpClientSession();
public:
	virtual void AddRef();
	virtual void Release();
	virtual void SetSessionAction(ISKYStreamAction *action);

	virtual void  SetRequestSession(int session);
	virtual int   GetRequestSession();
	virtual void  SetResponseSession(void *session);
	virtual void* GetResponseSession();

    virtual const char * GetSrcAddress(){};
    virtual int GetSrcPort(){};
    virtual const char * GetDstAddress(){};
    virtual int GetDstPort(){};

	virtual int SetRequestTimeout(long second) { return 0; }

	static UdpClientSession* CreateSession();
	static void DestorySession(UdpClientSession *session);

	char   src_address[64];
	int    src_port;
	char   dst_address[64];
	int    dst_port;
	int    fd;
	void * response_session;
	ISKYStreamAction * action_stream;
private:
	std::atomic<uint32_t> refcount;
};

#endif


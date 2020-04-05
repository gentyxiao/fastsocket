#ifndef I_STREAM_SESSION_H_
#define I_STREAM_SESSION_H_
/*****************************************************************************
*                     -------
*                    |       |
* --------Request--->|  Udp  |--->Response
*                    |       |
*                     -------
******************************************************************************/
#include <stdint.h>


class IStreamAction
{
public:
    virtual bool OnRecvData(char* pbuffer, int buffer_size, void * session) = 0;
    virtual bool OnTimeOut(void * session) = 0;
};


class IStreamSession
{
public:

    virtual void AddRef() = 0;
    virtual void Release() = 0;
    virtual void SetSessionAction(IStreamAction *action) = 0;
    virtual void  SetRequestSession(int session) = 0;
    virtual int   GetRequestSession() = 0;
    virtual void  SetResponseSession(void *session) = 0;
    virtual void* GetResponseSession() = 0;
    virtual const char * GetSrcAddress() = 0;
    virtual int GetSrcPort() = 0;
    virtual const char * GetDstAddress() = 0;
    virtual int GetDstPort() = 0;
	virtual int SetRequestTimeout(long second) = 0;
};
#endif


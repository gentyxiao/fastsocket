#include "UDPServerSession.h"


UdpServerSession::UdpServerSession()
:ref_count_(0),fd_(-1)
{
  timeout_seconds_ = -1;
}

UdpServerSession::~UdpServerSession()
{
    
}

void UdpServerSession::AddRef()
{
    ref_count_++;
#ifdef DEBUG
    printf("UdpServerSession (%p) AddRef=%d\n", this, (int)ref_count_);
#endif
}

void UdpServerSession::Release()
{
	int refcount = --ref_count_;
#ifdef DEBUG
	printf("UdpServerSession (%p) Release=%d\n", this, (int)refcount);
#endif
    if (refcount<=0){
        DeleteUdpSession(this);
    } 
}

void UdpServerSession::SetSessionAction(IStreamAction *action)
{ 
    callback_fun_ = action;
}

void  UdpServerSession::SetRequestSession(int session)
{ 
    fd_ = session;
}

int UdpServerSession::UdpServerSession::GetRequestSession()
{ 
    return fd_; 
}

void UdpServerSession::SetResponseSession(void *session)
{
    application_us_ = session;
}

void* UdpServerSession::GetResponseSession()
{ 
    return application_us_;
}

int UdpServerSession::SetRequestTimeout(long second)
{ 
	timeout_seconds_ = second;
}

long UdpServerSession::GetRequestTimeOut()
{ 
	return timeout_seconds_;
}

UdpServerSession* UdpServerSession::CreateUdpSession()
{
    UdpServerSession *us = new UdpServerSession();
    if (us != NULL){
        return us;
    }
    return NULL;
}

void UdpServerSession::DeleteUdpSession(UdpServerSession *us)
{
    if (us != NULL){
        delete us;
        us = NULL;
    }
}
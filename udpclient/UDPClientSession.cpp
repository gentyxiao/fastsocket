#include "UDPClientSession.h"


UdpClientSession::UdpClientSession()
:refcount(0),fd(-1)
{

}


UdpClientSession::~UdpClientSession()
{
}

void UdpClientSession::AddRef()
{
	refcount++;
}

void UdpClientSession::Release()
{
	refcount--;
	if (refcount == 0)
	{
		UdpClientSession::DestorySession(this);
	}
}

void UdpClientSession::SetSessionAction(ISKYStreamAction *action)
{
	action_stream = action;
}

void  UdpClientSession::SetRequestSession(int session)
{
	fd = session;
}

int UdpClientSession::UdpClientSession::GetRequestSession()
{
	return fd;
}

void UdpClientSession::SetResponseSession(void *session)
{
	response_session = session;
}

void* UdpClientSession::GetResponseSession()
{
	return response_session;
}

UdpClientSession* UdpClientSession::CreateSession()
{
	UdpClientSession *session = new UdpClientSession;

	return session;
}

void UdpClientSession::DestorySession(UdpClientSession *session)
{
	if (session)
	{
		delete session;
	}
}
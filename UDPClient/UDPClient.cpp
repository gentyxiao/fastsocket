#include "UDPClient.h"
#include "UDPClientSession.h"

#include <unistd.h>

UDPClient::UDPClient()
{
}


UDPClient::~UDPClient()
{
}

bool UDPClient::InitError(void *pError)
{
	m_errorInfo = (CErrorInfo*)pError;

	return true;
}

bool UDPClient::InitServer(int nMaxThread)
{

	return true;
}

bool UDPClient::CloseServer()
{

	return true;
}

ISKYStreamSession* UDPClient::CreateSession(char* ip, int Port)
{
	UdpClientSession* pSession = UdpClientSession::CreateSession();
	if (pSession == NULL) return NULL;

	strncpy(pSession->dst_address, ip, sizeof(pSession->dst_address));
	pSession->dst_port = Port;

	int socket_fd;
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	pSession->SetRequestSession(socket_fd);

	pSession->AddRef();

	return pSession;
}

int  UDPClient::DestorySession(ISKYStreamSession * session)
{
	if (session == NULL) return 0;
	UdpClientSession *pServiceSession = (UdpClientSession *)session;

	int fd = pServiceSession->fd;
	pServiceSession->fd = 0;
	close(fd);

	session->Release();

	return 1;
}

int  UDPClient::StartSession(ISKYStreamSession * session)
{

	return 1;
}

int  UDPClient::StopSession(ISKYStreamSession * session)
{

	return 1;
}


int  UDPClient::Dispatch(ISKYStreamSession * session, char *StreamData, int StreamSize)
{
	UdpClientSession *pServiceSession = (UdpClientSession *)session;

	struct sockaddr_in sendaddr = { 0 };
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_port = htons(pServiceSession->dst_port);
	sendaddr.sin_addr.s_addr = inet_addr(pServiceSession->dst_address);
	socklen_t len = sizeof(struct sockaddr_in);


	int ret = 0;
	ret = sendto(pServiceSession->fd, StreamData, StreamSize, MSG_DONTWAIT, (struct sockaddr *) &sendaddr, sizeof(struct sockaddr_in));

	if (ret < 0)
	{
		//perror("ret = sendto() == 0\n");
		return false;
	}
	if (ret < StreamSize)
	{
		//perror("sendto error\n");
		//m_errorInfo->Error((char*)"sendto error\n");
		return false;
	}
	
	return true;
}




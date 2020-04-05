#include "UDPServer.h"
#include "IStreamSession.h"
#include "UDPServerSession.h"
#include <list>
#include <vector>
#include <event.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

#define MAXSIZE 1500

enum TaskType{
    THREAD_TASK_ADD = (1<<0),
    THREAD_TASK_DEL = (1<<1)
};

void *lanuch_event(void *arg)
{
	struct event_base * pEventBase = (struct event_base *)arg;
	event_base_dispatch(pEventBase);
}

static void read_data_static(int  fd, short event, void *arg)
{
    UdpServerSession *us = (UdpServerSession*)arg;
    //Prevent the callback function to delete session
    us->AddRef();
    EventInfo *event_info = (EventInfo*)(us->GetEventInfo());
    if(EV_TIMEOUT == event){
        event_info->udpserver_->TimeOut(fd, event, us);
    }else if (EV_READ == event){
        event_info->udpserver_->ReadData(fd, event, us);
    }  
    us->Release();
}

static void write_data_static(int fd ,short event, void *arg)
{

}

static void time_out_static(int  fd, short event, void *arg)
{
	CUDPServer *server = (CUDPServer*)arg;
	server->TimeOut(fd , event, server);
}

static int add_event(IStreamSession *udp_session)
{
    UdpServerSession *us = (UdpServerSession*)(udp_session);
	if (us->fd_ == -1) return -1;

    EventItem *pevent_item = (EventItem*)(us->GetEventItem());
    EventInfo *pevent_info = (EventInfo*)(us->GetEventInfo());
    struct event *plistener_event = event_new(pevent_item->event_content_->event_base_, (evutil_socket_t)(us->fd_), EV_READ | EV_PERSIST, read_data_static, us);

	long  timeout_second = us->GetRequestTimeOut();
	if (timeout_second <= 0){
		event_add(plistener_event, NULL);
	}else{
		struct timeval timeout = { timeout_second, 0 }; // 设置event超时为40秒,40秒内没有数据，调用超时
		event_add(plistener_event, &timeout);
	}
    pevent_info->listener_event_ = plistener_event;
	return 1;
}

static int delete_event(IStreamSession *udp_session)
{
    UdpServerSession *us = (UdpServerSession*)(udp_session);
    EventInfo *pevent_info = us->GetEventInfo();
    if (pevent_info->listener_event_ != nullptr){
        event_del(pevent_info->listener_event_);
        event_free(pevent_info->listener_event_);
        pevent_info->listener_event_ = nullptr;
	}
	return 1;
}

static void lanuch_server(int fd, short event, void *arg)
{
	int  ret = 0;
    char buf[100] = {0};
	
    std::list<UdpServerSession*>::iterator iter;
	EventContext *pevent_context = (EventContext*)arg;
	switch (event){
	    case EV_READ:
            ret = read(pevent_context->pipe_read_, buf, 100);
            if (ret > 0){
                pevent_context->list_lock_.lock();
                while (1){
                    UdpServerSession *pSession = NULL;
                    if (pevent_context->event_list_.size() == 0){
                        break;
                    }
                    iter = pevent_context->event_list_.begin();
                    pSession = *iter;
                    pevent_context->event_list_.erase(iter);

                    if (pSession == NULL){
                        continue;
                    }
                    EventItem * pevent_item = pSession->GetEventItem();
                    if (pevent_item->event_state_ & THREAD_TASK_ADD){
                        pevent_item->event_state_ &= ~THREAD_TASK_ADD;
                        add_event(pSession);

                        pevent_context->event_size_++;

                    }else if (pevent_item->event_state_ & THREAD_TASK_DEL){
                        pevent_item->event_state_ &= ~THREAD_TASK_DEL;
                        delete_event(pSession);

                        pevent_context->event_size_--;

                        pSession->Release();
                    }
                }
                pevent_context->list_lock_.unlock();
            }
            break;
	}
}

CUDPServer::CUDPServer()
{
  
}

CUDPServer::~CUDPServer()
{

}

bool CUDPServer::InitError(void *pError)
{
}

bool CUDPServer::InitServer(int nMaxThread)
{
    int max_threads = 0;
    if (nMaxThread <= 0){
        max_threads = 1;
    }
	else{
		max_threads = nMaxThread;
	}
    struct event *listener_event;
    pthread_t tid;
	//evthread_use_pthreads();
	//event_enable_debug_mode();
    for (int cout = 0; cout < nMaxThread; cout++){
		int arr[2] = {0};
        EventContext *event_cont = new EventContext;
		if (pipe(arr) != 0) {
            printf("pipe create,write fd(%d),read fd(%d),errno(%d)\n",arr[0],arr[1],errno);
			return false;
		}
        event_cont->pipe_read_ = arr[0];
        event_cont->pipe_write_ = arr[1];
		
		struct event_base *pevent_base = event_base_new();
        if (!pevent_base){
			return false;
		}

        listener_event = event_new(pevent_base, event_cont->pipe_read_, EV_READ | EV_PERSIST, lanuch_server, event_cont);
		event_add(listener_event, NULL);
		// thread create
        if (pthread_create(&tid, NULL, lanuch_event, pevent_base) != 0){
			return false;
		}
	
        event_cont->event_base_ = pevent_base;
        event_cont->listener_event_ = listener_event;
        event_context_vector_.push_back(event_cont);
	}
    return true;
}

bool CUDPServer::CloseServer()
{
    int ret;
    std::vector<EventContext*>::iterator iter = event_context_vector_.begin();
    for (; iter != event_context_vector_.end(); ++iter){
        ret = event_base_loopexit((*iter)->event_base_, NULL);
        if (-1 == ret){
        }
        event_base_free((*iter)->event_base_);
        close((*iter)->pipe_read_);
        close((*iter)->pipe_write_);
        event_del((*iter)->listener_event_);
        event_free((*iter)->listener_event_);
    }
    event_context_vector_.clear();
    return true;
}

EventContext* CUDPServer::SelectEventContext()
{
    EventContext *pevent_context = NULL;
    if (event_context_vector_.size() == 0){
        return nullptr;
    }

    std::vector<EventContext*>::iterator it = event_context_vector_.begin();
    /*select the least task loaded thread*/
    pevent_context = *(it++);
    for (; it != event_context_vector_.end(); ++it){
        if (pevent_context->event_size_ > (*it)->event_size_){
            pevent_context = *it;
        }
    }
    return pevent_context;
}

IStreamSession* CUDPServer::CreateSession(char* ip, int Port)
{
    int fd;
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(Port);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0){
        return nullptr;
    }

	int success = 0;
	success = evutil_make_socket_nonblocking(fd);
	if (success != 0)
	{
		return nullptr;
	}
	int sendBufferSize = 512 * 1024;
	success = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char *)&sendBufferSize, sizeof(sendBufferSize));

	int recvBufferSize = 512 * 1024;
	success = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char *)&recvBufferSize, sizeof(recvBufferSize));

    int sock_opt = 1;
    if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(sock_opt))){
    }
    
    if (bind(fd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
        return nullptr;
    }
    IStreamSession *udp_session = UdpServerSession::CreateUdpSession();
    udp_session->SetRequestSession(fd);
    EventInfo *peventinfo = ((UdpServerSession*)udp_session)->GetEventInfo();
    peventinfo->udpserver_ = this;
    peventinfo->us_ = udp_session;
	peventinfo->listener_event_ = NULL;

    /*select the least loaded thread*/
    EventContext *pevent_context = SelectEventContext();
    EventItem *pevent_item = ((UdpServerSession*)udp_session)->GetEventItem();
    pevent_item->event_content_ = pevent_context;
    pevent_item->event_state_ = 0;
    pevent_item->event_info_ = peventinfo;
    
    udp_session->AddRef();
    return udp_session;
}

int CUDPServer::DestorySession(IStreamSession *session)
{
    if (NULL == session){
        return -1;
    }
    
    UdpServerSession *us = (UdpServerSession*)session;
    EventItem *pevitem = us->GetEventItem();
    if (us->fd_ != -1){
        close(us->fd_);
        us->fd_ = -1;
    }
    session->Release();
    return 0;
}

int CUDPServer::StartSession(IStreamSession * session)
{
    if (nullptr == session){
        return -1;
    }

    /*notify the thread starts work,use for pipe communication*/
    EventItem *pevent_item = ((UdpServerSession*)session)->GetEventItem();
    EventContext *pevent_context = ((UdpServerSession*)session)->GetEventItem()->event_content_;
  
    session->AddRef();
    pevent_context->list_lock_.lock();
    pevent_item->event_state_ = THREAD_TASK_ADD;
    pevent_context->event_list_.push_back((UdpServerSession*)session);
    int ret;
    char buf[2] = "1";
    ret = write(pevent_context->pipe_write_, buf, 1);
    pevent_context->list_lock_.unlock();

    return 0;
}

int CUDPServer::StopSession(IStreamSession * session)
{
    if (NULL == session){
        return -1;
    }
    EventItem *pevitem = ((UdpServerSession*)session)->GetEventItem();
    EventContext *pcontext = ((UdpServerSession*)session)->GetEventItem()->event_content_;
 
    pcontext->list_lock_.lock();
    pevitem->event_state_ |= THREAD_TASK_DEL;
    pcontext->event_list_.push_back((UdpServerSession*)session);
    char buf[2] = "1";
	int ret;
    ret = write(pcontext->pipe_write_, buf, 1);
    pcontext->list_lock_.unlock();

    return 0;
}

bool CUDPServer::ReadData(int fd, short event, void *arg)
{
    if (fd == -1){
        return false;
    }
	UdpServerSession  *us = (UdpServerSession*)(arg);
	struct sockaddr_in cliaddr = { 0 };
    char buffer[MAXSIZE];

	do 
	{
		socklen_t len = sizeof(struct sockaddr_in);
		int ret = recvfrom(fd, buffer, MAXSIZE, MSG_DONTWAIT, (struct sockaddr *) &cliaddr, &len);
		if (ret <= 0)
		{
			break;
		}
		us->callback_fun_->OnRecvData(buffer, ret, us);
	} while (1);
    return true;
}

bool CUDPServer::TimeOut(int fd, short event,void *arg)
{
	if (fd == -1){
		return false;
	}
    UdpServerSession *us = (UdpServerSession*)(arg);

	if (EV_TIMEOUT == event){
		delete_event(us);
        us->callback_fun_->OnTimeOut(us);
	}
	return true;
}

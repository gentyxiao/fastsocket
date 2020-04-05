#ifndef UDP_SESSION_H_
#define UDP_SESSION_H_
#include "IStreamSession.h"
#include <string.h>
#include <atomic>
#include <mutex>
#include <list>

class CUDPServer;
class UdpServerSession;
struct EventItem;
/*与线程绑定，与线程为对应关系*/
class EventContext
{
public:
    int pipe_write_;
    int pipe_read_;
    int event_size_;                   /*描述该线程中负载的任务数，EventContext对象与线程绑定*/
    std::mutex list_lock_;
    std::list<UdpServerSession*> event_list_;
    struct event *listener_event_;
    struct event_base* event_base_;
};

struct EventInfo
{
	EventInfo(){
		memset(this, 0x00, sizeof(struct EventInfo));
	}

    CUDPServer *udpserver_;             /*udpserver启动之后该对象的值不变*/
    void *us_;                          /*每次请求都会创建udpsession*/
    struct event *listener_event_;      /*每次请求都会创建监听事件，监听读事件*/
};

/*线程上的任务*/
struct EventItem
{
    EventItem(){
        memset(this, 0x00, sizeof(struct EventItem));
    }
    int  event_state_;                  /*事件类型*/
    EventContext *event_content_;       /*记录该请求所在线程*/
    EventInfo *event_info_;             /*每次请求调用event_new接口创建的事件信息*/
};

class UdpServerSession : public IStreamSession
{
public:
    UdpServerSession();
    ~UdpServerSession();
public:
    virtual void AddRef();
    virtual void Release();
    //set callback function
    virtual void SetSessionAction(IStreamAction *action);
    virtual void  SetRequestSession(int session);
    virtual int   GetRequestSession();
    virtual void  SetResponseSession(void *session);
    virtual void* GetResponseSession();
    virtual int SetRequestTimeout(long second);
    virtual const char * GetSrcAddress(){};
    virtual int GetSrcPort(){};
    virtual const char * GetDstAddress(){};
    virtual int GetDstPort(){};

	long GetRequestTimeOut();

    //get event item
    EventItem* GetEventItem(){ return &event_item_; }
    //get event info
    EventInfo* GetEventInfo(){ return &event_info_; }
    //create udpSession
    static UdpServerSession* CreateUdpSession();
    //delete udpSession
    void DeleteUdpSession(UdpServerSession *us);
    IStreamAction *callback_fun_;
    int fd_;
public:
	long timeout_seconds_;
    void *application_us_;
    std::atomic<int> ref_count_;
    struct EventItem event_item_;
    struct EventInfo event_info_;
};

#endif


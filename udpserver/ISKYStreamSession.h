#ifndef I_SKY_STREAM_SESSION_H_
#define I_SKY_STREAM_SESSION_H_
/*****************************************************************************
* \File: UdpServerSession.h
* \Date: 2016/05/04
* \Copyright: (C) 1990-2016 sky-light.com.hk
* \Author: liaozijun
* \Brief:
* \Version: 1.0.0.0
* \Note:
******************************************************************************
*                     -------
*                    |       |
* --------Request--->|  Udp  |--->Response
*                    |       |
*                     -------
******************************************************************************/
#include <stdint.h>


class ISKYStreamAction
{
public:
    /**
    * @brief callback function,use for recv data from read event
    * @param pbuffer is buffer addr,buffer_size is buffer measurement,session is context
    * @return bool return,false and true.
    */
    virtual bool OnRecvData(char* pbuffer, int buffer_size, void * session) = 0;
    /**
    * @brief callback function,use for timeout doing not recv data
    * @param pbuffer is buffer addr,buffer_size is buffer measurement,session is context
    * @return bool return,false and true.
    */
    virtual bool OnTimeOut(void * session) = 0;
};


class ISKYStreamSession
{
public:
    /**
    * @brief Reference count,increase once
    * @param NULL
    * @return void NULL
    */
    virtual void AddRef() = 0;
    /**
    * @brief Reference count,decrease once
    * @param NULL
    * @return void NULL
    */
    virtual void Release() = 0;
    /**
    * @brief save callbackfuntion for session subclass
    * @param class ISKYStreamAction object pointer
    * @return void NULL
    */
    virtual void SetSessionAction(ISKYStreamAction *action) = 0;
    /**
    * @brief save socket fd, correspondence to create session subclass
    * @param socket fd
    * @return void NULL
    */
    virtual void  SetRequestSession(int session) = 0;
    /**
    * @brief get socket fd, correspondence to create session subclass
    * @param NULL
    * @return int
    */
    virtual int   GetRequestSession() = 0;
    /**
    * @brief application layer save object, for callback data to app alyer and timely send
    * @param application layer object pointer
    * @return void NULL
    */
    virtual void  SetResponseSession(void *session) = 0;
    /**
    * @brief get application layer object pointer
    * @param null
    * @return void* application layer object pointer
    */
    virtual void* GetResponseSession() = 0;
    /**
    * @brief get src side ip addr
    * @param null
    * @return const char*
    */
    virtual const char * GetSrcAddress() = 0;
    /**
    * @brief get src side port
    * @param null
    * @return int
    */
    virtual int GetSrcPort() = 0;
    /**
    * @brief get dst side ip addr
    * @param null
    * @return const char*
    */
    virtual const char * GetDstAddress() = 0;
    /**
    * @brief get dts side port
    * @param null
    * @return int
    */
    virtual int GetDstPort() = 0;
	/**
    * @brief set epoll timeout,unit is second
    * @param unsigend int
    * @return null
    */
	virtual int SetRequestTimeout(long second) = 0;
};

#endif


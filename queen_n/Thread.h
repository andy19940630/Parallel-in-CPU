#ifndef __THREADPOOL_H__   /*Thread.h*/
#define __THREADPOOL_H__
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <sys/types.h>

#include <deque>
#include <vector>
typedef void (*CallBack)(void *);//rename the void pointer to ""
struct Task
{
    CallBack func;//void pointer1
    void* arg;//void pointer2
};

class ThreadPool
{
public:
    ThreadPool(int size);
    ~ThreadPool();//destructor
    void addTask(CallBack f,void* arg);//add threads to Task
private:
    static void* _worker(void* arg);
    bool _stopped;//bool parameter to show whether the thread has been stopped
    int  _size;
    pthread_cond_t _cond;//thread condition variable
    pthread_mutex_t _mutex;//mutex
    std::deque<Task> _tasks;//which task stores threads applying by user
    std::vector<pthread_t> _threadVec;
};
#endif

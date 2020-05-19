#ifndef __THREADPOOL_H__   /*ThreadPool.h*/
#define __THREADPOOL_H__
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <sys/types.h>

#include <deque>
#include <vector>
typedef void (*CallBack)(void *);
struct Task
{
    CallBack func;
    void* arg;
};

class ThreadPool
{
public:
    ThreadPool(int size);
    ~ThreadPool();
    //void start();
    void addTask(CallBack f,void* arg);
private:
    static void* _worker(void* arg);
    bool _stopped;
    int  _size;
    pthread_cond_t _cond;
    pthread_mutex_t _mutex;
    std::deque<Task> _tasks;
    std::vector<pthread_t> _threadVec;
};
#endif

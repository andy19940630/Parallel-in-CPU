#include "ThreadPool.h" /*ThreadPool.cpp*/
void* ThreadPool::_worker(void* arg)
{
    ThreadPool *obj = (ThreadPool*)arg;
    pid_t tid = syscall(SYS_gettid);
    printf("_worker ar %d\n",tid);
    while(1)
    {
        pthread_mutex_lock(&obj->_mutex);
        while(obj->_tasks.empty() && !obj->_stopped)
        {
            pthread_cond_wait(&obj->_cond,&obj->_mutex);
            printf("%d get signal\n",tid);
        }
        Task t;
        if(!obj->_tasks.empty())
        {
            printf("%d get a task\n",tid);
            t = obj->_tasks.front();
            obj->_tasks.pop_front();
            pthread_mutex_unlock(&obj->_mutex);
            t.func(t.arg);
            continue;
        }
        break;
    }
    pthread_mutex_unlock(&obj->_mutex);
    return (void*)0;

}
ThreadPool::ThreadPool(int size)
{
    pthread_t tid;
    _size = size;
    _stopped = 0;
    _cond = PTHREAD_COND_INITIALIZER;
    _mutex = PTHREAD_MUTEX_INITIALIZER;
    for(int i = 0;i < size;i++)
    {
        pthread_create(&tid,NULL,ThreadPool::_worker,this);
        _threadVec.push_back(tid);
    }
}
ThreadPool::~ThreadPool()
{

    pthread_mutex_lock(&_mutex);
    _stopped = 1;
    printf("threadpool stopped\n");
    pthread_mutex_unlock(&_mutex);
    int threadSize = _threadVec.size();
    for(int i = 0; i < threadSize; i++)
    {
        pthread_join(_threadVec[i],NULL);
        pthread_cond_broadcast(&_cond);
    }
    printf("all threads exit\n");
}

void ThreadPool::addTask(CallBack f,void* arg)
{
    Task t = {f,arg};
    pthread_mutex_lock(&_mutex);
    _tasks.push_back(t);
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
}

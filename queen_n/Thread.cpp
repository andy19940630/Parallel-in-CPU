#include "Thread.h" /*Thread.cpp*/
void* ThreadPool::_worker(void* arg)
{
    ThreadPool *obj = (ThreadPool*)arg;
    pid_t tid = syscall(SYS_gettid);//get the int type thread id by sys_gettid
    printf("_worker is %d\n",tid);//the first thread's id
    while(1)
    {
        pthread_mutex_lock(&obj->_mutex);//lock mutex
        while(obj->_tasks.empty() && !obj->_stopped)//if queue is empty and thread is not stopped 
        {
	    pthread_cond_wait(&obj->_cond,&obj->_mutex);//use mutex(each thread owns a mutex) after the result of condition 
	    /* printf("%d get signal\n",tid);*/
        }
        Task t;// getintermediate variable for thread of queue
        if(!obj->_tasks.empty())//if there are threads in queue
        {
	  /* printf("%d get a thread\n",tid);*/
	    t = obj->_tasks.front();
            obj->_tasks.pop_front();//pop
            pthread_mutex_unlock(&obj->_mutex);//unlock
            t.func(t.arg);//initialization
            continue;
        }
        break;//get out the loop
    }
    pthread_mutex_unlock(&obj->_mutex);//unclock
    return (void*)0;//convert the 0 to void pointer, it is just for convenience to return the void pointer

}
ThreadPool::ThreadPool(int size)//size is the number of thread applied by user
{
    pthread_t tid;
    _size = size;
    _stopped = 0;
    _cond = PTHREAD_COND_INITIALIZER;
    _mutex = PTHREAD_MUTEX_INITIALIZER;
    for(int i = 0;i < size;i++)
    {
      pthread_create(&tid,NULL,ThreadPool::_worker,this);//call the method named_worker of ThreadPool to create the real thread
      _threadVec.push_back(tid);//add its tid to vector,useless 
    }
}
ThreadPool::~ThreadPool()
{

    pthread_mutex_lock(&_mutex);
    _stopped = 1;//it means that the thread is stoped
    /* printf("threadpool stopped\n");*/
    pthread_mutex_unlock(&_mutex);
    int threadSize = _threadVec.size();
    for(int i = 0; i < threadSize; i++)
    {
        pthread_join(_threadVec[i],NULL);
        pthread_cond_broadcast(&_cond);
    }
    /*printf("all threads exit\n");*/
}

void ThreadPool::addTask(CallBack f,void* arg)
{
    Task t = {f,arg};//initialization
    pthread_mutex_lock(&_mutex);//lock its mutex
    _tasks.push_back(t);
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
}

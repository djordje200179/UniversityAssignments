#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.h"

void* operator new(size_t bytes);
void operator delete(void* ptr);

class Thread {
public:
    Thread(void (*body)(void*), void* arg) { thread_init(&myHandle,body,arg); }
    virtual ~Thread() = default;
    void start() { thread_start(myHandle); }
    static void dispatch() { thread_dispatch(); }
    static void sleep(time_t ticks) { time_sleep(ticks); }
protected:
    Thread() { thread_init(&myHandle,wrapper,this); }
    virtual void run() {}
private:
    thread_t myHandle;

	static void wrapper(void* arg) { ((Thread*)arg)->run(); }
};

class Semaphore {
public:
    Semaphore(unsigned init = 1) { sem_open(&myHandle, init); }
    virtual ~Semaphore() { sem_close(myHandle); }
    void wait() { sem_wait(myHandle); }
    void signal() { sem_signal(myHandle); }
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation() {}
private:
	static void wrapper(void* arg);
};

class Console {
public:
    static char getc() { return ::getc();}
    static void putc(char c) { ::putc(c); }
};

#endif // _syscall_cpp
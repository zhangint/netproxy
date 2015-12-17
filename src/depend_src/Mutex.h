#ifndef __CMutex_H
#define __CMutex_H



#include <pthread.h>

class CMutex
{
public:

    CMutex();

    ~CMutex();

    void lock_mutex();

    void unlock_mutex();

    int trylock_mutex();

private:

    pthread_mutex_t* _lock;

} ;


inline CMutex::CMutex()
{
    _lock = new pthread_mutex_t;

    pthread_mutex_init(_lock, NULL);
}

inline CMutex::~CMutex()
{
    pthread_mutex_destroy(_lock);

    delete _lock;
}

inline void CMutex::lock_mutex()
{
    pthread_mutex_lock(_lock);
}

inline  void CMutex::unlock_mutex()
{
    pthread_mutex_unlock(_lock);
}

inline int CMutex::trylock_mutex()
{
    return pthread_mutex_trylock(_lock);
}


#endif












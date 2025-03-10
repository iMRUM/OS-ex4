#ifndef ASingleton_H
#define ASingleton_H
#include "ScopeMutex.h"
#include <pthread.h>

template <typename T>
class ASingleton {
protected:
    static pthread_mutex_t mutex;
    static T* instance;

    ASingleton() {}
    virtual ~ASingleton() {}

public:
    ASingleton(const ASingleton&) = delete;
    ASingleton& operator=(const ASingleton&) = delete;

    static T* getInstance() {
        ScopeMutex guard(mutex);

        if (instance == nullptr) {
            instance = new T();
        }

        return instance;

    }
};

template <typename T>
pthread_mutex_t ASingleton<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

template <typename T>
T* ASingleton<T>::instance = nullptr;

#endif //ASingleton_H
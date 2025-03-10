#ifndef ASingleton_H
#define ASingleton_H

#include <pthread.h>

/**
 * @brief Thread-safe Singleton template class using pthread mutex
 *
 * This class implements the Singleton design pattern ensuring thread-safety
 * through mutex locking. It provides controlled access to a single instance
 * of the templated class throughout the application.
 *
 * @tparam T The class type to be instantiated as a singleton
 */
template <typename T>
class ASingletonDCLO {
protected:
    /**
     * @brief Mutex for thread synchronization
     */
    static pthread_mutex_t mutex;

    /**
     * @brief Pointer to the singleton instance
     */
    static T* instance;

    /**
     * @brief Protected constructor to prevent instantiation
     */
    ASingletonDCLO() {}

    /**
     * @brief Virtual destructor for proper cleanup in derived classes
     */
    virtual ~ASingletonDCLO() {}

public:
    /**
     * @brief Deleted copy constructor to prevent duplication
     */
    ASingletonDCLO(const ASingletonDCLO&) = delete;

    /**
     * @brief Deleted assignment operator to prevent duplication
     */
    ASingletonDCLO& operator=(const ASingletonDCLO&) = delete;

    /**
     * @brief Gets the singleton instance with thread safety
     *
     * If the instance doesn't exist, it creates one.
     * The mutex ensures that only one thread can create the instance.
     *
     * @return T* Pointer to the singleton instance
     */
    static T* getInstance() {
        pthread_mutex_lock(&mutex);
        if (instance == nullptr) {
            instance = new T();
        }
        pthread_mutex_unlock(&mutex);
        return instance;
    }
};

template <typename T>
pthread_mutex_t ASingletonDCLO<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

template <typename T>
T* ASingletonDCLO<T>::instance = nullptr;

#endif //ASingleton_H
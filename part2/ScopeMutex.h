#ifndef SCOPE_MUTEX_H
#define SCOPE_MUTEX_H
#include <pthread.h>

/**
 * @brief A RAII class for automatic locking and unlocking of pthread mutexes
 *
 * This class implements the scope mutex (guard) pattern for pthread mutexes.
 * It locks a mutex when constructed and automatically unlocks it when
 * destroyed, ensuring proper mutex handling even in the presence of exceptions.
 */
class ScopeMutex {
private:
    /** @brief Reference to the mutex being guarded */
    pthread_mutex_t& mutex;

    /**
     * @brief Copy constructor (deleted)
     * @param other Another scope mutex
     */
    ScopeMutex(const ScopeMutex&) = delete;

    /**
     * @brief Assignment operator (deleted)
     * @param other Another scope mutex
     * @return Reference to this object
     */
    ScopeMutex& operator=(const ScopeMutex&) = delete;

public:
    /**
     * @brief Constructs a scope mutex (guard) and locks the mutex
     * @param m Reference to an initialized pthread_mutex_t
     */
    explicit ScopeMutex(pthread_mutex_t& m) : mutex(m) {
        pthread_mutex_lock(&mutex);
    }

    /**
     * @brief Destructor that unlocks the mutex
     *
     * This automatically releases the mutex when the guard goes out of scope
     */
    ~ScopeMutex() {
        pthread_mutex_unlock(&mutex);
    }
};

#endif //SCOPE_MUTEX_H
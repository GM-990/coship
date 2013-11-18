/**@defgroup pthread pthread 提供pthread库的接口
@brief 提供pthread库的接口
@version 0.1 2008/10/28 演示版本
@{
*/
#ifndef _UDI_PTHREAD_H
#define _UDI_PTHREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MMCP_LOCKCHECK_SUPPORT

#ifndef _PTHREAD_H
#define _PTHREAD_H

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


enum
{
  PTHREAD_SCOPE_SYSTEM,
#define PTHREAD_SCOPE_SYSTEM	PTHREAD_SCOPE_SYSTEM
  PTHREAD_SCOPE_PROCESS
#define PTHREAD_SCOPE_PROCESS	PTHREAD_SCOPE_PROCESS
};

enum
{
  PTHREAD_INHERIT_SCHED,
#define PTHREAD_INHERIT_SCHED	PTHREAD_INHERIT_SCHED
  PTHREAD_EXPLICIT_SCHED
#define PTHREAD_EXPLICIT_SCHED	PTHREAD_EXPLICIT_SCHED
};

enum
{
  PTHREAD_CREATE_JOINABLE,
#define PTHREAD_CREATE_JOINABLE	PTHREAD_CREATE_JOINABLE
  PTHREAD_CREATE_DETACHED
#define PTHREAD_CREATE_DETACHED	PTHREAD_CREATE_DETACHED
};

enum
{
  PTHREAD_MUTEX_ADAPTIVE_NP,
  PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_TIMED_NP
#ifdef __USE_UNIX98
  ,
  PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_ADAPTIVE_NP,
  PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
#endif
#ifdef __USE_GNU
  /* For compatibility.  */
  , PTHREAD_MUTEX_FAST_NP = PTHREAD_MUTEX_ADAPTIVE_NP
#endif
};


#define PTHREAD_PRIO_INHERIT (4)
#define PTHREAD_PRIO_NONE  (5)


#define PTHREAD_CANCEL_ENABLE  0
#define PTHREAD_CANCEL_DISABLE 1
#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 1


/* Initializers.  */

/* Macros for lock initializers, using the above definition. */
#define __LT_SPINLOCK_INIT 0
#define __LOCK_INITIALIZER { 0, __LT_SPINLOCK_INIT }
#define __ALT_LOCK_INITIALIZER { 0, __LT_SPINLOCK_INIT }
#define __ATOMIC_INITIALIZER { 0, __LT_SPINLOCK_INIT }


#define PTHREAD_MUTEX_INITIALIZER 	{0, 0, 0, PTHREAD_MUTEX_ADAPTIVE_NP, __LOCK_INITIALIZER}

#define PTHREAD_COND_INITIALIZER {__LOCK_INITIALIZER, 0}


#define PTHREAD_ONCE_INIT ((pthread_once_t) 0xFFFFFFFF)
#endif
#define MMCP_DEADLINE_SUPPORT
#ifdef MMCP_DEADLINE_SUPPORT

#include "cs_lockcheck.h"

#define pthread_mutex_init(mutex, attr)	(dl_pthread_mutex_init (__FILE__,__LINE__,  mutex,  attr))

#define pthread_mutex_destroy(mutex)	(dl_pthread_mutex_destroy (__FILE__,__LINE__, mutex))

#define pthread_mutex_trylock(mutex)	(dl_pthread_mutex_trylock( __FILE__,__LINE__, mutex))

#define pthread_mutex_lock(mutex)	(dl_pthread_mutex_lock( __FILE__,__LINE__, mutex))

#define pthread_mutex_unlock(mutex)	(dl_pthread_mutex_unlock( __FILE__,__LINE__, mutex))

#define pthread_cond_init( cond, attr)	(dl_pthread_cond_init( __FILE__,__LINE__,cond, attr))

#define pthread_cond_destroy( cond)	(dl_pthread_cond_destroy ( __FILE__,__LINE__, cond))

#define pthread_cond_signal( cond)	(dl_pthread_cond_signal ( __FILE__,__LINE__, cond))

#define pthread_cond_broadcast( cond)	(dl_pthread_cond_broadcast ( __FILE__,__LINE__, cond))

#define pthread_cond_wait( cond, mutex)	(dl_pthread_cond_wait (__FILE__,__LINE__, cond,  mutex))

#define pthread_cond_timedwait(cond, mutex, abstime)	(dl_pthread_cond_timedwait (__FILE__,__LINE__, cond,  mutex, abstime))

#else

/**
@brief Initializes a mutex and sets its attributes.
@param[in] attr	Specifes the mutex attributes object to use for initializing the mutex. If the value is NULL, the default attributes values are used.
@param[out] mutex	Specifies the mutex to be created.
@return Upon successful completion, the new mutex is returned via the mutex parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutex_init (pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);

/**
@brief Deletes a mutex.
@param[in] mutex	Specifies the mutex to delete.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/

int pthread_mutex_destroy (pthread_mutex_t* mutex);

/**
@brief The function pthread_mutex_trylock is identical to pthread_mutex_lock except that if the mutex object referenced by mutex is currently locked (by any thread, including the current thread), the call returns immediately.
@param[in] mutex Specifies the mutex to lock.	
@return The function pthread_mutex_trylock returns zero if a lock on the mutex object referenced by mutex is acquired. Otherwise, an error number is returned to indicate the error.
*/
int pthread_mutex_trylock(pthread_mutex_t* mutex);

/**
@brief Locks a mutex.
@param[in] mutex Specifies the mutex to lock.
@return If successful, the pthread_mutex_lock functions return zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_mutex_lock(pthread_mutex_t* mutex);

/**
@brief unlocks a mutex.
@param[in] mutex Specifies the mutex to unlock.
@return If successful, the pthread_mutex_unlock functions return zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_mutex_unlock(pthread_mutex_t* mutex);

/**
@brief Initializes a condition variable and sets its attributes.
@param[in] attr	Specifies the condition attributes object to use for initializing the condition variable. If the value is NULL, the default attributes values are used.
@param[out] cond	Specifies the condition to be created.
@return Upon successful completion, the new condition variable is returned via the condition parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr);

/**
@brief Deletes a condition variable.
@param[in] cond	Specifies the condition variable to delete.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_cond_destroy (pthread_cond_t* cond);

/**
@brief Unblocks one or more threads blocked on a condition.
@param[in] cond	Specifies the condition to signal.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_cond_signal (pthread_cond_t* cond);

/**
@brief Unblocks one or more threads blocked on a condition.
@param[in] cond	Specifies the condition to signal.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_cond_broadcast (pthread_cond_t* cond);

/**
@brief Blocks the calling thread on a condition.
@param[in] cond	Specifies the condition variable to wait on.
@param[in] mutex Specifies the mutex used to protect the condition variable. The mutex must be locked when the subroutine is called.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_cond_wait (pthread_cond_t* cond, pthread_mutex_t* mutex);

/**
@brief Blocks the calling thread on a condition.
@param[in] cond	Specifies the condition variable to wait on.
@param[in] mutex	Specifies the mutex used to protect the condition variable. The mutex must be locked when the subroutine is called.
@param[in] abstime	Points to the absolute time structure specifying the blocked state timeout.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_cond_timedwait (pthread_cond_t* cond, pthread_mutex_t* mutex,
                const struct timespec* abstime);

#endif
/**
@brief Creates a mutex attributes object and initializes it with default values.
@param[out] attr  Specifies the mutex attributes object to be created. 
@return Upon successful completion, the new mutex attributes object is filled with default values and returned via the attr parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutexattr_init (pthread_mutexattr_t* attr);

/**
@brief Deletes a mutex attributes object.
@param[in] attr  Specifies the mutex attributes object to delete. 
@return Upon successful completion, the new mutex attributes object is filled with default values and returned via the attr parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutexattr_destroy (pthread_mutexattr_t* attr);

/**
@brief Creates a condition attributes object and initializes it with default values.
@param[out] attr	Specifies the condition attributes object to be created.
@return Upon successful completion, the new condition attributes object is filled with default values and returned via the attr parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_condattr_init(pthread_condattr_t* attr);

/**
@brief Deletes a condition attributes object.
@param[in] attr	Specifes the condition attributes object to delete.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_condattr_destroy(pthread_condattr_t* attr);


/**
@brief Sets the value of the contention-scope attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
@param[in] contentionscope	Specifies the contention scope to set. 
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setscope(pthread_attr_t *attr, int contentionscope);


/**
@brief Returns the value of the contention-scope attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
@param[out] contentionscope Points to where the contention-scope attribute value will be stored.
@return Upon successful completion, the value of the contention-scope attribute is returned via the contentionscope parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getscope(const pthread_attr_t *attr, int *contentionscope);

/**
@brief Sets the value of the inheritsched attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
@param[in] inheritsched Specifies the inheritance of scheduling properties to set. It must have one of the following values:
    -PTHREAD_EXPLICIT_SCHED	Specifies that the thread will take the scheduling properties specified in its attribute object.
 	-PTHREAD_INHERIT_SCHED	Specifies that the thread will inherit the scheduling properties from its creating thread. This is the default value.

@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);

/**
@brief Returns the value of the inheritsched attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
@param[out] inheritsched	Points to where the inheritsched attribute value will be stored.
@return Upon successful completion, the value of the inheritsched attribute is returned via the inheritsched parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);

/**
@brief Sets the value of the schedpolicy attribute of a thread attributes object.
@param[in] attr	Specifes the thread attributes object.
@param[in] policy	Specifies the scheduling policy to set. It must have one of the following values:
 -SCHED_FIFO	Denotes first-in first-out scheduling.
 -SCHED_RR	Denotes round-robin scheduling.
 -SCHED_OTHER Denotes the default AIX scheduling policy. It is the default value.

@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);

/**
@brief Returns the value of the schedpolicy attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
@param[out] policy	Points to where the schedpolicy attribute value will be stored.
 -SCHED_FIFO	Denotes first-in first-out scheduling.
 -SCHED_RR	Denotes round-robin scheduling.
 -SCHED_OTHER Denotes the default AIX scheduling policy. It is the default value.

@return Upon successful completion, the value of the schedpolicy attribute is returned via the policy parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);


/**
@brief Sets the value of the schedparam attribute of a thread attributes object.
@param[in] attr Specifies the thread attributes object.
@param[in] param Points to where the scheduling parameters to set are stored. The sched_priority field must be in the range from 1 to 127, where 1 is the least favored priority, and 127 the most favored.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);


/**
@brief Returns the value of the schedparam attribute of a thread attributes object.
@param[in] attr Specifies the thread attributes object.
@param[out] param Points to where the schedparam attribute value will be stored.
@return Upon successful completion, the value of the schedparam attribute is returned via the schedparam parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);


/**
@brief sets the current schedpolicy and schedparam attributes of a thread.
@param[in] thread 		Specifies the target thread.
@param[in] policy 	Points to the schedpolicy attribute to set. It must have one of the following values:
    -SCHED_FIFO	Denotes first-in first-out scheduling.
 	-SCHED_RR 	Denotes round-robin scheduling.
 	-SCHED_OTHER	 Denotes the default AIX scheduling policy. It is the default value.
@param[in] param to where the scheduling parameters to set are stored. The sched_priority field must be in the range from 1 to 127, where 1 is the least favored priority, and 127 the most favored.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_setschedparam (pthread_t thread, int policy, const struct sched_param* param);


/**
@brief Returns the current schedpolicy and schedparam attributes of a thread.
@param[in] thread Specifies the target thread.
@param[out] policy Points to where the schedpolicy attribute value will be stored.
@param[out] param   Points to where the schedparam attribute value will be stored.
@return Upon successful completion, the current value of the schedpolicy and schedparam attributes are returned via the schedpolicy and schedparam parameters, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_getschedparam (pthread_t thread, int *policy, struct sched_param* param);



/**
@brief Sets the value of the protocol attribute of a mutex attributes object.
@param[in] attr	Specifies the mutex attributes object.
@param[in] protocol	Specifies the protocol to set. It must have one of the following values:
    -PTHREAD_PRIO_NONE	Denotes no protocol. This is the default value.
 	-PTHREAD_PRIO_INHERIT	Denotes priority inheritance protocol. It is only available if the _POSIX_THREAD_PRIO_INHERIT symbol is defined. Otherwise, it is not supported.
 	-PTHREAD_PRIO_PROTECT	Denotes priority protection protocol. It is only available if the _POSIX_THREAD_PRIO_PROTECT symbol is defined. Otherwise, it is not supported.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutexattr_setprotocol(pthread_mutexattr_t* attr, int protocol);


/**
@brief Returns the value of the protocol attribute of a mutex attributes object.
@param[in] attr	Specifies the mutex attributes object.
@param[out] protocol	Points to where the protocol attribute value will be stored
@return Upon successful completion, the value of the protocol attribute is returned via the protocol parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t* attr, int* protocol);

/**
@brief Sets the value of the prioceiling attribute of a mutex attributes object.
@param[in] attr	Specifies the mutex attributes object.
@param[in] prioceiling Specifies the priority ceiling to set. It must be in the range from 1 to 127, where 1 is the least favored priority, and 127 the most favored; it should be the priority of the highest priority thread that can lock the mutex lock, or higher. Setting a lower value may cause priority inversions.
@return Upon successful completion, the current value of the prioceiling attribute is returned via the prioceiling parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t* attr, int prioceiling);


/**
@brief Returns the value of the prioceiling attribute of a mutex attributes object.
@param[in] attr	Specifies the mutex attributes object.
@param[out] prioceiling	Points to where the prioceiling attribute value will be stored.
@return Upon successful completion, the value of the prioceiling attribute is returned via the prioceiling parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutexattr_getprioceiling (const pthread_mutexattr_t* attr, int* prioceiling);


/**
@brief Dynamically sets the value of the prioceiling attribute of a mutex.
@param[in] mutex the current value of the prioceiling attribute of a mutex.
@param[out] prioceiling	Points to where the prioceiling attribute value will be stored.
@return Upon successful completion, the current value of the prioceiling attribute is returned via the prioceiling parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutex_getprioceiling (const pthread_mutex_t* mutex, int* prioceiling);

/**
@brief Dynamically sets the value of the prioceiling attribute of a mutex.
@param[in] mutex	Specifies the target mutex.
@param[in] prioceiling	Specifies the priority ceiling to set. It must be in the range from 1 to 127, where 1 is the least favored priority, and 127 the most favored; it should be the priority of the highest priority
@param[out] old_ceiling to where the previous prioceiling attribute value will be stored.
@return Upon successful completion, the previous value of the prioceiling attribute is returned via the oldceiling parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_mutex_setprioceiling (pthread_mutex_t* mutex, int prioceiling, int* old_ceiling);


/**
@brief Creates a thread attributes object and initializes it with default values..
@param[out] attr Specifies the thread attributes object to be created.
@return Upon successful completion, the new thread attributes object is filled with default values and returned via the attr parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_init(pthread_attr_t *attr);

/**
@brief Deletes a thread attributes object.
@param[in] attr Specifies the thread attributes object to delete.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_destroy(pthread_attr_t *attr);


/**
@brief Sets the value of the stacksize attribute of a thread attributes object.
@param[in] attr Specifies the thread attributes object.
@param[in] stacksize Specifies the minimum stack size, in bytes, to set. The default stack size is PTHREAD_STACK_MIN. The minimum stack size should be greater or equal than this value.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/**
@brief Returns the value of the stacksize attribute of a thread attributes object.
@param[in] attr Specifies the thread attributes object.
@param[out] stacksize Points to where the stacksize attribute value will be stored.
@return Upon successful completion, the value of the stacksize attribute is returned via the stacksize parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

/**
@brief Sets the value of the stackaddr attribute of a thread attributes object.
@param[in] attr Specifies the thread attributes object.
@param[in] stackaddr Specifies the stack address to set. It is a void pointer.
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);


/**
@brief Returns the value of the stackaddr attribute of a thread attributes object.
@param[in] attr Specifies the thread attributes object.
@param[out] stackaddr Points to where the stackaddr attribute value will be stored.
@return Upon successful completion, the value of the stackaddr attribute is returned via the stackaddr parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);

/**
@brief Sets the value of the detachstate attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
    -PTHREAD_CREATE_DETACHED	Specifies that the thread will be created in detached state. This is the default value.
 	-PTHREAD_CREATE_UNDETACHED	Specifies that the thread will be created in undetached state.
@param[in] detachstate	Specifies the detached state to set. It must have one of the following values:
@return Upon successful completion, 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);

/**
@brief Returns the value of the detachstate attribute of a thread attributes object.
@param[in] attr	Specifies the thread attributes object.
@param[out] detachstate	Points to where the detachstate attribute value will be stored.
@return Upon successful completion, the value of the detached state attribute is returned via the detachstate parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);


/**
@brief Creates a new thread, initializes its attributes, and makes it runnable.
@param[out] thread Points to where the thread ID will be stored.
@param[in] attr Specifies the thread attributes object to use in creating the thread. If the
value is NULL, the default attributes values will be used.
@param[in] start_routine Points to the routine to be executed by the thread.
@param[in] arg Points to the single argument to be passed to the start_routine routine.
@return If successful, the pthread_join function returns zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

/**
@brief Blocks the calling thread until the specified thread terminates.
@param[in] thread Specifies the target thread.
@param[out] value_ptr Points to where the termination status of the target thread will be stored. If the value is NULL, the termination status is not returned.
@return If successful, the pthread_join function returns zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_join(pthread_t thread, void **value_ptr);


/**
@brief Blocks the calling thread until the specified thread terminates.
@param[in] thread Specifies the target thread.
@return If successful, the pthread_join function returns zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_detach(pthread_t thread);

/**
@brief Terminates the calling thread.
@param[out] value_ptr Points to an optional termination status, it can be consulted from another thread using pthread_join. If no termination status is desired, its value should be NULL.
@return The pthread_exit function cannot return to its caller.
*/
void pthread_exit(void *value_ptr);


/**
@brief Returns the calling thread's ID.
@return The calling thread's ID is returned.
*/
pthread_t pthread_self(void);

/**
@brief Compares two thread IDs.
@param[in] t1 Specifies the first ID to be compared.
@param[in] t2 Specifies the second ID to be compared.
@return The pthread_equal function returns a non-zero value if t1 and t2 are equal; otherwise, zero is returned.
If either t1 or t2 are not valid thread IDs, the behaviour is undefined.

*/
int pthread_equal(pthread_t t1, pthread_t t2);

/**
@brief Executes a routine exactly once in a process.
@param[in] once_control Points to a synchronization control structure. This structure has to be initialized by the static initializer macro PTHREAD_ONCE_INIT.
@param[in] init_routine Points to the routine to be executed.
@return The function pthread_getspecific returns the thread-specific data value associated with the given key. If no thread-specific data value is associated with key, then the value NULL is returned. If successful, the pthread_setspecific function returns zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));


/**
@brief Creates a thread-specific data key.
@param[out] key Points to where the key will be stored.
@param[in] destructor Points to an optional destructor routine, used to cleanup data on thread termination. If no cleanup is desired, this pointer should be NULL.
@return The function pthread_getspecific returns the thread-specific data value associated with the given key. If no thread-specific data value is associated with key, then the value NULL is returned. If successful, the pthread_setspecific function returns zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_key_create(pthread_key_t* key, void (*destructor)(void*));




/**
@brief sets the thread-specific data associated with the specified key.
@param[in] key Specifies the key to which the value is bound.
@param[in] value Specifies the new thread-specific value.
@return If successful, the pthread_setspecific function returns zero. Otherwise, an error number is returned to indicate the error. 
*/
int pthread_setspecific(pthread_key_t key, const void* value);

/**
@brief Returns the thread-specific data associated with the specified key.
@param[out] key Specifies the key to which the value is bound.
@return The function pthread_getspecific returns the thread-specific data value associated with the given key. If no thread-specific data value is associated with key, then the value NULL is returned.
*/
void* pthread_getspecific(pthread_key_t key);

/**
@brief Deletes a thread-specific data key.
@param[in] key Specifies the key to delete.
@return If successful, the pthread_key_delete function returns zero. Otherwise, an error number is returned to indicate the error.
*/
int pthread_key_delete(pthread_key_t key);


/**
@brief Requests the cancellation of a thread.
@param[in] thread Specifies the thread to be canceled.
@return If successful, the pthread_cancel function returns zero. Otherwise, an error number is returned to indicate the error. 	
*/
int pthread_cancel(pthread_t thread);


/**
@brief Sets the calling thread's cancelability state.
@param[in] state Specifies the new cancelability state to set. It must have one of the following values:
 	-PTHREAD_CANCEL_DISABLE	Disables cancelability; the thread is not cancelable. Cancellation requests are held pending.
 	-PTHREAD_CANCEL_ENABLE	Enables cancelability; the thread is cancelable, according to its cancelability type. This is the default value.

@param[out] oldstate  Points to where the previous cancelability state value will be stored.
@return Upon successful completion, the previous value of the cancelability state is returned via the oldstate parameter, and 0 is returned. Otherwise, an error code is returned.
*/
int pthread_setcancelstate(int state, int *oldstate);

/**
@brief 需要添加说明
@return 需要添加说明
*/
int pthread_setcanceltype(int type, int *oldtype);

/**
@brief Creates a cancellation point in the calling thread.
*/
void pthread_testcancel(void);

#else
#include <pthread.h>
#endif	//endof  MMCP_LOCKCHECK_SUPPORT






#ifdef __cplusplus
}
#endif
/** @} */
#endif 

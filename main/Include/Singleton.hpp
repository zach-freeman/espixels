
//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef SINGLETON_HPP_INCLUDED
#define SINGLETON_HPP_INCLUDED

#include <cstdint>

#include "ZAssert.hpp"
#include "FreeRTOS.h"
#include "semphr.h"
//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================

//==============================================================================
// Public function prototypes
//==============================================================================

template<typename BASE_TYPE> class Singleton
{
public:
    static BASE_TYPE &GetInstance()
    {
#ifdef SIMPLE_SINGLETON
        // Create the object.
        if (mInstance == nullptr)
        {
            // All singletons must be created before the scheduler starts.
            ZAssert(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED);

            // Recursive instance creation detected.
            ZAssert(!mIsCreating);

            mIsCreating = true;
            mInstance   = new BASE_TYPE;
            ZAssert(mInstance);
            mIsCreating = false;
        }

        return *mInstance;
#else
        // We can quit immediately if the mInstance has been created.
        if (mInstance != nullptr)
        {
            return *mInstance;
        }

        if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
        {
            // If you're seeing this, you probably have hidden recursion in the
            // object you tried to create before the scheduler started. Oops.
            ZAssert(!mIsCreating);
        }
        else
        {
            // If you're seeing this, you didn't initialize the mutex before
            // the scheduler started.
            ZAssert(mMutex != nullptr);

            // If another task beats us here, we'll wait until they are done.
            xSemaphoreTake(mMutex, portMAX_DELAY);
        }

        // Perform a second check now in case we just blocked waiting
        // for someone else to create the instance.
        if (mInstance == nullptr)
        {
            mIsCreating = true;
            mInstance   = new BASE_TYPE;
            ZAssert(mInstance);
            mIsCreating = false;
        }

        // We're done!  Release the mutex.
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
        {
            xSemaphoreGive(mMutex);
        }

        return *mInstance;
#endif
    }

    static void CreateMutex()
    {
#ifndef SIMPLE_SINGLETON
        // You must create this mutex before the scheduler starts.
        ZAssert(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED);
        mMutex = xSemaphoreCreateMutex();
        ZAssert(mMutex);
#endif
    }

protected:
    Singleton() = default;

private:
    static BASE_TYPE *mInstance;
#ifndef SIMPLE_SINGLETON
    static SemaphoreHandle_t mMutex;
#endif
    // This is only used to detect recursion on objects like HAL that are
    // created before the scheduler has started.
    static bool mIsCreating;
};

// You'll end up with one of these for each BASE_TYPE singleton you create.
template<typename BASE_TYPE>
BASE_TYPE *Singleton<BASE_TYPE>::mInstance{nullptr};

#ifndef SIMPLE_SINGLETON
template<typename BASE_TYPE>
SemaphoreHandle_t Singleton<BASE_TYPE>::mMutex{nullptr};
#endif

template<typename BASE_TYPE> bool Singleton<BASE_TYPE>::mIsCreating{false};

#endif    // SINGLETON_HPP_INCLUDED

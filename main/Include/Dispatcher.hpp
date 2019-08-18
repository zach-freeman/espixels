//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef DISPATCHER_HPP_INCLUDED
#define DISPATCHER_HPP_INCLUDED

#include <list>

#include "FreeRTOS.h"
#include "OsTask.hpp"
#include "ReturnCode.hpp"
#include "Singleton.hpp"
#include "queue.h"
#include "task.h"


class Action;
class Store;
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
extern "C" void Dispatcher_TaskRunner(void *pvparams);

//==============================================================================
// Class declarations
//==============================================================================
class Dispatcher : public OsTask, public Singleton<Dispatcher>
{
    friend Singleton<Dispatcher>;

public:
    ~Dispatcher();
    uint32_t   GetNumberOfSubscribers();
    ReturnCode SendAction(Action &action,
                          uint32_t timeoutMillis = DEFAULT_DISPATCHER_POST_TIMEOUT);
    bool       Subscribe(Store &store);
    void       Task();
    void       Unsubscribe(Store &store);
    void       UnsubscribeAll();
    void       StartTask() override;

private:
    //==========================================================================
    // Private constants
    //==========================================================================
    static constexpr uint32_t DISPATCHER_MAX_QUEUE_SIZE{10};
    static constexpr uint32_t DEFAULT_DISPATCHER_POST_TIMEOUT{1000};
    //==========================================================================
    // Private data members
    //==========================================================================
    Dispatcher();
    std::list<Store *> mSubscribedStores;
    QueueHandle_t      mActionQueueHandle;
    TaskHandle_t       mTaskHandle;

    //==========================================================================
    // Private method prototypes
    //==========================================================================
    void ProcessAction(Action *action);
};

#endif    // DISPATCHER_HPP_INCLUDED

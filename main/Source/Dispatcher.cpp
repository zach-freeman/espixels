//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#include "Dispatcher.hpp"

#include <cassert>

#include "ZAssert.hpp"
#include "Store.hpp"
#include "TaskBase.hpp"
#include "TaskConfig.hpp"

class Action;

//==============================================================================
// Private defines and constants
//==============================================================================

//==============================================================================
// Private typedefs
//==============================================================================

//==============================================================================
// Private variables
//==============================================================================

//==============================================================================
// Private class member definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Dispatcher::ProcessAction(Action *action)
{
    for (auto store : mSubscribedStores)
    {
        action->Accept(*store);
    }
}

//==============================================================================
// Public function definitions
//==============================================================================

//==============================================================================
// Class member definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
extern "C" void Dispatcher_TaskRunner(void *pvparams)
{
    (static_cast<Dispatcher *>(pvparams))->Task();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Dispatcher::Dispatcher()
{

    mActionQueueHandle =
        xQueueCreate(DISPATCHER_MAX_QUEUE_SIZE, Store::GetLargestAction());


    if (!mActionQueueHandle)
    {
        while (true)
        {
            ;
        }
    }

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Dispatcher::~Dispatcher()
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
uint32_t Dispatcher::GetNumberOfSubscribers()
{
    return mSubscribedStores.size();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ReturnCode Dispatcher::SendAction(Action &action, uint32_t timeoutMillis)
{
    BaseType_t result;
    if (xPortIsInsideInterrupt())
    {
        BaseType_t woke;
        result = xQueueSendToBackFromISR(mActionQueueHandle, &action, &woke);
        portYIELD_FROM_ISR(woke);
    }
    else
    {
        result = xQueueSendToBack(mActionQueueHandle,
                                  &action,
                                  pdMS_TO_TICKS(timeoutMillis));
    }
    if ((result == pdTRUE) || (result == pdPASS))
    {
        return ReturnCode::Success;
    }
    else
    {
        return ReturnCode::ErrorBusy;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Dispatcher::Subscribe(Store &store)
{
    for (auto slot : mSubscribedStores)
    {
        if (slot == &store)
        {
            return true;
        }
    }
    mSubscribedStores.push_back(&store);
    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Dispatcher::Task()
{
    while (FOREVER())
    {
        uint8_t element[Store::GetLargestAction()];
        if (xQueueReceive(mActionQueueHandle, element, portMAX_DELAY))
        {
            Action *action{reinterpret_cast<Action *>(element)};

            ProcessAction(action);
            action->ReleaseResources();
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Dispatcher::Unsubscribe(Store &store)
{
    mSubscribedStores.remove(&store);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Dispatcher::UnsubscribeAll()
{
    mSubscribedStores.clear();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Dispatcher::StartTask()
{
    ZAssert(xTaskCreate(Dispatcher_TaskRunner,
                        "dsp",
                        DISPATCHER_TASK_STACK_SIZE,
                        this,
                        DISPATCHER_TASK_PRIORITY,
                        &mTaskHandle)
            == pdPASS)
}

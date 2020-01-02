//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef NETWORK_TIME_CONTROLLER_INCLUDED_HPP
#define NETWORK_TIME_CONTROLLER_INCLUDED_HPP
#include "TimeStore.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================
static constexpr const char *   NETWORK_TIME_CONTROLLER_TAG{"Network Time Controller"};
//==============================================================================
// Public function prototypes
//==============================================================================

class NetworkTimeController : public StoreSubscriber
{
public:
    NetworkTimeController(TimeStore &timeStore);
    void StartTask();
    void TaskStop() const;
    void NtpTask();
    void UpdateTime();
    void *ProcessChange(StoreSubscriber::ChangeType change,
                        Action::Source source) const override;

private:
    //==========================================================================
    // Private typedefs
    //=========================================================================
    typedef union {
        uint32_t rawBits;
            struct {
                uint32_t initializeTime : 1;
                uint32_t updateTime : 1;
            };
    } Event_t;
    //==========================================================================
    // Private constants
    //==========================================================================
    static constexpr uint32_t SNTP_SYNC_STATUS_DELAY_MS{2000};
    static constexpr uint32_t TIME_UPDATE_TIMER_MS{10000};
    //==========================================================================
    // Private data members
    //==========================================================================
    TimeStore &mTimeStore;
    TaskHandle_t mNtpSyncTaskHandle{};
    TimerHandle_t mTimeUpdateTimer{};
    Action::Source mActionSource;
    std::string mCurrentTimeString;
    //==========================================================================
    // Private method prototypes
    //==========================================================================
    void InitializeSntp();
};
#endif // NETWORK_TIME_CONTROLLER_INCLUDED_HPP

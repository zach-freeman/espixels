//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef NETWORK_TIME_CONTROLLER_INCLUDED_HPP
#define NETWORK_TIME_CONTROLLER_INCLUDED_HPP
#include "TimeStore.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

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
    void *ProcessChange(StoreSubscriber::ChangeType change,
                        Action::Source source) const override;

private:
    //==========================================================================
    // Private typedefs
    //=========================================================================
    typedef union {
        uint32_t rawBits;
            struct {
                uint32_t updateTime : 1;
            };
    } Event_t;
    //==========================================================================
    // Private constants
    //==========================================================================
    static constexpr uint32_t SNTP_SYNC_STATUS_DELAY{2000};

    //==========================================================================
    // Private data members
    //==========================================================================
    TimeStore &mTimeStore;
    TaskHandle_t mNtpSyncTaskHandle{};
    Action::Source mActionSource;
    std::string mCurrentTimeString;
    //==========================================================================
    // Private method prototypes
    //==========================================================================
    void InitializeSntp();
};
#endif // NETWORK_TIME_CONTROLLER_INCLUDED_HPP

//==============================================================================
// Copyright © 2020 Sparkwing
//==============================================================================
#ifndef UI_CONTROLLER_INCLUDED_HPP
#define UI_CONTROLLER_INCLUDED_HPP
#include "UIStore.hpp"
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
static constexpr const char *   UI_CONTROLLER_TAG{"UI Controller"};
//==============================================================================
// Public function prototypes
//==============================================================================

class UiController : public StoreSubscriber
{
public:
    UiController(UIStore &uiStore);
    void StartUiTask();
    void TaskUiStop();
    void UiTask();
    void *ProcessChange(StoreSubscriber::ChangeType change,
                        Action::Source source) const override;

private:
    //==========================================================================
    // Private typedefs
    //=========================================================================
    typedef union {
        uint32_t rawBits;
            struct {
                uint32_t showTime : 1;
            };
    } Event_t;
    //==========================================================================
    // Private constants
    //==========================================================================

    //==========================================================================
    // Private data members
    //==========================================================================
    UIStore &mUiStore;
    TaskHandle_t mUiTaskHandle{};
    Action::Source mActionSource;
    std::string mCurrentTimeString;
    lv_obj_t *mTimeLabel{nullptr};
    //==========================================================================
    // Private method prototypes
    //==========================================================================
};
#endif // UI_CONTROLLER_INCLUDED_HPP

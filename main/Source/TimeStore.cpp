#include "TimeStore.hpp"
#include "esp_log.h"

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
// Private function definitions
//==============================================================================

//==============================================================================
// Public function definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TimeStore::TimeStore()
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TimeStore::ProcessAction(NetworkAction &action)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TimeStore::ProcessAction(TimeAction &action)
{
    auto &actionTypes = action.GetTypes();
    StoreSubscriber::ChangeType changeType{StoreSubscriber::ChangeType::NONE};
    ESP_LOGE(TIME_STORE_TAG, "got an action");
    for (auto &type : actionTypes)
    {
        switch (type)
        {
            case TimeActionType::InitializeTime:
                ESP_LOGE(TIME_STORE_TAG, "got an initialize time action");
                changeType = StoreSubscriber::ChangeType::TIME;
                break;
            default:
                break;
        }
        if (changeType != StoreSubscriber::ChangeType::NONE)
        {
            PublishChange(Action::Source::Time, changeType);
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TimeStore::PublishChange(Action::Source actionSource,
                             StoreSubscriber::ChangeType changeType)
{
    // publish a message to subcribers
    for (auto sub : subscribers)
    {
        sub->ProcessChange(changeType, actionSource);
    }   
}                             



//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef TIME_ACTION_HEADER_INCLUDED
#define TIME_ACTION_HEADER_INCLUDED

#include <cstdint>
#include <memory>

#include "Action.hpp"
#include "ActionType.hpp"
//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================
enum class TimeActionType
{
    None,
    InitiailizeTime,
};
//==============================================================================
// Public variables
//==============================================================================
static constexpr int MAX_TIME_ACTION_TYPES{2};
//==============================================================================
// Public function prototypes
//==============================================================================
class TimeAction : public Action, public ActionType<TimeActionType, MAX_TIME_ACTION_TYPES>
{
public:
    TimeAction(TimeActionType action) : mActionType(action){ AddType(mActionType); };
    void   Accept(Store &) override;
    void   InitializeTime();

private:
    TimeActionType mActionType{};
};
#endif    // TIME_ACTION_HEADER_INCLUDED

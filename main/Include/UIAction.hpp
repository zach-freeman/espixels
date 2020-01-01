//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef UI_ACTION_HEADER_INCLUDED
#define UI_ACTION_HEADER_INCLUDED

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
enum class UIActionType
{
    None,
    Initialize
};
//==============================================================================
// Public variables
//==============================================================================
static constexpr int MAX_UI_ACTION_TYPES{2};
//==============================================================================
// Public function prototypes
//==============================================================================
class UIAction : public Action, public ActionType<UIActionType, MAX_UI_ACTION_TYPES>
{
public:
    UIAction(UIActionType action) : mActionType(action){ AddType(mActionType); };
    void   Accept(Store &) override;
    void   Initialize();
private:
    UIActionType mActionType{};
};
#endif    // UI_ACTION_HEADER_INCLUDED

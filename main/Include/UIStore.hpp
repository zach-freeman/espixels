//==============================================================================
// Copyright © 2020 Sparkwing
//==============================================================================
#ifndef UISTORE_HPP_INCLUDED
#define UISTORE_HPP_INCLUDED

#include <stdint.h>
#include "Store.hpp"

static constexpr const char *UI_STORE_TAG{"UI Store"};
static constexpr uint32_t LV_TICK_PERIOD{20};
//==============================================================================
// CLASS DECLARATION
//==============================================================================
class UIStore : public Store
{
public:
    //==========================================================================
    // Public defines, constants and typedefs
    //==========================================================================

    //==========================================================================
    // Public member variables
    //==========================================================================

    //==========================================================================
    // Public methods
    //==========================================================================
    UIStore();
    virtual ~UIStore() = default;
    void ProcessAction(NetworkAction &action) override;
    void ProcessAction(TimeAction &action) override;
    void ProcessAction(UIAction &action) override;

private:
    //==========================================================================
    // Private defines, constants and typedefs
    //==========================================================================

    //==========================================================================
    // Private member variables
    //==========================================================================

    //==========================================================================
    // Private methods
    //==========================================================================
    void PublishChange(Action::Source actionSource,
                       StoreSubscriber::ChangeType changeType);
};
#endif // UISTORE_HPP_INCLUDED
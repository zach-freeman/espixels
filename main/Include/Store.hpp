//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef STORE_HPP_INCLUDED
#define STORE_HPP_INCLUDED

#include <cstddef>
#include <list>
#include <memory>

#include "NetworkAction.hpp"
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

class StoreSubscriber
{
public:
    enum class ChangeType
    {
        FAN,
        LIGHT,
        LIGHTCOMM,
        NETWORK,
        FIRMWARE,
        DEVICE
    };

    virtual void *ProcessChange(ChangeType, Action::Source source) const = 0;
    StoreSubscriber()          = default;
    virtual ~StoreSubscriber() = default;
};

class Store
{
public:
    friend class StoreSubscriber;

    virtual std::unique_ptr<uint8_t[]> BuildApiUpdate(size_t &length) const
    {
        length = 0;
        return nullptr;
    }

    virtual void ProcessAction(NetworkAction &networkAction)
    {
    }



    // When new action types are added, they MUST be added to this union to
    // ensure that the Dispatcher queue elements are large enough to hold any
    // type of action.
    static constexpr size_t GetLargestAction()
    {
        /*union Actions {
            NetworkAction      networkAction;
        };*/
        return sizeof(NetworkAction);
    }

    void Subscribe(const StoreSubscriber *subscriber)
    {
        subscribers.push_back(subscriber);
    }
    void Unsubscribe(const StoreSubscriber *subscriber)
    {
        subscribers.remove(subscriber);
    }

protected:
    enum class Status
    {
        PENDING,
        SUCCESS,
        FAILURE
    };

    Store()          = default;
    virtual ~Store() = default;
    Status                             status;
    std::list<const StoreSubscriber *> subscribers;
};

#endif    // STORE_HPP_INCLUDED

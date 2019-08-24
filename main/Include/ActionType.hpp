//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef ACTION_TYPE_HPP_INCLUDED
#define ACTION_TYPE_HPP_INCLUDED

#include <vector>

//==============================================================================
// Public defines and constants
//==============================================================================
//==========================================================================
// Public typedefs
//==========================================================================
//==============================================================================
// Public variables
//==============================================================================

template <class T> 
class ActionType
{
public:

    //==========================================================================
    // Public function prototypes
    //==========================================================================
    virtual void AddType(T type) final { mTypes.push_back(type); }
    virtual std::vector<T> &GetTypes() final { return mTypes; }

    virtual ~ActionType() = default;

protected:
    ActionType() = default;

private:
    std::vector<T> mTypes;
};

#endif    // ACTION_TYPE_HPP_INCLUDED

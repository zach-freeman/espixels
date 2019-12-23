//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef ACTION_TYPE_HPP_INCLUDED
#define ACTION_TYPE_HPP_INCLUDED

#include <array>

//==============================================================================
// Public defines and constants
//==============================================================================
//==========================================================================
// Public typedefs
//==========================================================================
//==============================================================================
// Public variables
//==============================================================================

template <class T, int MAX_TYPES = 2> 
class ActionType
{
public:
    //==========================================================================
    // Public function prototypes
    //==========================================================================
    void AddType(T type) {
        if (mIndex < MAX_TYPES)
        {
            mTypes[mIndex++] = type;
        }

    }
    auto const &GetTypes() { return mTypes; }

    virtual ~ActionType() = default;

protected:

private:
    std::array<T, MAX_TYPES> mTypes{};
    size_t mIndex{0};
};

#endif    // ACTION_TYPE_HPP_INCLUDED

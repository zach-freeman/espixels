//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef STORE_PROVIDER_HPP_INCLUDED
#define STORE_PROVIDER_HPP_INCLUDED

#include <cstddef>
#include "NetworkStore.hpp"
#include "Singleton.hpp"
#include "TimeStore.hpp"
//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs                                            
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================

class StoreProvider: public Singleton<StoreProvider>
{
    friend Singleton<StoreProvider>;
public:
    //==============================================================================
    // Public function prototypes
    //==============================================================================
    virtual ~StoreProvider() = default;
    NetworkStore & GetNetworkStore();
    void SetNetworkStore(NetworkStore &networkStore);
    TimeStore & GetTimeStore();
    void SetTimeStore(TimeStore &timeStore);

protected:
private:
    //==========================================================================
    // Private constants
    //==========================================================================
    //==========================================================================
    // Private method prototypes
    //==========================================================================
    StoreProvider();
    //==========================================================================
    // Private data members
    //==========================================================================
    NetworkStore *mNetworkStore{nullptr};
    TimeStore *mTimeStore{nullptr};
    
};

#endif    // STORE_PROVIDER_HPP_INCLUDED

//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef STORE_PROVIDER_HPP_INCLUDED
#define STORE_PROVIDER_HPP_INCLUDED

#include <cstddef>
#include "NetworkStore.hpp"
#include "Singleton.hpp"
#include "TimeStore.hpp"
#include "UIStore.hpp"
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
    UIStore & GetUIStore();
    void SetUIStore(UIStore &uiStore);

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
    UIStore *mUIStore{nullptr};
    
};

#endif    // STORE_PROVIDER_HPP_INCLUDED

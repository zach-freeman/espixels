#ifndef NETWORK_STORE_HPP
#define NETWORK_STORE_HPP

#include <stdint.h>
#include "Singleton.hpp"
#include "Store.hpp"

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

class NetworkStore : public Store, public Singleton<NetworkStore>
{
public:
    friend Singleton<NetworkStore>;
    void ProcessAction(NetworkAction &action) override;
    virtual ~NetworkStore() = default;
private:
    NetworkStore();
    static NetworkStore * mInstance;
};

#endif    // NETWORK_STORE_HPP
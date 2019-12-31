#ifndef TIME_STORE_HEADER_INCLUDED
#define TIME_STORE_HEADER_INCLUDED

#include <stdint.h>
#include "Store.hpp"
#include "EspWifi.hpp"

//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================
static constexpr const char *   TIME_STORE_TAG{"Time Store"};
//==============================================================================
// Public function prototypes
//==============================================================================

class TimeStore : public Store
{
public:
    TimeStore();
    virtual ~TimeStore() = default;
    void ProcessAction(NetworkAction &action) override;
    void ProcessAction(TimeAction &action) override;

private:
    void InitializeSntp();

};

#endif    // TIME_STORE_HEADER_INCLUDED
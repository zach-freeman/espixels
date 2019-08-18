//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef RETURN_CODE_H
#define RETURN_CODE_H

#include <cstdint>

//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================

enum class ReturnCode : uint32_t
{
    Success,
    ErrorInternal,
    ErrorNoMemory,
    ErrorNotFound,
    ErrorNotSupported,
    ErrorInvalidParameter,
    ErrorInvalidState,
    ErrorInvalidLength,
    ErrorInvalidFlags,
    ErrorInvalidData,
    ErrorDataSize,
    ErrorTimeout,
    ErrorNull,
    ErrorForbidden,
    ErrorInvalidAddress,
    ErrorBusy,
    PartialRead

};

#endif    // RETURN_CODE_H

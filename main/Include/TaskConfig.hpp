//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef TASK_CONFIG_INCLUDED
#define TASK_CONFIG_INCLUDED

#include <cstdint>

#include "FreeRTOS.h"

//==============================================================================
// Public defines and constants
//==============================================================================
#ifdef portYIELD_FROM_ISR
#undef portYIELD_FROM_ISR
#define portYIELD_FROM_ISR(switchRequired)                                     \
    if (switchRequired != pdFALSE)                                             \
    {                                                                          \
        traceISR_EXIT_TO_SCHEDULER();                                          \
        _frxt_setup_switch();                                                  \
    }
#endif

static constexpr size_t DISPATCHER_TASK_STACK_SIZE{(configMINIMAL_STACK_SIZE * 7)};
static constexpr uint32_t DISPATCHER_TASK_PRIORITY{3};

static constexpr uint32_t NETWORK_TIME_SYNCH_TASK_PRIORITY{0};
static constexpr size_t   NETWORK_TIME_SYNCH_TASK_SIZE{configMINIMAL_STACK_SIZE + 3840};
static constexpr uint32_t UI_TASK_PRIORITY{0};
static constexpr size_t   UI_TASK_SIZE{configMINIMAL_STACK_SIZE + 3840};
static constexpr uint32_t WIFI_INFO_TASK_PRIORITY{0};
static constexpr size_t   WIFI_INFO_TASK_SIZE{configMINIMAL_STACK_SIZE + 3840};

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================

//==============================================================================
// Public function prototypes
//==============================================================================
#endif    // TASK_CONFIG_INCLUDED
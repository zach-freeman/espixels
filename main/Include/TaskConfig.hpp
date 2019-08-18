//==============================================================================
// Copyright © 2019 Big Ass FAns
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

static constexpr size_t   UART_RX_TASK_SIZE{configMINIMAL_STACK_SIZE + 2304};
static constexpr uint32_t UART_RX_TASK_PRIORITY{5};
static constexpr size_t   UART_TX_TASK_SIZE{configMINIMAL_STACK_SIZE + 1536};
static constexpr uint32_t UART_TX_TASK_PRIORITY{5};
static constexpr size_t   UART_ACK_TASK_SIZE{configMINIMAL_STACK_SIZE + 256};
static constexpr uint32_t UART_ACK_TASK_PRIORITY{5};
static constexpr size_t   UPDATE_TASK_SIZE{configMINIMAL_STACK_SIZE + 3072};
static constexpr uint32_t UPDATE_PRIORITY{3};
static constexpr size_t   TCP_SERVER_TASK_SIZE{configMINIMAL_STACK_SIZE + 1024};
static constexpr uint32_t TCP_SERVER_PRIORITY{3};
static constexpr size_t   TCP_RX_TASK_SIZE{configMINIMAL_STACK_SIZE + 1536};
static constexpr uint32_t TCP_RX_PRIORITY{3};
static constexpr size_t   BLE_DRIVER_TASK_SIZE{configMINIMAL_STACK_SIZE + 512};
static constexpr uint32_t BLE_DRIVER_TASK_PRIORITY{2};
static constexpr size_t   WIFI_SCAN_TASK_SIZE{configMINIMAL_STACK_SIZE + 750};
static constexpr uint32_t WIFI_SCAN_TASK_PRIORITY{0};
static constexpr size_t CLOUD_CLIENT_TASK_SIZE{configMINIMAL_STACK_SIZE + 3840};
static constexpr uint32_t CLOUD_CLIENT_TASK_PRIORITY{4};
static constexpr size_t   WEBSOCKET_RX_TASK_STACK_SIZE{configMINIMAL_STACK_SIZE
                                                     + 1000};
static constexpr uint32_t WEBSOCKET_RX_TASK_PRIORITY{4};
static constexpr size_t   WEBSOCKET_TX_TASK_STACK_SIZE{configMINIMAL_STACK_SIZE
                                                     + 1000};
static constexpr uint32_t WEBSOCKET_TX_TASK_PRIORITY{4};
static constexpr uint32_t HTTP_UPDATE_SERVER_TASK_STACK_SIZE{
    configMINIMAL_STACK_SIZE + 800};
static constexpr uint32_t HTTP_UPDATE_SERVER_TASK_PRIORITY{3};

static constexpr uint32_t PARSER_TASK_STACK_SIZE{configMINIMAL_STACK_SIZE + 1400};
static constexpr uint32_t PARSER_TASK_PRIORITY{3};

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
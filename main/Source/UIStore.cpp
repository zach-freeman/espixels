//==============================================================================
// Copyright © 2020 Sparkwing
//==============================================================================
#include "UIStore.hpp"
#include "FreeRTOS.h"
#include "esp_freertos_hooks.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "disp_spi.h"
#include "ili9341.h"
#include "lvgl/lvgl.h"

//==============================================================================
// Local defines and constants
//==============================================================================

//==============================================================================
// Local variables
//==============================================================================

//==============================================================================
// Static function definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static void lv_tick_task(void)
{
    lv_tick_inc(LV_TICK_PERIOD);
}

//==============================================================================
// Method definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
UIStore::UIStore()
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UIStore::ProcessAction(NetworkAction &action)
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UIStore::ProcessAction(TimeAction &action)
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UIStore::ProcessAction(UIAction &action)
{
    auto &actionTypes = action.GetTypes();
    ESP_LOGE(UI_STORE_TAG, "got an action");
    for (auto &type : actionTypes)
    {
        StoreSubscriber::ChangeType changeType{StoreSubscriber::ChangeType::NONE};
        switch (type)
        {
        case UIActionType::Initialize:
            ESP_LOGE(UI_STORE_TAG, "got an initialize action");
            lv_init();
            disp_spi_init();
            ili9341_init();
            static lv_color_t buf1[DISP_BUF_SIZE];
            static lv_color_t buf2[DISP_BUF_SIZE];
            static lv_disp_buf_t disp_buf;
            lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);
            lv_disp_drv_t disp_drv;
            lv_disp_drv_init(&disp_drv); 
            disp_drv.flush_cb = ili9341_flush;
            disp_drv.buffer = &disp_buf;
            lv_disp_drv_register(&disp_drv);
            esp_register_freertos_tick_hook(lv_tick_task);
            break;
        case UIActionType::ShowTime:
            // show the time
            changeType = StoreSubscriber::ChangeType::UI;
            break;
        default:
            break;
        }
        if (changeType != StoreSubscriber::ChangeType::NONE)
        {
            PublishChange(Action::Source::Internal, changeType);
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UIStore::PublishChange(Action::Source actionSource,
                             StoreSubscriber::ChangeType changeType)
{
    // publish a message to subcribers
    for (auto sub : subscribers)
    {
        sub->ProcessChange(changeType, actionSource);
    }   
} 
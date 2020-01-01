//==============================================================================
// Copyright © 2020 Sparkwing
//==============================================================================
#include "UiController.hpp"
#include "Dispatcher.hpp"
#include "esp_log.h"
#include "lvgl/lvgl.h"
#include "TaskBase.hpp"
#include "TaskConfig.hpp"
#include "TimeAction.hpp"
#include "StoreProvider.hpp"

//==============================================================================
// Local defines and constants
//==============================================================================

//==============================================================================
// Local variables
//==============================================================================

//==============================================================================
// Static function definitions
//==============================================================================
static void UiTaskRunner(void *params)
{
    static_cast<UiController *>(params)->UiTask();
}
//==============================================================================
// Method definitions
//==============================================================================
UiController::UiController(UIStore &uiStore)
    : mUiStore(uiStore)
{
    mUiStore.Subscribe(this);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void *UiController::ProcessChange(StoreSubscriber::ChangeType changeType,
                                  Action::Source source) const
{
    if (source == Action::Source::Internal)
    {
        Event_t event{};
        switch (changeType)
        {
        case StoreSubscriber::ChangeType::UI:
            event.helloWorld = 1;
            break;
        case StoreSubscriber::ChangeType::TIME:
            event.showTime =1;
            break;
        default:
            break;
        }
        xTaskNotify(mUiTaskHandle, event.rawBits, eSetBits);
    }
    return nullptr;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UiController::StartUiTask()
{
    if (xTaskCreate(UiTaskRunner,
                    "ui_task",
                    UI_TASK_SIZE,
                    this,
                    UI_TASK_PRIORITY,
                    &mUiTaskHandle) != pdPASS)
    {
        ESP_LOGE(UI_CONTROLLER_TAG, "Error: can't start ui task");
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UiController::UiTask()
{
    while (FOREVER())
    {
        Event_t event{};
        if (xTaskNotifyWait(ULONG_MAX, ULONG_MAX, &event.rawBits, portMAX_DELAY) == pdTRUE)
        {
            if (event.helloWorld)
            {
                lv_obj_t *screen = lv_disp_get_scr_act(NULL); /* Get current screen */
                /*Create a Label on the currently active screen*/
                lv_obj_t *label1 = lv_label_create(screen, NULL);

                /*Modify the Label's text*/
                lv_label_set_text(label1, "Hello world!");

                /* Align the Label to the center
                * NULL means align on parent (which is the screen now)
                * 0, 0 at the end means an x, y offset after alignment*/
                lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
            }
            else if (event.showTime)
            {
                lv_obj_t *screen = lv_disp_get_scr_act(NULL); /* Get current screen */
                /*Create a Label on the currently active screen*/
                lv_obj_t *label1 = lv_label_create(screen, NULL);

                /*Modify the Label's text*/
                std::string time = StoreProvider::GetInstance().GetTimeStore().GetTimeString();
                lv_label_set_text(label1, time.c_str());

                /* Align the Label to the center
                * NULL means align on parent (which is the screen now)
                * 0, 0 at the end means an x, y offset after alignment*/
                lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0); 
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_task_handler();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UiController::TaskUiStop()
{
    vTaskSuspend(mUiTaskHandle);
}
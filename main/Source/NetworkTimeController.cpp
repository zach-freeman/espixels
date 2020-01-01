//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#include "NetworkTimeController.hpp"
#include "Dispatcher.hpp"
#include "esp_log.h"
#include "esp_sntp.h"
#include "TaskBase.hpp"
#include "TaskConfig.hpp"
#include "TimeAction.hpp"

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
static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGE(TIME_STORE_TAG, "Notification of a time synch event");
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static void NetworkTimeTaskRunner(void * params)
{
    static_cast<NetworkTimeController *>(params)->NtpTask();
}

//==============================================================================
// Method definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
NetworkTimeController::NetworkTimeController(TimeStore &timeStore)
    : mTimeStore(timeStore)
{
    mTimeStore.Subscribe(this);
    InitializeSntp();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void *NetworkTimeController::ProcessChange(StoreSubscriber::ChangeType changeType,
                                           Action::Source source) const
{
    if (source == Action::Source::Time)
    {
        Event_t event{};
        event.updateTime = 1;
        xTaskNotify(mNtpSyncTaskHandle, event.rawBits, eSetBits);
    }
    return nullptr;
}                                           

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkTimeController::StartTask()
{
    if (xTaskCreate(NetworkTimeTaskRunner,
                    "ntp_synch",
                    NETWORK_TIME_SYNCH_TASK_SIZE,
                    this,
                    NETWORK_TIME_SYNCH_TASK_PRIORITY,
                    &mNtpSyncTaskHandle)
        != pdPASS)
    {

    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkTimeController::NtpTask()
{
    while (FOREVER())
    {
        Event_t event{};
        if (xTaskNotifyWait(ULONG_MAX, ULONG_MAX, &event.rawBits, portMAX_DELAY) == pdTRUE)
        {
            if (event.updateTime)
            {
                // wait for time to be set
                time_t now = 0;
                struct tm timeinfo = {};
                int retry = 0;
                const int retry_count = 10;
                while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
                    ESP_LOGI(NETWORK_TIME_CONTROLLER_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
                    vTaskDelay(pdMS_TO_TICKS(SNTP_SYNC_STATUS_DELAY));
                }
                time(&now);
                char strftime_buf[64];
                // Set timezone to Eastern Standard Time and print local time
                setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
                tzset();
                localtime_r(&now, &timeinfo);
                strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
                ESP_LOGI(NETWORK_TIME_CONTROLLER_TAG, "The current date/time in New York is: %s", strftime_buf);
                TimeAction timeAction(TimeActionType::None);
                timeAction.SetTime(strftime_buf);
                Dispatcher::GetInstance().SendAction(timeAction, portMAX_DELAY);
            }
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkTimeController::InitializeSntp()
{
    ESP_LOGE(NETWORK_TIME_CONTROLLER_TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}
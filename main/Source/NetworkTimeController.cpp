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
#include "UIAction.hpp"

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
static void TimeUpdateCallback(TimerHandle_t context)
{
    NetworkTimeController *ntController =
        reinterpret_cast<NetworkTimeController *>(pvTimerGetTimerID(context));
    ntController->UpdateTime();
}

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
    mTimeUpdateTimer = xTimerCreate("timer",
                                    pdMS_TO_TICKS(TIME_UPDATE_TIMER_MS),
                                    pdTRUE,
                                    reinterpret_cast<void *>(this),
                                    TimeUpdateCallback);
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
        event.initializeTime = 1;
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
        ESP_LOGE(NETWORK_TIME_CONTROLLER_TAG, "Error: can't start ntp task");
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
            if (event.initializeTime)
            {
                // wait for time to be set
                time_t now = 0;
                struct tm timeinfo = {};
                int retry = 0;
                const int retry_count = 10;
                while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
                    ESP_LOGI(NETWORK_TIME_CONTROLLER_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
                    vTaskDelay(pdMS_TO_TICKS(SNTP_SYNC_STATUS_DELAY_MS));
                }
                time(&now);
                char strftime_buf[64];
                // print local time
                localtime_r(&now, &timeinfo);
                strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
                ESP_LOGI(NETWORK_TIME_CONTROLLER_TAG, "The current date/time in New York is: %s", strftime_buf);
                // we got the time so now we can start the timer that updates the time
                xTimerStart(mTimeUpdateTimer, 0);
                struct timeval tv_now = { .tv_sec = now, .tv_usec = 0 };
                settimeofday(&tv_now, NULL);
                // send the set time action
                TimeAction timeAction(TimeActionType::None);
                timeAction.SetTime(strftime_buf);
                Dispatcher::GetInstance().SendAction(timeAction, portMAX_DELAY);
                // send the ui action
                UIAction uiAction(UIActionType::None);
                uiAction.ShowTime();
                Dispatcher::GetInstance().SendAction(uiAction, portMAX_DELAY);
            }
            else if (event.updateTime)
            {
                struct timeval tv_now = {};
                struct timezone tzone = {};
                gettimeofday(&tv_now, &tzone);
                struct tm timeinfo = {};
                time_t now = (time_t)tv_now.tv_sec;
                char strftime_buf[64];
                // print local time
                localtime_r(&now, &timeinfo);
                strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
                // send the set time action
                TimeAction timeAction(TimeActionType::None);
                timeAction.SetTime(strftime_buf);
                Dispatcher::GetInstance().SendAction(timeAction, portMAX_DELAY);
                // send the ui action
                UIAction uiAction(UIActionType::None);
                uiAction.ShowTime();
                Dispatcher::GetInstance().SendAction(uiAction, portMAX_DELAY);
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
    // Set timezone to Eastern Standard Time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
}

void NetworkTimeController::UpdateTime()
{
    Event_t event{};
    event.updateTime = 1;
    xTaskNotify(mNtpSyncTaskHandle, event.rawBits, eSetBits);
}
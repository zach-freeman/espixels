#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "ZAssert.hpp"
#include "NetworkAction.hpp"
#include "NetworkTimeController.hpp"
#include "Dispatcher.hpp"
#include "NetworkStore.hpp"
#include "PrivateInfo.h"
#include "StoreProvider.hpp"
#include "TaskRegistry.hpp"
#include "TimeStore.hpp"
#include "UIAction.hpp"
#include "UiController.hpp"
#include "UIStore.hpp"
#include "esp_log.h"
#include "mdns.h"
#include "nvs_flash.h"
//==============================================================================
// Private defines and constants
//==============================================================================
//==============================================================================
// Private typedefs
//==============================================================================
class SingletonInitializer
{
public:
    SingletonInitializer()
    {
        ESP_LOGI("Singleton", "Initialize singletons!");
        StoreProvider::CreateMutex();
        Dispatcher::CreateMutex();
        TaskRegistry::CreateMutex();
    }
};

//==============================================================================
// Private variables
//==============================================================================
// ESP guarantees that this constructor is called before the scheduler starts.
static SingletonInitializer singletonInitializer;
//==============================================================================
// Private function definitions
//==============================================================================

//==============================================================================
// Public function definitions
extern "C" void app_main()
{
    printf("Hello zach!\n");
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    StoreProvider::CreateMutex();
    Dispatcher::CreateMutex();
    Dispatcher::GetInstance().StartTask();

    NetworkStore &networkStore{*(new NetworkStore())};
    Dispatcher::GetInstance().Subscribe(networkStore);
    StoreProvider::GetInstance().SetNetworkStore(networkStore);

    TimeStore &timeStore{*(new TimeStore())};
    Dispatcher::GetInstance().Subscribe(timeStore);
    StoreProvider::GetInstance().SetTimeStore(timeStore);

    NetworkTimeController &networkTime{*(new NetworkTimeController(timeStore))};
    networkTime.StartTask();
    (void)networkTime;

    UIStore &uiStore{*(new UIStore())};
    Dispatcher::GetInstance().Subscribe(uiStore);
    StoreProvider::GetInstance().SetUIStore(uiStore);

    UiController &uiController{*(new UiController(uiStore))};
    uiController.StartUiTask();
    (void)uiController;

    NetworkAction networkAction = NetworkAction(NetworkActionType::ClientConnect);
    uint8_t *ssid = PrivateInfo::GetSsid();
    networkAction.SetSsid(ssid, sizeof(ssid));
    uint8_t *passphrase = PrivateInfo::GetPassphrase();
    networkAction.SetPassphrase(passphrase, sizeof(passphrase));
    Dispatcher::GetInstance().SendAction(networkAction);

    UIAction uiAction = UIAction(UIActionType::None);
    uiAction.Initialize();
    Dispatcher::GetInstance().SendAction(uiAction);

    for (;;)
    {
        ESP_LOGE("Heap", "%u", esp_get_free_heap_size());
        ESP_LOGE("HeapMin", "%u", esp_get_minimum_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

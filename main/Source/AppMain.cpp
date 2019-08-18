#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "ZAssert.hpp"
#include "Dispatcher.hpp"
#include "NetworkStore.hpp"
#include "TaskRegistry.hpp"
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
        NetworkStore::CreateMutex();
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

    Dispatcher::GetInstance().Subscribe(NetworkStore::GetInstance());
    TaskRegistry::GetInstance().StartTasks();
    for (;;)
    {
        ESP_LOGE("Heap", "%u", esp_get_free_heap_size());
        ESP_LOGE("HeapMin", "%u", esp_get_minimum_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

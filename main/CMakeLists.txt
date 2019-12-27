
idf_component_register(SRCS "Source/AppMain.cpp" 
                            "Source/Action.cpp" 
                            "Source/Dispatcher.cpp" 
                            "Source/EspWifi.cpp"
                            "Source/TaskRegistry.cpp"
                            "Source/NetworkAction.cpp"  
                            "Source/NetworkStore.cpp"
                            "Source/TimeAction.cpp"
                            "Source/TimeStore.cpp"
                            "Source/StoreProvider.cpp"
                    INCLUDE_DIRS "Include"
                                 "../esp-idf/components/freertos/include/freertos"
                    REQUIRES lvgl_ili9341 lvgl spi_flash nvs_flash mdns)
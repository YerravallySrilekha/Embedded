Prerequisites:
•	ESP-IDF installed and configured.
•	An ESP32 board.
•	A smartphone (Android/iOS) to run the ESP Provisioning app.
1. Create a New ESP-IDF Project:
idf.py create-project esp_prov_example
cd esp_prov_example
2. Add Required Components
In your CMakeLists.txt, make sure to include the required provisioning components:
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash app_update wifi_prov manager bt)
3. Configure the Project
Before building the project, configure it by running:
idf.py menuconfig
Make sure to enable Bluetooth in Component config > Bluetooth and configure Wi-Fi SSID and password in Example Connection 
4. Build and Flash the Project
Now, build and flash your project onto the ESP32 device:
idf.py build
idf.py flash monitor

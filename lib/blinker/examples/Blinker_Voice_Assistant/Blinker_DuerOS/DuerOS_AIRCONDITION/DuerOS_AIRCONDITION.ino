/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.7.4 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Make sure installed 1.0.5 or later ESP32/Arduino package,
 * if use ESP32 with Blinker.
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * Docs: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.5 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_DUEROS_AIRCONDITION

#include <Blinker.h>

char auth[] = "Your Device Secret Key";
char ssid[] = "Your WiFi network SSID or name";
char pwd[] = "Your WiFi network WPA password or WEP key";

#define BUTTON_1 "ButtonKey"

BlinkerButton Button1(BUTTON_1);

bool oState = false;
bool hsState = false;
bool vsState = false;
uint8_t setLevel;
uint8_t setTemp;
String wsMode = "AUTO";

void duerLevel(uint8_t level)
{
    BLINKER_LOG("need set level: ", level);
    // 0:AUTO MODE, 1-3 LEVEL

    setLevel = level;

    BlinkerDuerOS.level(level);
    BlinkerDuerOS.print();
}

void duerRelativeLevel(int32_t level)
{
    BLINKER_LOG("need set relative level: ", level);
    // 0:AUTO MODE, 1-3 LEVEL

    setLevel += level;

    BlinkerDuerOS.level(setLevel);
    BlinkerDuerOS.print();
}

void duerTemp(uint8_t temp)
{
    BLINKER_LOG("need set temp: ", temp);

    setTemp = temp;

    BlinkerDuerOS.temp(temp);
    BlinkerDuerOS.print();
}

void duerRelativeTemp(int32_t temp)
{
    BLINKER_LOG("need set relative temp: ", temp);

    setTemp += temp;

    BlinkerDuerOS.temp(setTemp);
    BlinkerDuerOS.print();
}

void duerMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);
    // COOL：制冷
    // HEAT：制热
    // AUTO：自动
    // FAN：送风
    // DEHUMIDIFICATION：除湿
    // SLEEP：睡眠
    // CLEAN：净化
    // NAI：负离子
    // NO_WIND_FEELING: 无风感
    // NO_UP_WIND_FEELING: 上无风感
    // NO_DOWN_WIND_FEELING: 下无风感
    // UP_DOWN_SWING: 上下摆风
    // LEFT_RIGHT_SWING: 左右摆风

    wsMode = mode;

    BlinkerDuerOS.mode(mode);
    BlinkerDuerOS.print();
}

void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);

    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        Button1.print("on");

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        Button1.print("off");

        oState = false;
    }

    BlinkerDuerOS.powerState(oState ? "on" : "off");
    BlinkerDuerOS.report();
    
    digitalWrite(LED_BUILTIN, oState);
}

void duerPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);

        BlinkerDuerOS.powerState("on");
        BlinkerDuerOS.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();

        oState = false;
    }
}

void duerQuery(int32_t queryCode)
{
    BLINKER_LOG("DuerOS Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :        
            BLINKER_LOG("DuerOS Query power state");
            BlinkerDuerOS.powerState(oState ? "on" : "off");
            BlinkerDuerOS.level(setLevel);
            BlinkerDuerOS.temp(setTemp);
            BlinkerDuerOS.mode(wsMode);
            BlinkerDuerOS.print();
            break;
        case BLINKER_CMD_QUERY_TIME_NUMBER :
            BLINKER_LOG("DuerOS Query time");
            BlinkerDuerOS.time(millis());
            BlinkerDuerOS.print();
            break;
        default :
            BlinkerDuerOS.powerState(oState ? "on" : "off");
            BlinkerDuerOS.level(setLevel);
            BlinkerDuerOS.temp(setTemp);
            BlinkerDuerOS.mode(wsMode);
            BlinkerDuerOS.print();
            break;
    }
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    // Blinker.vibrate();
    
    // uint32_t BlinkerTime = millis();
    
    // Blinker.print("millis", BlinkerTime);

    BlinkerDuerOS.powerState("off");
    BlinkerDuerOS.report();
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth, ssid, pwd);
    Blinker.attachData(dataRead);
    
    BlinkerDuerOS.attachPowerState(duerPowerState);
    BlinkerDuerOS.attachLevel(duerLevel);
    BlinkerDuerOS.attachRelativeLevel(duerRelativeLevel);
    BlinkerDuerOS.attachTemp(duerTemp);
    BlinkerDuerOS.attachRelativeTemp(duerRelativeTemp);
    BlinkerDuerOS.attachMode(duerMode);
    BlinkerDuerOS.attachQuery(duerQuery);
}

void loop()
{
    Blinker.run();
}
#include <Arduino.h>

#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define BLINKER_APCONFIG

#include "../lib/blinker/src/Blinker.h"

#define RESET_WIFI_BTN_PIN D5
#define OPEN_BTN_PIN D0
#define STOP_BTN_PIN D1
#define CLOSE_BTN_PIN D2
#define BTN_ON_VALUE LOW
#define BTN_OFF_VALUE HIGH

char auth[] = "b25ce3128acc";

// 新建组件对象
BlinkerButton Button1("btn1");//你的按钮名称


// 开关进度
unsigned long fullProcessTime = 20000; // 全部进度需要多长时间
int curProcess = 0;
int targetProcess = 0;
unsigned long updateProcessTime = 0;

void updateProcess(int target) {
    targetProcess = target;
    updateProcessTime = millis();
}

void miotPowerState(const String &state) {
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        curProcess = 0; // 强制完全打开
        updateProcess(100);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    } else if (state == BLINKER_CMD_OFF) {
        curProcess = 100; // 强制完全关闭
        updateProcess(0);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
}

void miotBright(const String &bright) {
    BLINKER_LOG("need set brightness: ", bright);
    BLINKER_LOG("cur brightness: ", curProcess);

    updateProcess(bright.toInt());

    BlinkerMIOT.brightness(targetProcess);
    BlinkerMIOT.print();
}

void dataRead(const String &data) {
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();

    uint32_t BlinkerTime = millis();

    Blinker.print("millis", BlinkerTime);
}

void setup() {
// write your initialization code here
    Serial.begin(115200);
    pinMode(RESET_WIFI_BTN_PIN, INPUT_PULLUP);
    pinMode(OPEN_BTN_PIN, OUTPUT);
    pinMode(STOP_BTN_PIN, OUTPUT);
    pinMode(CLOSE_BTN_PIN, OUTPUT);
    delay(100);
    digitalWrite(OPEN_BTN_PIN, BTN_OFF_VALUE);
    digitalWrite(STOP_BTN_PIN, BTN_OFF_VALUE);
    digitalWrite(CLOSE_BTN_PIN, BTN_OFF_VALUE);

    Blinker.begin(auth);
    BLINKER_DEBUG.stream(Serial);
    Blinker.attachData(dataRead);
    Button1.attach(miotPowerState);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachBrightness(miotBright);
}

void clickBtn(uint8_t pin) {
    digitalWrite(pin, BTN_ON_VALUE);
    delay(100);
    digitalWrite(pin, BTN_OFF_VALUE);
}

void doAction() {
    if (updateProcessTime <= 0) {
        return;
    }
    int deltaProcess = targetProcess - curProcess;
    if (deltaProcess != 0) {
        auto needTime = (unsigned long) (((float) abs(deltaProcess) / 100.0) * fullProcessTime);
        BLINKER_LOG("curProcess: ", curProcess, " targetProcess: ", targetProcess, " needTime: ", needTime);
        if (deltaProcess > 0) {
            clickBtn(OPEN_BTN_PIN);
        } else {
            clickBtn(CLOSE_BTN_PIN);
        }
        delay(needTime);
        clickBtn(STOP_BTN_PIN);
    }
    curProcess = targetProcess;
    updateProcessTime = 0;
    BLINKER_LOG("finish, curProcess: ", curProcess);
}

void loop() {
    Blinker.run();
    doAction();
    // 重置wifi
    if (digitalRead(RESET_WIFI_BTN_PIN) == LOW) {
        Serial.println("RESET_WIFI start");
        delay(3000);
        if (digitalRead(RESET_WIFI_BTN_PIN) == LOW) {
            Serial.println("RESET_WIFI end");
            Blinker.reset();
        }
    }
}
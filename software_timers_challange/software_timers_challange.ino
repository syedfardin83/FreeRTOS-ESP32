#include "driver/timer.h"

#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

String inputString = "";
bool stringEnd = false;
char ch;

static TimerHandle_t timerA = NULL;

#define led_pin NULL

void timerCallback(TimerHandle_t timer){
  digitalWrite(led_pin, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vTaskDelay(2000/portTICK_PERIOD_MS);
  Serial.println("------FREE RTOS Started--------");

  pinMode(led_pin,OUTPUT);
  digitalWrite(led_pin, HIGH);


  timerA = xTimerCreate(
    "Timer A",
    5000/portTICK_PERIOD_MS,
    pdTRUE,
    (void *)0,
    timerCallback
  );
}

void loop() {
  // put your main code here, to run repeatedly:
    if(SerialBT.available()){
      ch = SerialBT.read();
      if(ch=='\n'){
        stringEnd = true;
        inputString.trim();
      }else{
        inputString+=ch;
      }
    }
    if(stringEnd){
      Serial.println(inputString);

      inputString = "";
      stringEnd = false;

      xTimerReset(timerA,portMAX_DELAY);
    }
}

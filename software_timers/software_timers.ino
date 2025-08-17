#include "driver/timer.h"

#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

static TimerHandle_t one_shot_timer = NULL;

void timerCallback(TimerHandle_t timer){
  Serial.println("Timer Expired!");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vTaskDelay(2000/portTICK_PERIOD_MS);
  Serial.println("------FREE RTOS Started--------");

  one_shot_timer = xTimerCreate(
    "One-shot timer",
    3000/portTICK_PERIOD_MS,
    pdTRUE,
    (void *)0,
    timerCallback
  );
  if(one_shot_timer == NULL){
    Serial.println("Failed to create timers!");
    while(1){}
  }
  xTimerStart(one_shot_timer, portMAX_DELAY);

  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}

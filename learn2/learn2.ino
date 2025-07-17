#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

void blinkLED1(void *parameter){
  while(1){
    digitalWrite(13,HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(13,LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void blinkLED2(void *parameter){
  while(1){
    vTaskDelay(200/portTICK_PERIOD_MS);
    digitalWrite(13,HIGH);
    vTaskDelay(200/portTICK_PERIOD_MS);
    digitalWrite(13,LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  xTaskCreatePinnedToCore(
    blinkLED1,
    "BLINK LED 1",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );
  xTaskCreatePinnedToCore(
    blinkLED2,
    "BLINK LED 2",
    1024,
    NULL,
    2,
    NULL,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}

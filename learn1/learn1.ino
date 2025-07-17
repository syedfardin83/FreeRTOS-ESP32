//Check if esp32 is in dual core mode and if it is, use core 1 to prevent core 0 overload.
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1; 
#endif

static const int led_pin = 13; 

void blinkLED(void *parameter){
  while(1){
    digitalWrite(led_pin,HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_pin,LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  } 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin,OUTPUT);
  xTaskCreatePinnedToCore(
    blinkLED, //Function name
    "BLINK LED",  //Name of task 
    1024, //Stack size
    NULL, //Parameters
    1,  //Priority (0 to 25)
    NULL, //task handler
    app_cpu //Which core to run on
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}

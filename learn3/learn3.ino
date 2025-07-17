//LED blink frequency control - using two tasks
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int delay_time = 500;
bool stringComplete = false;
String inputString = "";
char ch;

void blinkLED(void *param){
  while(1){
    digitalWrite(13,HIGH);
    vTaskDelay(delay_time/portTICK_PERIOD_MS);
    digitalWrite(13,LOW);
    vTaskDelay(delay_time/portTICK_PERIOD_MS);
  }
}

void listen_update(void *param){
  while(1){
    if(Serial.available()){
      ch = (char)Serial.read();
      if (ch == '\n') {
        stringComplete = true;
      } else {
        inputString += ch;
      }
    }
    if(stringComplete){
      inputString.trim();
      delay_time = inputString.toInt();
      stringComplete = false;
      inputString = "";
    }

  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(
    blinkLED,
    "blink LED",
    3072,
    NULL,
    1,
    NULL,
    app_cpu
  );
  vTaskDelay(1000/portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(
    listen_update,
    "Listen and update",
    3072,
    NULL,
    1,
    NULL,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

static const uint8_t msg_queue_len = 5;

static QueueHandle_t msg_queue;

void printMsg(void *parameters){
  int item;

  while(1){
    if(xQueueReceive(msg_queue,(void*)&item,0)==pdTRUE){
      Serial.println(item);
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("---------FREE RTOS Started--------------");

  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  xTaskCreatePinnedToCore(
    printMsg,
    "Message Prinitng",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  static int num = 0;

  if(xQueueSend(msg_queue,(void*)&num,10)!=pdTRUE){
    Serial.println("Queue Full!");
  }
  num++;
  vTaskDelay(500/portTICK_PERIOD_MS);
}

#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

//  Queues
static QueueHandle_t arr_q = NULL;
int arr_qL = 5;

//  Mutexes
static SemaphoreHandle_t avg_m = NULL;

//  Timer
static uint16_t prescaler = 8;
static uint64_t timer_max = 1000000;
static hw_timer_t *my_timer = NULL;


static int isr_buffer[10];int p = 0;

//  Global shared variables
static float avg = 0;

//  Serial read variables
String inputString = "";

//  ISR
void IRAM_ATTR timer_interrupt(){
  int inp = analogRead(adc_pin);
  isr_buffer[p] = inp;

  BaseType_t task_woken = pdFALSE;
  p++;
  if(p==10){
    p=0;
    xQueueSendFromISR(arr_q, isr_buffer, &task_woken);
  }
  
  if(task_woken){
    portYIELD_FROM_ISR();
  }

}

void taskA(void* param){
  static int local_buffer[10];float local_avg = 0;static int i;

  while(1){
    if(xQueueReceive(arr_q, local_buffer, portMAX_DELAY)==pdTRUE){
      local_avg=0;
      for(i=0;i<10;i++){
        local_avg+=local_buffer[i];
      }

      local_avg=local_avg/10;

      xSemaphoreTake(avg_m, portMAX_DELAY);
      avg = local_avg;
      xSemaphoreGive(avg_m);

    }
    vTaskDelay(5);
  }
}

void taskB(void* param){
  static float local_avg = 0; 
  while(1){
    if(Serial.available()){
      inputString = Serial.readString();
      inputString.trim();

      if(inputString=="avg"){
        xSemaphoreTake(avg_m, portMAX_DELAY);
        local_avg = avg;
        xSemaphoreGive(avg_m);

        Serial.print("Average: ");
        Serial.println(local_avg);
      }else{
        Serial.println(inputString);
      }
    }
    vTaskDelay(5);
  }
}

#define adc_pin 34

void setup() {
  pinMode(adc_pin,INPUT);
  Serial.begin(9600);
  Serial.println("--------Serial Started------------");

  arr_q = xQueueCreate(arr_qL, sizeof(int)*10);
  avg_m = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    taskA,
    "task A",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    taskB,
    "task B",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );

  my_timer = timerBegin(0, prescaler, true);
  timerAttachInterrupt(my_timer, &timer_interrupt, true);
  timerAlarmWrite(my_timer, timer_max, true);
  timerAlarmEnable(my_timer);

  vTaskDelete(NULL);
}

void loop() {

}

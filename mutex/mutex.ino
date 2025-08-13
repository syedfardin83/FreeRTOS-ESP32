#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

int global_var = 0;
static SemaphoreHandle_t mutex;

void incTask(void *param){
  Serial.println("Task started");
  int local_var;
  while(1){

    if(xSemaphoreTake(mutex,0)==pdTRUE){
      local_var = global_var;
      local_var++;
      global_var = local_var;

      xSemaphoreGive(mutex);
    } 

    Serial.println(global_var);
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("----------FREE RTOS Started----------------");
  mutex = xSemaphoreCreateMutex();

  // put your setup code here, to run once:
  xTaskCreatePinnedToCore(
    incTask,
    "Task 1",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    incTask,
    "Task 2",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}

#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

static const uint64_t NUM_TASKS = 5;
static const uint64_t STACK_SIZE = 2048;

//  Semaphores
static SemaphoreHandle_t bin_sem;
static SemaphoreHandle_t done_sem;
static SemaphoreHandle_t chopsticks[NUM_TASKS];

//  The heirarchial solution
void eat_h(void *param){

  int num, stick_l, stick_r;
  num = *(int*)param;
  xSemaphoreGive(bin_sem);

  if(num==4){
    stick_r = 0;
    stick_l = 4;
  }else{
    stick_r = num;
    stick_l = num+1;
  }

  //  Take Right chopstick
  xSemaphoreTake(chopsticks[stick_r],portMAX_DELAY);
  Serial.print("Philosopher ");
  Serial.print(num);
  Serial.print(" took chopstick ");
  Serial.println(stick_r);

  //  Wait for a while
  vTaskDelay(1/portTICK_PERIOD_MS);

  //  Take left chopstick
  xSemaphoreTake(chopsticks[stick_l],portMAX_DELAY);
  Serial.print("Philosopher ");
  Serial.print(num);
  Serial.print(" took chopstick ");
  Serial.println(stick_l);

  //  Do some task
  Serial.print("Philosopher ");
  Serial.print(num);
  Serial.println(" is eating...");

  //  Give Right Chopstick
  xSemaphoreGive(chopsticks[stick_r]);

  //  Wait for a while
  vTaskDelay(1/portTICK_PERIOD_MS);

  //  Give Left Chopstick
  xSemaphoreGive(chopsticks[stick_l]);


  //  Give done_sem
  xSemaphoreGive(done_sem);
  Serial.print("Philosopher ");
  Serial.print(num);
  Serial.println(" is done!");

  //  Delete self
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n-------Free RTOS Philosopher problem---------");
  vTaskDelay(1000/portTICK_PERIOD_MS);

  //  Initialize all semaphores
  bin_sem = xSemaphoreCreateBinary();
  done_sem = xSemaphoreCreateCounting(NUM_TASKS,0);
  for(int i=0;i<NUM_TASKS;i++){
    chopsticks[i] = xSemaphoreCreateMutex();
  }

  for(int i=0;i<NUM_TASKS;i++){
    xTaskCreatePinnedToCore(
      eat_h,
      "Heirarchial eating",
      STACK_SIZE,
      (void*)&i,
      1,
      NULL,
      app_cpu
    );
    xSemaphoreTake(bin_sem, portMAX_DELAY);
  }

  for(int i=0;i<NUM_TASKS;i++){
    xSemaphoreTake(done_sem,portMAX_DELAY);
  }

  Serial.println("No deadlock occured!");
}

void loop() {
  // put your main code here, to run repeatedly:

}

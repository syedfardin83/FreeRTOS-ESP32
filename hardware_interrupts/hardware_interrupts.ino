#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

//Timer settings
static uint16_t prescaler = 80;
static unint64_t timer_max = 1000000;

//Timer handler
static hw_timer_t *my_timer = NULL;

static volatile int isr_counter = 0;

//  Spin Lock Handler
static portMUX_TYPE my_spinlock = portMUX_INITIALIZER_UNLOCKED;


//ISR defined in the internal RAM (not in Flash memory) for faster access
void IRAM_ATTR myTimerInterrupt(){

  portENTER_CRITICAL(&my_spinlock);

  isr_counter++;

  portEXIT_CRITICAL(&my_spinlock);

}

void printTask(void* params){
  while(1){
    Serial.println(isr_counter);

    portENTER_CRITICAL(&my_spinlock);
    isr_counter--;
    portEXIT_CRITICAL(&my_spinlock);
  }
}


void setup() {
  xTaskCreatePinnedToCore(
    printTask,
    "Print Task",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  //  Create and start timer: (timer, prescaler, count up)
  my_timer = timerBegin(0, prescaler, true);

  //  Providing ISR : (timer handler, function, edge)
  timerAttachInterrupt(my_timer, &myTimerInterrupt, true);

  //  Setting the alarm count :(,,autoreload)
  timerAlarmWrite(my_timer, timer_max, true);

  //  Allowing ISR to trigger
  timerAlarmEnable(ny_timer);

  vTaskDelete(NULL);

}

void loop() {

}

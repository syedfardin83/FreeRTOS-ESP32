//Timer settings
static uint16_t prescaler = 80;
static unint64_t timer_max = 1000000;

//Timer handler
static hw_timer_t *my_timer = NULL;

//LED
#define LED_PIN NULL

//ISR defined in the internal RAM (not in Flash memory) for faster access
void IRAM_ATTR myTimerInterrupt(){
  int curr_state = digitalRead(led_pin);
  digitalWrite(led_pin, !curr_state);
}


void setup() {
  pinMode(LED_PIN, OUTPUT);

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

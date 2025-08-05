#if CONFIG_FREERTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

char* ptr;int length = 0;
int buff_size = 255;
bool flag = false;


void task1(void *param){
  char ch;
  char buff[buff_size];
  int index = 0;
  int i;
  Serial.println("Task1 started");

  while(1){
    if(Serial.available()){
      ch = Serial.read();
      if(ch=='\n'){
        ptr = (char*)pvPortMalloc(sizeof(char)*(index+1));
        for(i=0;i<=index;i++){
          ptr[i] = buff[i];
        }
        length = index+1;
        flag = true;
        index = 0;
        Serial.print("Echo: ");
        for(i=0;i<length;i++){
          Serial.print(ptr[i]);
        }
        Serial.println("");
        continue;
      }
      buff[index] = ch;
      index++; 
    }
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void task2(void *param){
  int i;
  Serial.println("Task 2 started");
  while(1){
    if(flag){
      // Serial.println("Flagged");
      for(i=0;i<length;i++){
        Serial.print(ptr[i]);
      }
      Serial.println("");
      vPortFree(ptr);
      flag = false;
    }
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Serial port started");
  xTaskCreatePinnedToCore(
    task1,
    "task1",
    (buff_size*sizeof(char))+800,
    NULL,
    1,
    NULL,
    app_cpu
  );
    xTaskCreatePinnedToCore(
    task2,
    "task2",
    1024,
    NULL,
    2,
    NULL,
    app_cpu
  );
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}

#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

static const uint8_t max_queue_len = 5;

static QueueHandle_t queue1;
static QueueHandle_t queue2;

static const uint8_t led_pin = 23;

static const int buff_size = NULL;

String *commands;
String inputString = "";
bool stringEnd = false;



String* splitBySpace(String str){
  static String words[10];int i=0;String word = "";int wordIndex = 0;
  while(i<str.length()){
    if((str[i]==' ')){
      words[wordIndex] = word;
      word="";
      wordIndex++;i++;
      continue;
    }
    if(i==str.length()-1){
      word+=str[i];
      words[wordIndex] = word;break;
    }
    word+=str[i];
    i++;
  }
  return words;
}

void taskA(void *param){
  Serial.println("Task A Started!");
  char msg[8];char ch;int delay;
  while(1){
    if(xQueueReceive(queue2,msg,0)==pdTRUE){
      Serial.println(msg);
    }

    if(Serial.available()){
      ch = Serial.read();\
      inputString+=ch;
      if(ch=='\n'){
        inputString.trim();
        stringEnd = true;
      }
    }
    if(stringEnd){
      Serial.println(inputString);
      commands = splitBySpace(inputString);
      inputString = "";
      stringEnd = false;

      if(commands[0]=="delay"){
        delay = commands[1].toInt();
        if(xQueueSend(queue1,(void*)&delay,10)!=pdTRUE){
          Serial.println("Queue1 Full!");
        }
      }
    }
    vTaskDelay(10/portTICK_PERIOD_MS);

  }
}

void taskB(void *param){
  Serial.println("Task B Started!");
  int t=500, count=0, item;
  char msg[8];
  while(1){
    if(xQueueReceive(queue1, (void*)&item,10)==pdTRUE){
      t=item;
    }
    digitalWrite(led_pin,HIGH);
    vTaskDelay(t/portTICK_PERIOD_MS);
    digitalWrite(led_pin,LOW);
    vTaskDelay(t/portTICK_PERIOD_MS);
    count++;

    if(count>=5){
      strcpy(msg,"Blinked");
      if(xQueueSend(queue2, msg, 5)!=pdTRUE){
        Serial.println("Queue2 Full!");
      }
      count = 0;
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("------------FREE RTOS Started--------------------");

  pinMode(led_pin,OUTPUT);
  digitalWrite(led_pin,LOW);


  queue1 = xQueueCreate(max_queue_len,sizeof(int));
  queue2 = xQueueCreate(max_queue_len,sizeof(char)*10);

  xTaskCreatePinnedToCore(
    taskA,
    "Task A",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    taskB,
    "Task B",
    2048,
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

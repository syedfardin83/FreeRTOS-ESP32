#if CONFIG_FREE_RTOS_UNICORE
static const int app_cpu = 0;
#else
static const int app_cpu = 1;
#endif

static const uint8_t max_queue_len = 5;

static QueueHandle_t queue1;
static QueueHandle_t queue2;

static const uint8_t led_pin = NULL;

static const int buff_size = NULL;

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
  char msg[10];char ch;String *commands;int msg;
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
      inputSring = "";
      stringEnd = false;

      if(commands[0]=="delay"){
        msg = commands[1].toInt();
        if(xQueueSend(queue1,(void*)&msg,10)!=pdTRUE){
          Serial.println("Queue1 Full!");
        }
      }
    }

  }
}

void taskB(void *param){
  int t=500, count=0, item;
  char msg[10];
  while(1){
    if(xQueueReceive(queue1, (void*)&item,10)==pdTRUE){
      t=item;
    }
    digitalWrite(led_pin,HIGH);
    vTaskDelay(t/portTICK_PERIOD_MS);
    digitalWrite(led_pin,LOW);
    vTaskDelay(t/portTICK_PERIOD_MS);
    count++;

    if(count>=100){
      msg = "Blinked";
      if(xQueueSend(queue2, msg, 5)!=pdTRUE){
        Serial.println("Queue2 Full!");
      }
    }
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
}

void loop() {
  // put your main code here, to run repeatedly:

}

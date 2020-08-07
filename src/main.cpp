#include <Arduino.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle  nh;

TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int led1 = 2;
const int led2 = 4;

int a = 0;
//char chr_msg_receive[20] = "";
//String str_msg = "";
std_msgs::String str_msg;


void messageCb( const std_msgs::String& msg){
  str_msg = msg;
}
ros::Subscriber<std_msgs::String> sub("chatter_pc", messageCb);

ros::Publisher chatter_esp("chatter_esp", &str_msg);



//Serial Core
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
  //Serial.println("Task1 running on core ");
  //Serial.println(a);
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
  } 
}

//ROS Core
void Task2code( void * pvParameters ){
  //Serial.print("Task2 running on core ");
  //Serial.println(xPortGetCoreID());

  for(;;){
    //Serial.println("Task2 running on core ");
    
    /*
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW);
    delay(700);
    */


    chatter_esp.publish( &str_msg );
    nh.spinOnce();      
  }
}

void setup() {
  nh.initNode();
  nh.advertise(chatter_esp);
  nh.subscribe(sub);

  //Serial.begin(115200); 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);



  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
      
    delay(500); 
}

void loop() {
  
}
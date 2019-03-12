#include <Arduino.h>
#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+
#include <nRF24L01.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <avr/sleep.h> //library needed to use AVR based sleep API

#include <aws_iot_mqtt.h>
#include <aws_iot_version.h>
#include "aws_iot_config.h"

aws_iot_mqtt_client myClient; // init iot_mqtt_client
char msg[32];// read-write buffer
volatile float temp;// Temperatur
float data;
int rc = -100; // return value placeholder
bool success_connect = false; // whether it is connected

const int pinCE = 7; //This pin is used to set the nRF24 to standby (0) or active mode (1)
const int pinCSN = 8; //This pin is used to tell the nRF24 whether the SPI communication is a command or message to send out
byte gotByte = 0; //used to store payload from transmit module
volatile int count = 0; //tracks the number of interrupts from IRQ
int pCount = 0; //tracks what last count value was so know when count has been updated
RF24 radio(pinCE, pinCSN); // Declare object from nRF24 library (Create your wireless SPI) 
const byte address[6] = "00001";  //Create a pipe addresses for the 2 nodes to communicate over, the "LL" is for LongLong type
void interruptFunction(); //Funktion nochmal angeben in Cpp


void setup()   {
  //for debugging purposes
  pinMode(4, OUTPUT); //set pin 4 for red led signal
  pinMode(3, OUTPUT); //set pin 3 for blue led signal
  
  radio.begin();  //Start the nRF24 module
  radio.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  radio.enableAckPayload();         //allows you to include payload on ack packet
  radio.maskIRQ(1,1,0);               //mask all IRQ triggers except for receive (1 is mask, 0 is no mask)
  radio.setPALevel(RF24_PA_MIN); //Set power level to low, won't work well at higher levels (interfer with receiver)
  radio.openReadingPipe(1,address);      //open pipe o for recieving meassages with pipe address
  radio.startListening();                 // Start listening for messages
  attachInterrupt(1, interruptFunction, FALLING);  //Create interrupt: 0 for pin 2 or 1 for pin 3, the name of the interrupt function or ISR, and condition to trigger interrupt
}

/*Note: delay or serial and interrupt can't be used together!*/
void loop() {

    
  if(pCount < count) { //If this is true it means count was interated and another interrupt occurred

    //only if connected to cloud..
    if(success_connect) {

      //convert and round  
      char floatBuffer1[8];
      byte precision = 2;
      dtostrf(temp, precision+3, precision, floatBuffer1); 

      //Temp pubish
      sprintf(msg, "Temp: %s", floatBuffer1);
      myClient.publish("Temp", msg, strlen(msg), 1, false);
    
      
      pCount = count; //set pCount equal count
      digitalWrite(4, LOW); //red off
    }else{ //if not connected, connect..
        if((rc = myClient.setup(AWS_IOT_CLIENT_ID)) == 0) {
          // Load user configuration
          if((rc = myClient.config(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT, AWS_IOT_ROOT_CA_PATH, AWS_IOT_PRIVATE_KEY_PATH, AWS_IOT_CERTIFICATE_PATH)) == 0) {
          
          
          // Use default connect: 60 sec for keepalive
            if((rc = myClient.connect()) == 0) {
            success_connect = true;
            digitalWrite(3, HIGH); //blue on -> connected
           
            }
            
          }
        }
    }
      
  }    
}

/*This is the function called when the interrupt occurs (pin 2 goes high)
this is often referred to as the interrupt service routine or ISR
This cannot take any input arguments or return anything*/
void interruptFunction() {
    count++; //up the receive counter
    
    while(radio.available()) { //get data sent from transmit
        radio.read( &data, sizeof(data)); //read one byte of data and store it in temp variable
        temp=data; //write date to volatile temp var
        digitalWrite(4, HIGH); //red on
    }
    
}
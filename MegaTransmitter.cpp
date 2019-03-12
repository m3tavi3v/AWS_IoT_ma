#include <Arduino.h>
#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+
#include <nRF24L01.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include "DHT.h" //DHT Sensor Lib
DHT dht(2,DHT22); //DHT sensorobject with data signal at pin 2

const int pinCE = 7; //This pin is used to set the nRF24 to standby (0) or active mode (1)
const int pinCSN = 8; //This pin is used to tell the nRF24 whether the SPI communication is a command or message to send out
byte counter = 1; //used to count the packets sent
RF24 wirelessSPI(pinCE, pinCSN); // Create your nRF24 object or wireless SPI connection
const byte address[6] = "00001";              // Radio pipe addresses for the 2 nodes to communicate.
float temp; //define var for temp-value

void setup()  
{
  Serial.begin(57600);   //start serial to communicate process
  pinMode(13, OUTPUT); //set pin 4 for led signal
  wirelessSPI.begin();            //Start the nRF24 module
  wirelessSPI.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  wirelessSPI.enableAckPayload();               // Allow optional ack payloads
  wirelessSPI.setPALevel(RF24_PA_MIN);
  wirelessSPI.openWritingPipe(address);        // pipe address that we will communicate over, must be the same for each nRF24 module
  wirelessSPI.stopListening();        //transmitter so stop listening for data
  randomSeed(analogRead(0));    //use random ADC value to seed random number algorithm
  dht.begin(); // start sensorfunction
}

void loop() {
  digitalWrite(13, LOW);
  delay(5000); //3sec
  temp = dht.readTemperature(); //receive temp from sensor
  Serial.println("Sending temperature: "); 
  if (!wirelessSPI.write(&temp, sizeof(temp))){  //if the send fails let the user know over serial monitor
       Serial.println("temp delivery failed");
       digitalWrite(13, HIGH);  
  }
   Serial.println(temp);   
}
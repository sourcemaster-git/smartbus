#include "TimerOne.h"                      //Include Timer1 library for using Timer1 functions
#include <LiquidCrystal.h>                 //Include LCD library for using LCD display functions     

const int rs = 12, en = 13, d4 = 8, d5 = 9, d6 = 10, d7 = 11;   //Define the LCD pins 

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                      
#include <SoftwareSerial.h>
SoftwareSerial RfidSerial(6, 7); // RX, TX
volatile unsigned int counter=0;          
 volatile unsigned int rotation=0;          

float rotationinm=0;

unsigned int speed=0;

int SW=3;
int IR1=18;
int IR2=19;

int RELAY=14;

String RFID_buff="";


int Total_Balance=1000;
float rupees;
float Remaining_Balance;

void count()  // ISR for counts from the speed sensor

{

  counter++;  //increase the counter value by one

  rotation++; //Increase the rotation value by one

  delay(10); 

} 


void timerIsr()

{

 detachInterrupt(digitalPinToInterrupt(2));          //Stops the interrupt pin 2

 Timer1.detachInterrupt();                           //Stops the timer1 interrupt


 lcd.clear();          

 

 float speed = (counter / 20.0)* 60.0;              //Calcukate speed in minute (20-No of slots in Encoder Wheel)

 float rotations = 230*( rotation / 20);            //Calculate distance in cm (230-Circumference of the wheel assumed 20- No of slots)

 rotationinm = rotations/100;


 lcd.setCursor(0,0);

 lcd.print("Dist(m):");

 lcd.print(rotationinm);                            //Display rotationinm at LCD

 lcd.setCursor(0,1);

 lcd.print("Speed(RPM):");                       

 lcd.print(speed);                                  //Dsiplay speed in RPM

 

  counter=0;                                        //Reset counter to 0 


//  int analogip = analogRead(A0);                   // Analog read from pin A0
//
//  int motorspeed = map(analogip,0,1023,0,255);     //convert digital vales 0-1023 to 0-255
//
//  analogWrite(5,motorspeed);                      //Sets PWM value at pin 5 

 

 Timer1.attachInterrupt( timerIsr );                        //Starts timer1 again

 attachInterrupt(digitalPinToInterrupt(2), count, RISING);  //Attaches interrupt at pin2 again

}


void generatefare()                           //ISR to generate the fareamount

{

   detachInterrupt(digitalPinToInterrupt(2)); //Disables the Interrupt pin at 2

   Timer1.detachInterrupt();                  //Disables the Timer1 interrupt
   rupees = rotationinm*5;              //Muliply 5 with distance travelled (Rs 5 per meter )
   digitalWrite(RELAY,LOW);
   lcd.clear();                               //Clears LCD
   lcd.print("JOURNEY END");
   lcd.setCursor(0,0);
   lcd.print("Here...");
   delay(2000);
   Remaining_Balance=Total_Balance-rupees;
   lcd.clear();                               //Clears LCD
   lcd.setCursor(0,0);
   lcd.print("Fare Rs: ");       
   lcd.print(rupees);                        //Display fare amount 
  
   lcd.setCursor(0,1);
   lcd.print("Bal Amt Rs:");       
   lcd.print(Remaining_Balance);
   delay(5000);

   Journey_End();
   while(1);
 

}
void Journey_End()
{
  int x=0;
  for(x=0;x<=1;x++)
  {
    String one = "$Your Journey End Here Fare Amount is:";
    String two = " Remaining Amount is:" ;   
    String message = one + rupees + two + Remaining_Balance+'#';
    
    // Convert String to char array
    int str_len = message.length() + 1;
    char textmessage[str_len];
    message.toCharArray(textmessage,str_len);
       Serial.println(textmessage);
       delay(1000);
  }
 delay(3000);
}

void setup() 

{

  pinMode(A0,INPUT);                 //Sets pin A0 as INPUT 

  pinMode(5,OUTPUT);                 //Sets pin 5 as OUTPUT 
  pinMode(RELAY,OUTPUT);  

  pinMode(3,INPUT_PULLUP);
  pinMode(IR1,INPUT); 
  pinMode(IR2,INPUT); 
  digitalWrite(RELAY,LOW);

  Serial.begin(9600);
  RfidSerial.begin(9600);

  lcd.begin(16,2);                   //Sets LCD as 16x2 type

  lcd.setCursor(0,0);                //The following code displays welcome messages

  lcd.print("SMART BUS");

  lcd.setCursor(0,1);

  lcd.print("TICKETING..");

  delay(3000);

  lcd.clear();

  lcd.print("LETS START :)");

  delay(1000);

  lcd.clear();


//  Timer1.initialize(1000000);          //Initilize timer1 for 1 second 
//
//  Timer1.attachInterrupt( timerIsr );  //ISR routine to be called for every one second
//
//  
//
//  attachInterrupt(digitalPinToInterrupt(2), count, RISING);     // Pin 2 as Interrupt pin with count ISR is called when LOW to RIGH happens.
//
//  attachInterrupt(digitalPinToInterrupt(3), generatefare, LOW); //Pin 3 as Interrupt pin with generatefare ISR is called when HIGH is detected.

 } 


void loop()
{
  RFID_CHECK();

}
void RFID_CHECK()
{
  
  SEAT_AVAILABILITY();
  lcd.clear();
  lcd.print("Swipe Your Card");
  delay(2000);
  while(1)
  {
    //SEAT_AVAILABILITY();
    if(RfidSerial.available()>0)
    {
      RFID_buff=RfidSerial.readString();
      Serial.print(RFID_buff);
      delay(1000);
  
      if(RFID_buff[0]=='$') 
      {
        Serial.println("Valid Card");
         lcd.clear();
         lcd.print("Valid Card");
         delay(1000);

         lcd.clear();
         lcd.print("Name:ABC Your");
         lcd.setCursor(0,1);
         lcd.print("Journey Start Here");
         Serial.println("$Name:ABC..  Your Journey Start Here#");
        
         delay(3000);
          digitalWrite(RELAY,HIGH);

         Calculate_distance_From_Interrupt();
      }
    
    }
  }
  
}
void Calculate_distance_From_Interrupt()
{
   Timer1.initialize(1000000);          //Initilize timer1 for 1 second 

  Timer1.attachInterrupt(timerIsr);  //ISR routine to be called for every one second

  attachInterrupt(digitalPinToInterrupt(2), count, RISING);     // Pin 2 as Interrupt pin with count ISR is called when LOW to RIGH happens.

  attachInterrupt(digitalPinToInterrupt(3), generatefare, LOW); //Pin 3 as Interrupt pin with generatefare ISR is called when HIGH is detected.

  

}
void SEAT_AVAILABILITY()
{
  if((digitalRead(IR1)==LOW)&&(digitalRead(IR2)==LOW))
  {
    lcd.clear();
    lcd.print("Seat Not Available");
    Serial.println("$Seat Not Available#");
    delay(1000);
    lcd.clear();
    lcd.print("Wait for the ");
    lcd.setCursor(0,1);
    lcd.print("Next bus..");
    delay(1000);
    lcd.clear();
    lcd.print("Please Reset...");
    delay(1000);
    while(1);
  }
   if((digitalRead(IR1)==LOW)&&(digitalRead(IR2)==HIGH))
  {
    lcd.clear();
    lcd.print("Seat2 Available");
    Serial.println("$Seat2 Available#");
    delay(1000);
  }
 if((digitalRead(IR1)==HIGH)&&(digitalRead(IR2)==LOW))
  {
    lcd.clear();
    lcd.print("Seat1 Available");
    Serial.println("$Seat1 Available#");
    delay(1000);
  }
   if((digitalRead(IR1)==HIGH)&&(digitalRead(IR2)==HIGH))
  {
    lcd.clear();
    lcd.print("Seat1 and 2 are");
    lcd.setCursor(0,1);
    lcd.print("Available");
    Serial.println("$Seat1 and 2 Available#");
    delay(1000);
  }

}

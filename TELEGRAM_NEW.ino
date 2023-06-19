#include<ESP8266WiFi.h>
#include<WiFiClientSecure.h>
#include<UniversalTelegramBot.h>
#include<ArduinoJson.h>
const char* ssid = "TECNO POVA";
const char* password ="onetwothre";
//  with your network credentials

// Initialize Telegram BOT
#define BOTtoken "6012716593:AAGJvivxDrZ6d-xDKzej54RVpPh8oyi-ch8" // your Bot Token (Get from Botfather)

// Use @myidbot to find out the c''hat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1602476441"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


char Start_buff[70]; 
int i,z;             
char ch;
int str_len;
 char textmessage[20];

 String text;

void MESSAGE_SEND();
void WAITING();
void setup() 
{
    // initialize the Serial
    Serial.begin(115200);
    Serial.println("Starting TelegramBot...");

    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//  bot.sendMessage(CHAT_ID, "Bot started up", "");
    bot.sendMessage(CHAT_ID, "CROP PROTECTION SYSTEM..");
   
    
}
void loop() 
{
  
  WAITING();
    
}
void MESSAGE_SEND()
{
  bot.sendMessage(CHAT_ID, "SEND START TO CONTINUE"); 
  // myBot.sendMessage(msg_sender_id1, "WELCOME TO ATM"); 
  
}


char Serial_read(void)
{
      char ch;
      while(Serial.available() == 0);
      ch = Serial.read(); 
      return ch;
}
void WAITING()
{
   Serial.println("WAITE");
  
  buffer_clear();
//  msg.text[0]='\0';
//        msg.text[1]='\0';
//         msg.text[2]='\0' ;
//         msg.text[3]='\0';
//          msg.text[4]='\0';
//           msg.text[5]='\0';
           
 while(1)
 { 
      if (Serial.available() > 0)
      {
         //Serial.println("halo");
        
          while(Serial_read()!='$');
          i=0;
          while((ch=Serial_read())!='#')
          {
            Start_buff[i] = ch;
             i++;
          }  
          Start_buff[i]='\0';
       }
       Serial.println(Start_buff);
       bot.sendMessage(CHAT_ID, Start_buff); 
     delay(100);
      
       if((Start_buff[0]=='M'))
       {
        //Serial.println(Start_buff);
             bot.sendMessage(CHAT_ID, Start_buff); 
            delay(100);
            Waiting_for_Response();
       }
    }
}
void buffer_clear()
{
  for(z=0;z<60;z++)
  {
    Start_buff[z]='\0';
//    textmessage[z]='\0';
    
  } 
}
void buffer1_clear()
{
  for(z=0;z<5;z++)
  {
   
   text[z]='\0';
    
  } 

}


void Waiting_for_Response()
{
  buffer1_clear();
  while(1) 
  {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
      while (numNewMessages)
      {
        Serial.println("got response");
       handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
  }

}
  


void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
Serial.print(text);
    if (text == "PUMP ON")
    {
 
      Serial.println("*PUMP ON#");
      delay(1000);
      WAITING();
    }
    if (text == "PUMP OFF")
    {
 
      Serial.println("*PUMP OFF#");
      delay(1000);
      WAITING();
    }
    

  }
//  numNewMessages=0;
//  buffer1_clear();
}

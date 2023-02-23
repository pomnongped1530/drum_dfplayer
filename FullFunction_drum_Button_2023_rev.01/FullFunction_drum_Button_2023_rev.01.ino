/***************************************************
 DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the all the function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
<https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
//#include <Bounce2.h>
#include <Keypad.h>

#define LED_PIN  13
#define BUSY_PIN 8   // -----> Pin  DFPlayer
int busyState = 0;
int prevBusyState = 0;

//int ledState = LOW;



const byte ROWS = 1; //four rows
const byte COLS = 1; //three columns
char keys[ROWS][COLS] =
{
  {'1'}

};

/*{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},

}; */

//byte rowPins[ROWS] = {A0, A1, A2}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {A1}; //connect to the row pinouts of the keypad

//byte colPins[COLS] = {5, 6, 7}; //connect to the column pinouts of the keypad
byte colPins[COLS] = {7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//tombol
int Mode = 0;
int next = 3; 
int prev = 2; 
/*int Snare = 5; int SnareHit = 0;
int Hhat = 6; int HhatHit = 0;
int Kick = 7; int KickHit = 0;
int Crash = 8; int CrashHit = 0;
int Tom1 = 10; int Tom1Hit = 0;
*/


//#define BUTTON_PIN 5     // กำหนดขาที่ใช้งาน  input D4
//#define LED_PIN 13       //  กำหนด ไฟ output   D13

//int ledState = LOW;
//Bounce debouncer = Bounce();     // สร้าง debouncer object

//int knockSensor = 0;  
//byte val  = 0;

//int THRESHOLD = 223;  //Tiger
//int THRESHOLD = 35;    // ค่าต้านทานเริ่มต้นที่กำหนด   Mic


SoftwareSerial mySoftwareSerial(5, 6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  pinMode( LED_PIN, OUTPUT );       // กำหนด  LED  PIN 
  pinMode( BUSY_PIN, INPUT_PULLUP );
 //  digitalWrite(LED_PIN, ledState);   
// กำหนดให้ debouncer object ใช้ BUTTON_PIN ในโหมด INPUT_PULLUP (จะแก้เป็น INPUT, INPUT_PULLUP หรือ OUTPUT ก็ได้ขึ้นอยู่กับการต่อวงจร)
 //  debouncer.attach(BUTTON_PIN, INPUT_PULLUP);
 //  debouncer.interval(20);                 // กำหนดเวลาการเปลี่ยนสถานะให้กับ debouncer object ที่ 25 มิลลิวินาที
   
  //  pinMode(LED_PIN, OUTPUT);                //  กำหนด  LED output
  //  digitalWrite(LED_PIN, ledState);         //  กำหนด ขา pin input


  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }

  
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(15);  //Set volume value (0~30).
  //myDFPlayer.volumeUp(); //Volume Up
 // myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
  myDFPlayer.enableDAC();  //Enable On-chip DAC
//  myDFPlayer.disableDAC();  //Disable On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15
  
  //----Mp3 play----
 /* myDFPlayer.next();  //Play next mp3
  delay(1000);
  myDFPlayer.previous();  //Play previous mp3
  delay(1000);
  myDFPlayer.play(1);  //Play the first mp3
  delay(1000);
  myDFPlayer.loop(1);  //Loop the first mp3
  delay(1000);
  myDFPlayer.pause();  //pause the mp3
  delay(1000);
  myDFPlayer.start();  //start the mp3 from the pause
  delay(1000);
  myDFPlayer.playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
  delay(1000);
  myDFPlayer.enableLoopAll(); //loop all mp3 files.
  delay(1000);
  myDFPlayer.disableLoopAll(); //stop loop all mp3 files.
  delay(1000);
  myDFPlayer.playMp3Folder(4); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
  delay(1000);
  myDFPlayer.advertise(3); //advertise specific mp3 in SD:/ADVERT/0003.mp3; File Name(0~65535)
  delay(1000);
  myDFPlayer.stopAdvertise(); //stop advertise
  delay(1000);
  myDFPlayer.playLargeFolder(2, 999); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
  delay(1000);
  myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
  delay(1000);
  myDFPlayer.randomAll(); //Random play all the mp3.
  delay(1000);
  myDFPlayer.enableLoop(); //enable loop.
  delay(1000);
  myDFPlayer.disableLoop(); //disable loop.
  delay(1000);
  */

  //----Read imformation----
/*  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //read file counts in folder SD:/03
}  */

pinMode(next, INPUT_PULLUP); 
pinMode(prev, INPUT_PULLUP); 
    /* pinMode(Kick, INPUT_PULLUP);
        pinMode(Snare, INPUT_PULLUP);
        pinMode(Hhat, INPUT_PULLUP);
        pinMode(Crash, INPUT_PULLUP);
        pinMode(Tom1, INPUT_PULLUP);*/

//  กำหนด  Pin  BUSY
// initialise the BUSY states
  busyState = digitalRead( BUSY_PIN );
  prevBusyState = busyState;
        
}

void loop() {




 // debouncer.update();      // อัปเดตสถานะให้กับ debouncer object

 // put your main code here, to run repeatedly:
 //val = analogRead(knockSensor);     // กำหนด pin = val

while(digitalRead(next)==0 || digitalRead(prev)==0)
              {
                if (digitalRead(next) == 0)
                {
                  if(Mode < 3)
                  Mode = Mode + 1;
                  Serial.println("Mode : ");
                  Serial.println(Mode);
                  while(digitalRead(next)==0);
                  delay(500);  
                }
                else if (digitalRead(prev) == 0)
                  {
                    if(Mode > 1)
                    Mode = Mode-1;
                    Serial.println("Mode : ");
                    Serial.println(Mode);
                    while(digitalRead(prev)==0);
                    delay(500);  
                  }
              }

//—————————————————------------Mode 1 —————————————————------
  char key = keypad.getKey();
if (Mode == 0)
{
  switch (key)
  {
    case '1':
    myDFPlayer.play(1);  //Play the first mp3
   // myDFPlayer.play(1);      //Play file mp3  |  folder  Mp3
    //SdPlay.setFile("1_snare.raw");  
    Serial.println("Kick");
    //SdPlay.play();
    delay(70);
    break;

    case '2':
    myDFPlayer.play(2);      //Play file mp3  |  folder  Mp3
    //SdPlay.setFile("1_kick.raw");  
    Serial.println("snare");
    //SdPlay.play();
     delay(70);
    break;

    case '3':
    myDFPlayer.play(3);      //Play file mp3  |  folder  Mp3
   // SdPlay.setFile("1_Hhat.raw");  
    Serial.println("Hihat");
    //SdPlay.play();
    delay(70);
    break;

    case '4':
    myDFPlayer.play(4);      //Play file mp3  |  folder  Mp3
  //  SdPlay.setFile("1_crash.raw");  
    Serial.println("Crash");
   // SdPlay.play();
   delay(70);
    break;
  
    case '5':
    myDFPlayer.play(5);      //Play file mp3  |  folder  Mp3
   // SdPlay.setFile("1_Tom1.raw");  
    Serial.println("Tom1");
   // SdPlay.play();
   delay(70);
    break;

    case '6':
    myDFPlayer.play(6);      //Play file mp3  |  folder  Mp3
   // SdPlay.setFile("1_Tom2.raw");  
    Serial.println("Tom2");
   // SdPlay.play();
   delay(70);
    break;

    case '7':
    myDFPlayer.play(7);      //Play file mp3  |  folder  Mp3
   // SdPlay.setFile("1_floor.raw");  
    Serial.println("FloorTom");
   // SdPlay.play();
   delay(70);
    break;

    case '8':
    myDFPlayer.play(8);      //Play file mp3  |  folder  Mp3
   // SdPlay.setFile("1_ride.raw");  
    Serial.println("Ride");
   // SdPlay.play();
   delay(70);
    break;

    case '9':
    myDFPlayer.play(9);      //Play file mp3  |  folder  Mp3
  //  SdPlay.setFile("1_crash2.raw");  
    Serial.println("Crash2");
   // SdPlay.play();
   delay(70);
    break;
  }

  // copy the state of the BUSY pin onto the UNO built-in LED
  busyState = digitalRead( BUSY_PIN );
  digitalWrite( LED_PIN, busyState );

  // has the player BUSY pin changed state?
  if ( busyState != prevBusyState ) {
    // state change - has the player stopped playing?
    if ( busyState == HIGH ) {

      // stopped playing so start next track
     // myDFPlayer.next();     // กำหนดให้เล่น เพลงถัดไป

    }

    prevBusyState = busyState;
  }

//  delay( 2000 );

 
 }
}

 /* static unsigned long timer = millis();


  if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  } */


// กำหนดเงื่อนไขให้โค้ดโปรแกรมในวงเล็บปีกกาทำงานเมื่อสถานะปุ่มกดเปลี่ยนจาก HIGH เป็น LOW โดยเช็คจากฟังก์ชั่น fell()
   // หากต้องการเช็คสถานะจาก LOW เป็น HIGH ให้แทนที่ฟังก์ชั่น fell() ด้วยฟังก์ชั่น rose()
/*   if ( debouncer.fell() ) { 
      ledState = !ledState;     // สลับสถานะติด/ดับของ LED
     // tmrpcm.play("cowbell1.wav");
      myDFPlayer.play("1.mp3");    //   สั่งให้เล่นเสียง ที่กำหนด
      Serial.println("kik");         //  แสดงข้อความ ตามที่กดปุ่ม button
    //  digitalWrite(LED_PIN, ledState); // สั่งให้ LED ติด หรือดับ
     // delay(5);   //millisec
      
   }  

}  */


/*
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}  */

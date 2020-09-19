#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".

const int LEDDO = 0; 
const int LEDXANH =1;
/*
Ket noi voi Arduino Uno hoac Mega
 ----------------------------------------------------- 
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               51                MOSI
 * SPI MISO   12               50                MISO
 * SPI SCK    13               52                SCK

 */
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);       
unsigned long uidDec, uidDecTemp; // hien thi so UID dang thap phan

int code[] = {206,81,86,211}; //This is the stored UID

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("RFID Lock");
  pinMode(LEDDO, OUTPUT);
  pinMode(LEDXANH, OUTPUT);
  //servo.attach(6);// Khai bao ket noi servo voi chan 6
  SPI.begin();            
  mfrc522.PCD_Init();
  digitalWrite(0,HIGH);
  digitalWrite(1,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  Serial.print("\n New Card Present");
  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("\n Read Card");
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  //Check is the PICC of Classic MIFARE type

  Serial.println("Scanned PICC's UID:");

  printDec(mfrc522.uid.uidByte, mfrc522.uid.size);

  String uidString = String(mfrc522.uid.uidByte[0])+" "+String(mfrc522.uid.uidByte[1])+" "+String(mfrc522.uid.uidByte[2])+ " "+String(mfrc522.uid.uidByte[3]);

  Serial.println("");  
  Serial.println("UID String:" + uidString);

  int i = 0;
  boolean match = true;
  while(i<mfrc522.uid.size)
    {
      if(!(mfrc522.uid.uidByte[i] == code[i]))
      {
           match = false;
      }
      i++;
    }

  if(match)
    {
      Serial.println("\nI know this card!");      
    }else
    {
      Serial.println("\nUnknown Card");
    }

  //uidDec = 0;
  // Hien thi so UID cua the
  //for (byte i = 0; i < mfrc522.uid.size; i++) {
  //  uidDecTemp = mfrc522.uid.uidByte[i];
  //  uidDec = uidDec*256+uidDecTemp;
  //} 

  //Serial.print("\n Id="+uidDec);
  //need to delay :-) to run :-)
  delay(3000);

}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

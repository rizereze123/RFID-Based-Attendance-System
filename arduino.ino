#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(5, 6);

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
String OldCardID = "";
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  espSerial.begin(115200);
}

void loop() {
  if (millis() - previousMillis >= 15000) {
    previousMillis = millis();
    OldCardID="";
  }
  delay(50);
  //---------------------------------------------
  //look for new card
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;//got to start of loop if there is no card present
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }
  String CardID ="";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    CardID += mfrc522.uid.uidByte[i];
  }
  //---------------------------------------------
  if( CardID == OldCardID ){
    return;
  }
  else{
    OldCardID = CardID;
  }
  //---------------------------------------------
  //  Serial.println(CardID);
  //  SendCardID(CardID);
//    mfrc522.PICC_HaltA();
//  mfrc522.PCD_StopCrypto1();  
//  espSerial.print(CardID);
  Serial.println(CardID);
  delay(1000);
}

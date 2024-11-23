#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 7 // Adjust this pin as needed

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;

byte allowedTagID1[] = {0xD3, 0x67, 0x3B, 0x1C};
byte allowedTagID2[] = {0xC3, 0x84, 0x28, 0xF5};

int scanCount1 = 0;
int scanCount2 = 0;
 
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  myservo.attach(SERVO_PIN);
  myservo.write(0); // Initial position
}

void loop() {
  // Check for new card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte cardId[4];
    for (byte i = 0; i < 4; i++) {
      cardId[i] = mfrc522.uid.uidByte[i];
    }

    // Compare the card ID with the allowed tag IDs
    if (memcmp(cardId, allowedTagID1, 4) == 0) {
      // Increment the scan count for the first tag
      scanCount1++;
      
      // Check if the scan count is within the limit
      if (scanCount1 <= 3) {
        // Rotate the servo
        myservo.write(180); // Adjust angle as needed
        delay(2000); // Hold the position for 2 seconds
        myservo.write(0); // Return to initial position
      } else {
        Serial.println("Tag 1 limit reached.");
      }
    } else if (memcmp(cardId, allowedTagID2, 4) == 0) {
      // Increment the scan count for the second tag
      scanCount2++;
      
      // Check if the scan count is within the limit
      if (scanCount2 <= 3) {
        // Rotate the servo
        myservo.write(180); // Adjust angle as needed
        delay(2000); // Hold the position for 2 seconds
        myservo.write(0); // Return to initial position
      } else {
        Serial.println("Tag 2 limit reached.");
      }
    }

    // Halt the PICC
    mfrc522.PICC_HaltA();
  }
}

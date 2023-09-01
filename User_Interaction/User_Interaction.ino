#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int code[] = {35, 42, 244, 3};
String uidString;
Servo servo1;
Servo servo2;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  // initialize pins
  servo1.attach(7);
  servo2.attach(8);
  Serial.begin(9600);
  SPI.begin();
  // initialize servo display
  rfid.PCD_Init();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  lcd.begin(16, 2);
  lcd.print("Scan student ID ");
  lcd.setCursor(0, 1);
  lcd.print("        here>>>>");
}

void loop()
{
  if (rfid.PICC_IsNewCardPresent())
  {
    readRFID();
  }
  delay(100);
}

void readRFID()
{
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);

  int i = 0;
  boolean match = true;
  while (i < rfid.uid.size)
  {
    if (!(rfid.uid.uidByte[i] == code[i]))
    {
      match = false;
      break;
    }
    i++;
  }

  if (match)
  {
    Serial.println("\nI know this card!");
    printUnlockMessage();
  }
  else
  {
    Serial.println("\nUnknown Card");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void printInstruction()
{
  // display random ordernumber
  long randBox = random(1, 3);
  lcd.clear();
  lcd.print("Processing");
  lcd.setCursor(0, 1);
  lcd.print("please wait!");
  delay(5000);
  lcd.clear();
  lcd.print("Your order is");
  lcd.setCursor(0, 1);
  char str[20];
  sprintf(str, "ready at box %d", randBox);
  lcd.print(str);

  if (randBox == 1)
  {
    servo1.write(90);
    delay(3000);
    servo1.write(0);
  }
  else if (randBox == 2)
  {
    servo2.write(90);
    delay(3000);
    servo2.write(0);
  }
  else
  {
    servo1.write(0);
    servo2.write(0);
  }
}

void printUnlockMessage()
{
  long randOrder = random(100, 200);
  // initialize the user end display
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(30, 10);
  display.print(randOrder);
  display.display();

  printInstruction();
  delay(10000);

  display.clearDisplay();
  delay(5000);
  lcd.clear();
  // tell student to scan their id
  lcd.print("Scan student ID ");
  lcd.setCursor(0, 1);
  lcd.print("        here>>>>");
  // display order number
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.print("Serving order #");
  display.display();
}

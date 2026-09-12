#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

// ----------------------
// Student Database
// ----------------------

struct Student {
  byte uid[4];
  const char *name;
  bool present;
  int studentAttendance;
};

Student students[] = {
  {{0x3E, 0x65, 0x02, 0x07}, "Rishaan", false, 0}, //3E 65 02 07
  {{0x82, 0x5A, 0x17, 0x06}, "Rohit", false, 0}, //82 5A 17 06
  {{0x67, 0x67, 0x67, 0x67}, "Abhishek", false, 0} // 67 67 67 67
};

const int totalStudents = sizeof(students) / sizeof(students[0]);

// ----------------------
// Compare two UIDs
// ----------------------

bool compareUID(byte *uid1, byte *uid2, byte size)
{
  for (byte i = 0; i < size; i++)
  {
    if (uid1[i] != uid2[i])
      return false;
  }
  return true;
}

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("RFID Attendance System Ready");
}

int attendance = 0;

void loop()
{ 

if (Serial.available() > 0) { 

    String command = Serial.readStringUntil('\n');
    command.trim();
  
    if (command == "getAttendance") {
      Serial.println();
      Serial.println(String("Total Students: ")+ attendance);
    }

  for (int i = 0; i < totalStudents; i++){
    if (command == students[i].name){
      Serial.println();
      Serial.println("--------------------------");
      Serial.println(String("Here are ") + students[i].name + "'s stats:");
      Serial.println(String("Total Attendance: ") + students[i].studentAttendance + " days");
      Serial.println("--------------------------");
      Serial.println();
    }

    if(command == "allData") {
      int a;
      Serial.println();
      Serial.println("Here is all the data collected till now:");
      Serial.println();
      Serial.println(String("Total Attendance: ") + attendance);
      Serial.println();
      Serial.println("Studentwise Attendance History:");
      for ( a = 0; a < totalStudents; a++) {
        Serial.println("--------------------------");
        Serial.println(String("  ") + students[a].name + String(":"));
        Serial.println(String("    total days present: ") +  students[i].studentAttendance);
        Serial.println("--------------------------");
      }
      Serial.println();
    }
      
    }
  }
  
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  bool found = false;

  for (int i = 0; i < totalStudents; i++)
  {
    
    if (compareUID(mfrc522.uid.uidByte, students[i].uid, mfrc522.uid.size))
    {
      students[i].present = true;
      Serial.println("--------------------------");
      Serial.print("Attendance Marked: ");
      Serial.println(students[i].name);
      Serial.println("--------------------------");
      students[i].studentAttendance++;
      attendance++;

      found = true;
      break; 
    }
  }

  if (!found){
    Serial.println("Unknown Card!");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}

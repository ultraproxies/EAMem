/* 
EDB_SD.pde, now EDB_DB_SD.ino
 Extended Database Library + Internal Arduino EEPROM Demo Sketch 
 
 http://www.arduino.cc/playground/Code/ExtendedDatabaseLibrary

 SD Card Write code, using ExtendedDatabaseLibrary, located here:
 http://blog.brauingenieur.de/2014/01/20/extended-database-library-using-an-sd-card/

 This example sketch was modified by Bruce Knipe Feb. 3, 2016 and renamed to
 EDB_DB_SD.ino.

 Basically this sketch creates an EDB demo database table, EDB_SD.db, storing
 some information in it then closing and reopening the table before displaying
 the contents to the Serial window. Demonstrating the functionality.
 
 EDB_DB_SD.ino is part of a group of Arduino sketches, Python scripts, EDB
 database tables and text files that make up the Extended Arduino Memory
 project, (EDMem). The project reads & writes to flash memory (SD cards) as
 well as internal and external EEPROMs found on the ATmeaga328p and most of
 the AT24Cxxx external EEPROM modules.

 Project  Goal:
 
      Use the PC's editing tools to create all the menu, help, prompts and
      error messages, burn to EEPROM and then used by an Arduino sketch.
      This information must be burned onto an Arduino microcontroller's EEPROM
      (ATmega328p chip) or one of the external AT24Cxxx EEPROM modules.

      Read, edit and write to EDB_SD02.db (an EDB database table) as the
      source and burn each searchable record onto the AT24Cxxx or 
      ATmega328p's EEPROM. Burn both the record number and string from each
      record. The record number is used by a sketch to locate the string
      instantly in EEPROM negating the need to load anything more than the
      string the sketch needs at that moment.
 
 Approach:Write some Arduino sketches and Python scripts to create, edit, read
      and write strings save to an SD card and to burn onto EEPROM memory.

      Use EDB and an SD card so we can easily move between both the Arduino
      and PC environments. The SD card is used as the link between the
      Arduino and a PC. The Arduino must be able to read the file created
      using the PC and vice versa.
 
      Use Python on the PC so we can work in almost any operating system
      without the need to provide executables or use a compiler.
 
  Step 1)  Use and editor on the PC to edit EDB_SD.txt and the run the
      EDB_Text.py script to create the EDB_SD02.db table.
 
  Step 2) Run the Arduino sketch EDB_EEPROM.ino file or EDB_EEPROM_AT24Cxxx.ino
      script to read the EDB_SD02.db table created in step 1 from the SD
      card.
 
  Step 3)  Read one record at a time and burn the record # and associated string
      onto the EEPROM. Limited by the number of records that fit in the
      EEPROM you are using. Each record uses 2 bytes for the record number,
      another 60 for each string and 2 for padding. So 64 divided into the
      size of your EEPROM is your maximum database table size.
 
  Step 4) Close EDB_SD02.db and exit().

 Below ia list of the files used, their purpose along with the hardware and
 pin connections to the Arduino.

 Files:
           
 Arduino:    EDB_DB_SD.ino:       Demonstrate EDB capabilities by creating
                                  EDB_SD.db table on the SD card using the
                                  Arduino.

             EDB_ReadDB.ino:      Read EDB_SD.db from the SD card and display
                                  that info.  to the Serial window using
                                  Arduino Serial.println().

             EDB_EEPROM.ino:      Uses EDB_SD02.db and burns the data onto the
                                  Atmega328p's 1K of EEPROM memory. EDB_SD02
                                  is created using the PC and a text editor.

             EDB_EEPROM_AT24Cxxx.ino:  Uses EDB_SD02.db and burns the data onto
                                  the AT24Cxxxx EEPROMs. This EEPROM can be
                                  found individually, as serial modules as well
                                  as on the DS1307 and DS3231 RTC modules. 
                                       
                                  Following are some of the potential EEPROMs
                                  used:

                                  AT24C32      32K EEPROM module,
                                  AT24C256's   256K EEPROM module,
                                  AT24C512's   512K EEPROM module,
                                  AT24C1024's  1024K EEPROM module. Except
                                  to use it we must first change the read()
                                  and write() modules below to use u_long
                                  instead of u_int addresses. Same goes for
                                  the EDB associated functions

 PC Apps:    EDB_ReadDBDemo.py:   Demonstrate that we can read, write and
                                  create an EDB database by parsing EDB_SD.db,
                                  created using the Arduino, and create
                                  EDB_SD01.db.

             EDB_ReadDB.py:       Read EDB_SD.db and produce a human readable
                                  version of the database called EDB_SD.txt.

             EDB_TextDB.py:       Use a text editor to edit EDB_SD.txt, adding
                                  the strings you want to burn into EEPROM.
                                  Then run this script to parse and create EDB
                                  file, EDB_SD02.db, which is in turn used by
                                  the Arduino sketches to read the table on the
                                  SD card and burn it on to EEPROM.

 Data Files: EDB_SD.db:           DB table created by Arduino sketch
                                  EDB_DB_SD.ino.
             EDB_SD01.db:         DB table created by Python script
                                  EDB_ReadDBDemo.py.
             EDB_SD02.db:         DB table created by Python script
                                  EDB_TextDB.py
             EDB_SD.txt:          Human readable text file created by Python
                                  script EDB_ReadDB.py

 Hardware:   A PC with an SD card reader/writer, Arduino Uno, SD card module,
             breadboard and one of the external AT24Cxxx EEPROM modules if you
             want to use more than 1024K bytes of extra memory with your
             Arduino.

             Both the SD card and AT24Cxxx modules are connected to the Arduino
             5V and GND.

 SD card pin connections:
             
             MISO  digital pin 12
             SCK   digital pin 13
             MOSI  digital pin 11
             CS    digital pin 10

 AT24Cxxx external EEPROM module pin connections:

             SCL analog pin 5
             SDA analog pin 4

 Modifications to EDB_DB_SD.ino are as follows:
 
  Added #ifdef EDB_WriteSD and #ifdef EDB_ReadSD so it is simple to test the db separately.
        When I ran the sketch read only the first time I found the db was actually empty even
        though this sketch displayed the records after writing them. Leaving one with the
        impression the records were still in the db after exiting. The author didn't add
        a close() statement and nothing was being saved, so I simply added dbFile.close()
        at the end of setup().

  Added code to open the EDB_SD.db database table if EDB_ReadSD is #defined.

  Added +1 to the 'for' loop that reads and displays the records. What was there wasn't catching
        the last record.

  Added a delay between initialisation of the SD card and the SD.open() call. Before the delay
        was added there were random initialisation errors.
        
  Added a delay between each record being saved. This delay was added because giberish was
        randomly being written instead of the data that was being sent to the SD card.
        Adding a delay there eliminated the issue. It slows down creating the records a bit,
        but better that than potentially corrupted data.

  Bugs  Updated EDB.cpp with all documented bug fixes.
  
  Note: when #define EDB_ReadSD is declared the program becomes a read only sketch and no longer
        creates the EDB_SD.DB database, it only displays the contents on the Serial window.

 */
#include "Arduino.h"
#include <EDB.h>
#include <SPI.h>
#include <SD.h>


File dbFile;
// Comment out #define EDB_ReadSD to recreate the database.
//#define EDB_ReadSD // Just read then display the database files.

#ifndef EDB_ReadSD 
  #define EDB_WriteSD // Create the database, write some records to it and display them.
#endif
 
#define TABLE_SIZE 5184
#define RECORDS_TO_CREATE 10
#define TEXT_LEN 60
#define PADDING 2 
struct LogEvent {
  int id;
  //int id2;
  char text[TEXT_LEN+PADDING]; // Leave room for /n, /r or /r/n
} 
logEvent;
 
void writer(unsigned long address, byte data)
{
  dbFile.seek(address); 
  dbFile.write(data); 
  dbFile.flush();
}
 
byte reader(unsigned long address)
{
  dbFile.seek(address); 
  return dbFile.read(); 
}
 
 
EDB db(&writer, &reader);
 
void setup()
{ int recno;
  char *filename = "EDB_SD.db"; // Warning filenames can't be any larger than 8.3!
  Serial.begin(9600);
    
  Serial.print("Initializing SD card...");
   
  pinMode(10, OUTPUT);
  delay(100);
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
   
  Serial.println("initialization done.");
  delay(500); 
  Serial.print("Opening ");
  Serial.print(filename);
  Serial.print(" Rec. Len: ");
  Serial.print(sizeof(logEvent));
  Serial.println(" ...");

#ifdef EDB_WriteSD
  dbFile = SD.open(filename, FILE_WRITE);
  delay(100);
  if( ! dbFile ) {
    Serial.print( "File error! Can't open ");
    Serial.print( filename );
    Serial.println("!");
    return;    
  }

  db.create(0, TABLE_SIZE, sizeof(logEvent));
  Serial.print("Record Count: "); Serial.println(db.count());
  delay(100);
  Serial.println("Creating Records...");
  char *exampleStr = "012345678901234567890123456789012345678901234567890123456789";
  int exampleLen;
  int cnt = 0;
  for (recno = 1; recno <= RECORDS_TO_CREATE; recno++)
  {
    logEvent.id = recno; 
    // logEvent.id2 = recno * 2;
    exampleLen = strlen(exampleStr);
    if( strlen(exampleStr) >= TEXT_LEN) { // test in loop so we can load from file
      exampleLen = TEXT_LEN;
    }
    strncpy( logEvent.text, exampleStr, TEXT_LEN );
    delay(100);  // Slows things down quite a bit when writing, but prevents buffer overflow issues.
    db.appendRec(EDB_REC logEvent);
    Serial.print(".");
    if( ++cnt == 10 ){
      cnt = 0;
      Serial.println(recno);
    }
  }
#else
  Serial.print("Repening ");
  Serial.print(filename);
  Serial.print(" Rec. Len: ");
  Serial.print(sizeof(logEvent));
  Serial.println(" ...");

  dbFile = SD.open(filename, FILE_READ);
  delay(100);
  if( ! dbFile ) {
    Serial.print( "File error! Can't open ");
    Serial.print( filename );
    Serial.println("!");
    return;    
  }
  db.open(0);  
#endif

  Serial.print("Record Count: "); Serial.println(db.count());
  for (recno = 1; recno < db.count()+1; recno++)
  {
    db.readRec(recno, EDB_REC logEvent);
    Serial.print("ID: "); Serial.println(logEvent.id);
    //Serial.print("ID2: "); Serial.println(logEvent.id2);
    Serial.print("Text: "); Serial.println(logEvent.text);   
  }
  dbFile.close();
  delay(100);
  exit(0);
}
 
 
void loop()
{
  
}

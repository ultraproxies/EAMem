/*
 Based on EDB_SD.pde
 Extended Database Library + Internal Arduino EEPROM Demo Sketch

 The Extended Database library project page is here:
 http://www.arduino.cc/playground/Code/ExtendedDatabaseLibrary

 Based on code from:
 Database library for Arduino 
 Written by Madhusudana das
 
 http://www.arduino.cc/playground/Code/DatabaseLibrary

 SD Card Write code, using ExtendedDatabaseLibrary, located here:
 http://blog.brauingenieur.de/2014/01/20/extended-database-library-using-an-sd-card/

 This sketch was written by Bruce Knipe, February 5, 2016

 Basically this code reads the EDB table, EDB_SD02.db, and burns the information
 onto the EEPROM in the ATmega328p. Use the EDB_AT24Cxxx.ino sketch to burn
 external EEPROMs.

 Purpose: Read the EDB database EDB_SD02.db, from an SD card, and burn each record
          onto the ATmega328p's EEPROM. Burn both the record number and string from
          each record. The record number is used by any sketch to locate the string
          instantly in EEPROM and to eliminate the need to load anything more than
          the string the sketch needs at that moment.

          This sketch uses the internal EEPROM and is only able to load about 10
          or 12 records onto the EEPROM because it is only 1024 bytes. For more
          than that you need to add an external EEPROM like the AT24C32. See
          below for a list of the sketches that work with those EEPROMs.
                
 Step 1)  Initialize the SD card and open the EDB table, EDB_SD02.db.
 Step 2)  Read one record at a time and burn the record # and associated string
          onto the ATmega328p's EEPROM. Limited by the number of records that
          fit in 1024 bytes. Each record uses 2 bytes for the record number and
          another 60 for each string, so 62 / 1024 = 16 records max that can be
          written.
 Step 3)  Close EDB_SD02.db and exit().

 This is one of a group of sketches, Python scripts, EDB tables, and text files
 needed to create, read & write to the EDB database tables, use an SD card, and
 edit the information in the EDB tables using a text editor on a PC. Following is
 a list of the files used and their purpose.

 The SD card is used as the link between the Arduino and a PC. The Arduino
 must be able to read the file created using the PC and vice versa.

 EDB_EEPROM.ino is part of a group of Arduino sketches, Python scripts, EDB
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

 Both the SD card and AT24Cxxx modules are connected to the Arduino 5V and GND.

 SD card pin connections:
             
             MISO  digital pin 12
             SCK   digital pin 13
             MOSI  digital pin 11
             CS    digital pin 10

 AT24Cxxx external EEPROM module pin connections:

             SCL analog pin 5
             SDA analog pin 4
 */
#include "Arduino.h"
#include <Wire.h>
#include <EEPROM.h>
#include <EDB.h>
#include <SPI.h>
#include <SD.h>


File dbFile;

// Declaring #define EDB_WriteEEPROM will cause this sketch to burn anything in EDB_SD02.db to EEPROM.

// if EDB_WriteEEPROM is defined you must decide whether to create the EEPROM EDB table or just open it.
// Declaring #define EDB_ReWriteEEPROM forces an open() insted of create() call and prevents EEPROM wear.
// Any data written to the EEPROM will be selective and only overwrite bytes that need to be changed.
//This matters as there a limited number of times the EEPROM can be written to (approximately 100,000).

// Comment out EDB_WriteEEPROM to prevent EEPROM burn while testing.                          
#define EDB_WriteEEPROM   // Burn up to 1024 bytes of data from EDB_SD02.db on SD card to EEPROM.
//#define EDB_ReWriteEEPROM // Reburn data in EEPROM, but only writing bytes that need changing, saving wear.

// Uncomment EDB_ReadEEROM If you just want to read what is in the EEPROM table.
//#define EDB_ReadEEPROM  // Defining this will undefine EDB_WriteEEPROM automatically.

#ifdef EDB_ReadEEPROM
  #ifdef EDB_WriteEEPROM
    #undef EDB_WriteEEPROM   // Undefine EDB_WriteEEPROM if EDB_ReadEEPROM is defined.
  #endif
  #ifdef EDB_ReWriteEEPROM
    #undef EDB_ReWriteEEPROM // Undefine EDB_ReWriteEEPROM if EDB_ReadEEPROM is defined.
  #endif
#endif 

bool testIt = false;           // DEBUG flag. Prevents any writing to the EEPROM and displays byte instead.
bool displayIt = true;        // Display Append, Update, etc. for each type of action beforehand.
// The following flags are set below in setup().
bool createEEPROM = false;    // If true sketch will create a new EEPROM EDB table.
bool rewriteEEPROM = false;   // If true sketch will open an existing EEPROM EDB table.
bool writeEEPROM = false;     // If true sketch will write to EEPROM.
bool readEEPROM = false;      // If true sketch will read from EEPROM and disable any EEPROM burning/writing.


#define TABLE_SIZE 1024
#define RECORDS_TO_CREATE 10
#define TEXT_LEN 60
#define PADDING 2 



struct LogEvent {
  int id;
  char text[TEXT_LEN+PADDING];
}
logEvent;

// SD Write function.
void writer(unsigned long address, byte data){
 dbFile.seek(address);
 dbFile.write(data);
 dbFile.flush();
}

byte reader(unsigned long address)
{
  dbFile.seek(address);
  return dbFile.read();
}

void eeprom_writer(unsigned long address, byte data){
  if( writeEEPROM && !testIt )      // Prevent EEPROM burn if testIt is false.
    EEPROM.write( address,  data ); // Burn byte onto internal EEPROM.
  else {
    Serial.print(char(data));       // If not burning byte onto EEPROM then display it.
    Serial.print(" - ");
    Serial.print(data);
    
  }
}

byte eeprom_reader(unsigned long address){
  return EEPROM.read( address);
}

EDB db(&writer, &reader);
EDB eeprom_db(&eeprom_writer, &eeprom_reader);


void setup()
{ int recno;
  char *filename = "EDB_SD02.db";
  EDB_Status result = EDB_OK;

  Serial.begin(9600);
  delay(50);
#ifdef EDB_ReadEEPROM
  readEEPROM = true;      // Read from EEPROM instead of SD card.
#endif 
#ifdef EDB_WriteEEPROM
  createEEPROM = true;    // Create EEPROM EDB table.
  writeEEPROM = true;
#endif
#ifdef EDB_ReWriteEEPROM
  rewriteEEPROM = true;  // Overwrite existing EEPROM EDB table.
  writeEEPROM = true;
#endif
  if( displayIt ) {
#ifdef EDB_ReadEEPROM
  Serial.println( "EDB_ReadEEPROM true! Data loading from EEPROM" );
#endif 
#ifdef EDB_WriteEEPROM
  Serial.println( "EDB_WriteEEPROM" );
#endif
#ifdef EDB_ReWriteEEPROM
  Serial.println( "EDB_ReWriteEEPROM" );
#endif
  }

#ifndef EDB_ReadEEPROM
  Serial.print("Initializing SD card...");

  pinMode(10, OUTPUT);
  delay(100);
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.print("Opening ");
  Serial.print(filename);
  Serial.print(" Rec. Len: ");
  Serial.print(sizeof(logEvent));
  Serial.println(" ...");
  dbFile = SD.open(filename, FILE_READ);
  delay(100);
  db.open(0);
  if( ! dbFile ) {
    Serial.print( "File error! Can't open ");
    Serial.print( filename );
    Serial.println("!");
    return;    
  }

  Serial.print("Record Count: "); Serial.println(db.count()); 
#endif // ndef EDB_ReadEEPROM

  if( createEEPROM ) {
    // create a table with starting address 0
    Serial.print("Creating table...");
    result = eeprom_db.create(0, TABLE_SIZE, (unsigned int)sizeof(logEvent));
    Serial.println("DONE");
  }
  else
    result = eeprom_db.open(0);
  if (result != EDB_OK) {
     printError(result);
     dbFile.close();
  }
 
  for (recno = 1; recno <= RECORDS_TO_CREATE; recno++)
  {
#ifdef EDB_WriteEEPROM
    if( recno > db.count() )
      break;
#endif
    if( displayIt )
      Serial.println("Read a record");    
#ifdef EDB_ReadEEPROM
    if( recno > eeprom_db.count() )
      break;
    result = eeprom_db.readRec(recno, EDB_REC logEvent);
#else
    if( recno > db.count() )
      break;
    result = db.readRec(recno, EDB_REC logEvent);
#endif
    Serial.print("Rec# "); Serial.print(logEvent.id);
    
#ifdef EDB_WriteEEPROM
    if( createEEPROM ){
      if( displayIt ) {
        Serial.println("append a record");
      }
      result = eeprom_db.appendRec(EDB_REC logEvent);
    }
    else {
      if( displayIt ) {
        Serial.println("update a record");
      }
      result = eeprom_db.updateRec(recno, EDB_REC logEvent);
    }
#endif
    delay(30);  // Slows things down quite a bit when writing, but prevents buffer overflow issues.
    
    if (result != EDB_OK) {
       printError(result);
       break;
    }
    if( displayIt ) {
#ifdef EDB_WriteEEPROM
        Serial.println("");
#endif
        Serial.print(" Text: ["); Serial.print(logEvent.text);
        Serial.println("]");
    } else if( testIt ) { // If testit is true each byte sent to EEPROM is displayed, so we need a CR here.
        Serial.println("");
    }
  }
#ifndef EDB_ReadEEPROM
  dbFile.close();
#endif
  delay(100);
  exit(0);
}

void printError(EDB_Status err)
{
  Serial.println("");
  Serial.print("ERROR: ");
  switch (err)
  {
    case EDB_OUT_OF_RANGE:
      Serial.println("Recno out of range");
      break;
    case EDB_TABLE_FULL:
      Serial.println("Table full");
      break;
    case EDB_OK:
      Serial.println("OK");
      break;
    default:
      Serial.println("Unknown Error");
      break;
  }
}

void loop()
{

}

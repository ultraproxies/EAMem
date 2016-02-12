/*
 Based on EDB_SD.pde
 Extended Database Library + external Arduino EEPROM Demo Sketch

 The Extended Database library project page is here:
 http://www.arduino.cc/playground/Code/ExtendedDatabaseLibrary

 SD Card Write code, using ExtendedDatabaseLibrary, located here:
 http://blog.brauingenieur.de/2014/01/20/extended-database-library-using-an-sd-card/

 Based on code from:
 Database library for Arduino 
 Written by Madhusudana das
 
 http://www.arduino.cc/playground/Code/DatabaseLibrary

 SD Card Write code, using ExtendedDatabaseLibrary, located here:
 http://blog.brauingenieur.de/2014/01/20/extended-database-library-using-an-sd-card/

 This sketch was written by Bruce Knipe, February 6, 2016

 Basically this code reads the EDB table, EDB_SD02.db, and burns the information
 onto the EEPROM in the ATmega328p. Use the EDB_AT24Cxxx.ino sketch to burn
 external EEPROMs.

 Purpose: Read the EDB database EDB_SD02.db, from an SD card, and burn each record
          onto the AT24Cxxx EEPROM. Burn both the record number and string from
          each record. The record number is used by any sketch to locate the string
          instantly in EEPROM and to eliminate the need to load anything more than
          the string the sketch needs at that moment.

          This sketch uses the external EEPROM and is able to load as many as
          available EEPROM mem / 64 bytes.
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
 edit the information in the EDB tables using a text editor on a PC. 

 EDB_EEPROM_AT24C is part of a group of Arduino sketches, Python scripts, EDB
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

 int eepromAddress = 0x52; (serial port address) // Was 0x50, but changed it due to conflict with RTC DS1307.

*/
#include "Arduino.h"  // Included to prevent errors generated from including Wire.h when compiling.
#include <Wire.h>     // Needed for EEPROM support.
#include <SPI.h>      // Included to prevent errors generated from including SD.h when compiling.
#include <SD.h>       // Needed for SD card support.
#include <I2CEEPROM.h>// Needed for reading and writing to AT24Cxxx EEPROM modules.
#include <EDB.h>      // Needed as our database for both the SD and EEPROM.


File dbFile;              // EDB database on the SD card.
i2cEEPROM i2c = i2cEEPROM();
int eepromAddress = 0x52; // Change this from 0x50 to 0x52 when using the RTC DS1307 due to conflict.

// Declaring #define EDB_WriteEEPROM will cause this sketch to burn anything in EDB_SD02.db to EEPROM.

// if EDB_WriteEEPROM is defined you must decide whether to create the EEPROM EDB table or just open it.
// Declaring #define EDB_ReWriteEEPROM forces an open() insted of create() call and prevents EEPROM wear.
// Any data written to the EEPROM will be selective and only overwrite bytes that need to be changed.
//This matters as there a limited number of times the EEPROM can be written to (approximately 100,000).

// Comment out EDB_WriteEEPROM to prevent EEPROM being written to while testing.                          
#define EDB_WriteEEPROM   // Burn up to 1024 bytes of data from EDB_SD02.db on SD card to EEPROM.

//#define EDB_ReWriteEEPROM // Burn data in EEPROM, but only writing bytes that need changing, saving wear.

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

bool testIt = false;          // DEBUG flag. Prevents any writing to the EEPROM and displays byte instead.
bool displayIt = true;        // Display Append, Update, etc. for each type of action beforehand.

// The following flags are set true below in setup() depending on the #define statements declared above.
bool createEEPROM = false;    // If true sketch will create a new EEPROM EDB table.
bool rewriteEEPROM = false;   // If true sketch will open an existing EEPROM EDB table.
bool writeEEPROM = false;     // If true sketch will write to EEPROM.
bool readEEPROM = false;      // If true sketch will read from EEPROM and disable any EEPROM burning/writing.


#define TABLE_SIZE (1024 * 10)// Limit database table size to whatever amount of EEPROM memory is connected.
#define RECORDS_TO_CREATE 50  // Limit number of records that can be written to table.
#define TEXT_LEN 60           // Maximum string length in LogEvent struct.
#define PADDING 2             // Extra characters for possible /r/n carriage return / linefeed at EOL.



struct LogEvent {
  int id;                     // Record #. Used to instantly find record in EEPROM.
  char text[TEXT_LEN+PADDING];// Stores prompts, error messages, menu items, etc. Allows for larger sketches.
}
logEvent;                     // Structure used by EDB table to easily move data to and from the SD card and EEPROM.

// SD Write proceedure.
void writer(unsigned long address, byte data){
 dbFile.seek(address);
 dbFile.write(data);
 dbFile.flush();
}

// SD read function.
byte reader(unsigned long address){ 
  dbFile.seek(address);
  return dbFile.read();
}

// EEPROM write proceedure.
void eeprom_writer(unsigned long address, byte data){
  if( writeEEPROM && !testIt )   {   // Prevent EEPROM burn if testIt is false.
    //i2c.i2c_eeprom_write_byte( eepromAddress, (unsigned int)address,  data ); // Burn byte onto external EEPROM.
    i2c.i2c_eeprom_write_byte( eepromAddress, address, data);
   
    delay(2);
   //byte b = i2c.i2c_eeprom_read_byte(eepromAddress, address); //access an address from the memory
   //   Serial.print((char)b); //print content to serial port
   //   Serial.print(b, HEX);
    
  }
  else {
    Serial.print(char(data));       // If not burning byte onto EEPROM then display it.
    //Serial.print(" - ");
    //Serial.print(data);
    
  }
}

// EEPROM read function.
byte eeprom_reader(unsigned long address){
     byte b = i2c.i2c_eeprom_read_byte(eepromAddress, address); //access an address from the memory
     // Serial.print((char)b); //print content to serial port
     // Serial.print(b, HEX);

  return b ;// i2c.i2c_eeprom_read_byte(eepromAddress, (unsigned int)address);
}

// Assign respective read() and write() function addresses for EDB table calls.
EDB db(&writer, &reader);
EDB eeprom_db(&eeprom_writer, &eeprom_reader);


void setup()
{ int recno;
  char *filename = "EDB_SD02.db";
  EDB_Status result = EDB_OK;
 Wire.begin();
  Serial.begin(9600);
  delay(1000);
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
  Serial.println( "EDB_ReadEEPROM true!" );
  Serial.println( "Data loading from EEPROM" );
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

  Serial.println(" ");
  
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
    result = eeprom_db.readRec(recno, EDB_REC logEvent);  // Read from EEPROM
#else
    if( recno > db.count() )
      break;
    result = db.readRec(recno, EDB_REC logEvent);         // Read from SD card.
#endif

    Serial.print("Rec# "); Serial.print(logEvent.id);

#ifdef EDB_WriteEEPROM
    if( createEEPROM ){
      if( displayIt ) {
        Serial.print(" append a record");
      }
      result = eeprom_db.appendRec(EDB_REC logEvent);
    }
    else {
      if( displayIt ) {
        Serial.println(" update a record");
      }
      result = eeprom_db.updateRec(recno, EDB_REC logEvent);
    }
#endif
    
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

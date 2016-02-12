
 Extended Arduino Memory (EAMem)
 ===============================

 This is a group of sketches, Python scripts, EDB tables, and text files that
 can be used to create, read & write to the EDB database tables on an SD card
 as well as the internal and external Arduino related EEPROMs.

 Basically this code reads the EDB table, EDB_SD02.db, and burns the information
 onto the EEPROM in the ATmega328p or the external AT24Cxxx EEPROMs.

 You can edit the information in the EDB tables using a text editor on a PC.
 
 readme.txt is part of a group of Arduino sketches, Python scripts, EDB
 database tables and text files that make up the  Extended Arduino Memory
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

 Below is the logic, a list of the files used, their purpose along with the
 hardware and pin connections to the Arduino.


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

 Modified Libraries:              Included in the package for simplicity's sake.
             EDB                  Modified and patched arduino.cc contrib. file.
             I2CEEPROM            Created from arduino.cc contributed file.

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

Note: There are no references on where to find the Arduino, Wire, SD or EEPROM
      libraries because they are part of the standard Arduino libraries
      packaged with the Arduino IDE.
 

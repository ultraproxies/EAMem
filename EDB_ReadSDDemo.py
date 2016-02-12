#!/usr/bin/python
#
# EDB_ReadDBDemo.py: Testing script to prove we can read and create an EDB
#                    database table. Running this program will create a
#                    duplicate of EDB_SD.db using the Python struct library
#                    tools to parse and create the header and records in a
#                    new database file called EDB_SD01.db.
#
# Step 1) Opens the EDB table, EDB_SD.db, created using the EDB_DB_SD.ino
#         Arduino sketch.
# Step 2) Reads and parses the header which contains #records, rec size and
#         maximum size of the table.
# Step 3) Creates a new file, EDB_SD01.db.
# Step 4) Using the Python struct tools captures the information from
#         EDB_SD.db's header and creates the header for EDB_SD01.db.
# Step 5) Iterates through EDB_SD.db's records and writes them to EDB_SD01.db.
# Step 6) Closes both files and exits.
# 
# At this point copy EDB_SD01.db to the SD card and read it using the Arduino
# sketch EDB_ReadDB.ino. This will read and display all the information in the
# database to the Serial output window using Serial.println().
#
# These steps prove the compatibility between the two environments.
#
# Written by Bruce W. Knipe, February 4th 2016.
#
# The SD card is used as the link between the Arduino and a PC. The Arduino
# must be able to read the file created on the PC and vice versa.
#
# EDB_ReadSDDemo.py is part of a group of Arduino sketches, Python scripts,
# EDB database tables and text files that make up the Extended Arduino
# Memory project, (EDMem). The project reads & writes to flash memory
# (SD cards) as # well as internal and external EEPROMs found on the
# ATmeaga328p and most of the AT24Cxxx external EEPROM modules.
#
# Project  Goal:
#
#      Use the PC's editing tools to create all the menu, help, prompts and
#      error messages, burn to EEPROM and then used by an Arduino sketch.
#      This information must be burned onto an Arduino microcontroller's EEPROM
#      (ATmega328p chip) or one of the external AT24Cxxx EEPROM modules.
#
#      Read, edit and write to EDB_SD02.db (an EDB database table) as the
#      source and burn each searchable record onto the AT24Cxxx or 
#      ATmega328p's EEPROM. Burn both the record number and string from each
#      record. The record number is used by a sketch to locate the string
#      instantly in EEPROM negating the need to load anything more than the
#      string the sketch needs at that moment.
#
# Approach:Write some Arduino sketches and Python scripts to create, edit, read
#         and write strings save to an SD card and to burn onto EEPROM memory.
#
#         Use EDB and an SD card so we can easily move between both the Arduino
#         and PC environments. The SD card is used as the link between the
#         Arduino and a PC. The Arduino must be able to read the file created
#         using the PC and vice versa.
#
#         Use Python on the PC so we can work in almost any operating system
#         without the need to provide executables or use a compiler.
#
# Step 1)  Use and editor on the PC to edit EDB_SD.txt and the run the
#         EDB_Text.py script to create the EDB_SD02.db table.
#
# Step 2)  Run the Arduino sketch EDB_EEPROM.ino file or EDB_EEPROM_AT24Cxxx.ino
#         script to read the EDB_SD02.db table created in step 1 from the SD
#          card.
#
# Step 3)  Read one record at a time and burn the record # and associated string
#         onto the EEPROM. Limited by the number of records that fit in the
#         EEPROM you are using. Each record uses 2 bytes for the record number,
#         another 60 for each string and 2 for padding. So 64 divided into the
#         size of your EEPROM is your maximum database table size.
#
# Step 4)  Close EDB_SD02.db and exit().
#
# See readme.txt for more information and a list of all the associated files,
# hardware and Arduino pin connections between those devices.
#                                  EDB_ReadDB.py

import struct	# Use struct to parse the header as well as create our own.
debugIt = True
debugIt2 = False
perfectCopy = False # if set True copy records faithfully, else use our str.

fmt_data =\
	'hh60s';	# Struct format string to parse record

fmt = 'Lhh';	# Struct format string to parse header

header_size = struct.calcsize(fmt);	# Length of header in bytes 

records = 0		# Index for header tuple: number of records in table
rec_size = 1	# Index for header tuple: record size
table_size = 2	# Index for header tuple: Table size
iID = 1			# Index for record tuple: ID
iText = 2		# Index for record tuple: Text

# Open our PC created and modified version of the database table file.
fout = open("EDB_SD01.db", "wb"); # Create a duplicate using struct tools.

# Open database table. This file was created using the Arduino UNO
#                      and an SD card (by using sketch EDB_SD_DB.ino).
with open('EDB_SD.db','rb') as fin:
	if debugIt:
		print "fmt:", fmt
		print "header size", header_size
	header = fin.read(header_size)				# Load header
	if debugIt:
		print "header str", header
	header_tuple = struct.unpack(fmt,header);	# u_long, u_int, u_long
	rsize = header_tuple[rec_size];				# Record size
	tsize = header_tuple[table_size];			# Table size
	numRecords = header_tuple[records];			# Number of records in table
	# Write header to new SD database table EDB_SD01.db (sets up perfect rec pos).
	repacked_header = struct.pack(fmt, numRecords, rsize, tsize)
	print( "rsize type = %s"%type(tsize));
	fout.write(repacked_header);# Now we are in position to write the first record.

	if debugIt:
		print("tuple:", header_tuple);
		print("record size:", header_tuple[rec_size]);
		print("number of records:", header_tuple[records]);
		print("table_size:", header_tuple[table_size]);
	
	# Begin processing each record and save each one in our new table.
	for i in range(0,numRecords): 				# loop thru all the records
		data_str = fin.read(rsize)				# Load record
		if( debugIt2 and i == 0 ):
			print "len(data_str) =", len(data_str)
			print "data str", data_str
		data_tuple = struct.unpack(fmt_data,data_str);  # int, int, 60 chars.
		if perfectCopy:							# If we want to prove duplication.
		  packed = struct.pack(fmt_data,
			data_tuple[0], data_tuple[iID], data_tuple[iText])
		else:									# If we want to prove capability.
		  packed = struct.pack(fmt_data,
			data_tuple[0], data_tuple[iID], "my data string")
		fout.write(packed);						# Write encoded record to table.

		if debugIt2:
			print("Data tuple:", data_tuple);
		# Remove any trailing padding from data and insure max len is respected.
		stringLen = data_tuple[iText].find('\x00')	# Remove any \x00's
		if stringLen <= 0 or stringLen > len(data_tuple[iText]):
			stringLen = len(data_tuple[iText]);
		if debugIt:
			print("ID: %d, Str: %s"%
				(data_tuple[iID], data_tuple[iText][0:stringLen]));
		#print("ID:", data_tuple[iID]);
		#print("Str:", data_tuple[iText][0:stringLen]);
if fout:
	fout.write(b'\x00\x00'); # ADD 00 00 to match Arduino SD written EOF.
	fout.close();



#!/usr/bin/python
#
# EDB_ReadDB.py: Read EDB_SD.db and produce a human readable version
#                of the database table.
#
# Step 1) Opens EDB table, EDB_SD.db.
# Step 2) Creates text file, EDB_SD.txt.
# Step 3) Copies data from table and writes it out to EDB_SD.txt for editing.
# Step 4) Closes both files and exits.
#
# At this point you should modify the EDB_SD.txt file. Put whatever prompts,
# menu items, error messages or whatever data want to store in EEPROM memory
# to be used later by your Arduino sketches.
#
# After editing EDB_SD.txt run EDB_Text.py. It creates the EDB table
# EDB_SD02.db which is used by the Arduino sketch, EDB_EEPROM.ino, to burn
# into your Atmega328p EEPROM memory. Alternatively you can use
# EDB_External.ino to burn the data into an AT24Cxxx EEPROM module.
#
# Written by Bruce W. Knipe, February 5th 2016.
#
# The SD card is used as the link between the Arduino and a PC. The Arduino
# must be able to read the file created on the PC and vice versa.
#
# EDB_ReadDB.py is part of a group of Arduino sketches, Python scripts, EDB
# database tables and text files that make up the Extended Arduino Memory
# project, (EDMem). The project reads & writes to flash memory (SD cards) as
# well as internal and external EEPROMs found on the ATmeaga328p and most of
# the AT24Cxxx external EEPROM modules.
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

import struct	# Use struct to parse the header as well as create our own.
debugIt = True
debugIt2 = False

fmt_data =\
	'hh60s';	# Struct format string to parse record

fmt = 'Lhh';	# Struct format string to parse header

header_size = struct.calcsize(fmt);	# Length of header in bytes 

records = 0		# Index for header tuple: number of records in table
rec_size = 1	# Index for header tuple: record size
table_size = 2	# Index for header tuple: Table size
iID = 1			# Index for record tuple: ID
iText = 2		# Index for record tuple: Text

# Open a human readable and editable text file that can later be reintegrated.
fout = open("EDB_SD.txt", "w");

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
	## Write header to new SD database table EDB_SD01.db
	#repacked_header = struct.pack(fmt, numRecords, rsize, tsize)
	#fout.write(repacked_header);

	if debugIt:
		print("tuple:", header_tuple);
		print("record size:", header_tuple[rec_size]);
		print("number of records:", header_tuple[records]);
		print("table_size:", header_tuple[table_size]);

	for i in range(0,numRecords): 				# loop thru all the records
		data_str = fin.read(rsize)				# Load record
		if( debugIt2 and i == 0 ):
			print "len(data_str) =", len(data_str)
			print "data str", data_str
		data_tuple = struct.unpack(fmt_data,data_str);  # int, int, 60 chars.
#		if debugIt2:
#			print("Data tuple:", data_tuple);
		stringLen = data_tuple[iText].find('\x00')	# Remove any \x00's
		if stringLen <= 0 or stringLen > len(data_tuple[iText]):
			stringLen = len(data_tuple[iText]);
		print("ID: %d, Str: %s"%
				(data_tuple[iID], data_tuple[iText][0:stringLen]));
		fout.write( data_tuple[iText][0:stringLen]);
		fout.write( "\n" );
		#print("ID:", data_tuple[iID]);
		#print("Str:", data_tuple[iText][0:stringLen]);
if fout:
	fout.close();


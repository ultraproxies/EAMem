#!/usr/bin/python
#
# EDB_Text.py: Load text file and create EDB table for use by the Arduino
#               to burn information into an EEPROM (internal or external).
#
# Running this script creates the EDB database table, EDB_SD02.db, which is
# used by the Arduino sketch EDB_DB_EEPROM.ino to burn data into the
# Atmega328p's EEPROM memory. Alternatively you can use EDB_AT24Cxxx.ino
# sketches to burn the data onto the associated AT24Cxxx EEPROM modules.
#
# Written by Bruce W. Knipe, February 4th 2016.
#
# The SD card is used as the link between the Arduino and a PC. The Arduino
# must be able to read the file created on the PC and vice versa.
#
# EDB_Text.py is part of a group of Arduino sketches, Python scripts, EDB
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

import struct	# Use struct to create the EDB db table header and records.
debugIt = True
debugIt2 = False

fmt_data =\
	'hh60s';	# Struct format string to construct record

fmt = 'Lhh';	# Struct format string to construct header

header_size = struct.calcsize(fmt);	# Length of header in bytes 
# EDB_SD.db Stats:
# header size 8
# record size: 64 # because 2 bytes for the id, 60 for the string and 2 extra for padding.

header_size = struct.calcsize(fmt);	# Length of header in bytes
tsize = 5184
rsize = 64 # because 2 bytes for the id, 60 for the string and 2 extra for padding.
lineLength = 60
numRecords = 0

records = 0		# Index for header tuple: number of records in table
rec_size = 1	# Index for header tuple: record size
table_size = 2	# Index for header tuple: Table size
iID = 1			# Index for record tuple: ID
iText = 2		# Index for record tuple: Text

# Open the EDB_SD.txt text file and convert it to an EDB db table.
fout = open("EDB_SD02.db", "wb"); # Create database table using struct tools.

# Open database table. This file was created using the Arduino UNO
#                      and an SD card (by using sketch EDB_SD_DB.ino).
if fout:
 with open('EDB_SD.txt','r') as fin:
	header_tuple = struct.pack(fmt, 0, 0, 0);
	fout.write(header_tuple);					# Write header.
	if debugIt:
		print "fmt:", fmt
		print "header size", header_size
	i = 0;
	line = fin.readline();						# Load lines from EDB_SD.txt
	while line != '':
		numRecords+=1;			 				# count each line.
		line = line.replace('\r','');
		line = line.replace('\n','');
		stringLen = len(line);
		if stringLen <= 0 or stringLen > lineLength:
			stringLen = lineLength
		packed = struct.pack(fmt_data, 0, numRecords, line[0:stringLen]);
		fout.write(packed);

		if debugIt:
			print("ID: %d, Str: %s"%(numRecords, line[0:stringLen]));
		line = fin.readline();
	fout.write(b'\x00\x00'); # ADD 00 00 to match Arduino SD written EOF.
	# Write header to new SD database table EDB_SD02.db
	#tsize = (rsize * numRecords + header_size + 2)
	fout.seek(0);
	header_tuple = struct.pack(fmt, numRecords, rsize, tsize)
	fout.write(header_tuple);
	if debugIt:
		print("tuple:", header_tuple);
		print("record size:", rsize);
		print("number of records:", numRecords);
		print("table_size:", tsize)
if fout:
	fout.close();
#with open("Example2.db", "rb") as f:
#	f.seek(0)
#	g = open("Example2.test", "wb")
#	byte = f.read(1)
#	count = 0
#	while byte:
#	# Do stuff with byte.
#		if ord(byte) > 128:
#			mod = True
#			#print 'Found Shitty byte at [{0}]: ord [{1}] -- [{2}]'.\
#			#		format( count, ord(byte), byte )
#			byte = chr(38) # '&'
#			#print '[{0}]'.format( byte )
#		g.write(byte)
#	byte = f.read(1) 
#	count+=1
#	g.close()


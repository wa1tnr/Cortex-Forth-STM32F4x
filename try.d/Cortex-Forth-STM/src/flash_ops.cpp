// flash_ops.cpp  wa1tnr
// Tue Sep  3 08:25:26 UTC 2019

// Wed Aug 21 02:15:00 UTC 2019 0.1.8 good-compiler-aa-bb  shred: abn-515

/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// #include <SPI.h>
// #include "SdFat.h"
// #include "Adafruit_SPIFlash.h"

extern void _FLOAD(void);
extern void _SPACE(void);
extern void forth_words(void);
extern void sam_editor(void);


#undef WANT_MKDIR_FORTH
#define WANT_MKDIR_FORTH

// File thisFile;
#include "../common.h"
// #define WORKING_DIR "/forth"
// #define VERBIAGE_AA #undef VERBIAGE_AA

#if defined(__SAMD51__) || defined(NRF52840_XXAA)
#ifdef NOT_DEFINED_STM32F405
  Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
#endif // #ifdef NOT_DEFINED_STM32F405
#else
  #if (SPI_INTERFACES_COUNT == 1)
    Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
  #else
#ifdef NOT_DEFINED_STM32F405
    Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
#endif // #ifdef NOT_DEFINED_STM32F405
  #endif
#endif

#ifdef NOT_DEFINED_STM32F405
Adafruit_SPIFlash flash(&flashTransport);
#endif // #ifdef NOT_DEFINED_STM32F405

// file system object from SdFat
FatFileSystem fatfs;

File myFile;

void _OK_OOB(void) { // OK, out of band
  SERIAL_LOCAL_C.println (" Ok");
}

void mkdir_forth(void) {
  if (!fatfs.exists(WORKING_DIR)) {
    Serial.print(WORKING_DIR);
    Serial.println(" directory not found, creating...");
    if (!fatfs.mkdir(WORKING_DIR)) {
      Serial.print("Error, failed to create ");
      Serial.print(WORKING_DIR);
      Serial.println(" directory!");
      Serial.println("Entering an endless loop (as a trap) after a delay of 4 seconds.");
      delay(4000);
      while(1);
    }
#ifdef VERBIAGE_AA
    Serial.print("Created ");
    Serial.print(WORKING_DIR);
    Serial.println(" directory!");
#else
    Serial.print(" +dcr ");
#endif // #ifdef VERBIAGE_AA
  } else {
#ifdef VERBIAGE_AA
      Serial.print(WORKING_DIR);
      Serial.println("  directory was previously created.  No worries.  Continuing.. ");
#else
      Serial.print(" +dpc ");
#endif // #ifdef VERBIAGE_AA
  }
}

// modeled on:

/*
 75   if (!fatfs.exists("/test")) {
 76     Serial.println("Test directory not found, creating...");
 77     // Use mkdir to create directory (note you should _not_ have a trailing slash).
 78     if (!fatfs.mkdir("/test")) {
 79       Serial.println("Error, failed to create test directory!");
 80       while(1);
 81     }
 82     Serial.println("Created test directory!");
 83   }

*/

/*
#define WRITE_FORTH(a) {myFile.print((a));}
*/

extern void _PINMODE(void);
extern void _PINWRITE(void);
extern void push(int n);
extern int pop(void);

void setup_blink_gpio(void) {
  push(1); push(13); _PINMODE();
}

void blink_awaiting_serial(void) {
  push(1); push(13); _PINWRITE(); // turn on LED D13
  delay(2); // rest
  push(0); push(13); _PINWRITE(); // turn off LED D13
  delay(3000);
}








void serial_setup(void) {
  // Open serial communications and wait for port to open:
  setup_blink_gpio(); // TODO: split out
  Serial.begin(38400);
  while (!Serial) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
    blink_awaiting_serial();
  }
  Serial.println("Connection to serial port established!");
}

void _LOAD_FILE(void) {
  push(-953);
}

void _WRITE_FILE(void) {
  push(-961);
}

void _REMOVE_FILE(void) {
  push(-927);
}

void _PRINT_FILE(void) {
  push(-949);
}

// void flash_setup(void) {
void _FL_SETUP(void) { // now an official Forth word

#ifdef VERBIAGE_AA
  Serial.print("\r\nInitializing Filesystem on external flash...");
#else
  Serial.print(" ckpt AA ");
#endif // #ifdef VERBIAGE_AA
  
  // Init external flash
#ifdef NOT_DEFINED_STM32F405
  flash.begin();
#endif // #ifdef NOT_DEFINED_STM32F405

  // Init file system on the flash
#ifdef NOT_DEFINED_STM32F405
  fatfs.begin(&flash);
#endif // #ifdef NOT_DEFINED_STM32F405

#ifdef VERBIAGE_AA
  Serial.println("initialization done.");
  _OK_OOB();
#else
  Serial.print(" ckpt BB ");
#endif // #ifdef VERBIAGE_AA
}







void _FILE_WRITE(void) {

/*
#ifdef WANT_MKDIR_FORTH
  mkdir_forth(); // tnr kludge
#endif // #ifdef WANT_MKDIR_FORTH
*/

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // myFile = fatfs.open(FILE_NAME, FILE_WRITE);

  // thisFile = (File) myFile; // local tnr kludge

  // if the file opened okay, write to it:
// if (myFile) {



#ifdef NOT_DEFINED_STM32F405
   if (thisFile) {
#ifdef VERBIAGE_AA
    Serial.print("Writing to "); Serial.print(FILE_NAME); Serial.print(" ");
#else
    Serial.print(" ckpt CC ");
#endif // #ifdef VERBIAGE_AA

// file contents - - - - - - - - - - - - - - - -

    forth_words();

    sam_editor(); // future: sam.fs and named file loading

// file contents - - - - - - - - - - - - - - - -

    // close the file:
    // myFile.close();
    thisFile.close();
#ifdef VERBIAGE_AA
    Serial.println(" has now been done.");
#else
    Serial.print(" ckpt DD ");
#endif // #ifdef VERBIAGE_AA
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening "); Serial.println(FILE_NAME);
  }
#endif // #ifdef NOT_DEFINED_STM32F405
}



















void _FILE_PRINT(void) { // re-open the file for reading: 
  myFile = fatfs.open(FILE_NAME); // thisFile = (File) myFile; // local tnr kludge 
  if (myFile) {
#ifdef VERBIAGE_AA
    Serial.print(FILE_NAME); Serial.println(" .. will now be read and printed");
    Serial.println("to the console.\r\n"); // no longer uses strange line endings scheme ;)
#else
    Serial.print(" ckpt EE ");
#endif // #ifdef VERBIAGE_AA // read from the file until there's nothing else in it:
    while (myFile.available()) {
#ifdef VERBIAGE_AA
      Serial.write(myFile.read());
#else
#ifdef FBUFF_PRN // suppress when undefined
      Serial.print("+");
#endif
      int ij = myFile.read(); // do the read anyway - try to break something
#endif // #ifdef VERBIAGE_AA
    } // close the file:
    myFile.close();
#ifdef VERBIAGE_AA
    Serial.println("\r\n");
    Serial.print(FILE_NAME); Serial.println(" .. is now closed, safely.");
#else
    Serial.print(" ckpt FF ");
#endif // #ifdef VERBIAGE_AA // re-open the file for reading:
 } else { // if the file didn't open, print an error:
    Serial.print("error opening "); Serial.println(FILE_NAME);
 }
}


#ifdef NOT_DEFINED_STM32F405
void _FILE_LOAD(void) { // temporary name for this 12 SEP 20:10z

  File dataFile = fatfs.open(FILE_NAME, FILE_READ);

  // Serial.println("DEBUG: LINE 281 reached");

  if (dataFile) {

    // Serial.println("DEBUG: LINE 285 reached");

#undef DONT_TEMP_PRINT
#define DONT_TEMP_PRINT

#ifndef DONT_TEMP_PRINT
    Serial.print(FILE_NAME);
    Serial.println(" is now re-opened (for reading).");
#endif

#ifdef VERBIAGE_AA
    Serial.print(FILE_NAME);
    Serial.println(" is now re-opened (for reading).");
#else
    Serial.print(" ckpt GG ");
#endif // #ifdef VERBIAGE_AA

    thisFile = (File) dataFile;
    thisFile.rewind();
#ifdef VERBIAGE_AA
    // Serial.println("FILE STAYS OPEN (and rewound) (for a possible fload).");
    Serial.println(" .. todo .. update this comment ;)");
#else
    Serial.print(" ckpt HH ");
#endif // #ifdef VERBIAGE_AA
    // _FLOAD(); //  sketchy to call this here tnr 12 SEP
  }
}
#endif // #ifdef NOT_DEFINED_STM32F405


#ifdef NOT_DEFINED_STM32F405
void _FILE_OPS(void) {
  int fflag = pop(); // must have a directive flag pushed onto the stack

  if (fflag == -927) { // -927 means remove the file
    if (!fatfs.remove(FILE_NAME)) {
      Serial.print("Failed to remove "); Serial.println(FILE_NAME);
      // Serial.println("Trap error Line 169");
      return;
    } else {
      // _OK();
      _OK_OOB();
      return;
    }
  } // -927 requested

  if (fflag == -949) { // -949 means print file contents to the terminal (read file, write console)
    _FILE_PRINT(); // big kludge two
    _OK_OOB();
    return;
  }

  if (fflag == -953) { // -953 means load the forth source code from 'disk' and compile/interpret it
    // Serial.println("FLAG seen: load");
    _FILE_LOAD();
    // Serial.println("TEST trap LINE 338");
    // while(-1);
    // Serial.println("OOBIFABI");
    _FLOAD(); // set the instruction ptr
    // Serial.println("_FLOAD did finish, perhaps.  LINE 344 of flash_ops.cpp");
    _OK_OOB();
    return;
  }

  if (fflag == -961) { // -961 means write out the forth source code to 'disk'
    _FILE_WRITE(); // big kludge
    _OK_OOB();
    return;
  }
  if (fflag == -982) { // -982 means mkdir_forth()
    mkdir_forth();
    _OK_OOB();
    return;
  }
  if (fflag == -555) { // -555 means generic and is just a test value
    _OK_OOB();
    return;
  }
  if
    (
      (fflag != -927) &&
      (fflag != -949) && // print
      (fflag != -953) && // load (open for reading to prep for fload word at the moment, MAYBE)
      (fflag != -961) &&
      (fflag != -982) &&
      (fflag != -555)
    ) {
    Serial.println("Trap error Line 172: UNRECOGNIZED file ops request");
    while(-1); // trap
  }
}

#endif // #ifdef NOT_DEFINED_STM32F405

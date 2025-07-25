//
// code taken from openFrameworks (http://www.openframeworks.cc)
//

#pragma once

#include "ofConstants.h"
//#include "ofTypes.h"

#if defined(TARGET_OSX) || defined(TARGET_LINUX) || defined(TARGET_ANDROID)
#include <termios.h>
#else
#include <iostream>
#include <regstr.h>
#include <setupapi.h>
#include <string.h>
#include <tchar.h>
#include <winbase.h>
#define MAX_SERIAL_PORTS 256
#include <winioctl.h>
#ifdef __MINGW32__
#define INITGUID
#include <initguid.h> // needed for dev-c++ & DEFINE_GUID
#endif
#endif

#include "../PluginManager/OpenEphysPlugin.h"
#define OF_SERIAL_NO_DATA -2
#define OF_SERIAL_ERROR -1
// notes below

class PLUGIN_API ofSerialDeviceInfo
{
    friend class ofSerial;

public:
    ofSerialDeviceInfo (string devicePathIn, string deviceNameIn, int deviceIDIn)
    {
        devicePath = devicePathIn;
        deviceName = deviceNameIn;
        deviceID = deviceIDIn;
    }

    ofSerialDeviceInfo()
    {
        deviceName = "device undefined";
        deviceID = -1;
    }

    string getDevicePath()
    {
        return devicePath;
    }

    string getDeviceName()
    {
        return deviceName;
    }

    int getDeviceID()
    {
        return deviceID;
    }

protected:
    string devicePath; //eg: /dev/tty.cu/usbdevice-a440
    string deviceName; //eg: usbdevice-a440 / COM4
    int deviceID; //eg: 0,1,2,3 etc

    //TODO: other stuff for serial ?
};

//----------------------------------------------------------------------
class PLUGIN_API ofSerial
{
public:
    ofSerial();
    virtual ~ofSerial();

    void listDevices();

    //old method - deprecated
    void enumerateDevices();

    vector<ofSerialDeviceInfo> getDeviceList();

    void close();
    bool setup(); // use default port, baud (0,9600)
    bool setup (string portName, int baudrate);
    bool setup (int deviceNumber, int baudrate);

    int readBytes (unsigned char* buffer, int length);
    int writeBytes (unsigned char* buffer, int length);
    bool writeByte (unsigned char singleByte);
    int readByte(); // returns -1 on no read or error...
    void flush (bool flushIn = true, bool flushOut = true);
    int available();

    void drain();

protected:
    void buildDeviceList();

    string deviceType;
    vector<ofSerialDeviceInfo> devices;

    bool bHaveEnumeratedDevices;

    bool bInited;

#ifdef TARGET_WIN32

    char** portNamesShort; //[MAX_SERIAL_PORTS];
    char** portNamesFriendly; ///[MAX_SERIAL_PORTS];
    HANDLE hComm; // the handle to the serial port pc
    int nPorts;
    bool bPortsEnumerated;
    void enumerateWin32Ports();
    COMMTIMEOUTS oldTimeout; // we alter this, so keep a record

#else
    int fd; // the handle to the serial port mac
    struct termios oldoptions;
#endif
};

//----------------------------------------------------------------------

// this serial code contains small portions of the following code-examples:
// ---------------------------------------------------
// http://todbot.com/arduino/host/arduino-serial/arduino-serial.c
// web.mac.com/miked13/iWeb/Arduino/Serial%20Write_files/main.cpp
// www.racer.nl/docs/libraries/qlib/qserial.htm
// ---------------------------------------------------

// notes:
// ----------------------------
// when calling setup("....") you need to pass in
// the name of the com port the device is attached to
// for example, on a mac, it might look like:
//
//      setup("/dev/tty.usbserial-3B1", 9600)
//
// and on a pc, it might look like:
//
//      setup("COM4", 9600)
//
// if you are using an arduino board, for example,
// you should check what ports you device is on in the
// arduino program

// to do:
// ----------------------------
// a)   support blocking / non-blocking
// b)   support numChars available type functions
// c)   can we reduce the number of includes here?

//  useful :
//  http://en.wikibooks.org/wiki/Serial_Programming:Unix/termios
//  http://www.keyspan.com/downloads-files/developer/win/USBSerial/html/DevDocsUSBSerial.html
// ----------------------------
// (also useful, might be this serial example - worth checking out:
// http://web.mit.edu/kvogt/Public/osrc/src/
// if has evolved ways of dealing with blocking
// and non-blocking instances)
// ----------------------------

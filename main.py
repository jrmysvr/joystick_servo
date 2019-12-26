import sys
import serial
import time
from colorama import init, Fore, Back, Style
from pynput.keyboard import Listener, Key

RIGHT = Key.right
LEFT = Key.left
UP = Key.up
DOWN = Key.down

ARROWS = [UP, DOWN, LEFT, RIGHT]
messages = [b'0', b'1', b'2', b'3']
msg_dict = dict(zip(ARROWS, messages))

init()
joystick = serial.Serial("/dev/ttyUSB0", timeout=60)


def send_key(key, serial):
    msg = msg_dict[key]
    serial.flush()
    serial.write(msg)
    serial.write(b"\n")


def on_press(key):
    if key in ARROWS:
        sys.stdout.flush()
        sys.stdout.write("\r")
        sys.stdout.write(Fore.GREEN + "Sending: " + str(key))
        sys.stdout.write("\n\n")
        send_key(key, joystick)


listener = Listener(on_press=on_press)
listener.start()

print(Back.RED + "STREAMING FROM ARDUINO", end="")
print(Back.RESET + "")
while True:
    joystick.flush()
    data = joystick.read_until(b'\r\n').decode('ascii')
    data = data.strip("\n")
    if len(data) > 1:
        sys.stdout.flush()
        sys.stdout.write("\r")
        sys.stdout.write(Fore.RED + data)
        sys.stdout.write("\n\n")

    time.sleep(.01)

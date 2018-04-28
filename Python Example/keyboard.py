#!/usr/bin/env python
import time
import sys
import tty, termios
import homie
import logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

config = homie.loadConfigFile("homie-python.json")
Homie = homie.Homie(config)
keyboardNode = Homie.Node("hardware", "keyboard")

def getch():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)
 
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch
 
def main():
    Homie.setFirmware("macbook", "1.0.0")
    Homie.setup()

    while True:
        keypress = str(getch())
        Homie.setNodeProperty(keyboardNode, "keypress", keypress, True)

if __name__ == '__main__':
    try:
        main()
    except (KeyboardInterrupt, SystemExit):
        logger.info("Quitting.")

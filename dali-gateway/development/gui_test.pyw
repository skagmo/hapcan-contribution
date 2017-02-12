#!/usr/bin/python

import dali
import Tkinter
from Tkinter import *
import Tkinter as ttk

d = dali("/dev/serial/by-id/pci-FTDI_FT232R_USB_UART_A700eYlU-if00-port0")

def set_dimlevel(level):
    level = int(level)
    if level > 0:
        d.set_arc(1, 150+level)
    else:
        d.set_arc(1, 0)

master = Tkinter.Tk()
master.title("DALI test")
dimlevel = DoubleVar()
Scale(master, orient=HORIZONTAL, length=300, command=set_dimlevel, from_=0, to=104, tickinterval=50).grid(column=0, row=0)
master.mainloop()

##5. Verify the programmed short address with VERIFY SHORT ADDRESS command
##6. This ballast is removed from the search process by WITHDRAW command
##7. This procedure is repeated until all ballasts have a short address.
##8. The process is stopped by the TERMINATE command 

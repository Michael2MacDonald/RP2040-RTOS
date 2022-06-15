
F_CPU := (120*1000000) # (120 MHz) This is the target core clock frequency. This my not be the actual clock frequency before the clock is configured.


# # USB device vendor ID
# VID_USB := 0x239A # 0x239A = Adafruit
# # USB device product ID
# PID_USB := 0x80f1 # 0x80f1 = Adafruit Feather RP2040
# # USB version that the device conforms to
# BCD_USB := 0x0200 # 0x0200 = USB 2.0
# USB_DEFINES := -DUSB_VID=$(VID_USB) -DUSB_PID=$(PID_USB) -DUSB_BCD=$(BCD_USB)


BOARD_DEFINES := -DF_CPU=$(F_CPU) #$(USB_DEFINES) #-D$(MCU_DEF) -D$(BOARD_DEF)
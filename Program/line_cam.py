import sensor, image, lcd, pyb, ustruct
#lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)

blue_threshold   = (0,   30,  -11,   15,   -15,   20)
ibiggest = [0,0,0,0]
angle = 0

bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit() # Fully reset I2C device...
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

while(True):
    img=sensor.snapshot()
    blobs=img.find_blobs([blue_threshold], x_stride=10, pixels_threshold=10, area_threshold=10, merge=True, margin=12)
    if (blobs == []):
        angle = -255

    for blob in blobs:

            tmp=img.draw_rectangle(blob[0:4])
            tmp=img.draw_cross(blob[5], blob[6])
            c=img.get_pixel(blob[5], blob[6])
            #print("Х= ", b[5]-160 , "Y= ", b[6]-120)
            angle=(blob[5]-160)*70/320
            #print("ANGLE= ",angle)


    try:
        bus.send(ustruct.pack("<h", len(str(angle))), timeout=100)
        try:
            bus.send(str(angle), timeout=100)
        except OSError as err:
            pass
    except OSError as err:
        pass
    #lcd.display(img)

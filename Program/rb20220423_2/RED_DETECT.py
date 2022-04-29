import sensor, image, lcd, pyb, ustruct
#lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)

blue_threshold   = (50,   200,  0,   100,   0,   100)
angle = 0

bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit() # Fully reset I2C device...
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

while(True):
    img=sensor.snapshot()
    r_blob = img.find_blobs([blue_threshold], pixels_threshold=200, area_threshold=500)
    for blob in r_blob:
            tmp=img.draw_rectangle(blob[0:4])
            tmp=img.draw_cross(blob[5], blob[6])
            c=img.get_pixel(blob[5], blob[6])
            #print("Х= ", b[5]-160 , "Y= ", b[6]-120)



    if (r_blob != []):
        is_line = 1
    else:
        is_line = -1



    print(r_blob)
    #print(blobs_b)
    #print(blobs_w)
    #print(blobs_large)

    #if (is_line == 1)
     #   last_b = time.clock()


    try:
        bus.send(ustruct.pack("<h", len(str(is_line))), timeout=100)
        try:
            bus.send(str(is_line), timeout=100)
        except OSError as err:
            pass
    except OSError as err:
        pass

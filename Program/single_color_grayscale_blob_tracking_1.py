# Single Color Grayscale Blob Tracking Example
#
# This example shows off single color grayscale tracking using the OpenMV Cam.

import sensor, image, time, math, pyb, ustruct, lcd

# Color Tracking Thresholds (Grayscale Min, Grayscale Max)
# The below grayscale threshold is set to only find extremely bright white areas.
thresholds_b = (0, 30)
thresholds_w = (230, 255)

bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit() # Fully reset I2C device...
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

is_line = -1
while(True):
    clock.tick()
    img = sensor.snapshot()
    img.draw_rectangle(135, 80, 55, 30, [10, 255, 10],  1, False)

    blobs_b = img.find_blobs([thresholds_b], roi=[135, 80, 55, 30], pixels_threshold=100, area_threshold=100, merge=True)
    for blob in blobs_b:
        # These values depend on the blob not being circular - otherwise they will be shaky.
        img.draw_cross(blob.cx(), blob.cy(), color=127)
    if (blobs_b != []):
        is_line = 1

    print(clock.fps())


    try:
        bus.send(ustruct.pack("<h", len(str(angle))), timeout=100)
        try:
            bus.send(str(is_line), timeout=100)
        except OSError as err:
            pass
    except OSError as err:
        pass

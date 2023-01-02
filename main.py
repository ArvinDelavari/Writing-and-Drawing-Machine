import sys
from svgpathtools import svg2paths2
import math
import time
import turtle
#import serial
#import serial.tools.list_ports
import paho.mqtt.client as mqtt

#print("Choose the serial port from the list below:")
#for portx, descx, hwidx in serial.tools.list_ports.comports():
#    print(portx)
#port = input()
#ser = serial.Serial(port, 9600)

broker = "mqtt.eclipseprojects.io"
port = 1883
keepalive = 60
topic = "writingmachine/instructions"
client = mqtt.Client()
client.connect(broker, port, keepalive)


rev_per_min = 5000
rev_per_sec = rev_per_min / 60
steps_per_rev = 200
trans_per_rev = 1 # (mm)
trans_per_step = trans_per_rev / steps_per_rev
regular_step_count = 200
regular_trans = regular_step_count * trans_per_step # (mm)

filename = sys.argv[1]
pos = 0 + 0j
pen_down = False

pen = turtle.Turtle()
screen = turtle.Screen()
pen.speed(0)
pen.up()
pen.goto(-300, 300)
pen.down()

paths, attributes, svg_attributes = svg2paths2(filename)

for path in paths:
    for seg in path:
        pen_down = False

        seg_length = seg.length()
        n = math.ceil(seg_length / regular_trans)
        if (n == 0):
            continue
        dt = 1 / n
        for i in range(n):
            point = seg.point(dt * i)
            dp = point - pos
            steps = dp / trans_per_step
            penpos = pen.pos()
            pen.goto(penpos[0] + 3.7795275591 * steps.real * trans_per_step, penpos[1] - 3.7795275591 * steps.imag * trans_per_step)
            pos = point

            sleep_time = max(abs(steps.real), abs(steps.imag)) / steps_per_rev / rev_per_sec + 0.3
            x_steps = int(round(steps.real))
            y_steps = int(round(steps.imag))
            x_dir = "+" if x_steps >= 0 else "-"
            y_dir = "+" if y_steps >= 0 else "-"
            serial_str = f"{10*abs(x_steps)};{x_dir};{10*abs(y_steps)};{y_dir};"
            #ser.write(serial_str.encode('utf-8'))
            client.publish(topic, serial_str)
            print(serial_str)
            print(f"sent: {serial_str}, sleep: {sleep_time}")
            time.sleep(sleep_time)

            if (i == 0):
                pen_down = True
                pen.down()

input("Press enter to reset")

dp = -pos
steps = dp / trans_per_step
pen.goto(penpos[0] + 3.7795275591 * steps.real * trans_per_step, penpos[1] - 3.7795275591 * steps.imag * trans_per_step)
x_steps = int(round(steps.real))
y_steps = int(round(steps.imag))
x_dir = "+" if x_steps >= 0 else "-"
y_dir = "+" if y_steps >= 0 else "-"
serial_str = f"{10*abs(x_steps)};{x_dir};{10*abs(y_steps)};{y_dir};"
#ser.write(serial_str.encode('utf-8'))
client.publish(topic, serial_str)
print(serial_str)
print(f"sent: {serial_str}")

#ser.close()
client.disconnect()

screen.update()
screen.mainloop()
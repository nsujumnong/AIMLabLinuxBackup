# Abhishek Kashyap

# subscriber script to convert ADC values to voltage

import rospy
from rosserial_arduino.msg import Adc
from std_msgs.msg import Float32

global msg
msg = Float32()

def ros_cd(data):
	global msg
	msg = float(data.adc0)
	msg = (msg * 5.0) / 1024.0
	print 'adc {} msg {} '.format(data.adc0, msg)

rospy.init_node('myNode')
sub = rospy.Subscriber('/adc', Adc, ros_cd, queue_size = 1)

pub = rospy.Publisher('/processed', Float32, queue_size = 1)
rate = rospy.Rate(1000)

while not rospy.is_shutdown():
	pub.publish(msg)
	rate.sleep()

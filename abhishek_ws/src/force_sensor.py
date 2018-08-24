# Abhishek Kashyap

# convert voltage variations to velocity

import rospy
from rosserial_arduino.msg import Adc
from std_msgs.msg import Float32, Float64, Header
from sensor_msgs.msg import JointState
import numpy as np
#from numpy import interp

global sensor1,sensor2, sensor3, sensor4
sensor1 = Float32()
sensor2 = Float32()
sensor3 = Float32()
sensor4 = Float32()

def ros_cd(data):
	global sensor1, sensor2, sensor3, sensor4

	#bias_S1, bias_S2, bias_S3, bias_S4 = 2.5, 2.8, 2.8, 2.7;
	bias_S1, bias_S2, bias_S3, bias_S4 = 0.0, 0.0, 0.0, 0.0;
	
	sensor1 = (float(data.adc0) * 5.0 / 1024.0) - bias_S1
	sensor2 = (float(data.adc1) * 5.0 / 1024.0) - bias_S2
	sensor3 = (float(data.adc2) * 5.0 / 1024.0) - bias_S3
	sensor4 = (float(data.adc3) * 5.0 / 1024.0) - bias_S4

	#print 'S1 = {}, S2 = {}, S3 = {}, S4 = {}'.format(sensor1, sensor2, sensor3, sensor4)


rospy.init_node('nodeA')
rospy.Subscriber('/adc', Adc, ros_cd, queue_size = 1)
#print "testing=", sensor1
#outer_yaw_joint_vel = np.interp(sensor1, [2.6, 3.1], [0, 5])
outer_yaw_joint_vel = sensor1.data
print "outer yaw joint vel = ", outer_yaw_joint_vel
#print "type: ", type(outer_yaw_joint_vel)


new_state = rospy.Publisher('/joint_states', JointState, queue_size = 1)


#S1 = rospy.Publisher('/sensor1', Float32, queue_size = 1)
#S2 = rospy.Publisher('/sensor2', Float32, queue_size = 1)
#S3 = rospy.Publisher('/sensor3', Float32, queue_size = 1)
#S4 = rospy.Publisher('/sensor4', Float32, queue_size = 1)


msg = JointState()
msg.name = 'right_outer_yaw_joint'
print "msg.position = ", msg.position
print "type: ", type(msg.position)
#msg.position = msg.position[0] + outer_yaw_joint_vel
#msg.position = [0.0]
#print "msg.position = ", msg.position[0]
#print "type: ", type(msg.position)


rate = rospy.Rate(100)

while not rospy.is_shutdown():
	#S1.publish(sensor1)
	#S1.publish(sensor2)
	#S1.publish(sensor3)
	#S1.publish(sensor4)

	#current_position = np.array(msg.position)
	#delta_position = np.array(outer

	#msg.position = [msg.position[0] + outer_yaw_joint_vel]
	new_state.publish(msg)
	rate.sleep()


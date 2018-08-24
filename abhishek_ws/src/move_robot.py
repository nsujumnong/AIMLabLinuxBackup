# Abhishek Kashyap

# convert voltage variations to velocity

import rospy
from std_msgs.msg import Float32, Float64

rospy.init_node('nodeB')
velocity = rospy.Subscriber('/yaw_vel', Float64, queue_size = 1)

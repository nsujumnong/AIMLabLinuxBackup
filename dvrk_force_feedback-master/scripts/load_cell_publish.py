import dvrk
import utilities
from std_msgs.msg import Float32
import rospy
import numpy as np


if __name__ == '__main__':
    # create node
    rospy.init_node('force_data', anonymous=True)
    # create publishers
    pub_lc = rospy.Publisher('/force_data/load_cell', Float32, queue_size=1)
    lc = utilities.XYdataFromADC(0)

    rate = rospy.Rate(500)

    npz_lc_data = np.load("load_cell_linear_equation_parameters.npz")
    lc_lin_eq_param = npz_lc_data['lc_equation_parameters']
    print 'a=', lc_lin_eq_param[0]
    print 'b=', lc_lin_eq_param[1]

    while not rospy.is_shutdown():
        force_m = lc_lin_eq_param[0] * lc.get_value() + lc_lin_eq_param[1]

        # publish data
        pub_lc.publish(Float32(force_m))

        rate.sleep()

#  Uses Polaris and two optical markers to find forces in x, y, z

import utilities
import rospy
import numpy as np
from std_msgs.msg import Float32
from collections import deque

force_m = 0     # force magnitude measured from the load cell
size = 100
deq_adc_z = deque(maxlen=size)


if __name__ == '__main__':

    # create node
    rospy.init_node('force_load_cell', anonymous=True)
    pub_f_lc = rospy.Publisher('/force_load_cell/force_lc', Float32, queue_size=1)

    # create classes
    lc = utilities.ZLCdataFromADC(0)

    npz_lc_data = np.load("load_cell_linear_equation_parameters.npz")
    lc_lin_eq_param = npz_lc_data['lc_equation_parameters']
    rospy.sleep(1.)
	    # get reading when force is 0
    for i in range(0, 100):
        deq_adc_z.append(lc.get_value())

    # find average b parameter to delete offset error
    b_lc = -np.mean(deq_adc_z)*lc_lin_eq_param[0]
    rate = rospy.Rate(500)
    while not rospy.is_shutdown():
            # use calibration equation to transform ADC values to Forces
            force_m = lc_lin_eq_param[0] * lc.get_value() + b_lc
            # publish data
            pub_f_lc.publish(Float32(force_m))
            rate.sleep()



#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
/*
 * Vision Pipeline -AIM Labs
 * -This code, specificaly the subscribing, converting, and publishing of images,
 * has been adapted from the cv_bridge ROS package tutorial
 * (on how to create a c++ based image bridge)
 *
 * Code was assembled for initial Vision Pipeline use by Matt Rafferty (contact at: rmrafferty@wpi.edu)
 * */

//Declare constants for display window names
static const std::string OPENCV_WINDOW_RIGHT = "Image window Right";
static const std::string OPENCV_WINDOW_LEFT = "Image window Left";

//Initilize class for stereo Image subscribing, editing, and publishing
class StereoEdit
{
  ros::NodeHandle nh_;
  //declare image transport variable
  image_transport::ImageTransport it_;
  //declare image subscribers and publishers for left and right camera
  image_transport::Subscriber image_sub_right;
  image_transport::Publisher image_pub_right;
  image_transport::Subscriber image_sub_left;
  image_transport::Publisher image_pub_left;
  
public:
  //create class instance and image transport instance
  StereoEdit()
    : it_(nh_)
  {
    //Right eye
    // Subscribe to input video feed and publish output video feed
    image_sub_right = it_.subscribe("/dv_vp_camera/right", 1, &StereoEdit::imageCbR, this);
    image_pub_right = it_.advertise("/dv_vp_camera/edited_right", 1);

    cv::namedWindow(OPENCV_WINDOW_RIGHT);

    //Left Eye
    // Subscribe to input video feed and publish output video feed
    image_sub_left = it_.subscribe("/dv_vp_camera/left", 1, &StereoEdit::imageCbL, this);
    image_pub_left = it_.advertise("/dv_vp_camera/edited_left", 1);

    cv::namedWindow(OPENCV_WINDOW_LEFT);
  }

  ~StereoEdit()
  {
    cv::namedWindow(OPENCV_WINDOW_RIGHT);
    cv::namedWindow(OPENCV_WINDOW_LEFT);
  }

  void imageCbR(const sensor_msgs::ImageConstPtr& msg)
  {
    //create pointer to cv image
    cv_bridge::CvImagePtr cv_ptr;

    //try to copy ROS image message to cv_bridge style message (holder for cv Mat image)
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }


    //---------------------------------------------------------------------------------
    /*
     * EDIT RIGHT IMAGE CODE HERE
     * -address image with:
     *      cv_ptr->image
     *
     * */
    //Example Function: Draw a Red Circle
    // Draw an example circle on the video stream
    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));




    //---------------------------------------------------------------------------------
	

    // Update GUI Window
    cv::imshow(OPENCV_WINDOW_RIGHT, cv_ptr->image);
    cv::waitKey(3);
    
    // Output modified video stream
    image_pub_right.publish(cv_ptr->toImageMsg());
  }

  void imageCbL(const sensor_msgs::ImageConstPtr& msg)
  {
    //create pointer to cv image
    cv_bridge::CvImagePtr cv_ptr;

    //try to copy ROS image message to cv_bridge style message (holder for cv Mat image)
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }


    //---------------------------------------------------------------------------------
    /*
     * EDIT LEFT IMAGE CODE HERE
     * -address image with:
     *      cv_ptr->image
     *
     * */
    //Example Function: Draw a Red Circle
    // Draw an example circle on the video stream
    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));




    //---------------------------------------------------------------------------------

    // Update GUI Window
    cv::imshow(OPENCV_WINDOW_LEFT, cv_ptr->image);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_left.publish(cv_ptr->toImageMsg());
  }

};

int main(int argc, char** argv)
{
  //Indicate code start on terminal
  std::cout << "Code Start: pkg video_access_point script stereo_edit \n";

  //Initilize node
  ros::init(argc, argv, "stereo_edit");
  StereoEdit ic;
  ros::spin();
  return 0;
}

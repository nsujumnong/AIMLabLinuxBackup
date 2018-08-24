#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
/*
 * Vision Pipeline -AIM Labs
 *
 * stereo_edit_dual_access
 * -Modified stereo_edit node that allows acccess to both images at once for stereo image calculations
 * -Edit at the sedctions indicated by //------ to edit images
 *
 * -This code, specificaly the subscribing, converting, and publishing of images,
 * has been adapted from the cv_bridge ROS package tutorial.
 * (on how to create a c++ based image bridge)
 *
 * Code was assembled for initial Vision Pipeline use by Matt Rafferty (contact at: rmrafferty@wpi.edu)
 * */

//Declare constants for display window names
static const std::string OPENCV_WINDOW_RIGHT = "Image window Right";
static const std::string OPENCV_WINDOW_LEFT = "Image window Left";
static const std::string OPENCV_WINDOW_AUG_RIGHT = "Image window Augment Right";
static const std::string OPENCV_WINDOW_AUG_LEFT = "Image window Augment Left";
//Create flags to indicate arrivale and edit of images per eye
bool newEditRight = false;
bool newEditLeft = false;
bool newEditAugRight = false;
bool newEditAugLeft = false;
//Create image pointers to hold ROS image messages converted into cv_bridge format wit OpenCV Mat image format
cv_bridge::CvImagePtr cv_ptr_right;
cv_bridge::CvImagePtr cv_ptr_left;
cv_bridge::CvImagePtr cv_ptr_aug_right;
cv_bridge::CvImagePtr cv_ptr_aug_left;


//Initilize class for stereo Image subscribing, editing, and publishing
class StereoEditQuadAccess
{
  ros::NodeHandle nh_;
  //declare image transport variable
  image_transport::ImageTransport it_;
  //declare image subscribers and publishers for left and right camera
  image_transport::Subscriber image_sub_right;
  image_transport::Publisher image_pub_right;
  image_transport::Subscriber image_sub_left;
  image_transport::Publisher image_pub_left;

  image_transport::Subscriber image_sub_aug_right;
  image_transport::Publisher image_pub_aug_right;
  image_transport::Subscriber image_sub_aug_left;
  image_transport::Publisher image_pub_aug_left;


public:
  //create class instance and image transport instance
  StereoEditQuadAccess()
    : it_(nh_)
  {
    //Right eye
    // Subscribe to input video feed and publish output video feed
    image_sub_right = it_.subscribe("/dv_vp_camera/right", 1, &StereoEditQuadAccess::imageCbR, this);
    image_pub_right = it_.advertise("/dv_vp_camera/edited_right", 1);

    cv::namedWindow(OPENCV_WINDOW_RIGHT);

    //Left Eye
    // Subscribe to input video feed and publish output video feed
    image_sub_left = it_.subscribe("/dv_vp_camera/left", 1, &StereoEditQuadAccess::imageCbL, this);
    image_pub_left = it_.advertise("/dv_vp_camera/edited_left", 1);

    cv::namedWindow(OPENCV_WINDOW_LEFT);

    //Virtual Camera Right eye
    // Subscribe to input video feed and publish output video feed
    image_sub_aug_right = it_.subscribe("/dv_vp_camera/right", 1, &StereoEditQuadAccess::imageCbAR, this); //TODO Change this subsciption address to that of your right eye virtual camera in the da Vinci Simulation system
    image_pub_aug_right = it_.advertise("/dv_vp_camera/edited_aug_right", 1);

    cv::namedWindow(OPENCV_WINDOW_AUG_RIGHT);

    //Virtual Camera Left Eye
    // Subscribe to input video feed and publish output video feed
    image_sub_aug_left = it_.subscribe("/dv_vp_camera/left", 1, &StereoEditQuadAccess::imageCbAL, this); //TODO Change this subsciption address to that of your left eye virtual camera in the da Vinci Simulation system
    image_pub_aug_left = it_.advertise("/dv_vp_camera/edited_aug_left", 1);

    cv::namedWindow(OPENCV_WINDOW_AUG_LEFT);



  }

  ~StereoEditQuadAccess()
  {
    cv::namedWindow(OPENCV_WINDOW_RIGHT);
    cv::namedWindow(OPENCV_WINDOW_LEFT);
    cv::namedWindow(OPENCV_WINDOW_AUG_RIGHT);
    cv::namedWindow(OPENCV_WINDOW_AUG_LEFT);
  }
  //call back function for frame grabs

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
      ROS_ERROR("cv_bridge exception at copy right eye image message: %s", e.what());
      return;
    }




    cv_ptr_right = cv_ptr;



    //Raise Flag to tell main that a new right edited image is ready to be published
    newEditRight = true;

    //cv::imshow(OPENCV_WINDOW_RIGHT, cv_ptr_right->image);
    //cv::waitKey(3);




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
      ROS_ERROR("cv_bridge exception at copy left eye image message: %s", e.what());
      return;
    }

    //assign reterived value to the global variable
    cv_ptr_left = cv_ptr;




    //Raise Flag to tell main that a new left edited image is ready to be published
    newEditLeft = true;


  }

  //Call back functions for augmentation frames from virtual cameras------

  void imageCbAR(const sensor_msgs::ImageConstPtr& msg)
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
      ROS_ERROR("cv_bridge exception at copy right virtual eye image message: %s", e.what());
      return;
    }




    cv_ptr_aug_right = cv_ptr;



    //Raise Flag to tell main that a new virtual camera right edited image is ready to be published
    newEditAugRight = true;

    //cv::imshow(OPENCV_WINDOW_AUG_RIGHT, cv_ptr_aug_right->image);
    //cv::waitKey(3);




  }




  void imageCbAL(const sensor_msgs::ImageConstPtr& msg)
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
      ROS_ERROR("cv_bridge exception at copy left eye image message: %s", e.what());
      return;
    }

    //assign reterived value to the global variable
    cv_ptr_aug_left = cv_ptr;




    //Raise Flag to tell main that a new virtual camera left edited image is ready to be published
    newEditAugLeft = true;


  }






  void imagesEditPublish(cv_bridge::CvImagePtr right, cv_bridge::CvImagePtr left, cv_bridge::CvImagePtr augRight, cv_bridge::CvImagePtr augLeft){


      //---------------------------------------------------------------------------------
      /*
       * EDIT RIGHT AND LEFT IMAGE CODE HERE
       * -address image with:
       *      right->image
       *      left->image
       *
       * */
      //TODO Replace circle overlays with your custom vision applcation image processing
      //Example Function: Draw a Red Circle
      // Draw an example circle on the right video stream
      if (right->image.rows > 60 && right->image.cols > 60)
        cv::circle(right->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));



      //Example Function: Draw a Red Circle
      // Draw an example circle on the left video stream
      if (left->image.rows > 60 && left->image.cols > 60)
        cv::circle(left->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));



      //Example Function: Draw a Circle
      // Draw an example circle on the right virtual camera video stream
      if (augRight->image.rows > 60 && augRight->image.cols > 60)
        cv::circle(augRight->image, cv::Point(50, 50), 10, CV_RGB(0,0,255));



      //Example Function: Draw a Circle
      // Draw an example circle on the left virtual camera video stream
      if (augLeft->image.rows > 60 && augLeft->image.cols > 60)
        cv::circle(augLeft->image, cv::Point(50, 50), 10, CV_RGB(0,0,255));


      //---------------------------------------------------------------------------------

      //publish edited images
      image_pub_right.publish(right->toImageMsg());
      image_pub_left.publish(left->toImageMsg());
      image_pub_aug_right.publish(augRight->toImageMsg());
      image_pub_aug_left.publish(augLeft->toImageMsg());

      //display edited images Update GUI Window
      cv::imshow(OPENCV_WINDOW_RIGHT, right->image);
      cv::waitKey(3);
      cv::imshow(OPENCV_WINDOW_LEFT, left->image);
      cv::waitKey(3);
      cv::imshow(OPENCV_WINDOW_AUG_RIGHT, augRight->image);
      cv::waitKey(3);
      cv::imshow(OPENCV_WINDOW_AUG_LEFT, augLeft->image);
      cv::waitKey(3);

  }

};

int main(int argc, char** argv)
{
  //Indicate code start on terminal
  std::cout << "Code Start: pkg video_access_point script stereo_edit_quad_access info access \n";

  //Initilize node
  ros::init(argc, argv, "stereo_edit_quad_access");
  StereoEditQuadAccess seqa;


  //loopingly publish edited images once both are created

 ros::Rate r(60); // 60 hz loop rate
  while(ros::ok()){
      if(newEditRight == true && newEditLeft == true && newEditAugRight == true && newEditAugLeft == true){
          newEditRight= false;
          newEditLeft = false;
          newEditAugRight= false;
          newEditAugLeft = false;

         // copy latest images to constant variables (to avoid updates resulting in out of sync image frame sets)
          cv_bridge::CvImagePtr rightHold = cv_ptr_right;
          cv_bridge::CvImagePtr leftHold = cv_ptr_left;
          cv_bridge::CvImagePtr augRightHold = cv_ptr_aug_right;
          cv_bridge::CvImagePtr augLeftHold = cv_ptr_aug_left;

          //call edit function
          seqa.imagesEditPublish(rightHold,leftHold, augRightHold, augLeftHold);

          //display edited images Update GUI Window
          cv::imshow(OPENCV_WINDOW_RIGHT, cv_ptr_right->image);
          cv::imshow(OPENCV_WINDOW_LEFT, cv_ptr_left->image);
          cv::imshow(OPENCV_WINDOW_AUG_RIGHT, cv_ptr_aug_right->image);
          cv::imshow(OPENCV_WINDOW_AUG_LEFT, cv_ptr_aug_left->image);


      }
      ros::spinOnce();
      r.sleep();

 }
  ros::spin();
  return 0;
}

#ifndef APRILTAG_DETECTOR_SLAM_H
#define APRILTAG_DETECTOR_SLAM_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include<nav_msgs/Odometry.h>
#include <AprilTags/TagDetector.h>
#include <tf/transform_broadcaster.h>
//#include<slam_apriltag_isam_single/PoseStampedArray.h>
#include<slam_april_laser_odom/PoseStampedArray.h>
namespace apriltags_ros{


class AprilTagDescription{
 public:
  AprilTagDescription(int id, double size, std::string &frame_name,double x,double y,double z):id_(id), size_(size), frame_name_(frame_name),lo_x(x),lo_y(y),lo_z(z){}
  double size(){return size_;}
  int id(){return id_;} 
  double getx(){return lo_x;}
  double gety(){return lo_y;}
  double getz(){return lo_z;}
  std::string& frame_name(){return frame_name_;} 
 private:
  int id_;
  double size_;
  std::string frame_name_;
  //wxf:add
  double lo_x;
  double lo_y;
  double lo_z;
};


class AprilTagDetector{
 public:
  AprilTagDetector(ros::NodeHandle& nh, ros::NodeHandle& pnh);
  ~AprilTagDetector();
 private:
  void imageCb(const sensor_msgs::ImageConstPtr& msg);
  std::map<int, AprilTagDescription> parse_tag_descriptions(XmlRpc::XmlRpcValue& april_tag_descriptions);

 private:
  std::map<int, AprilTagDescription> descriptions_;
  std::string sensor_frame_id_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  ros::Publisher detections_pub_;
  ros::Publisher pose_pub_;
  tf::TransformBroadcaster tf_pub_;
  boost::shared_ptr<AprilTags::TagDetector> tag_detector_;
private:
  //wxf: for location
  ros::Subscriber turtle_sub_;
  ros::Publisher odomAndpose_pub_;
  tf::Transform turtle_odom;
  void OdomCb(const nav_msgs::OdometryConstPtr& msg);
  tf::Transform camera2world;
  //test for tag_id
  /*
  std::vector<double>l0={0,0,0,0,0,0,1};
   std::vector<double> l1={0.6,0,0,0,0,0,1};
   std::vector<double> l2={1.2,0,0,0,0,0,1};
   std::vector<double> l3={0,0,0,0,0,0,1};
  int tag_id[50]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
                 36,37,38,39,40,41,42,43,44,45,46,47,48,49};
  std::map< int,  std::vector<double> > maplo;
  tf::Pose GetTagPose(int tag_id);
  */
  //*****************************************************
};



}


#endif

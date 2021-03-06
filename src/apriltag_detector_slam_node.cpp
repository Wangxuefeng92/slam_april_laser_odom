#include <apriltags_ros/apriltag_detector_slam.h>
#include <ros/ros.h>

int main(int argc, char **argv){
  ros::init(argc, argv, "apriltag_detector_slam");
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  apriltags_ros::AprilTagDetector detector(nh, pnh);
  //wxf:for location
  //the location of tag in the world map;
  //detector.maplo.insert(std::pair<int,std::vector<double> >(detector.tag_id[0],detector.l0));
  //detector.maplo.insert(std::pair<int,std::vector<double> >(detector.tag_id[1],detector.l1));
  //detector.maplo.insert(std::pair<int,std::vector<double> >(detector.tag_id[2],detector.l2));
  //detector.maplo.insert(std::pair<int,std::vector<double> >(detector.tag_id[3],detector.l3));
  //************************************************
  ros::spin();
}

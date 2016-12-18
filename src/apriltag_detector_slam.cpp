#include <apriltags_ros/apriltag_detector_slam.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <boost/foreach.hpp>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>
//#include <apriltags_ros/AprilTagDetection.h>
//#include <apriltags_ros/AprilTagDetectionArray.h>
#include<slam_april_laser_odom/PoseStampedArray.h>
#include <AprilTags/Tag16h5.h>
#include <AprilTags/Tag25h7.h>
#include <AprilTags/Tag25h9.h>
#include <AprilTags/Tag36h9.h>
#include <AprilTags/Tag36h11.h>
#include <XmlRpcException.h>
#include<eigen3/Eigen/Eigen>
#include<Eigen/Eigen>
#include<tf2_eigen/tf2_eigen.h>
namespace apriltags_ros{

AprilTagDetector::AprilTagDetector(ros::NodeHandle& nh, ros::NodeHandle& pnh): it_(nh){

  XmlRpc::XmlRpcValue april_tag_descriptions;
  if(!pnh.getParam("tag_descriptions", april_tag_descriptions)){
    ROS_WARN("No april tags specified");
  }
  else{
    try{
      descriptions_ = parse_tag_descriptions(april_tag_descriptions);
    } catch(XmlRpc::XmlRpcException e){
      ROS_ERROR_STREAM("Error loading tag descriptions: "<<e.getMessage());
    }
  }

  if(!pnh.getParam("sensor_frame_id", sensor_frame_id_)){
   sensor_frame_id_ = "";
  }
  //convet camera to world frame
  tf::Matrix3x3 m2;
  //m2.setValue(0,1,0,1,0,0,0,0,-1);
  m2.setValue(0,-1,0, -1,0,0,0,0,-1);
  camera2world.setIdentity();
  camera2world.setBasis(m2);
  //*************************************************
  AprilTags::TagCodes tag_codes = AprilTags::tagCodes36h11;
  tag_detector_= boost::shared_ptr<AprilTags::TagDetector>(new AprilTags::TagDetector(tag_codes));
  image_sub_ = it_.subscribe("image_rect", 1, &AprilTagDetector::imageCb, this);
  //wxf:add for subscribe odom
  //turtle_sub_=nh.subscribe<nav_msgs::Odometry>("correctOdom",1, &AprilTagDetector::OdomCb, this);
  turtle_sub_=nh.subscribe<nav_msgs::Odometry>("odom",1, &AprilTagDetector::OdomCb, this);
  odomAndpose_pub_ = nh.advertise<slam_april_laser_odom::PoseStampedArray>("tag_odom_pose", 1);
  //***********************************************************************************************************
  image_pub_ = it_.advertise("tag_detections_image", 1);
  //detections_pub_ = nh.advertise<AprilTagDetectionArray>("tag_detections", 1);
  //pose_pub_ = nh.advertise<geometry_msgs::PoseArray>("tag_detections_pose", 1);
  pose_pub_ = nh.advertise<geometry_msgs::PoseStamped>("tag_detections_pose", 1);
}
AprilTagDetector::~AprilTagDetector(){
  image_sub_.shutdown();
}
/*
tf::Pose AprilTagDetector::GetTagPose(int tag_id)
{
    tf::Pose TagPose;
    vector<double> temp_location;
    std::map<int,vector<double> > ::iterator it;
    it=maplo.find(tag_id);
    if(it!=maplo.end())
    {
        temp_location=it->second;
        tf::Vector3 orig(temp_location[0],temp_location[1],temp_location[2]);
        tf::Quaternion Q(temp_location[3],temp_location[4],temp_location[5],temp_location[6]);
        TagPose.setOrigin(orig);
        TagPose.setRotation(Q);
    }
    return TagPose;
}
*/
void AprilTagDetector::OdomCb(const nav_msgs::OdometryConstPtr &msg)
{
    turtle_odom.setOrigin(tf::Vector3(msg->pose.pose.position.x,msg->pose.pose.position.y,msg->pose.pose.position.z));
    turtle_odom.setRotation(tf::Quaternion(msg->pose.pose.orientation.x,msg->pose.pose.orientation.y,msg->pose.pose.orientation.z,msg->pose.pose.orientation.w));
ROS_INFO("first odom(x:%f,y:%f)",msg->pose.pose.position.x,msg->pose.pose.position.y);
}
void AprilTagDetector::imageCb(const sensor_msgs::ImageConstPtr& msg){
  cv_bridge::CvImagePtr cv_ptr;
  try{
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e){
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  //ROS_INFO_STREAM("haha");
  cv::Mat gray;
  cv::cvtColor(cv_ptr->image, gray, CV_BGR2GRAY);
  std::vector<AprilTags::TagDetection>	detections = tag_detector_->extractTags(gray);
  ROS_DEBUG("%d tag detected", (int)detections.size());
//ROS_INFO("%d tag detected", (int)detections.size());

  sensor_msgs::CameraInfoPtr cam_info(new sensor_msgs::CameraInfo);
  boost::array<double,9> param={691.743632581011, 0.0, 376.5170843870497, 0.0, 691.7146437795241, 250.7975382456949, 0.0, 0.0, 1.0};
  cam_info->K =  param;
  double fx = cam_info->K[0];
  double fy = cam_info->K[4];
  double px = cam_info->K[2];
  double py = cam_info->K[5];

  if(!sensor_frame_id_.empty())
    cv_ptr->header.frame_id = sensor_frame_id_;

//ROS_INFO("%s",cv_ptr->header.frame_id.c_str());


  //AprilTagDetectionArray tag_detection_array;
  //geometry_msgs::PoseArray tag_pose_array;
  //tag_pose_array.header = cv_ptr->header;

  BOOST_FOREACH(AprilTags::TagDetection detection, detections){
    std::map<int, AprilTagDescription>::const_iterator description_itr = descriptions_.find(detection.id);
    if(description_itr == descriptions_.end()){
      ROS_WARN_THROTTLE(10.0, "Found tag: %d, but no description was found for it", detection.id);
      continue;
    }
    AprilTagDescription description = description_itr->second;
    double tag_size = description.size();
    stringstream stream;
    stream<<description.id();
    string id_str;
    stream>>id_str;

    //  ROS_INFO("%lf",tag_size);

    //detection.draw(cv_ptr->image);
    Eigen::Matrix4d transform = detection.getRelativeTransform(tag_size, fx, fy, px, py);
    Eigen::Matrix3d rot = transform.block(0,0,3,3);
    Eigen::Quaternion<double> rot_quaternion = Eigen::Quaternion<double>(rot);
//WXF:add
    //first:get tag pose
    tf::Vector3 orig(description.getx(),description.gety(),description.getz());
    tf::Pose TagPose;
    TagPose.setIdentity();
    TagPose.setOrigin(orig);
    //ROS_INFO("this tag's location is(x:%.3f, y:%.3f. z:%.3f,,,,,x:%.3f, y:%.3f. z:%.3f w:%.3f) ",TagPose.getOrigin().getX(),TagPose.getOrigin().getY(),TagPose.getOrigin().getZ(),TagPose.getRotation().getX(),TagPose.getRotation().getY(),TagPose.getRotation().getZ(),TagPose.getRotation().getW());
    //second:获取tag和camera的相对tf信息
    //get the transform between tag and camera
    tf::Transform tag2camera_tf,tag2robot;
    tf::Quaternion Q(rot_quaternion.x(),rot_quaternion.y(),rot_quaternion.z(),rot_quaternion.w());
    tf::Vector3 origin(transform(0,3),transform(1,3),transform(2,3));
    tag2camera_tf.setRotation(Q);
    tag2camera_tf.setOrigin(origin);
    //test:
    //tag2camera_tf.setIdentity();
    //tag2camera_tf.setOrigin(tf::Vector3(1,2,0));
    tag2robot=camera2world.operator *(tag2camera_tf);
    double _yew_robot,_pich_robot,_roll_robot;
    double _yew_camera,_pich_camera,_roll_camera;
    tag2robot.getBasis().getEulerZYX(_yew_robot,_pich_robot,_roll_robot);
    _yew_robot=_yew_robot+M_PI/2;
    if(_yew_robot>M_PI)
    {
        _yew_robot-=2*M_PI;
    }
    else if(_yew_robot<-M_PI)
    {
        _yew_robot+=2*M_PI;
    }
    tf::Quaternion qq=tf::createQuaternionFromRPY(_roll_robot,_pich_robot,_yew_robot);
    tag2robot.setRotation(qq);
    tag2camera_tf.getBasis().getEulerZYX(_yew_camera,_pich_camera,_roll_camera);
    ROS_INFO("tag2camera_tf is(x:%.3f, y:%.3f. z:%.3f,,,,,angle:%.3f) ",tag2camera_tf.getOrigin().getX(),tag2camera_tf.getOrigin().getY(),tag2camera_tf.getOrigin().getZ(),_yew_camera*180/3.1415926);
    ROS_INFO("tag2robot is(x:%.3f, y:%.3f. z:%.3f,,,,,angle:%.3f) ",tag2robot.getOrigin().getX(),tag2robot.getOrigin().getY(),tag2robot.getOrigin().getZ(),_yew_robot*180/3.1415926);
    //third:get the tag pose according to odom message
     tf::Pose tagPose_measure;
    tagPose_measure=turtle_odom.operator *(tag2robot);
    double _yew_world,_pich_world,_roll_world;
    tagPose_measure.getBasis().getEulerZYX( _yew_world,_pich_world,_roll_world);
    //ROS_INFO("tag2odom is(x:%.3f, y:%.3f. z:%.3f,,,,,angle:%.3f) ",tagPose_measure.getOrigin().getX(),tagPose_measure.getOrigin().getY(),tagPose_measure.getOrigin().getZ(),_yew_world*180/3.1415926);

    geometry_msgs::PoseStamped MsgtagPose_measure;
    MsgtagPose_measure.pose.position.x=tagPose_measure.getOrigin().getX();
    MsgtagPose_measure.pose.position.y=tagPose_measure.getOrigin().getY();
    MsgtagPose_measure.pose.position.z=tagPose_measure.getOrigin().getZ();
    MsgtagPose_measure.pose.orientation.x=tagPose_measure.getRotation().getX();
    MsgtagPose_measure.pose.orientation.y=tagPose_measure.getRotation().getY();
    MsgtagPose_measure.pose.orientation.z=tagPose_measure.getRotation().getZ();
    MsgtagPose_measure.pose.orientation.w=tagPose_measure.getRotation().getW();
    //convert tagPose to massage
    geometry_msgs::PoseStamped MsgtagPose_predict;
    MsgtagPose_predict.pose.position.x=TagPose.getOrigin().getX();
    MsgtagPose_predict.pose.position.y=TagPose.getOrigin().getY();
    MsgtagPose_predict.pose.position.z=TagPose.getOrigin().getZ();
    MsgtagPose_predict.pose.orientation.w=TagPose.getRotation().getW();
    MsgtagPose_predict.pose.orientation.x=TagPose.getRotation().getX();
    MsgtagPose_predict.pose.orientation.y=TagPose.getRotation().getY();
    MsgtagPose_predict.pose.orientation.z=TagPose.getRotation().getZ();
    // Create a array to store tags for publishing
    slam_april_laser_odom::PoseStampedArray tag_pose_array;
    tag_pose_array.header.frame_id=id_str;
    tag_pose_array.odom=MsgtagPose_measure;
    tag_pose_array.pose=MsgtagPose_predict;
    odomAndpose_pub_.publish(tag_pose_array);
    //tf::Stamped<tf::Transform> tag_transform;
    //tf::poseStampedMsgToTF(tag_pose, tag_transform);
    //tf_pub_.sendTransform(tf::StampedTransform(tag_transform, tag_transform.stamp_, tag_transform.frame_id_, description.frame_name()));





    /*
    cameraPose.getBasis().getEulerZYX(_yew,_pich,_roll);
    //ROS_INFO("pose is(x:%.3f, y:%.3f. angle:%.3f)  ",cameraPose.getOrigin().getX(),cameraPose.getOrigin().getY(),_yew*180/M_PI);
    geometry_msgs::PoseStamped robotPose;
    robotPose.header.frame_id="tag";
    robotPose.header.stamp=ros::Time::now();
    robotPose.pose.position.x=cameraPose.getOrigin().getX();
    robotPose.pose.position.y=cameraPose.getOrigin().getY();
    robotPose.pose.position.z=cameraPose.getOrigin().getZ();
    robotPose.pose.orientation.w=cameraPose.getRotation().getW();
    robotPose.pose.orientation.x=cameraPose.getRotation().getX();
    robotPose.pose.orientation.y=cameraPose.getRotation().getY();
    robotPose.pose.orientation.z=cameraPose.getRotation().getZ();
    //tag_pose_array.poses.push_back(robotPose);
    tag_pose_array.pose=robotPose;
    odomAndpose_pub_.publish(tag_pose_array);
    //pose_pub_.publish(robotPose);
    ROS_INFO("robot pose is(x:%.3f, y:%.3f. angle:%.3f)  ",robotPose.pose.position.x,robotPose.pose.position.y,_yew*180/M_PI);
    //ROS_INFO("this tag's location is(x:%.3f, y:%.3f. z:%.3f) ",TagPose.getOrigin().getX(),TagPose.getOrigin().getY(),TagPose.getOrigin().getZ());
    */
//**********************************************
   //ROS_INFO("\nTagID:%d\nposition:%lf,%lf,%lf\norientation:%lf,%lf,%lf,%lf",detection.id,tag_pose.pose.position.x,tag_pose.pose.position.y,tag_pose.pose.position.z,
    //tag_pose.pose.orientation.x,tag_pose.pose.orientation.y,tag_pose.pose.orientation.z,tag_pose.pose.orientation.w);
//ROS_INFO_STREAM(detection.id);
//ROS_INFO("this tag's location is(x:%.3f, y:%.3f. z:%.3f) ",TagPose.getOrigin().getX(),TagPose.getOrigin().getY(),TagPose.getOrigin().getZ());
//transform：获取tag和camera的相对tf信息
    //AprilTagDetection tag_detection;
    //tag_detection.pose = tag_pose;
    //tag_detection.id = detection.id;
    //tag_detection.size = tag_size;
    //tag_detection_array.detections.push_back(tag_detection);
    //tag_pose_array.poses.push_back(tag_pose.pose);


  }
  //detections_pub_.publish(tag_detection_array);
  //pose_pub_.publish(tag_pose_array);
  //image_pub_.publish(cv_ptr->toImageMsg());
}


std::map<int, AprilTagDescription> AprilTagDetector::parse_tag_descriptions(XmlRpc::XmlRpcValue& tag_descriptions){
  std::map<int, AprilTagDescription> descriptions;
  ROS_ASSERT(tag_descriptions.getType() == XmlRpc::XmlRpcValue::TypeArray);
  for (int32_t i = 0; i < tag_descriptions.size(); ++i) {
    XmlRpc::XmlRpcValue& tag_description = tag_descriptions[i];
    ROS_ASSERT(tag_description.getType() == XmlRpc::XmlRpcValue::TypeStruct);
    ROS_ASSERT(tag_description["id"].getType() == XmlRpc::XmlRpcValue::TypeInt);
    ROS_ASSERT(tag_description["size"].getType() == XmlRpc::XmlRpcValue::TypeDouble);
    //wxf:add
    ROS_ASSERT(tag_description["x"].getType() == XmlRpc::XmlRpcValue::TypeDouble);
    ROS_ASSERT(tag_description["y"].getType() == XmlRpc::XmlRpcValue::TypeDouble);
    ROS_ASSERT(tag_description["z"].getType() == XmlRpc::XmlRpcValue::TypeDouble);
    double x = (double)tag_description["x"];
    double y = (double)tag_description["y"];
    double z = (double)tag_description["z"];
    //************************************************8
    int id = (int)tag_description["id"];
    double size = (double)tag_description["size"];

    std::string frame_name;
    if(tag_description.hasMember("frame_id")){
      ROS_ASSERT(tag_description["frame_id"].getType() == XmlRpc::XmlRpcValue::TypeString);
      frame_name = (std::string)tag_description["frame_id"];
    }
    else{
      std::stringstream frame_name_stream;
      frame_name_stream << id;
      frame_name = frame_name_stream.str();
    }
    AprilTagDescription description(id, size, frame_name,x,y,z);
    ROS_INFO_STREAM("Loaded tag config: "<<id<<", size: "<<size<<", frame_name: "<<frame_name<<"x:"<<x<<"y:"<<y<<"z:"<<z);
    descriptions.insert(std::make_pair(id, description));
  }
  return descriptions;
}


}

#include "ros/ros.h"
#include "std_msgs/String.h"
#include<std_msgs/Float32.h>

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::StringConstPtr& msg)
{
  //ROS_INFO("I heard: ");
    ROS_INFO_STREAM(msg->data);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/foo", 1000, chatterCallback);
  ros::spin();

  return 0;
}


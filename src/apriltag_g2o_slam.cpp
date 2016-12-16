#include<cmath>
#include <ros/ros.h>
#include<apriltags_ros//apriltag_g2o.h>
#include<g2o/types/slam2d/edge_se2.h>
#include<g2o/types/slam2d/vertex_se2.h>
#include<g2o/types/slam2d/parameter_se2_offset.h>
#include<g2o/core/sparse_optimizer.h>
#include<g2o/core/block_solver.h>
#include "g2o/core/factory.h"
#include "g2o/core/optimization_algorithm_factory.h"
//#include "g2o/core/optimization_algorithm_gauss_newton.h"
#include"g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/solvers/csparse/linear_solver_csparse.h"
using namespace std;
slam_g2o::slam_g2o()
{
    poseReceive=nh.subscribe<slam_april_laser_odom::PoseStampedArray>("tag_odom_pose",1,&slam_g2o::aprilCallback,this);
    posePub=nh.advertise<geometry_msgs::TransformStamped>("/tf",5);
    //posePub=nh.advertise<>();
    linearSolver = new SlamLinearSolver();
    linearSolver->setBlockOrdering(false);
    blockSolver = new SlamBlockSolver(linearSolver);
    //solver = new g2o::OptimizationAlgorithmGaussNewton(blockSolver);
    solver=new g2o::OptimizationAlgorithmLevenberg(blockSolver);
    optimizer.setAlgorithm(solver);
    //set offset
    g2o::SE2 sensorOffsetTransf(0,0,0);
    sensorOffset=new g2o::ParameterSE2Offset;
    sensorOffset->setOffset(sensorOffsetTransf);
    sensorOffset->setId(0);
    optimizer.addParameter(sensorOffset);
    index=0;
    copyEnable=true;
    test_haha=true;
    //set information
    information=Eigen::Matrix3d::Identity();
    //information(0,0)=information(1,1)=information(2,2)=100;
}
void slam_g2o::aprilCallback(const slam_april_laser_odom::PoseStampedArrayConstPtr &PoseArray)
{
    if(test_haha)
    {
    //ROS_INFO_STREAM("oh!yes!");
        string id_str=PoseArray->header.frame_id;
        int id=atoi(id_str.c_str());
        //ROS_INFO_STREAM(id);
    //topic数据将会转换为SE2的数据类型
    tf::Quaternion new_odom_orientation=tf::Quaternion(PoseArray->odom.pose.orientation.x, PoseArray->odom.pose.orientation.y,
                                   PoseArray->odom.pose.orientation.z, PoseArray->odom.pose.orientation.w);;
    new_pose.pose_measure_theta=new_odom_orientation.getAngle() * new_odom_orientation.getAxis().getZ();
    new_pose.pose_measure_x=PoseArray->odom.pose.position.x;
    new_pose.pose_measure_y=PoseArray->odom.pose.position.y;
    new_pose.pose_predict_x=PoseArray->pose.pose.position.x;
    new_pose.pose_predict_y=PoseArray->pose.pose.position.y;
    tf::Quaternion new_april_orientation=tf::Quaternion(PoseArray->pose.pose.orientation.x,
    PoseArray->pose.pose.orientation.y,PoseArray->pose.pose.orientation.z,PoseArray->pose.pose.orientation.w);
    new_pose.pose_predict_theta=new_april_orientation.getAngle()*new_april_orientation.getAxis().getZ();
    new_pose.ID=id;
//第一次数据的处理方式
    if(copyEnable)
    {
        /*
        odom_theta_old=odom_theta_new;
        odom_x_old=odom_x_new;
        odom_y_old=odom_y_new;
        */
        old_pose.pose_measure_theta=new_pose.pose_measure_theta;
        old_pose.pose_measure_x=new_pose.pose_measure_x;
        old_pose.pose_measure_y=new_pose.pose_measure_y;
        old_pose.pose_predict_theta=new_pose.pose_predict_theta;
        old_pose.pose_predict_x=new_pose.pose_predict_x;
        old_pose.pose_predict_y=new_pose.pose_predict_y;
        new_pose.index=index;
        pose_buffer.push_back(new_pose);
        copyEnable=false;
        //for test
        /*
        apriltag_x_old=apriltag_x_;
        apriltag_y_old=apriltag_y_;
        apriltag_theta_old=apriltag_theta_;
        */
    }
    processing();
    }
}
void slam_g2o::processing()
{
    double dx=new_pose.pose_measure_x-old_pose.pose_measure_x;
    double dy=new_pose.pose_measure_y-old_pose.pose_measure_y;
    double dtheta=new_pose.pose_measure_theta-old_pose.pose_measure_theta;
/*
    //test
    double dx=new_pose.pose_predict_x-old_pose.pose_predict_x;
    double dy=new_pose.pose_predict_y-old_pose.pose_predict_y;
    double dtheta=new_pose.pose_predict_theta-old_pose.pose_predict_theta;
*/

    //判断关键帧
    //if(sqrt(dx*dx+dy*dy)>0.05||fabs(dtheta)>0.05)
    if((fabs(dx)>0.4||fabs(dy)>0.4)||index==0)
    {
        //序号更新
        ROS_INFO_STREAM(index);
        ROS_INFO("ID:%d,tag pose is(x:%.3f, y:%.3f. angle:%.3f)  ",new_pose.ID,new_pose.pose_predict_x,
                         new_pose.pose_predict_y,new_pose.pose_predict_theta);
        ROS_INFO("ID:%d,robot measure is(x:%.3f, y:%.3f. angle:%.3f)  ",new_pose.ID,new_pose.pose_measure_x,
                         new_pose.pose_measure_y,new_pose.pose_measure_theta);

        old_pose.pose_measure_theta=new_pose.pose_measure_theta;
        old_pose.pose_measure_x=new_pose.pose_measure_x;
        old_pose.pose_measure_y=new_pose.pose_measure_y;
        old_pose.pose_predict_theta=new_pose.pose_predict_theta;
        old_pose.pose_predict_x=new_pose.pose_predict_x;
        old_pose.pose_predict_y=new_pose.pose_predict_y;
        //增加节点
        g2o::SE2 robotPose(new_pose.pose_predict_x,new_pose.pose_predict_y,new_pose.pose_predict_theta);
        g2o::VertexSE2* robot=new g2o::VertexSE2;
        if(index==0) robot->setFixed(true);
        robot->setId(index);
        robot->setEstimate(robotPose);
        optimizer.addVertex(robot);
        //增加约束边
        if(index>0)
        {
            g2o::SE2 odomPose(dx,dy,dtheta);
            g2o::EdgeSE2* odometry=new g2o::EdgeSE2;
            odometry->vertices()[0]=optimizer.vertex(index-1);
            odometry->vertices()[1]=optimizer.vertex(index);
            odometry->setMeasurement(odomPose);
            odometry->setInformation(information);
            optimizer.addEdge(odometry);
        }

        vector<FRAME>::iterator it=find(pose_buffer.begin(),pose_buffer.end(),new_pose.ID);
        if(it!=pose_buffer.end())
        {
            //tryCloseLoop
            double edx=new_pose.pose_measure_x-(it->pose_measure_x);
            double edy=new_pose.pose_measure_y-(it->pose_measure_y);
            double edtheta=new_pose.pose_measure_theta-(it->pose_measure_theta);
            g2o::SE2 eodomPose(edx,edy,edtheta);
            g2o::EdgeSE2* eodometry=new g2o::EdgeSE2;
            eodometry->vertices()[0]=optimizer.vertex(it->index);
            eodometry->vertices()[1]=optimizer.vertex(index);
            eodometry->setMeasurement(eodomPose);
            eodometry->setInformation(information);
            optimizer.addEdge(eodometry);
        }

        //update buffer
        new_pose.index=index;
        index++;
        pose_buffer.push_back(new_pose);
        //if(index>=20&&index!=0)
        //if(index==100)
        ros::Duration duration=ros::Time::now()-present;
        if(duration.toSec()>110)
        {
            test_haha=false;
            //ROS_INFO_STREAM("oh!no!");
            //optimizer.save("tutorial_before.g2o");
            //optimizer.setVerbose(true);
            optimizer.initializeOptimization();
            optimizer.optimize(20);
            if(optimizer.save("tutorial_after.g2o")) ROS_INFO_STREAM("saved");
            exit(0);
            /*
            g2o::VertexSE2* v=dynamic_cast<g2o::VertexSE2*>(optimizer.vertex(index-1));
            g2o::SE2 p=v->estimate();
            Eigen::Vector3f pos(p[0],p[1],p[2]);
            ROS_INFO("g2ox:%f,y:%f,theta:%f",pos(0),pos(1),pos(2));
            geometry_msgs::TransformStamped april_odom_pose;
            april_odom_pose.child_frame_id="robot";
            april_odom_pose.header.stamp=ros::Time::now();
            april_odom_pose.header.frame_id="world";
            april_odom_pose.transform.translation.x=pos(0);
            april_odom_pose.transform.translation.y=pos(1);
            april_odom_pose.transform.translation.z=0;
            tf::Quaternion q=tf::createQuaternionFromYaw(pos(2));
            april_odom_pose.transform.rotation.w=q.getW();
            april_odom_pose.transform.rotation.x=q.getX();
            april_odom_pose.transform.rotation.y=q.getY();
            april_odom_pose.transform.rotation.z=q.getZ();
            posePub.publish(april_odom_pose);
            //test
            // freeing the graph memory
            //optimizer.clear();
            //test_haha=false;
            */
        }
    }
}
void slam_g2o::tryCloseLoop()
{

}
int main(int argc, char **argv){
    ros::init(argc, argv, "apriltag_g2o");
    slam_g2o haha;
    haha.present=ros::Time::now();
    ros::spin();
    return 0;
}

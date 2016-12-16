#ifndef APRILTAG_G2O_H
#define APRILTAG_G2O_H
#include<vector>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include<tf/transform_broadcaster.h>
#include <tf/LinearMath/Vector3.h>
#include<slam_april_laser_odom/PoseStampedArray.h>
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
// 帧结构
struct FRAME
{
    int ID;
    int index;
    double pose_predict_x;
    double pose_predict_y;
    double pose_predict_theta;
    double pose_measure_x;
    double pose_measure_y;
    double pose_measure_theta;
    bool operator==(const int &i);
};
bool FRAME::operator ==(const int &i)
{
    return this->ID==i;
}
class slam_g2o{
public:
    slam_g2o();
    ros::Time present;

    FRAME new_pose;
    FRAME old_pose;
    std::vector<FRAME> pose_buffer;
private:
    /*********************************************************************************
     * ROS config
     ********************************************************************************/
	ros::NodeHandle nh;
    ros::Subscriber poseReceive;
    ros::Publisher posePub;
    /*********************************************************************************
     * creating the optimization problem
     ********************************************************************************/
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<-1,-1>> SlamBlockSolver;
    typedef g2o::LinearSolverCSparse<SlamBlockSolver::PoseLandmarkMatrixType> SlamLinearSolver;
    // allocating the optimizer
    g2o::SparseOptimizer optimizer;
    SlamLinearSolver* linearSolver;
    SlamBlockSolver* blockSolver ;
    //g2o::OptimizationAlgorithmGaussNewton* solver;
    g2o::OptimizationAlgorithmLevenberg* solver;
    g2o::ParameterSE2Offset* sensorOffset;
    //record for optimization
    int index;
    bool test_haha;
    Eigen::Matrix3d information;
    // Information extract from odom
    double odom_x_old;
    double odom_y_old;
    double odom_theta_old;
    double odom_x_new;
    double odom_y_new;
    double odom_theta_new;
    double apriltag_x_;
    double apriltag_y_;
    double apriltag_theta_;
    double apriltag_x_old;
    double apriltag_y_old;
    double apriltag_theta_old;
    bool copyEnable;
private:
    void aprilCallback(const slam_april_laser_odom::PoseStampedArrayConstPtr &PoseArray);
    void tryCloseLoop();
    void processing();
};
#endif

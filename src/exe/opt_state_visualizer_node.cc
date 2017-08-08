/**
@file    opt_state_visualizer_node.cpp
@author  Alexander W. Winkler (winklera@ethz.ch)
@date    26.05.2017
@brief   Visualizes a single optimization state in rviz.
 */

#include <ros/ros.h>

#include <xpp/rviz_marker_builder.h>
#include <xpp/ros/ros_helpers.h>
#include <xpp/ros/topic_names.h>

using StateMsg       = xpp_msgs::RobotStateCartesian;
using TrajMsg        = xpp_msgs::RobotStateCartesianTrajectory;
using ParamsMsg      = xpp_msgs::OptParameters;
using MarkerArrayMsg = visualization_msgs::MarkerArray;

using namespace ros;

static Publisher rviz_pub;
static xpp::RvizMarkerBuilder marker_builder;

static void StateCallback (const StateMsg& state_msg)
{
  xpp::RobotStateCartesian state = xpp::ros::RosHelpers::RosToXpp(state_msg);
  MarkerArrayMsg rviz_marker_msg = marker_builder.BuildStateMarkers(state);
  rviz_pub.publish(rviz_marker_msg);
}

static void TrajectoryCallback (const TrajMsg& traj_msg)
{
  xpp::RvizMarkerBuilder msg_builder;
  auto traj = xpp::ros::RosHelpers::RosToXppCart(traj_msg);
  MarkerArrayMsg rviz_traj_msg = msg_builder.BuildTrajectoryMarkers(traj);
  rviz_pub.publish(rviz_traj_msg);
}

static void ParamsCallback (const ParamsMsg& params_msg)
{
  ROS_INFO_STREAM("received current optimization parameters");
  marker_builder.SetOptimizationParameters(params_msg);
}

int main(int argc, char *argv[])
{
	init(argc, argv, "optimization_state_visualizer");

	NodeHandle n;
	rviz_pub = n.advertise<MarkerArrayMsg>(xpp_msgs::rviz_optimized, 1);

  Subscriber parameters_sub;
  parameters_sub = n.subscribe(xpp_msgs::opt_parameters, 1, ParamsCallback);

	Subscriber state_sub;
	state_sub = n.subscribe(xpp_msgs::curr_robot_state, 1, StateCallback);

	Subscriber traj_sub;
	traj_sub = n.subscribe(xpp_msgs::robot_trajectory_cart, 1, TrajectoryCallback);

	spin();

	return 1;
}



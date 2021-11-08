#include "ros/ros.h"
#include<move_base_msgs/MoveBaseAction.h>
#include<actionlib/client/simple_action_client.h>
#include <actionlib/server/simple_action_server.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <waypoint_navi/wpexecuteAction.h>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct goal_msgs
{
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> theta_z;
	std::vector<float> theta_w;
};

goal_msgs ReadFile(std::string file_location)
	{
		  //std::ofstream myfile(file_location);
		  std::ifstream myfile;
		  myfile.open(file_location);
		  std::string line;
		  //std::vector<goal_msgs> point_vec;
		  goal_msgs data,point;
		  float valx,valy,valtheta_z, valtheta_w;
		  int i=0;
		  while(std::getline(myfile,line))
		  {
		  	std::string token;
		  	std::stringstream ss(line);
		  	//ss>> valx >> valy >> valtheta;
		  	std::getline(ss,token,',');
		  	valx=std::stod(token);
		  	std::getline(ss,token,',');
		  	valy=std::stod(token);
		  	std::getline(ss,token,',');
		  	valtheta_z=std::stod(token);
		  	std::getline(ss,token,',');
		  	valtheta_w=std::stod(token);
		  	point.x.push_back(valx);
		  	point.y.push_back(valy);
		  	point.theta_z.push_back(valtheta_z);
		  	point.theta_w.push_back(valtheta_w);
		  	
		  }
		  myfile.close();
		  for (int i=0; i<point.theta_z.size();i++)
		  {
		  	std::cout << point.y[i];
		  }
		  
		  //point.x={0,1,1};
		  //point.y={0,1,1};
		  //point.theta={0,1,1};
		  return point;		  
	}
class WpAction
{
	
public:
	ros::NodeHandle n;
	MoveBaseClient movebase_ac;
	ros::Publisher init_pose_pub_;
	ros::Subscriber amcl_pose_sub_;
	WpAction();
	actionlib::SimpleActionServer<waypoint_navi::wpexecuteAction> Action;
	void amclPoseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);
	void executeCallback(const waypoint_navi::wpexecuteGoalConstPtr &goal_);
	goal_msgs ReadFile(std::string file_location);
	float current_x_, current_y_, current_qz_, current_qw_;
	~WpAction(void)
	{
	std::cout << "Object is being deleted" << std::endl;
	}
	
};
	WpAction::WpAction() :
          movebase_ac("move_base", true),Action(n, "waypoint_server", boost::bind(&WpAction::executeCallback, this, _1), false)
	{
		Action.start();
		init_pose_pub_ = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 2);
		amcl_pose_sub_ = n.subscribe("/amcl_pose", 2, &WpAction::amclPoseCallback, this);
	}

void WpAction::amclPoseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
	current_x_ = msg->pose.pose.position.x;
	current_y_ = msg->pose.pose.position.y;
	current_qz_ = msg->pose.pose.orientation.z;
	current_qw_ = msg->pose.pose.orientation.w;
	std::cout<<"\namcl:  "<<current_x_;
}
goal_msgs WpAction::ReadFile(std::string file_location)
	{
		  //std::ofstream myfile(file_location);
		  std::ifstream myfile;
		  myfile.open(file_location);
		  std::string line;
		  //std::vector<goal_msgs> point_vec;
		  goal_msgs data,point;
		  float valx,valy,valtheta_z, valtheta_w;
		  int i=0;
		  while(std::getline(myfile,line))
		  {
		  	std::string token;
		  	std::stringstream ss(line);
		  	//ss>> valx >> valy >> valtheta;
		  	std::getline(ss,token,',');
		  	valx=std::stod(token);
		  	std::getline(ss,token,',');
		  	valy=std::stod(token);
		  	std::getline(ss,token,',');
		  	valtheta_z=std::stod(token);
		  	std::getline(ss,token,',');
		  	valtheta_w=std::stod(token);
		  	point.x.push_back(valx);
		  	point.y.push_back(valy);
		  	point.theta_z.push_back(valtheta_z);
		  	point.theta_w.push_back(valtheta_w);
		  	
		  }
		  myfile.close();
		  for (int i=0; i<point.theta_z.size();i++)
		  {
		  	std::cout << point.y[i];
		  }
		  
		  //point.x={0,1,1};
		  //point.y={0,1,1};
		  //point.theta={0,1,1};
		  return point;		  
	}
void WpAction::executeCallback(const waypoint_navi::wpexecuteGoalConstPtr &goal_){
	bool success = true;
	//MoveBaseClient ac("move_base", true);
	goal_msgs point_;
	//goal_msgs point_;
	ROS_INFO("navi_waypoint");
	point_=ReadFile("/home/sutd_wasp/nav_points.csv");
    	while(!movebase_ac.waitForServer(ros::Duration(15.0)))
    	{
    		ROS_INFO("Sending goal[%ld]", (long int)point_.x.size());
        	ROS_INFO("Waiting for the move_base action server");
		
        }
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();
	for( int i = 0; i < point_.x.size(); i++ ) 
	{
	    success = false;
	    goal.target_pose.pose.position.x = point_.x[i];
	    goal.target_pose.pose.position.y = point_.y[i];

	    goal.target_pose.pose.orientation.z = point_.theta_z[i];
	    goal.target_pose.pose.orientation.w = point_.theta_w[i];
	    ROS_INFO("Sending goal[%ld]", (long int)i);
	    movebase_ac.sendGoal(goal);

	    movebase_ac.waitForResult();

	      	if(movebase_ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
	        	ROS_INFO("Robot reached waypoint[%d]",(long int)i);
			geometry_msgs::PoseWithCovarianceStamped p;
			p.pose.pose.position.x = current_x_;
			p.pose.pose.position.y = current_y_;
			p.pose.pose.orientation.z = current_qz_;
			p.pose.pose.orientation.w = current_qw_;
			p.pose.covariance[0] = 0.25; //x deviation
			p.pose.covariance[7] = 0.25;
			//p.pose.covariance[6] = -0.011;
			//p.pose.covariance[7] = 0.19; //y deviation
			p.pose.covariance[35] = 0.0685;//yaw
			init_pose_pub_.publish(p);

			ros::Duration(2).sleep();
			if(i ==point_.x.size()-1)
				{
				success = true;
				}
			}

	      	else{
			std::cout<<"current_x: "<<current_x_;
	        	ROS_INFO("The robot failed to move for some reason");
			}
       }
}
int main(int argc, char** argv){
	ros::init(argc, argv, "waypoint_navigation");
	//ros::Time::init();	
	//ros::NodeHandle n;
	WpAction wp;	
	//wp.executeCallback();
	//int timer_flag=1;
      ros::spin();
      return 0;
    }





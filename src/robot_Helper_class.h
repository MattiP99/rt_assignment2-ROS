#ifndef HELPER_CLASS_H_
#define HELPER_CLASS_H_
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "second_assignment/UserInputService.h"
#include "std_srvs/Empty.h"
#include "std_msgs/String.h"

#include <iostream>
using namespace std;


class Helper{

   public:
   	Helper(ros::NodeHandle *n);
   	//Helper(ros::Publisher publisher, ros::Publisher publisher2,ros::ServiceClient clt1, ros::ServiceClient clt2);
   	~Helper();
   	//void resetPositionModifyVelocityCallback(const geometry_msgs::Twist::ConstPtr& vel);
   	void resetPositionModifyVelocityCallback();
   	
		
	
	
	//void initializePubClt();
   private:
   	ros::NodeHandle nh;
	ros::Publisher pub;
	ros::Publisher pub2;
	//ros::Subscriber sub;
	
	ros::ServiceClient client1;
	ros::ServiceClient client2;
	
	
	
};

#endif


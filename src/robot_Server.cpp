#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "second_assignment/UserInputService.h"
#include "std_srvs/Empty.h"
#include "std_msgs/String.h"

#include <iostream>
using namespace std;

/*
//Service for resetting the position of the car
bool servicePositionCallback(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res){
	return true;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "wallServer");
	ros::NodeHandle n;
	//ros::ServiceServer service= n.advertiseService("/input_user", serviceVelocityCallback);
	ros::ServiceServer service= n.advertiseService("/reset_positions", servicePositionCallback);
	ros::spin();
	return 0;
}
*/

struct Reset_Modify {
	string input;
	float acceleration;
	
};

class Reset_Modify_Car{

   public:
   	Reset_Modify_Car(){}
   	bool serviceResetPositionCallback(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res){
   		ROS_INFO("Calling reset position service");
		return true;
	};
		
	bool serviceUserInputCallback(second_assignment::UserInputService::Request &request, second_assignment::UserInputService::Response &response){
  
  		if(request.input == "w"){ response.acceleration = 20/100;
  			}
  		else if(request.input == "s"){ response.acceleration = -20/100;
  		}
  		return true;
	}
	

};


int main(int argc, char **argv)
{
	ros::init(argc, argv, "wallServer");
	ros::NodeHandle n;
	//ros::ServiceServer service= n.advertiseService("/input_user", serviceVelocityCallback);
	Reset_Modify_Car resMod;
	ros::ServiceServer srv1 = n.advertiseService("/reset_positions", &Reset_Modify_Car::serviceResetPositionCallback, &resMod);
	ros::ServiceServer srv2 = n.advertiseService("/input_user", &Reset_Modify_Car::serviceUserInputCallback, &resMod); 
	ros::spin();
	return 0;
}


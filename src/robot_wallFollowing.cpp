#include "robot_wallFollowing_class.h"
#include "second_assignment/UserInputService.h"
#include "std_srvs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <std_msgs/String.h>
#include <math.h>
#include <iostream>
using namespace std;
#define PI 3.141592
 

WallFollowing::WallFollowing(ros::Publisher pub, double wallDist, int dir, double pr, double di, double an)
{
  wallDistance = wallDist;
  direction = dir;
  P = pr;
  D = di;
  angleCoef = an;
  e = 0;
  angleMin = 0;  //angle, at which was measured the shortest distance
  pubMessage = pub;
  DriveSpeed = 2;
  acceleration = 0;
  
  
 }

//Distructor
WallFollowing::~WallFollowing(){}

//Function for managing the response input from UserNode
bool WallFollowing::serviceUserInputCallback(second_assignment::UserInputService::Request &request, second_assignment::UserInputService::Response &response){
  		
  		geometry_msgs::Twist msg;
  		
  		if(request.input == 'w'){ 
  			
  			acceleration = 0.5;
  			response.acceleration = acceleration;
  			ROS_INFO("acceleration is %f\n",response.acceleration);
  			
  		}
  		else if (request.input == 's'){
  			
  			acceleration = -0.5;
  			response.acceleration = acceleration;
  			ROS_INFO("acceleration is %f\n",response.acceleration);
  			
  		}
  		else{
  			ROS_ERROR("Error in receving from client");
  		
  		}
		
		DriveSpeed += acceleration;
		return true;
  		
	}
	


//Publisher of the velocity
void WallFollowing::publishMessage()
{
	geometry_msgs::Twist msg;
	//second_assignment::UserInputService srv;
	
	
	//preparing message
  	
  	msg.angular.z = direction*(P*e + D*diffE) + angleCoef * (angleMin - PI*direction/2);    //PD controller

  	if (distFront < wallDistance){
    	msg.linear.x = 0;
  	}
  	else if (distFront < wallDistance * 2){
    	msg.linear.x = 0.5* DriveSpeed;
  	}
  	else if (fabs(angleMin)>1.75){
    	msg.linear.x = 0.2* DriveSpeed;
  	} 
  	
  	else {
    	msg.linear.x = DriveSpeed;
  	}

 pubMessage.publish(msg);
  //publishing message
  
}


//Subscriber for evaluating obstacles thanks to car lasers
void WallFollowing::messageCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  int size = msg->ranges.size();

  //Variables whith index of highest and lowest value in array.
  int minIndex = size*(direction+1)/4;
  int maxIndex = size*(direction+3)/4;

  //This cycle goes through array and finds minimum
  for(int i = minIndex; i < maxIndex; i++)
  {
    
    if (msg->ranges[i] < msg->ranges[minIndex] && msg->ranges[i] > 0.0){
      minIndex = i;
    }
  }

  //Calculation of angles from indexes and storing data to class variables.
  angleMin = (minIndex-size/2)*msg->angle_increment;
  double distMin;
  
  distMin = msg->ranges[minIndex];
  distFront = msg->ranges[size/2];
  diffE = (distMin - wallDistance) - e;
  e = distMin - wallDistance;

  //Invoking method for publishing message
  publishMessage();
}

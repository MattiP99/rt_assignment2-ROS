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
  DriveSpeed = 3;
  
 }

WallFollowing::~WallFollowing(){}


bool WallFollowing::serviceUserInputCallback(second_assignment::UserInputService::Request &request, second_assignment::UserInputService::Response &response){
  		
  		geometry_msgs::Twist msg;
  		if(request.input){ 
  			DriveSpeed += response.acceleration;
  		}
  		else{
  			ROS_ERROR("Error in receving from client");
  		}
  		msg.linear.x = DriveSpeed;
  		publishMessage(0);
  		return true;
	}
//Publisher
void WallFollowing::publishMessage(bool from)
{
geometry_msgs::Twist msg;
if (from == 1){

  
  //preparing message
  	// NOTA::: puoi considerare due casi... il primo è se la distFront >> 0 allora vai di PD altrimenti vai di laterali control brandequalcosa control Potrebbe non funzionare....
  	msg.angular.z = direction*(P*e + D*diffE) + angleCoef * (angleMin - PI*direction/2);    //PD controller

  	if (distFront < wallDistance){
    	msg.linear.x = 0;
  	}
  	else if (distFront < wallDistance * 2){
    	msg.linear.x = 0.5*DriveSpeed;
  	}
  	else if (fabs(angleMin)>1.75){
    	msg.linear.x = 0.1*DriveSpeed;
  	} 
  	
  	else {
    	msg.linear.x = DriveSpeed;
  	}
	
	
}
else if (from == 0){
	ROS_INFO("Modifying velocity from service");
}
  
 pubMessage.publish(msg);
  //publishing message
  
}


//Subscriber
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
  publishMessage(1);
}

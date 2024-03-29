#include "ros/ros.h"
#include "robot_wallFollowing_class.h"
#include "robot_Helper_class.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "second_assignment/UserInputService.h"
#include "std_srvs/Empty.h"
#include "std_msgs/String.h"

#include <iostream>
using namespace std;

#define SUBSCRIBER_BUFFER_SIZE 100  // Size of buffer for subscriber.
#define PUBLISHER_BUFFER_SIZE 1000  // Size of buffer for publisher.
#define WALL_DISTANCE 0.70

#define P 10    // Proportional constant for controller
#define D 9    // Derivative constant for controller
#define ANGLE_COEF 1   // Proportional constant for angle controller
#define DIRECTION -1 // 1 for wall on the left side of the robot (-1 for the right side).

#define PUBLISHER_TOPIC "/cmd_vel"


#define SUBSCRIBER_TOPIC "/base_scan"

#define SERVICE_MESSAGE "/input_user"

#define SERVICE_MESSAGE2 "/reset_positions"


int main(int argc, char **argv)
{
  //Initialization of node
  ros::init(argc, argv, "controller_node");
  ros::NodeHandle n;
  

  //Creating publisher for changes of velocity
  ros::Publisher pubMessage = n.advertise<geometry_msgs::Twist>(PUBLISHER_TOPIC, PUBLISHER_BUFFER_SIZE);
  
  
  
  //Creating object, which stores data from sensors and has methods for
  //publishing and subscribing
  
  WallFollowing *wallFollowing = new WallFollowing(pubMessage, WALL_DISTANCE, DIRECTION, P, D, 1);
  
  
  //Creating subscriber 
  ros::Subscriber sub = n.subscribe(SUBSCRIBER_TOPIC, SUBSCRIBER_BUFFER_SIZE, &WallFollowing::messageCallback, wallFollowing);
  /*
  Creating a server of type UserInputService 		input
  							------
  							acceleration
  */
  ros::ServiceServer vel_srv = n.advertiseService(SERVICE_MESSAGE, &WallFollowing::serviceUserInputCallback, wallFollowing);
  
  
  ros::spin();

  return 0;
}

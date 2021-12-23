#include "ros/ros.h"
#include "robot_Helper_class.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "second_assignment/UserInputService.h"
#include "std_srvs/Empty.h"
#include "std_msgs/String.h"

#include <iostream>
using namespace std;



//Publisher and subscriber
/*
ros::Publisher publisher;
ros::Publisher publisher2;

//Service
ros::ServiceClient clt1;
ros::ServiceClient clt2;

*/

// Initialize a raw string literal to print the menu
const char * menu = R"(
   MENU
   --------------------------------------------
   w : increase speed
   s : decrease speed
   r : reset robot position
   q : quit
)";

Helper::Helper(ros::NodeHandle *nh){
	/*
	pub = publisher;
 	pub2 = publisher2;
 	client1 = clt1;
 	client2 = clt2;
 	*/
 	
 	//sub = nh->subscribe("/cmd_vel",100, &Helper::resetPositionModifyVelocityCallback,this);
 	
 	pub = nh->advertise<nav_msgs::Odometry>("/odom", 10); 
 	pub2 = nh->advertise<geometry_msgs::Twist>("/cmd_vel", 10); 
    	client1 =  nh->serviceClient<std_srvs::Empty>("/reset_positions");
    	client2 =  nh->serviceClient<second_assignment::UserInputService>("/input_user");
    	 
}

Helper::~Helper(){}



	
void Helper::resetPositionModifyVelocityCallback(){

		//ROS_INFO("Current velocity linear and angular of the car are [%f,%f]\n", in->linear.x, in->angular.z);
		//cout << "digit q in order to reset the position of the car\n";
	
		nav_msgs::Odometry msg;
		geometry_msgs::Twist vel;
		
		std_srvs::Empty reset_srv;
		second_assignment::UserInputService vel_srv;
		
		
		
			
		// i need to populate the request field of my velocity message vel_srv
	        char inputChar;
   	        scanf("%c", &inputChar);
   		switch(inputChar){
  		  	case 'q':
  				ros::shutdown();
  			break;
  			
  		  	case 'p':
  		
  				client1.waitForExistence();
  				if(client1.call(reset_srv)){
  					printf("RESET POSITION and VELOCITY");
   				
   				msg.pose.pose.position.x = 0.0;
  				msg.pose.pose.position.y = 0.0;
  				vel.linear.x = 0.0;
  				vel.angular.z = 0.0;
  				pub.publish(msg);
  				pub2.publish(vel);
  				
  				}
  				else{
				printf("Error in connecting with the server");				
  				}
   			break;
   		 	
   		 	case 'w':
   		 		vel_srv.request.input = inputChar;
   		 		client2.waitForExistence();
   		 		if(client2.call(vel_srv)){
   					printf("INCREASING VELOCITY");
   					vel_srv.response.acceleration = 40/100;
					//vel.linear.x += vel_srv.response.acceleration;
				}
				else{
					printf("Error in connecting with UserInputServer");
				}
			 	break;
   		
   			case 's':
   				
   				client2.waitForExistence();
   				vel_srv.request.input = inputChar;
   				
   				if(client2.call(vel_srv)){
   					printf("DECREASING VELOCITY");
   					vel_srv.response.acceleration = -40/100;
   					//vel.linear.x -= vel_srv.response.acceleration;
   		 	 	}
   		 	 	else{
   		 	 		printf("error in connecting with UserInputServer");
   		 	 	}
				break;
  		 	default: 
  		 		
  		 		break;
  		 	}
  		 
  	        	
  			
  		
  		}
   	

			


int main(int argc, char **argv)
{
	ros::init(argc, argv, "reset_node");
	ros::NodeHandle n;
	printf( "%s", menu );
   	Helper *helper = new Helper(&n) ;
	
	
	ros::spin();
	return 0;
}




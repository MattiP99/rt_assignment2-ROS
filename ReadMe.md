# rt_assignment2-ROS
Institution: Università degli Studi Di Genova

Course: Msc in Robotics Engineering

Subject: Research Track 1

Author : Mattia Piras

ASSIGNMENT 2

Introduction
-------------------------------------------------------------------
This is the second assignment of the "Research Track 1", for the Robotics Engineering Degree of the University of Genoa.
Its aim is the implementation of a controller for an autonomous driving car for the Stage Ros simulation.

How to run the program
--------------------------------------------------------------------
First of all you should have ROS installed and set up. 
Clone the repository into your ROS workspace inside the src folder.
Turn back in the workspace and run "catkin_make"

We can choose two option for running this code:

- Option 1:
  - Run stage ros:
    - $ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
  - Run the main_wall_node and the user_node
    - $ rosrun second_assignment main_wall_node
    - $ rosrun sesond_assignment reset_node
- Option 2:
  - Run stage ros:
    - $ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
    
  - In one line you can use the launch file 
    - $ roslaunch second_assignment start.launch
   
 Project Structure:
 ----------------------------------------------------------------------
 
 
 
 <img width="656" alt="assignment2_structure" src="https://user-images.githubusercontent.com/92534255/147602760-054e40f1-8dcc-40e0-b363-57fed975fb6a.PNG">



Three nodes are implemented in order to fullfill the requirements:

 ° world: already provided for this project. It aims to recreate the complete simulation environment of circuit and robot. 
 
 ° main_wall_node: it aims to guide the robot without bumping into the walls of the circuit. It communicates with world through messages: it sends messages to impose the right speed on the robot and it  receives pieces of information regarding the position of the walls provided by the laser scanners which the robot is equipped with. In addition, it also offers a service that can change the speed of the robot according to the command received.
 
 ° user_node: it aims to receive commands from the user and call up the right services.

I created a personal service UserInputService(folder srv) whose aim is to help the comunication between the user node and the main node. To do so, the user has 
to press a particular input to modify certain parameters.

World:
--------------------------------------------------------------------------------------------



![percorso](https://user-images.githubusercontent.com/92534255/147251242-23cd5c29-8eb8-4cf9-b165-b36a9e23d213.JPG)






The track, designed by our professor, is the following and the car/Robot is the little blue dot 
whose scanner visibility is highlighted with this blue area

The world node publishes messages on the base_scan topic. Messages are LaserScan type, from sensor_msg package, and they provide the result of a single scan by the robot's laser sensors.

It also subscribes to the topic cmd_vel to impose a linear and angular velocity to the robot along the three axis. It receives Twist type messages from geometry_msgs package.

main_wall_node:
---------------------------------------------------------------------------------------------

This node is in charge of the control of the car by using a PD (Proportional and Derivative) controller. Its aim is to move the car through the circuit preventing it from crashing.
It uses wallFollowing class' methods and variables and it is created to organize the code and the program. 

  - ros::init

  - ros::Publisher(cmd_vel)

  - WallFollowing *wallfollowing = new WallFollowing(publisher, parameters)

  - ros::subscriber(base_scan, wallfollowing function)

  - ros::server(input_user, wallfollowing function)
 
wallFollowing class.h
-------------------------------------------------------------------------------------------------

public:
  - constructor WallFollowing(Publisher, parameters)
  - distructor ~WallFollowing
  - publishMessage() Function for publishing
  - messageCallback(LaserSensor)
  - serviceUserInputCallback
  - variables
  - Publisher

wallFollowing class.cpp
------------------------------------------------------------------------------------------------- 
Implementation of all methods of the PD controller and all functions for managing the service UserInputService callback


  
user_node:
----------------------------------------------------------------------------------------------

This node is in charge of collecting the input from the user and it acts accordingly to his/her choice. It allows to increase or decrease the velocity of the car, to reset its position and to quit the program with the command ros::shutdown.
A client is implemented here to control the received message from the server in the main node.
This node is helped by the Helper class.

The user interface prints on the screen a menu with three choices:

    "w" : increase velocity
    "s" : decrease velocity
    "r" : reset the system
    "q" : quit

This node uses two services:

    Empty type of service in the std_srvs package. It is very useful to reset the position of the robot if the user pressed 'r'.
    Service, a custom service defined in second_assignment package. It is useful to set the velocity of the robot.

The operation of this node can be explained by the following pseudocode:



initialize node

create clients for services

initialize servers for the services

print menu and actual velocity

key = r

while(roscore is running)
   key = command read from user
   
   if(key == q)
         kill this node
   
   else if(key == r)
         send a request to Empty service
         
   else if(key == w )
         send a request to UserInputService to incrcease the velocity
      
   else if((key == s)
         send a request to UserInputService to decrcease the velocity
     
   else
         print(invalid command)

Helper_class.h
-----------------------------------------------------------------------------------------------------------------
	
    public:
	- Helper
	- ~ Helper
	//Function for the service callback (in charged of evaluating user input)
	- resetPositionModifyVelocityCallback()
	
    private:
    	- NodeHandle
    	- Publisher on odom topic
    	- Publisher on cmd_vel topic
    	- Client for std_srvs::Emtpy service
    	- Client for UserInputService 



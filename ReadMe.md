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
  - Run the main_wall_node and the reset_node
    - $ rosrun second_assignment main_wall_node
    - $ rosrun sesond_assignment reset_node
- Option 2:
  - Run stage ros:
    - $ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
    
  - In one line you can use the launch file 
    - $ roslaunch second_assignment start.launch
   
 Project Structure:
 ----------------------------------------------------------------------
 
  

Three nodes are implemented in order to fullfill the riqueriments:

 ° world: already provided for this project. It aims to recreate the complete simulation environment of circuit and robot. 
 
 ° main_wall_node: it aims to guide the robot without bumping into the walls of the circuit. It communicates with world through messages: it sends messages to impose the right speed on the robot and receives information regarding the position of the walls provided by the laser scanners with which the robot is equipped. In addition, it also offers a service that can change the speed of the robot according to the command received.
 
 ° reset_node: it aims to receive commands from the user and call up the right services.

I created a personal service UserInputService(folder srv) whose aim is to helping the comunication between the user node and the main node. To do so the user has 
to press a particular input for modifying certain parameters.

World:
--------------------------------------------------------------------------------------------



![percorso](https://user-images.githubusercontent.com/92534255/147251242-23cd5c29-8eb8-4cf9-b165-b36a9e23d213.JPG)






The track, designed by our professor, is the following and the car/Robot is the little blue dot
whose scanner visibility is highlighted with that blue area

The world node publishes messages on the base_scan topic. Messages are LaserScan type, from sensor_msg package, and they provide the result of a single scan by the robot's laser sensors.

It also subscribes to the topic cmd_vel to impose a linear and angular velocity to the robot along the three axis. It receives Twist type messages from geometry_msgs package.



# Circuit Simulator

## _Assignment no. 2 for  the Research Track 1 course_

### Professor [Carmine Recchiuto](https://github.com/CarmineD8)

The aim of this project is to make a robot move autonomously inside the Autodromo Nazionale di Monza without hitting the track limits.
In particular, we were asked to:
- use ROS (Robot-Operating-System) for controlling the robot;
- create a node for the control of the robot;
- create an additional node to increase/decrease the velocity and reset the position of the robot.

The following figure shows the circuit that was used for the simulation.
<p align="center">
<img src="https://user-images.githubusercontent.com/62473854/146391484-86abc5b8-617c-4b11-849f-dd28f87321b6.jpg" width=70%, height=70%>
</p>

As we can see in the image, the little blue dot represents the robot, which is endowed with laser scanners that allow the robot to detect obstacles in front of itself in a cone of 180 degrees, from 0 degrees (extreme right) to 180 degrees (extreme left).
In particular, the robot has 720 sensors, so each sensor gives feedback regarding a 0.25 degrees range.

## Installing and running
The simulation requires the installation of ROS, in particular the Noetic Release of ROS.
### Longer version
To run the program, first you need to open a shell window in your ROS workspace and to build the workspace with the command `catkin_make` in the shell; the you must follow this instructions:
1) run the master node with the following command:
```sh
roscore &
```
2) open the environment with the following command:
```sh
stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
3) open another shell window in your ROS workspace and run the following command to run the node relative to the controller of the robot:
```sh
rosrun second_assignment robot_controller
```
4) open another shell window in your ROS workspace and run the following command to run the node relative to the server of the robot:
```sh
rosrun second_assignment robot_server
```
5) open another shell window in your ROS workspace and run the following command to run the node relative to the user interface:
```sh
rosrun second_assignment robot_UI
```
So in the end you should have four opened shell windows.

### Shorter version
Alternatively, you could use the following command:
```sh
roslaunch second_assignment launch.launch
```
which will run the environment and all the nodes (including the master node) and only one shell window will be opened. 
Moreover, using this version allows the user to use a keybord input to kill all the nodes, as will be explained in the section regarding the service.

## Controller node
This node allows the robot to drive for an indefinite amount of time around the arena: when the robot approaches a track limit the controller makes it turn right or left.

### robotCallback
The controller uses the `/base_scan` topic to get information about the surrounding environment after subscribing to it in the `main` function. So when a message is received from the subscriber relative to this topic, the controller enters in the `robotCallback` function.

This function gets the ranges vector (which has 720 values) and saves it in another variable called `distances[]`. Then it studies three subsections of these vector via the function `getMinimum(int start, int end, float distances[])` (which gets the lowest value of a `distances[]` array in a subsection that goes from index `start` to index `end` of said array).
The three subsections are:
- right, which is studied calling `getMinimum(0, 49, distances[])`;
- front, which is studied calling `getMinimum(310, 409, distances[])`;
- left, which is studied calling `getMinimum(620, 719, distances[])`;

After getting the minimum value from each subsection, the robotCallback compares them:
- if the closest obstacle on the front of the robot is at a distance of at least 1.5, then the robot drives forward.
- if the closest obstacle on the front of the robot is at a distance lower than 1.5, then the right and left values are compared and the robot will turn based on which track limit is closest to the robot.

### velCallback
The controller also uses a custom service, `/vel`, to get information about the velocity (in particular if it should be increased/decreased) after subscribing to it in the `main` function. So when a message from this subscriber is received, the controller enters in the `velCallback` function.

This function updates a global variable, which is `acc_factor`, a variable that sets the velocity of the robot when it is driving forward. The user can decide whether to increase or decrease this variable via a user interface.

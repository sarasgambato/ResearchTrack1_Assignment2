# Circuit Simulator

## _Assignment no. 2 for  the Research Track 1 course_

### Professor [Carmine Recchiuto](https://github.com/CarmineD8)

The aim of this project is to make a robot move autonomously inside the Monza circuit without hitting the walls.
In particular, we were asked to:
- use ROS for controlling the robot;
- create a node for the control of the robot;
- create an additional node to increase/decrease the velocity and reset the position of the robot.

The following figure shows the circuit that was used for the simulation.
<p align="center">
<img src="https://user-images.githubusercontent.com/62473854/146391484-86abc5b8-617c-4b11-849f-dd28f87321b6.jpg" width=70%, height=70%>
</p>

As we can see in the image, the little blue dot represents the robot, which is endowed with laser scanners that allow the robot to detect obstacles in front of itself in a cone of 180 degrees, from 0 degrees (extreme right) to 180 degrees (extreme left).
In particular, the robot has 720 sensors, so each sensor gives feedback regarding a 0.25 degrees range.

## Installing and running
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

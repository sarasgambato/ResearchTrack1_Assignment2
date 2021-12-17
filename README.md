# Circuit Simulator

## _Assignment no. 2 for  the Research Track 1 course_

### Professor [Carmine Recchiuto](https://github.com/CarmineD8)

The aim of this project is to make a robot move autonomously inside the Autodromo Nazionale di Monza without hitting the track limits.
In particular, we were asked to:
- use ROS (Robot-Operating-System) for controlling the robot;
- create a node for the control of the robot;
- create an additional node which interacts with the user to increase/decrease the velocity and reset the position of the robot.

Regarding the last point, what I decided to do was to create two separate nodes (apart from the [Controller node](#controller-node)), one to interact with the user ([UI node](#ui-node)) and one to actually modify the robot position and velocity ([Service node](#service-node)). The two nodes work closely together.

The following figure shows the circuit that was used for the simulation.
<p align="center">
<img src="https://user-images.githubusercontent.com/62473854/146391484-86abc5b8-617c-4b11-849f-dd28f87321b6.jpg" width=70%, height=70%>
</p>

As we can see in the image, the little blue dot represents the robot, which is endowed with laser scanners that allow the robot to detect obstacles in front of itself in a cone of 180 degrees, from 0 degrees (extreme right) to 180 degrees (extreme left).
In particular, the robot has 720 sensors, so each sensor gives feedback regarding a 0.25 degrees range.

## Installing and running
The simulation requires the installation of ROS, in particular the Noetic Release of ROS.

To run the program, first you need to open a shell window in your ROS workspace and to build the workspace with the command `catkin_make` in the shell; then you must follow one of the following set of instructions.

### <a id="longer"></a> Longer version

1) run the master node with the following command:
```sh
$ roscore &
```
2) open the environment with the following command:
```sh
$ stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
3) open another shell window in your ROS workspace and run the following command to run the Controller node:
```sh
$ rosrun second_assignment robot_controller
```
4) open another shell window in your ROS workspace and run the following command to run the Service node:
```sh
$ rosrun second_assignment robot_server
```
5) open another shell window in your ROS workspace and run the following command to run the UI node:
```sh
$ rosrun second_assignment robot_UI
```
So in the end you should have four opened shell windows.

### <a id="shorter"></a> Shorter version
Alternatively, you could use the following command:
```sh
$ roslaunch second_assignment launch.launch
```
which will run the environment and all the nodes (including the master node) and only one shell window will be opened. 
Moreover, using this version allows the user to use a keybord input to kill all the nodes, as will be explained in the section regarding the service.

## <a id="controller-node"></a> Controller node
This node allows the robot to drive for an indefinite amount of time around the circuit: when the robot approaches a track limit the controller makes it turn right or left, then information about the velocity is published via the `/cmd_vel` topic.

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
The controller also uses  `/vel`, managed by the UI node, to get information about the velocity (in particular if it should be increased/decreased) after subscribing to it in the `main` function. So when a message from this subscriber is received, the controller enters in the `velCallback` function.

This function updates a global variable, which is `acc_factor`, that sets the angular velocity and the linear velocity of the robot. The user can decide whether to increase or decrease this variable via a user interface (described in the following section).

## <a id="ui-node"></a> UI node
This node prints on the shell a menu which tells the user which keybord inputs can be used to increase/decrease the velocity of the robot, to reset the position and to kill the nodes. The inputs are:
- a, or A, to increase the velocity (accelerate);
- d, or D, to decrease the velocity (decelerate);
- r, or R, to reset the robot to the initial position (reset);
- e, or E, to kill all nodes (exit), to be used only if the command `roslaunch` is used.

When a correct input is inserted by the user, the node sends it to the Service node, which manages the velocity, and publishes the new velocity when the Service node sends it back.

## <a id="service-node"></a> Service node
This node prints on the shell the velocities of the robot and the acceleration factor and it manages the acceleration factor based on the input received by the UI node. 

If the user prompts the Service node to increase (or decrease) the velocity, first the `serverCallback` function checks whether the maximum (or minimum) acceleration factor has been reached (which is `MAX_ACC = 2`, or `MIN_ACC = 0`); if not, the acceleration factor is inceased (or decreased) of `0.25`.

If the user prompts the Service node to reset the robot position, the `serverCallback` function uses the service `/reset_positions` to reset both the position and the velocity of the robot to their initial value.

If the user prompts the Service node to kill all the nodes, this will work only if `roslaunch` was used, because this command opens only one shell window. Otherwise only the Service node will be killed.

If the user presses a wrong input, the Service node will notify the user that an invalid input has been received.

## Flowchart and graph
### graph
Using the [shorter version](#shorter) to run the nodes, this is the graph that illustrates the relatioships between the nodes. It can be seen using the command:
```sh
$ rqt_graph
```
<p align="center">
<img src="https://user-images.githubusercontent.com/62473854/146533011-c3f710ff-c721-4df4-97ae-0b4719c91ee5.png" width=75%, height=75%>
 </p>

### flowchart
The following flowchart shows the behaviour of the Controller node, since it is the one that directly controls the robot.
<p align="center">
<img src="https://user-images.githubusercontent.com/62473854/146542515-7bb9006e-55e3-4e50-86ad-7e1b415f0c5e.png" width=70%, height=70%>
</p>

### Possible improvements
1. Implementing the functionality to kill all nodes even when the [longer version](#longer) to run the nodes is used.
2. To make the robot move more swiftly, since it moves with a zig-zag movement to avoid hitting the track limits.

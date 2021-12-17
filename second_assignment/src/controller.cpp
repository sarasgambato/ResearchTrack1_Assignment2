#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "second_assignment/Vel.h"
#include "second_assignment/Service.h"
#include "std_srvs/Empty.h"

#define SIZE 100

// declaration of publisher and message
ros::Publisher pub;
geometry_msgs::Twist my_vel;

// declaration of global variables;
float acc_factor; // variable which represents the acceleration factor

float getMinimum(int start, int end, float distances[])
{
    /* Function to get the minimum distance
       Arguments:  - distances(float[]): array of distances from the obstacles, divided in subsections
                   - start(int): first point of the subsection
                   - end(int): last point of the subsection

       Returns:    - min(float): minimum value of the subsection */
    
    float min = 50;
    for(int i = start; i < end; i++) {
        if (distances[i] < min)
            min = distances[i];
    }
    return min;
}

void robotCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    /* Function to control the robot */

    // we know that the robot can see obstacles in a range of 180 degrees, and that it stores
    // distances from obstacles in a vector with 720 elements, where the first element
    // represents the distance on the far right and the last element represents the distance
    // on the far left. So we get the minimum value of three subsections from this vector: right, left, front.

    float distances[720];

    for(int i = 0; i < 720; i++)
        distances[i] = msg->ranges[i];

    float min_right = getMinimum(0, 49, distances);
    float min_front = getMinimum(310, 409, distances);
    float min_left = getMinimum(670, 719, distances);

    if (min_front > 1.5) {
        my_vel.linear.x = 1.5 + acc_factor;
        my_vel.angular.z = 0;
    }
    else {
        if (min_right < min_left) {
            my_vel.angular.z = 1;
            my_vel.linear.x = 0.5;
        }
        else {
            my_vel.angular.z = -1;
            my_vel.linear.x = 0.5;
        }
    }
    
    pub.publish(my_vel);
}

void velCallback(const second_assignment::Vel::ConstPtr& msg)
{
    /* Function to update the acceleration factor*/ 

    acc_factor = msg->my_msg;
}

int main(int argc, char **argv)
{
    // initialize the node, set up the NodeHandle for handling the communication with the ROS system
    ros::init(argc, argv, "control");
    ros::NodeHandle nh;

    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    // initialize 2 subscriber 
    ros::Subscriber sub1 = nh.subscribe("/base_scan", 1, robotCallback);
    ros::Subscriber sub2 = nh.subscribe("/vel", 1, velCallback);

    ros::spin();

    return 0;
}
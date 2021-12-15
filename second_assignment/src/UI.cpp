#include "ros/ros.h"
#include "second_assignment/Vel.h"
#include "second_assignment/Service.h"

ros::Publisher pub;
ros::ServiceClient client;

void uiCallbakc()
{
    second_assignment::Service srv;
    second_assignment::Vel vel;

    char input;

    std::cout << "Press:\ta/A to accelerate the robot;\n";
    std::cout << "Press:\td/D to decelerate the robot;\n";
    std::cout << "Press:\tr/R to reset the position of the robot;\n";
    std::cout << "Press:\te/E to exit the program.\n";

    std::cin >> input;

    srv.request.input = input;

    client.waitForExistence();
    client.call(srv);

    vel.my_msg = srv.response.output;

    pub.publish(vel);
}

int main(int argc, char **argv)
{
    // initialize the node, set up the NodeHandle for handling the communication with the ROS system
    ros::init(argc, argv, "UI");
    ros::NodeHandle nh;

    pub = nh.advertise<second_assignment::Vel>("/vel", 1);
    client = nh.serviceClient<second_assignment::Service>("/service");

    while(ros::ok()) {
        uiCallbakc();
        ros::spinOnce();
    }

    return 0;
}
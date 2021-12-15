#include "ros/ros.h"
#include "second_assignment/Service.h"
#include "std_srvs/Empty.h"

#define ACC 0.25

std_srvs::Empty rst;

float acc_factor = 0;

bool serverCallback(second_assignment::Service::Request &req, second_assignment::Service::Response &res)
{
    switch(req.input) {

        // accelerate
        case 'a':
        case 'A':
            acc_factor += ACC;
            break;

        // decelerate
        case 'd':
        case 'D':
            acc_factor -= ACC;
            break;

        // reset position
        case 'r':
        case 'R':
            ros::service::call("/reset_position", rst);
            acc_factor = 0;
            break;

        case 'e':
        case 'E':
            ros::shutdown();
            break;

        // wrong key pressed
        default:
            std::cout << "Invalid input, please try again.\n";
            break;
    }

    res.output = acc_factor;

    return true;
}

int main(int argc, char **argv)
{
    // initialize the node, set up the NodeHandle for handling the communication with the ROS system
    ros::init(argc, argv, "server");
    ros::NodeHandle nh;

    ros::ServiceServer service = nh.advertiseService("/service", serverCallback);

    ros::spin();

    return 0;
}
#include "ros/ros.h"
#include "second_assignment/Service.h"
#include "std_srvs/Empty.h"

// delta to increase the linear velocity
#define ACC 0.25

// thresholds for the acceleration factor
#define MAX_ACC 1.5
#define MIN_ACC -1.5

std_srvs::Empty rst;

// initialization of the acceleration factor to 0
float acc_factor = 0;

bool serverCallback(second_assignment::Service::Request &req, second_assignment::Service::Response &res)
{
    /* Function to update the acceleration factor based on the user input */

    switch(req.input) {

        // accelerate
        case 'a':
        case 'A':
            if (acc_factor < MAX_ACC)
                acc_factor += ACC;

            else
                std::cout << "Maximum acceleration factor reached.\n";
            break;

        // decelerate
        case 'd':
        case 'D':
            if (acc_factor > MIN_ACC)
                acc_factor -= ACC;
            else
                std::cout << "Minimum acceleration factor reached.\n";
            break;

        // reset position
        case 'r':
        case 'R':
            ros::service::call("/reset_positions", rst);
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
    
    std::cout << "During turns: linear velocity = (0.5, 0, 0), angular velocity = (0, 0, +/-1)\n";
    std::cout << "Driving forward: linear velocity = (" << 1.5+acc_factor << ", 0, 0), angular velocity = (0, 0, +/-1)\n";
    std::cout << "Acceleration factor = " << acc_factor << "\n";

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
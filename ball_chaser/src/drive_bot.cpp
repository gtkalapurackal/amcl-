#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/drivetotarget.h"
#include <string.h>
#include <signal.h>

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

void shutdowncall(int sig)
{
        geometry_msgs::Twist motor_command;
        
        motor_command.linear.x =0.0;
        motor_command.angular.z = 0.0;
        
        motor_command_publisher.publish(motor_command);
	ROS_INFO_STREAM("commmading the robot to shutdown");
  	ros::shutdown();


}
bool move_request(ball_chaser::drivetotarget::Request& req,
    ball_chaser::drivetotarget::Response& res)
{
        geometry_msgs::Twist motor_command;
        
        motor_command.linear.x =req.linear_x;
        motor_command.angular.z = req.angular_z;
        
        motor_command_publisher.publish(motor_command);
        //ros::Duration(1).sleep();
	
	
    // Return a response message
    res.msg_feedback = "linear  & angular velocity SET  ";
    
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot",ros::init_options::NoSigintHandler);

    // Create a ROS NodeHandle object
    ros::NodeHandle n;
    // shutdown handler 
	signal(SIGINT, shutdowncall);	
    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 3);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot",move_request);


    ROS_INFO("ready to sent velocity");
   

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}

#include "ros/ros.h"
#include "../include/Triangle.h"
#include "turtlesim/Pose.h"
#include "turtlesim/SetPen.h"
#include <math.h>
#include "geometry_msgs/Twist.h"

turtlesim::Pose currentPosition = turtlesim::Pose();
geometry_msgs::Twist Velocity = geometry_msgs::Twist();

bool cw;
float sideLenght;

void callback(const assignment1::Triangle::ConstPtr& message)
{
  cw = message->cw;
  sideLenght = message->sideLength;
}

void positionCallback(const turtlesim::Pose::ConstPtr& msg)
{
  currentPosition.x = msg->x;
  currentPosition.y = msg->y;
  currentPosition.theta = msg->theta;
}

ros::Publisher publisher;

void stop()
{
  Velocity.angular.z = 0;
  Velocity.linear.x = 0;
  publisher.publish(Velocity);
}

void drawCounterClockWiseTriangle(turtlesim::Pose startPosition, float lineLenght)
{
  while(currentPosition.x < startPosition.x + lineLenght)
  {
    Velocity.linear.x = 0.5;
    publisher.publish(Velocity);
    ros::spinOnce();
  }

  stop();

  while(currentPosition.theta < 120 * (M_PI / 180))
  {
    Velocity.angular.z = 0.2;
    publisher.publish(Velocity);
    ros::spinOnce();
  }

  stop();

  while(currentPosition.x > startPosition.x + (0.5 * lineLenght))
  {
    Velocity.linear.x = 0.2;
    publisher.publish(Velocity);
    ros::spinOnce();
  }

  stop();

  while(currentPosition.theta < ( -2 * M_PI / 3 ) - 0.001 || currentPosition.theta > ( -2 * M_PI / 3 ) + 0.001)
  {
    Velocity.angular.z = 0.2;
    publisher.publish(Velocity);
    ros::spinOnce();
  }

  stop();

  while(currentPosition.x > startPosition.x)
  {
    Velocity.linear.x = 0.2;
    publisher.publish(Velocity);
    ros::spinOnce();
  }

  stop();
}

// Goes out to the left from starting point.

void drawClockWiseTriangle(turtlesim::Pose startPosition, float lineLenght)
{

    while(currentPosition.theta < ( -1 * M_PI ) - 0.001 || currentPosition.theta > ( -1 * M_PI ) + 0.001)
    {
      Velocity.angular.z = 0.2;
      publisher.publish(Velocity);
      ros::spinOnce();
    }

    stop();

     while(currentPosition.x > startPosition.x - lineLenght)
    {
      Velocity.linear.x = 0.2;
      publisher.publish(Velocity);
      ros::spinOnce();

      std::cout << "current X == " << std::to_string(currentPosition.x) << " --> " << std::to_string(startPosition.x - lineLenght) << '\n';
    }

    stop();

    while(currentPosition.theta > 60 * (M_PI / 180) + 0.001 || currentPosition.theta < 60 * (M_PI / 180) - 0.001 )
    {
      Velocity.angular.z = -0.2;
      publisher.publish(Velocity);
      ros::spinOnce();
    }

    stop();

      while(currentPosition.x < startPosition.x - (0.5 * lineLenght))
      { 
        Velocity.linear.x = 0.2;
        publisher.publish(Velocity);
        ros::spinOnce();
      }

    stop();
  
    while(currentPosition.theta < ( -1 * M_PI / 3 ) - 0.001 || currentPosition.theta > ( -1 * M_PI / 3 ) + 0.001)
    {
      Velocity.angular.z = -0.2;
      publisher.publish(Velocity);
      ros::spinOnce();
    }

    stop();
    std::cout << "on the home stretch: " + std::to_string(currentPosition.x) + " start: " + std::to_string(startPosition.x) + '\n';

    while(currentPosition.x < startPosition.x)
    {
      Velocity.linear.x = 0.2;
      publisher.publish(Velocity);
      ros::spinOnce();
    }

    stop();

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Draw_triangle");
  ros::NodeHandle drawTriangleNode;
  turtlesim::Pose startPosition = turtlesim::Pose();

  publisher = drawTriangleNode.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
  ros::Subscriber subscriber = drawTriangleNode.subscribe("/turtle1/pose", 10, positionCallback);
  ros::Subscriber subTriangle = drawTriangleNode.subscribe("/cmd", 10, callback);

  while(startPosition.x == turtlesim::Pose().x && startPosition.y == turtlesim::Pose().y)
  {
    startPosition = currentPosition;
    ros::spinOnce();
  }

  while(sideLenght == 0)
  { 
   // std::cout << "waiting for a valid triangle. \n";
        ros::spinOnce();
  }

    std::cout << "received sidelength: " + std::to_string(sideLenght) << "\n";

  if(cw)
  {
    drawClockWiseTriangle(startPosition, sideLenght);
  }
  else
  {
    drawCounterClockWiseTriangle(startPosition, sideLenght);
  }

  ros::spinOnce();
  return 0;
}


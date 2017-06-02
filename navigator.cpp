#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "std_msgs/Float64.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Pose2D.h"

#include <cstddef>
#include <functional>
#include <queue>
#include <vector>
#include <list>
#include <deque>

#include "a_star/waypoint_data.h"
#include "a_star/pathdata_array.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;

int    width      = 500; 
int    height     = 500;

int goal_x,goal_y;

int startxpos = 49;  //  width/2- 1
int startypos = 0;

bool path_found = true;

float  map_res = 0.25;

int stop_xpos(49);
int stop_ypos(99);

float  *map_data   = NULL;

int w1 = 10;
int w2 = 0;
int w3 =35;

bool map_received=false, goal_set=false, currhead_received=false;
bool new_path_required=false, newmap_set=false;

Mat mapImgn;

ros::Publisher map_pub,blown_map_pub;

pair<float,float> convToCart(int i,float r)
{
   float ang = i*0.25*(3.14/180);
   float x = r*cos(ang);
   float y = -1*r*sin(ang);
   return make_pair(x,y);
}


void set_goal()  // <----- add some aruments here    optimise this by exact local destination within lical masomp
{
   std::cout<<"Run1";
  
  if (map_received == true && currhead_received == true && goal_set == false) // <- how or where to assign these boolean values ?
  {
    float des_head=0; 
    des_head = waypointdata->angle;   // angle is made with the vertical 

    if (goal_y >= 0)
    {
      stop_ypos = goal_y;
      
      if (stop_ypos >= height)
       stop_ypos = height - 1;    // fixed it at the top if it is outta bounds 
    }
    
    else if (stop_ypos < 0) 
    {
     yaw(10);
     gps();
     set_goal();
     return;
    }
    
    stop_xpos = (width/2-1) + goal_x;

    if (stop_xpos >= width)
       stop_xpos = width-1;
    
    if (stop_xpos < 0) 
      {
        yaw(10);
        gps();
        set_goal();
        return;
      }

    if (map_data[stop_ypos*width + stop_xpos] != 0)   
     { int flag=0;
      for(int i=0;i<500;i++)
      {
        for(int k=-i;k<=i;k++)
        {
          for(int j=-i;j<=i;j++)
          {if((stop_ypos+j >=0)&&(stop_ypos+j <height)&&((stop_xpos+k)<width)&&((stop_xpos+k)>=0))
            {if(map_data[(stop_ypos+j)*width + (stop_xpos+k)] = 0)
              {
                stop_ypos+=j;
                stop_xpos+=k;
                flag=1;
                if(flag!=0)
                  goto jmp;
              }
            }   
          }
        }
      }
    }    jmp: 
  }    goal_set=true;
}


ros::Publisher route;

int main(int argc,char** argv)
{
   ros::init(argc,argv,"nav_map");
   ros::NodeHandle g1,g2,g3;
   ros::Subscriber lidar_sub=n1.subscribe/*package::nodetopic&buffer size&call function here probably (sensor_msgs/NavSatFix)  also pass args */;
   map_pub=n2.advertise<nav_msgs::OccupancyGrid>/*add publishing data type and buff size */;
   
   cv::namedWindow("nav_map",CV_WINDOW_NORMAL);
   ros::Rate loop_rate(15.0);

   while(ros::ok())
   {
     ros::spinOnce();
     loop_rate.sleep(); 
     gps();
     set_goal();
     a_star();
     pathvector();
     pathvector_to_gps();
   }
   return 0; 
}

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
//using namespace std;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;

int    width      = 100; 
int    height     = 100;

int startxpos = 49;
int startypos = 0;

bool path_found = true;

float  map_res = 0.05;

int stop_xpos(49);
int stop_ypos(99);

float curr_head   = 0;
float last_head   = 0;
float curr_xpos   = 0;
float curr_ypos   = 0;
float distance_to_plan = 10.0;

std::list<int> fullpathx;
std::list<int> fullpathy;

std::list<double> sm_pathx;
std::list<double> sm_pathy;   

float  *map_data   = NULL;

int w1 = 10;
int w2 = 0;
int w3 =35;

bool map_received(false), goal_set(false), currhead_received(false);
bool new_path_required(false), newmap_set(false);

Mat mapImgn;

// define degree to rad and rad to degree
        

ros::Publisher map_pub,blown_map_pub;


pair<float,float> convToCart(int i,float r) //convert from polar to cartesian co-ordinates
{
   float ang = i*0.25*(3.14/180);
   float x = r*cos(ang);
   float y = -1*r*sin(ang);
   return make_pair(x,y);
}

//  this function \/ is chaka chak no need to fiddle further  

float *createcostmap(int width, int height, int stopxpos, int stopypos) // this function will be calledwhenever the stop xpos and stop y pos is updated 
{   std::cout<<"Run2";
  float *heucostdata = new float[width*height];
  Mat costmap = Mat::zeros(height, width, CV_8UC1);
  mapImgn.convertTo(costmap, CV_8UC1, 255, 0);    //   <-----wtf review this ?????????????
  for (int i=0;i<width*height;i++)
  {
    heucostdata[i]  = (abs(stopypos - (i/width)) + abs(stopxpos - (i%width)))*(float)w1 ;
    if (map_data[i] == 1)
    {
      heucostdata[i] = 999999;   //infinity basically 
    }
  }

  for (int i=0;i<height;i++)  //this seems to be similar to g-vale just on steroids 
  {
    for (int j=0;j<width;j++)
    {
      if(map_data[i*width+j] > 0)
      {
        for (int k=(i-1);k<(i+1);k++)
        {
          for (int l=(j-1);l<(j+1);l++)     // creates a square window of side 30 
          {
            if (k >=0 && k < height && l >= 0 && l < width && k != i && l != j)  // k and l are within the grid and not tat the centre then ....
            {
              heucostdata[k*width+l] +=  (float(w3)*sqrt(pow(abs(i - k),2) + sqrt(abs(j - l),2) ));  // some additional cost added for no reason 
            }
          }
        }
      } 
    } 
  }

  for (int i=0;i<height;i++)   // useless loda lehsun for visualization 
  {
    for (int j=0;j<width;j++)
    {
      costmap.at<unsigned char>(i , j) = int(heucostdata[i*width+j])%256;
    }
  }
  std::cout<<"Run2";
  return heucostdata;
}


void set_goal(const a_star::waypoint_data::ConstPtr& waypointdata)  // optimise this by exact local destination within lical map
{ std::cout<<"Run1";
  if (map_received == true && currhead_received == true && goal_set == false)
  {
    float des_head=0; 
    des_head = waypointdata->angle;   // angle is made with the vertical 

    if (int((distance_to_plan*cos((curr_head-des_head)*DEG_TO_RAD))/map_res) >= 0)
    {
      stop_ypos = int((distance_to_plan*cos((curr_head-des_head)*DEG_TO_RAD))/map_res);
      
      if (stop_ypos >= height)
       stop_ypos = height - 1;    // fixed it at the top if it is outta bounds 
    }
    
    else if (stop_ypos < 0) {stop_ypos = 0;} // how will stop_ypos be negative ?
    
    stop_xpos = (width/2-1) + int((distance_to_plan*sin((curr_head-des_head)*DEG_TO_RAD))/map_res);
    
    if (stop_xpos >= width)
      { stop_xpos = width-1;}
    if (stop_xpos < 0) 
      {stop_xpos = 0;}

    if (map_data[stop_ypos*width + stop_xpos] != 0)   // <-----what does this doo ??
      
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




  
  /*fix orientation of destination wrt current location */
  /*receive blown occupancy grid data*/
  /*if else ladder */
  /* if destination out of local grid then turn the quad s.t. line of erientation connects current and destination */
  /* apply astar with destination as the point at top mid point or nearest unocupied one in blown map */
  /*publish data */
  /*plotnavigation*/



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
   }
   return 0; 
}

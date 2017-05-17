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

void set(int, void*){
}

// Path finding Functions
int alpha100 = 10 ;   //coo wieghts
int beta100 = 40; //smoothen weight
float alpha = 0.1 ;   //coo wieghts
float beta = 0.4; //smoothen weight
 double tolreance = 0.01;//tolreance for smooth
 double temp0 = 0;
 double temp1 = 0;
 double aux = 0;
int movementcost(int x1, int y1);
void calculatepath(float *hcost);
void path_smooth();



class mapnode{		
		public:
			
            mapnode(){

		     address.xpos = 0;
		     address.ypos = 0;

		     gcost = 0;
		     hcost = 0;
		     fcost = 0;
                   
             nodetype = -1;
	            }


		struct nodeaddress{
			int xpos, ypos;	
		} address; 

		int gcost, hcost, fcost, nodetype;// -1 for not explored ; 0 for open list ; 1 for closed list
                 
  
	};


class nodeCompare{
		public:
			bool operator()(const mapnode &t1, const mapnode &t2){
				return t1.fcost > t2.fcost;
			}
                  };


//Map finding functions


void current_heading(const geometry_msgs::Pose2D::ConstPtr& msg){
	curr_head = msg->theta;
	currhead_received = true;
}


void newpath_req(const std_msgs::Bool::ConstPtr& msg_in){
	new_path_required = msg_in->data;
}

float *createcostmap(int width, int height, int stopxpos, int stopypos)
{   std::cout<<"Run2";
	float *heucostdata = new float[width*height];
	Mat costmap = Mat::zeros(height, width, CV_8UC1);
	mapImgn.convertTo(costmap, CV_8UC1, 255, 0);    //   <-----wtf review this ?????????????
	for (int i=0;i<width*height;i++)
	{
		heucostdata[i]  = pow((pow(abs(stopypos - (i/width)),2.0) + pow(abs(stopxpos - (i%width)),2.0)), 0.5)*(float)w1 + (float(255-costmap.at<unsigned char>(int(i/width), int(i%width)))*(float)w2);//< -- what is the second part of this cost sum denotig to.. ??
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
				for (int k=(i-15);k<(i+15);k++)
				{
					for (int l=(j-15);l<(j+15);l++)     // creates a square window of side 30 
					{
						if (k >=0 && k < height && l >= 0 && l < width && k != i && l != j)  // k and l are within the grid and not tat the centre then ....
						{
							heucostdata[k*width+l] +=  (float(w3)/(abs(i - k) + abs(j - l)));  // some additional cost added for no reason 
						}
					}
				}
			}	
		}	
	}

	for (int i=0;i<height;i++){
		for (int j=0;j<width;j++){
			costmap.at<unsigned char>(i , j) = int(heucostdata[i*width+j])%256;
		}
	}
	std::cout<<"Run2";
	return heucostdata;
}


void set_goal(const a_star::waypoint_data::ConstPtr& waypointdata)  // optimisethis by exact local destination within lical map
{	std::cout<<"Run1";
	if (map_received == true && currhead_received == true && goal_set == false)
	{
		float des_head=0; 
	 	des_head = waypointdata->angle;

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

	 	{
	 		int i(1), k(stop_ypos), l(stop_xpos);  //not req
	 		
	 		bool goal(false);
	 		
	 		float slope(0), denom(0), num(0), dx(-1.0), dy(-1.0);
	 	
	 		denom = stop_xpos - startxpos;
	 		num = stop_ypos - startypos;
	 		
	 		if (denom == 0){
	 			slope = 100;
	 		} else slope = num/denom;

	 		while (goal != true && i <= 50){
	 			stop_xpos += int(dx);
	 			stop_ypos += int(dy);
	 			if (stop_ypos >= 0 && stop_ypos < height && stop_xpos >= 0 && stop_xpos < width && map_data[stop_ypos*width + stop_xpos] == 0){
	 				goal = true;
	 			} else i++;
	 			std::cout<<i<<std::endl;
	 			std::cout<<stop_ypos<<" "<<stop_xpos<<std::endl;
	 			std::cout<<std::endl;
			}
	 	}

	 	if (stop_xpos > width) stop_xpos = width-1;
	 	if (stop_xpos < 0) stop_xpos = 0;
	 	if (stop_ypos > height) stop_ypos = height-1;
	 	if (stop_ypos < 0) stop_ypos = 0;
	 	if (stop_xpos == (width/2 - 1)) stop_xpos = width/2;
        std::cout<<"Run1";
	 	goal_set = true;
	}
}



void map_to_plan(const nav_msgs::OccupancyGrid::ConstPtr& map_in)
{   std::cout<<"Run0";
	if (map_received == false)
	{
		width    = map_in->info.width;
		height   = map_in->info.height;
		map_res  = map_in->info.resolution;
		
		startxpos = (width/2 - 1);
        
		if (newmap_set == false){
		float* mdata = new float[width*height];
		map_data = mdata;
		newmap_set = true;
		}

		bool check(false);
		for (int i=0;i<(width*height);i++){
			if (map_in->data[i] > 0){
				check = true;
			}
		}
		if (check == true){
			w2 = 8;  //8
		} else w2 = 0;

		for (int i=0;i<(width*height);i++){
			if (map_in->data[i] >0)
				map_data[i] = 1;
			else map_data[i] = 0;
		}

		
		if(map_data[startypos*width + startxpos] == 0){
			std::cout<<"IN"<<std::endl;
			Mat mapImg = Mat::zeros(height, width, CV_8UC1);
			for (int i=0;i<height;i++){
				for (int j=0;j<width;j++){
					if (map_data[i*width+j] == 0){
						mapImg.at<unsigned char>(i , j) = 200;
					}
					else mapImg.at<unsigned char>(i , j) = 0;
				}
			}
            	
			distanceTransform (mapImg, mapImgn, CV_DIST_L2, 5);
			normalize (mapImgn, mapImgn, 0.0, 1.0, NORM_MINMAX);

			for (int i=0;i<(width*height);i++){             //i dont know why it is required again
				if ( float(mapImg.at<unsigned char>(int(i/width), int(i%width))) > 0)
					map_data[i] = 0;
				else map_data[i] = 1;
			}
		
			std::list<double>::iterator ity1=sm_pathy.begin();
			std::list<double>::iterator itx1=sm_pathx.begin();
			for (int i=0;i<(fullpathx.size()-0);i++){
                 mapImg.at<unsigned char>(*ity1 , *itx1) = 0;
                 itx1++;
                 ity1++;
			}

			fullpathx.clear();
			fullpathy.clear();
			sm_pathx.clear();
			sm_pathy.clear();
			
			Mat mapImgfinal = Mat::zeros(height, width, CV_8UC1);

			for (int i=0;i<height;i++){
				for (int j=0;j<width;j++){
					mapImgfinal.at<unsigned char>(i , j) = mapImg.at<unsigned char>(height-1-i , j);  
				}
			}

			imshow("grid", mapImgfinal);
			waitKey(10);
	        std::cout<<"Run0";
			map_received = true;
		}
	}
}

int main(int argc, char** argv){
	ros::init(argc, argv, "astar");
	ros::NodeHandle n;

	ros::Subscriber sub1 = n.subscribe("/newpath_required", 10, newpath_req);
	ros::Subscriber sub2 = n.subscribe("/imu/HeadingTrue_degree", 10, current_heading);
	ros::Subscriber sub3 = n.subscribe("/scan/blown_local_map", 10, map_to_plan); // For sandeep code
	//ros::Subscriber sub3 = n.subscribe("/scan/OccGrd", 10, map_to_plan);
	ros::Subscriber sub4 = n.subscribe("/waypoint_data", 10, set_goal);
	

	ros::Publisher pub   = n.advertise<a_star::pathdata_array>("/planned_path", 10);
	ros::Publisher pub1  = n.advertise<std_msgs::Bool>("/path_status", 10);


	ros::Rate loop_rate(10);
        
   std::cout<<"Distance to plan in Meter = ";
   std::cin>>distance_to_plan;

	namedWindow("grid", CV_WINDOW_NORMAL);

	createTrackbar("goal", "grid", &w1, 100, set);
    createTrackbar("obst", "grid", &w2, 100, set);
	createTrackbar("side", "grid", &w3, 100, set);
	createTrackbar("rough", "grid", &alpha100, 50, set);
	createTrackbar("smooth", "grid", &beta100, 50, set);

	a_star::pathdata_array plannedpath;
	std_msgs::Bool path_stat;
	path_stat.data = true;



	while(ros::ok()){
		std::cout<<"new_path_required"<<new_path_required<<" , "<<"goal_set"<<goal_set<<" , "<<"map_received"<<map_received<<std::endl;
		
	if (new_path_required == true && goal_set == true && map_received == true){
                     // std::cout<<"difbsdhbv"<<std::endl;
		
			float *hcost;
			hcost = createcostmap(width, height, stop_xpos, stop_ypos);
			map_data[stop_ypos*width + stop_xpos] = 0;
			
			calculatepath(hcost);
            std::cout<<"Run4";
			delete hcost;
            std::cout<<"Run4";

            if(path_found==true){path_smooth();}

            for (int i=0;i<width*height;i++){
				map_data[i] = 0;
			}
			//plannedpath clear to be cleared
			if(path_found==true){last_head = curr_head;}

			std::list<double>::iterator sity1=sm_pathy.begin();
			std::list<double>::iterator sitx1=sm_pathx.begin();

	
        if(path_found==true){		 
				plannedpath.x1 = ((*sitx1-(width/2))*cos((curr_head-90)*DEG_TO_RAD))-((*sity1)*sin((curr_head-90)*DEG_TO_RAD));
                plannedpath.y1 = ((*sitx1-(width/2))*sin((curr_head-90)*DEG_TO_RAD))+((*sity1)*cos((curr_head-90)*DEG_TO_RAD));
                plannedpath.x1_nt = *sitx1-(width/2); //not transformed
                plannedpath.y1_nt = *sity1;

                    sity1++;
                    sitx1++; 
          
				plannedpath.x2 = ((*sitx1-(width/2))*cos((curr_head-90)*DEG_TO_RAD))-((*sity1)*sin((curr_head-90)*DEG_TO_RAD));
                plannedpath.y2 = ((*sitx1-(width/2))*sin((curr_head-90)*DEG_TO_RAD))+((*sity1)*cos((curr_head-90)*DEG_TO_RAD));
                plannedpath.x2_nt = *sitx1-(width/2);
                plannedpath.y2_nt = *sity1;}

        if(path_found==false){		 
				plannedpath.x1 = 0;
                plannedpath.y1 = 0;
                plannedpath.x1_nt = 0; 
                plannedpath.y1_nt = 0;
                plannedpath.x2 = (1*cos((last_head-90)*DEG_TO_RAD))-(0*sin((last_head-90)*DEG_TO_RAD));
                plannedpath.y2 = (1*sin((last_head-90)*DEG_TO_RAD))+(0*cos((last_head-90)*DEG_TO_RAD));
                plannedpath.x2_nt = 1;
                plannedpath.y2_nt = 0;}                

                   /* sity1++;
                    sitx1++;            
				plannedpath.x3 = ((*sitx1-(width/2))*cos((curr_head-90)*DEG_TO_RAD))-((*sity1)*sin((curr_head-90)*DEG_TO_RAD));
                plannedpath.y3 = ((*sitx1-(width/2))*sin((curr_head-90)*DEG_TO_RAD))+((*sity1)*cos((curr_head-90)*DEG_TO_RAD)); */  
            path_stat.data = path_found;           
            pub1.publish(path_stat);            
			pub.publish(plannedpath);
			map_received = false;
			goal_set = false;
		}	
	     std::cout<<"Run6"<<std::endl;
		ros::spinOnce();
		loop_rate.sleep();
	}
}
//PAth finding BODY
//Do change upper declations



int movementcost(int x1, int y1){
		int positiondifference = x1+y1;
		if (positiondifference == 2||positiondifference == -2||positiondifference == 0 ) return 14;
		else if ( positiondifference == 1||positiondifference == -1 ) return 10;
	                        }

void calculatepath(float *hcost){std::cout<<"Run3";

		std::priority_queue<mapnode, std::vector<mapnode>, nodeCompare> openlist;    //open_list                                 

                mapnode *mapnode_list = new mapnode[width*height];    //To database every node
                int *actionlist = new int[width*height];     //0 for start        //action it took to get there 1-2-3-4-5-6-7-8 == (right)-(Rightdown)-(down)-(Downleft)-(left)-so on
     
                for (int i=0;i<height;i++){
	     	      for (int j=0;j<width;j++){
                     actionlist[i*width + j]= -1;
                                    }
		                         }

                actionlist[startypos*width + startxpos] = 0;

                mapnode_list[startypos*width + startxpos].address.xpos=startxpos;
                mapnode_list[startypos*width + startxpos].address.ypos=startypos;
                
                mapnode_list[startypos*width + startxpos].nodetype = 0; //set start to open list
                mapnode_list[startypos*width + startxpos].gcost=0;
                mapnode_list[startypos*width + startxpos].hcost=hcost[startypos*width + startxpos];
                mapnode_list[startypos*width + startxpos].fcost= hcost[startypos*width + startxpos]+mapnode_list[startypos*width + startxpos].gcost;

                openlist.push(mapnode_list[startypos*width + startxpos]);

                int new_x=0;
                int new_y=0;
                int currn_x=0;
                int currn_y=0;              

                 while(true){


                  if(openlist.size()==0){     //open list is empty return fail
                      std::cout<<"nothing found"<<std::endl;
                      path_found = false;
                      break;            }

                currn_x = (openlist.top()).address.xpos;
                currn_y = (openlist.top()).address.ypos;
                openlist.pop();

                    if(currn_x==stop_xpos && currn_y==stop_ypos){ path_found = true;
                    	                                          break;}

                    else{
                       mapnode_list[currn_y*width + currn_x].nodetype = 1;  //pop set to closed list

                         for (int deltax = -1; deltax < 2; deltax++){
                                 for (int deltay = -1; deltay < 2; deltay++){

                                 if (deltax == 0 && deltay == 0){// If it's current point then pass
                                 continue;}

                                 new_x = currn_x+deltax;
                                 new_y = currn_y+deltay;


                                 if (new_x<0 || new_x>=width-1){// If it's corner in x
                                 continue;}

                                 if (new_y<0 || new_y>=height-1){// If it's corner in y
                                 continue;}

                                 if (map_data[new_y*width+new_x] > 0){//for avoiding obstacle
			                     continue;}	                                               

                                 if (mapnode_list[new_y*width+new_x].nodetype == 1){// If it's closed
                                 continue;}

                                 if (mapnode_list[(new_y)*width + (new_x)].nodetype==0){// If it's already in open list

                                  // If it has a worse g score than the one that pass through the current point
                                  // then its path is improved when it's parent is the current point
                                    if (mapnode_list[(new_y)*width + (new_x)].gcost > (mapnode_list[(currn_y)*width + currn_x].gcost + movementcost(deltax,deltay))) {

                                       // Change its parent and g score and update
                                      mapnode_list[(new_y)*width + (new_x)].gcost = mapnode_list[(currn_y)*width + currn_x].gcost + movementcost(deltax,deltay) ;

                                     mapnode_list[(new_y)*width + (new_x)].fcost = mapnode_list[(new_y)*width + (new_x)].gcost+ hcost[(new_y)*width + new_x]  ;


                                  if(deltax==1&&deltay==0){actionlist[(new_y)*width + (new_x)]=1;}
                                  if(deltax==1&&deltay==-1){actionlist[(new_y)*width + (new_x)]=2;}
                                  if(deltax==0&&deltay==-1){actionlist[(new_y)*width + (new_x)]=3;}
                                  if(deltax==-1&&deltay==-1){actionlist[(new_y)*width + (new_x)]=4;}
                                  if(deltax==-1&&deltay==0){actionlist[(new_y)*width + (new_x)]=5;}
                                  if(deltax==-1&&deltay==1){actionlist[(new_y)*width + (new_x)]=6;}
                                  if(deltax==0&&deltay==1){actionlist[(new_y)*width + (new_x)]=7;}
                                  if(deltax==1&&deltay==1){actionlist[(new_y)*width + (new_x)]=8;}


                                                                                                                                                                      }
                                                                                       }
                                 else{


                                  // Add it to the openList with current point as parent
                                  mapnode_list[(new_y)*width + (new_x)].address.xpos = new_x;
                                  mapnode_list[(new_y)*width + (new_x)].address.ypos = new_y;
                                  // Compute it's g, h and f score
                                  mapnode_list[(new_y)*width + (new_x)].gcost = mapnode_list[(currn_y)*width + (currn_x)].gcost + movementcost(deltax,deltay);
                                  mapnode_list[(new_y)*width + (new_x)].hcost = hcost[new_y*width + new_x];
                                  mapnode_list[(new_y)*width + (new_x)].fcost = mapnode_list[(new_y)*width + (new_x)].gcost + hcost[new_y*width + new_x];
                                  mapnode_list[(new_y)*width + (new_x)].nodetype=0; //setting next node open in closedlist

                                    openlist.push(mapnode_list[new_y*width + (new_x)]);

                                  if(deltax==1&&deltay==0){actionlist[(new_y)*width + (new_x)]=1;}
                                  if(deltax==1&&deltay==-1){actionlist[(new_y)*width + (new_x)]=2;}
                                  if(deltax==0&&deltay==-1){actionlist[(new_y)*width + (new_x)]=3;}
                                  if(deltax==-1&&deltay==-1){actionlist[(new_y)*width + (new_x)]=4;}
                                  if(deltax==-1&&deltay==0){actionlist[(new_y)*width + (new_x)]=5;}
                                  if(deltax==-1&&deltay==1){actionlist[(new_y)*width + (new_x)]=6;}
                                  if(deltax==0&&deltay==1){actionlist[(new_y)*width + (new_x)]=7;}
                                  if(deltax==1&&deltay==1){actionlist[(new_y)*width + (new_x)]=8;}

                                      }

                                                                            }

                                                                      }
                            }
                                              }
         std::cout<<"Run3_mid";
            //to cal path
           if(path_found==true){
          bool stopfound = false;
          int currentpointx = stop_xpos;
          int currentpointy = stop_ypos;
          fullpathx.push_front (currentpointx);
          fullpathy.push_front (currentpointy);
          while(stopfound == false){
                                       if(actionlist[currentpointy*width + currentpointx] == 1){currentpointx = currentpointx - 1;
                                                                                               currentpointy = currentpointy - 0;
                                                                                               
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy);
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy); }
                                       if(actionlist[currentpointy*width + currentpointx] == 2){currentpointx = currentpointx - 1;
                                                                                               currentpointy = currentpointy + 1;
                                                                                               
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy);
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);     }
                                       if(actionlist[currentpointy*width + currentpointx] ==3){currentpointx = currentpointx - 0;
                                                                                               currentpointy = currentpointy + 1;
                                                                                               
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy);     }
                                       if(actionlist[currentpointy*width + currentpointx] == 4){currentpointx = currentpointx + 1;
                                                                                               currentpointy = currentpointy + 1;
                                                                                               
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy);
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);     }
                                       if(actionlist[currentpointy*width + currentpointx] == 5){currentpointx = currentpointx + 1;
                                                                                               currentpointy = currentpointy - 0;
                                                                                               
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy);
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);     }

                                       if(actionlist[currentpointy*width + currentpointx] == 6){currentpointx = currentpointx + 1;
                                                                                               currentpointy = currentpointy - 1;
                                                                                               
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy);
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);     }
                                       if(actionlist[currentpointy*width + currentpointx] == 7){currentpointx = currentpointx - 0;
                                                                                               currentpointy = currentpointy - 1;
                                                                                               
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy); 
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);     }
                                       if(actionlist[currentpointy*width + currentpointx] == 8){currentpointx = currentpointx - 1;
                                                                                               currentpointy = currentpointy - 1;
                                                                                              
                                                                                               fullpathx.push_front (currentpointx);
                                                                                               fullpathy.push_front (currentpointy); 
                                                                                               sm_pathx.push_front (currentpointx);
                                                                                               sm_pathy.push_front (currentpointy);    }

                                      if (actionlist[currentpointy*width + currentpointx]==0){stopfound = true;}


                                 }
                             }

                  /*  //cout action list
        for (int i=(height-1);i>=0;i--){
	     	for (int j=0;j<width;j++){
                    cout<<actionlist[i*width + j]<<" , ";
                                    }      cout<<endl;
		                        }*/

                      delete[] mapnode_list; 
                      delete[] actionlist;
                      std::cout<<"Run3";
}


void path_smooth(){  std::cout<<"Run5";
	                  alpha = float(alpha100)/100;
	                  beta = float(beta100)/100;

                      double change = tolreance;
                      bool noobs = true;

			std::list<int>::iterator ity=fullpathy.begin();
			std::list<int>::iterator itx=fullpathx.begin();
			std::list<double>::iterator sity=sm_pathy.begin();
			std::list<double>::iterator sitx=sm_pathx.begin();
    
   int str_xpt = *itx;
   for (int i=0;i<15;i++){ itx++; ity++; }

   int stop_xpt = *itx;
   int stop_ypt = *ity;

    sitx++;
    sity++;

   if(str_xpt == stop_xpt){ for (int i=0;i<15;i++){ 
   	                                                if (map_data[i*width+stop_xpt] > 0 ){noobs=false;}	 
                                                  } 
                           if(noobs==true){*sitx = stop_xpt;
                                           *sity = stop_ypt;}
                           }
   else if(stop_ypt == 0){ for (int i=0;i<15;i++){ 
  	                                                if (map_data[i+stop_xpt] > 0 ){noobs=false;}	 
                                                  } 
                           if(noobs==true){*sitx = stop_xpt;
                                           *sity = stop_ypt;}

                     }  

    else{
    float slop_new = fabs(float(stop_ypt)/(float(stop_xpt)-float(str_xpt)));
    if(str_xpt<stop_xpt){
    for (int i = str_xpt; i <= stop_xpt; i++){
                           if (map_data[int((slop_new*(float(i)-float(str_xpt)))*width+i)] > 0 ){noobs=false;}	
                }
                          if(noobs==true){*sitx = stop_xpt;
                                          *sity = stop_ypt;}

                         }

    if(str_xpt>stop_xpt){
    for (int i = str_xpt; i >= stop_xpt; i--){
                           if (map_data[int((slop_new*(-float(i)+float(str_xpt)))*width+i)] > 0 ){noobs=false;}	
                }
    
    	                  if(noobs==true){*sitx = stop_xpt;
                                          *sity = stop_ypt;}
                         }                
        }
std::cout<<"Run5_mid";
if(noobs==false){
   while(change>=tolreance){


       ity=fullpathy.begin();
       itx=fullpathx.begin();
       sity=sm_pathy.begin();
       sitx=sm_pathx.begin();
            itx++;
            ity++;
            sitx++;
            sity++;


       	change = 0;

            for (int i=1;i<(sm_pathx.size()/4)-1;i++){   //how much to smooth can be changed      
                    
                           aux = *sitx;
                           sitx--;
                           temp0 = *sitx;
                           sitx++;
                           sitx++;
                           temp1 = *sitx;
                           sitx--;
                           *sitx += alpha * (double(*itx) - *sitx);
                           *sitx += beta * (temp0+temp1-(2*(*sitx)));
                           change += abs(aux-*sitx);

                           aux = *sity;
                           sity--;
                           temp0 = *sity;
                           sity++;
                           sity++;
                           temp1 = *sity;
                           sity--;
                           *sity += alpha * (double(*ity) - *sity);
                           *sity += beta * (temp0+temp1-(2*(*sity)));
                           change += abs(aux-*sity);

                        itx++;
                        ity++;
                        sitx++;
                        sity++;

                                                     }
                     }
                 }
                 std::cout<<"Run5";
  }

      

        



                           

               

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

float  *map_data   = NULL;

int w1 = 10;
int w2 = 0;
int w3 =35;

bool map_received(false), goal_set(false), currhead_received(false);
bool new_path_required(false), newmap_set(false);


struct node
{
  int key_value;
  node *left=nullptr;
  node *right=nullptr;
  int f=0,g=0,h=0;

};

class btree
{
    public:
        btree();
        ~btree();

        void insert(node key);
        bool searchbool(int key_val,node searchnode);
        node *search_return_node(int key_val,node root);
        void destroy_tree();
        void parent_reorient(node curr, node parent );
        node *minnode(btree tree);
    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        node *root;
};

node *btree::minnode(node *tree)
{
  if(tree.left==nullptr&&tree.right==nullptr)
    return tree;
  else
  {
    if(tree.left!=nullptr)
    {
      return minnode(tree.left);
      else 
      {
        if(tree.right!=nullptr)
        {
          return minnode(tree.right);

        }

      }
    }
  }

}

btree::btree()
{
  root=NULL;
}

void btree::destroy_tree(node *leaf)
{
  if(leaf!=NULL)
  {
    destroy_tree(leaf->left);
    destroy_tree(leaf->right);
    delete leaf;
  }
}


void btree::insert(node *leaf)
{
  if(key< leaf->key_value)
  {
    if(leaf->left!=NULL)
     insert(key, leaf->left);
    else
    {
      leaf->left=new node;
      leaf->left->key_value=key;
      leaf->left->left=NULL;    //Sets the left child of the child node to null
      leaf->left->right=NULL;   //Sets the right child of the child node to null
    }  
  }
  else if(key>=leaf->key_value)
  {
    if(leaf->right!=NULL)
      insert(key, leaf->right);
    else
    {
      leaf->right=new node;
      leaf->right->key_value=key;
      leaf->right->left=NULL;  //Sets the left child of the child node to null
      leaf->right->right=NULL; //Sets the right child of the child node to null
    }
  }
}


void btree::insert(int key)
{
  if(root!=NULL)
    insert(key, root);
  else
  {
    root=new node;
    root->key_value=key;
    root->left=NULL;
    root->right=NULL;
  }
}

bool btree::searchbool(int key_val,node *searchnode)
{
  if(key_val==searchnode.key_value)
  {
    return true;
  }
  else
  {
    if(searchnode.right=searchnode.left==nullptr)
    {
      return false;
    }
      else 
      {
        if(key_value>searchnode.key_value)
        {
          if(searchnode.right!=nullptr)
          {
            node* temp =searchnode.right;
            return searchbool(key_val,temp);
          }
        }
        else 
        {
          if(searchnode.left!=nullptr)
          {
            node* tmep= searchnode.left;
            return searchbool(key_val,temp);
          }
        }

      }       
  }
}

node *btree::search_return_node(int key_val,node root)
{
  if(key_val==root.key_value)
  {
    return root;
  }
  else
  {
    if(root.right=root.left==nullptr)
    {
      return root;
    }
      else 
      {
        if(key_value>root.key_value)
        {
          if(root.right!=nullptr)
          {
            node* temp =root.right;
            return search_return_node(key_val,temp);
          }
        }
        else 
        {
          if(root.left!=nullptr)
          {
            node* tmep= root.left;
            return search_return_node(key_val,temp);
          }
        }

      }       
  }
}


void btree::destroy_tree()
{
  destroy_tree(root);
}


node *current_node;
current_node->key_value=curr_xpos+curr_ypos*width;

btree opentree,closedtree;

int openlist_memcount=0,closedlist_memcount=0;

for(int i=0;i<height*width;i++)
{
  if(map_data[i]!=0)
  {
    node temp;
    temp.key_value=i;
    closedtree.insert(temp);

  }
  
}

float astar_w1=0,astar_w2=0;

void astar(int x,int y,int goal)
{
 
  node temp;  
  temp.key_value=curr_xpos+curr_ypos*width;
  
  closedtree.insert(current_node);

    
  int t=0;

  for(int i=-1;i<=1;i++)
  {
    for(int j=-1;j<=1;j++)
    {
      if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
      { 
        node *tempo;

        tempo.keyval=curr_xpos+curr_ypos*width;curr_xpos+curr_ypos*width;
        tempo.g =temp.g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ; // add distance here 
        tempo.h = astar_w2*( abs(curr_xpos+i-x,2) + abs(curr_ypos+j-y,2) ); // distance from sucesso 
        tempo.f=tempo.g+tempo.h;
        node searchresopen = opentree.search_res(tempo.keyval);
        node searchresclosed = closedtree.search_res(tempo.keyval);

          

        if(opentree.search(tempo)&&searchresopen.f<tempo.f)
          goto ifexit; // add jump here
        if(closedtree.search(tempo)&&searresclosed.f<tempo.f)
          goto ifexit; // add jump here
        tempo.parent=&current_node;     
        opentree.insert(tempo);

        ifexit:;
      }
    }
  } 
  
  current_node=opentree.minnode();
  if(opentree.min()==nullptr)
    break
  else
  astar(current_node.keyval%width, current_node_keyval-current.keyval%width,goal);            
}

/*









*/



pair<float,float> convToCart(int i,float r)
{

   float ang = i*0.25*(3.14/180);
   float x = r*cos(ang);
   float y = -1*r*sin(ang);
   return make_pair(x,y);
}


void set_goal(const a_star::waypoint_data::ConstPtr& waypointdata)  // change function arguments accordingly
{
   std::cout<<"Run1";

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
     set_goal();     //    <----------- check this 
     astar();        //    <------------ add arg here too 
    route_list_generate();   //    < add definition of this function 


     loop_rate.sleep(); 
   }
   return 0; 
}

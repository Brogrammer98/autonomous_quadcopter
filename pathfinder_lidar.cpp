f#include <unistd.h>-
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <deque>

#include "a_star/waypoint_data.h"
#include "a_star/pathdata_array.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <time.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>

using namespace std;
using namespace cv;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;

float currentgpsNS=0;
float currentgpsEW=0;

float goalgpsEW=0; // edit these before burning the code 
float goalgpsNS=0; // edit these before burning the code 


int    width      = 500;
int    height     = 500;

float Re=6400000;
float distance_to_plan_real =10;
float distance_to_plan=distance_to_plan_real*20;

int map_data[width*height];

bool rotate_quad=false;

float curr_head=0;
bool currhead_received=false;


bool map_received=false;

int startxpos=0;
int startypos=0;

int stopxpos=0;
int stopypos=0;

float des_head=0;

struct node
{
  int64_t key_value;
  int route_parent_key=0;
  float f=0,g=0,h=0;
  node *tree_parent=nullptr;
  node *left=nullptr;
  node *right=nullptr;
node()
  {
    left=nullptr;
    right=nullptr;
    tree_parent=nullptr;

    route_parent_key=0;
  }
~node()
  {

  }
};

class btree
{
    public:
          btree();
          void insert_keyval(int key,float fval,float gval,float hval,node *rot);
          void insert_keyval(int key,float fval,float gval,float hval,int rp,node *rot);
          void destroy_leaf_keyval(int key , node *rot);
          void insert_f(int key,float fval,float gval,float hval,node *rot);
          void insert_f(int key,float fval,float x,float gval,float hval,node *rot);
          void destroy_leaf_f(int key,float fval , node *rot);
          void destroy_whole(node *rot);
          float return_f(int key,node *rot);
          bool search_bool(int key,node *rot);
          bool search_keyval_fval_comp(int key,float fval,node *rot);
          node *search_node(int key,node *rot);
          node *minnode(node *rot);
          node *root;
};

btree::btree()
{
  root=(node*) malloc(sizeof(node));
  root=nullptr;
}

float btree::return_f(int key,node *rot)
{
  if(key==rot->key_value)
    return rot->f;
  else
  {
    if(key>rot->key_value)
      return search_bool(key,rot->right);
    else
      return search_bool(key,rot->left);
  }
}

node *btree::minnode(node *rot)
{
  if(rot->left==nullptr)
    return rot;
  else
    return minnode(rot->left);
}
void btree::insert_keyval(int key,float fval,float gval,float hval,int rp,node *rot)
{

  if(rot==nullptr)
  {
      rot= new node;
      rot->key_value=key;
      rot->f=fval;
      rot->g=gval;
      rot->h=hval;
      rot->route_parent_key=rp;
      return;
  }
 else
   {

    if(key<rot->key_value)
        {
          if(rot->left!=nullptr)
            {
              insert_keyval(key,fval,gval,hval, rp,rot->left);
              return;
            }
          else
            {

              rot->left=new node;
                rot->left->key_value=key;
                rot->left->f=fval;
                rot->left->g=gval;
                rot->left->h=hval;
                rot->left->route_parent_key=rp;
                rot->left->tree_parent=rot;
                return;
              }
        }

      if(key>rot->key_value)
      {
          if(rot->right!=nullptr)
            {
                insert_keyval(key,fval,gval,hval, rp, rot->right);
                return;
            }
          else
            {
              rot->right=new node;
                rot->right->key_value=key;
                rot->right->f=fval;
                rot->right->g=gval;
                rot->right->h=hval;
                rot->right->route_parent_key=rp;
                rot->right->tree_parent=rot;
                return;
            }
        }

      if(key==rot->key_value)
        {
        rot->f=fval;
          rot->g=gval;
          rot->h=hval;
          rot->route_parent_key=rp;
          return;
        }
    }
}

void btree::insert_keyval(int key,float fval,float gval,float hval,node *rot)
{
 if(rot==nullptr)
 {
   rot= new node;
   rot->key_value=key;
   rot->f=fval;
   rot->g=gval;
   rot->h=hval;
   return;
 }
 else
 {
   if(key<rot->key_value)
    {
     if(rot->left!=nullptr)
      {
       insert_keyval(key,fval,gval,hval, rot->left);
       return;
      }
     else
     { // cout<<"an insertion   ("<<key%width<<","<<key/width<<")"<<endl;
       rot->left=new node;
       rot->left->key_value=key;
       rot->left->f=fval;
       rot->left->g=gval;
       rot->left->h=hval;
       rot->left->tree_parent=rot;
       return;
      }
    }
   if(key>rot->key_value)
    {
     if(rot->right!=nullptr)
      {
       insert_keyval(key,fval,gval,hval, rot->right);
       return;
      }
     else
      {//cout<<"an insertion   ("<<key%width<<","<<key/width<<")"<<endl;
       rot->right=new node;
       rot->right->key_value=key;
       rot->right->f=fval;
       rot->right->g=gval;
       rot->right->h=hval;
       rot->right->tree_parent=rot;
       return;
      }
    }

    if(key==rot->key_value)
    {//cout<<"an insertion   ("<<key%width<<","<<key/width<<")"<<endl;
    rot->f=fval;
     rot->g=gval;
     rot->h=hval;
     return;
    }
  }
}

void btree::insert_f(int key,float fval,float gval,float hval,node *rot)
{
 if(rot==nullptr)
  {
   rot= new node;
   rot->key_value=key;
   rot->f=fval;
   rot->g=gval;
   rot->h=hval;
   return;
  }
 else
  {
   if(fval<rot->f)
    {
     if(rot->left!=nullptr)
      {
       insert_f(key,fval,gval,hval,rot->left);
       return;
      }
     else
      {
       rot->left=new node;
       rot->left->key_value=key;
       rot->left->f=fval;
       rot->left->g=gval;
       rot->left->h=hval;
       rot->left->tree_parent=rot;
       rot->left->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval>rot->f)
    {
     if(rot->right!=nullptr)
      {
       insert_f(key,fval,gval,hval, rot->right);
       return;
      }
     else
      {
       rot->right=new node;
       rot->right->key_value=key;
       rot->right->f=fval;
       rot->right->g=gval;
       rot->right->h=hval;
       rot->right->tree_parent=rot;
       rot->right->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval==rot->f)
     if(key!=rot->key_value)
     insert_f(key,fval,gval,hval,rot->right);
     else
     {  rot->f=fval;
        rot->g=gval;
        rot->h=hval;
     }
  }
}

void btree::insert_f(int key,float fval,float x,float gval,float hval,node *rot)
{
 if(rot==nullptr)
  {
   rot= new node;
   rot->key_value=key;
   rot->f=fval;
   rot->g=gval;
   rot->h=hval;
   return;
  }
 else
  {
   if(fval<rot->f)
    {
     if(rot->left!=nullptr)
      {
       insert_f(key,fval,gval,hval,rot->left);
       return;
      }
     else
      {
       rot->left=new node;
       rot->left->key_value=key;
       rot->left->f=fval;
       rot->left->g=gval;
       rot->left->h=hval;
       rot->left->tree_parent=rot;
       rot->left->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval>rot->f)
    {
     if(rot->right!=nullptr)
      {
       insert_f(key,fval,gval,hval, rot->right);
       return;
      }
     else
      {
       rot->right=new node;
       rot->right->key_value=key;
       rot->right->f=fval;
       rot->right->g=gval;
       rot->right->h=hval;
       rot->right->tree_parent=rot;
       rot->right->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval==rot->f)
    {
     if(key!=rot->key_value)
     insert_f(key,fval,gval,hval,rot->right);
     else
     {  rot->f=x;
        rot->g=gval;
        rot->h=hval;
     }
    }
  }
}

bool btree::search_bool(int key,node *rot)
{
  if(key==rot->key_value)
  return true;

  else
  {
    if(rot->right==nullptr&&rot->left==nullptr)
      return false;
   else
   {
      if(key>rot->key_value)
      {
        if(rot->right!=nullptr)
          return search_bool(key,rot->right);
        else
          return false;
      }
      else
      {
        if(rot->left!=nullptr)
        return search_bool(key,rot->left);
        else
        {
          return false;
        }
      }
    }
  }
}

node *btree::search_node(int key,node *rot)
{
 if(key==rot->key_value)
  return rot;
 else
  {
   if(rot->right==nullptr&&rot->left==nullptr)
     return nullptr;
   else
    {
     if(key>rot->key_value)
      {
       if(rot->right!=nullptr)
        return search_node(key,rot->right);
       else
        return nullptr;
      }
     else
      {
       if(rot->left!=nullptr)
        return search_node(key,rot->left);
       else
        return nullptr;
      }
    }
  }
}

void btree::destroy_leaf_keyval(int key , node *rot)
{
 if(key<rot->key_value)
  {
   if(rot->left!=nullptr)
    {
     destroy_leaf_keyval(key,rot->left);
     return;
    }
 else
  return;
  }

 else
  {
   if(key>rot->key_value)
    {
     if(rot->right!=nullptr)
      {
       destroy_leaf_keyval(key,rot->right);
       return;
      }
     else
      return;
    }
  }

 if(( key==rot->key_value ))
  {//cout<<"exec del"<<endl;
   node *seed;
   seed=new node;
   if(!rot->right)
    goto jumpo;

    seed=minnode(rot->right);
    rot->f=seed->f;
    rot->key_value=seed->key_value;
    rot->g=seed->g;
    rot->h=seed->h;
    destroy_leaf_keyval(seed->key_value,rot->right);

    goto pop;

    jumpo:
    if(rot->tree_parent->right==rot&&rot->tree_parent!=nullptr)
    rot->tree_parent->right=nullptr;
    if(rot->tree_parent->left==rot&&rot->tree_parent!=nullptr)
    rot->tree_parent->left=nullptr;
    pop:
    return;
  }
}


void btree::destroy_leaf_f(int key,float fval , node *rot)
{
 if(fval<rot->f)
  {
   if(rot->left!=nullptr)
    {
     destroy_leaf_f(key,fval,rot->left);
     return;
    }
   else
    return;
  }

 else
  {
   if(fval>rot->f)
    {
     if(rot->right!=nullptr)
     {
       destroy_leaf_f(key,fval,rot->right);
      return;
     }
     else
      return;
    }
  }
 if(( fval==rot->f ))
  {
    if(key!=rot->key_value)
    {
     destroy_leaf_f(key,fval,rot->right);
     return;
    }
   else
    {
     if(( key==rot->key_value ))
      {
       node *seed;
       seed=new node;

       if(!rot->right)
       goto jumpo;

       seed=minnode(rot->right);
       rot->f=seed->f;
       rot->key_value=seed->key_value;
       rot->g=seed->g;
       rot->h=seed->h;

       destroy_leaf_f(seed->key_value,seed->f,rot->right);
       goto pop;
       jumpo:
       if((rot->tree_parent->right==rot)&&rot->tree_parent!=nullptr)
        rot->tree_parent->right=nullptr;
       if((rot->tree_parent->left==rot)&&rot->tree_parent!=nullptr)
        rot->tree_parent->left=nullptr;
       pop:
       return;
      }
    }
  }
}

bool btree::search_keyval_fval_comp(int key,float fval, node *rot)
{
 if(key==rot->key_value)
  {
   if(fval>=rot->f)
    return false;
   else
    return true;
  }
 else
  {
   if(rot->right==nullptr&&rot->left==nullptr)
    return true;
   else
    {
     if(key>rot->key_value)
      {
       if(rot->right!=nullptr)
        return search_keyval_fval_comp(key,fval,rot->right);
       else
        return true;
      }
     else
      {
       if(rot->left!=nullptr)
        return search_keyval_fval_comp(key,fval,rot->left);
       else
        return true;
      }
    }
  }
}

void btree::destroy_whole(node *rot)
{
  if(rot->left)
    destroy_whole(rot->left);
  if(rot->right)
    destroy_whole(rot->right);
  if(!rot->left&&!rot->right)
  {
    if(!rot->tree_parent)
      rot=nullptr;
    else
    {
      if(rot==rot->tree_parent->left)
      rot->tree_parent->left=nullptr;
      if(rot==rot->tree_parent->right)
      rot->tree_parent->right=nullptr;
    }
  }
}

void astar(node *current_node,int goal,btree &opentree_f,btree &opentree_keyval,btree &closedtree,btree &master_tree)
{ 
  int curr_xpos=(int)current_node->key_value%width;
  int curr_ypos=(int)(current_node->key_value-curr_xpos)/width;
  int t=current_node->key_value;
  
  if(current_node->key_value==goal)
  {
    cout<<"goal has been found ";
    return;
  }
  else
  {
    for(int i=-1;i<=1;i++)
    {
      for(int j=-1;j<=1;j++)
      { 
        int key;
        float fl,gl,hl;
        int x,y,goalx,goaly;
        
        key=t+i+width*j;
        goalx=goal%width;
        goaly=goal/width;
        
        x=(int)t%width +i;
        y=(int)t/width +j;
        
        gl =i*i +j*j;

        hl =sqrt(pow(goalx-x,2) + pow(goaly-y,2));
        fl =0.01*gl+hl;

        if(x>=0&&y>=0&&x<width&&y<height&&key!=t)
        {
          if(opentree_keyval.search_keyval_fval_comp(key,fl,opentree_keyval.root)&&!closedtree.search_bool(key,closedtree.root))
          {
            if( opentree_keyval.search_bool(key,opentree_keyval.root) )
            {
              float x;
              x=opentree_keyval.return_f(key,opentree_keyval.root);
              opentree_f.insert_f(key,x,fl,gl,hl,opentree_f.root);
              opentree_keyval.insert_keyval(key,fl,gl,hl,opentree_keyval.root);
              master_tree.insert_keyval(key,fl,gl,hl,current_node->key_value,master_tree.root);
            }
            else
            {
              opentree_f.insert_f(key,fl,gl,hl,opentree_f.root);
              master_tree.insert_keyval(key,fl,gl,hl,current_node->key_value,master_tree.root);
              opentree_keyval.insert_keyval(key,fl,gl,hl,opentree_keyval.root);
            }
          }
        }
      }
    }
    closedtree.insert_keyval(current_node->key_value,current_node->f,current_node->g,current_node->h,closedtree.root);
    opentree_keyval.destroy_leaf_keyval(current_node->key_value,opentree_keyval.root);
    opentree_f.destroy_leaf_f(current_node->key_value,current_node->f,opentree_f.root);
    node *temps;
    temps=new node;
    temps=opentree_f.minnode(opentree_f.root);
 
    if(!temps)
      return;
    
    else
    {
      astar(temps,goal,opentree_f,opentree_keyval,closedtree,master_tree);
      return;
    }
  }
}

void set_goal(const a_star::waypoint_data::ConstPtr& waypointdata)  // <----- add some aruments here    optimise this by exact local destination within lical masomp
{  
   
  des_head = waypointdata->angle;

  if (map_received == true && currhead_received == true && goal_set == false) // <- how or where to assign these boolean values ?
  {
    if(distance_to_plan<=1.41*height*map_res)
    { 
      stop_ypos = int((distance_to_plan*cos((curr_head-des_head)*DEG_TO_RAD))/map_res);
      stop_xpos = int((distance_to_plan*sin((curr_head-des_head)*DEG_TO_RAD))/map_res)+(int)width/2;

      if (stop_ypos>=height)
        stop_ypos = height - 1;  

      if(stop_xpos>=width)
        stop_xpos = width - 1;   

      if(stop_ypos<0)
      {
        rotate_quad=true;
        goal_set=false;
      } 
    }
    goal_set=true;  
  }
 
  else 
  {
    if(abs((curr_head-des_head))>atan(0.5))
    {
      stop_ypos=(int)(width/2)*cot((curr_head-des_head)*DEG_TO_RAD);
      stop_xpos=width-1;

      else 
      {
        stop_ypos=height-1;
        stop_xpos=(int)(height)*cot((curr_head-des_head)*DEG_TO_RAD) +(int)width/2;
      }
    }

    if (map_data[stop_ypos*width + stop_xpos] != 0)   
    { 
      int flag=0;
      for(int i=0;i<500;i++)
      {
        for(int k=-i;k<=i;k++)
        { 
          for(int j=-i;j<=i;j++)
          {
            if((stop_ypos+j >=0)&&(stop_ypos+j <height)&&((stop_xpos+k)<width)&&((stop_xpos+k)>=0))
            {
              if(map_data[(stop_ypos+j)*width + (stop_xpos+k)] = 0) 
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

float magz,magx,magy;

void current_dir(const sensor_msgs::MagneticField::ConstPtr& mag)
{
  magx=mag->magnetic_field.x;  
  magy=mag->magnetic_field.y;
  magz=mag->magnetic_field.z;

  curr_head = atan(magz/magx);
  currhead_received = true;
}

void current_pos(const sensor_msgs::NavSatFix::ConstPtr& info)
{
  currentgpsEW=info->latitude;
  currentgpsNS=info->longitude;
}


float *returngpsNS(int vec[],int size) // the trigonometric funtion to return 
{
  float beta=0.05*1/6400000;
  float beatap=beta/cos(current_heading);

  float ns[size];

  for(int i=0;i<size;i++)
  {
    int xa,ya;
    xa=vec[i]%width -curr_xpos;
    ya=vec[i]/width; 
    ns[i]=currentgpsNS +beta*(-1*xa*sin(current_heading)+ya*cos(current_heading));
  }  
  return ns;
}

float *returngpsEW(int vec[],int size)
{
  float beta=0.05*1/6400000;
  float beatap=beta/cos(current_heading);

  float ns[size];

  for(int i=0l i<size;i++ )
  {
    int xa,ya;
    xa=vec[i]%width -curr_xpos;
    ya=vec[i]/width; 
    ns[i]=currentgpsEW +betap*(ya*sin(current_heading)+xa*cos(current_heading));
  }  
  return ns;
}


void map_to_plan(const nav_msgs::OccupancyGrid::ConstPtr& map_in)
{  
  if (map_received == false)
  {
    width    = map_in->info.width;
    height   = map_in->info.height;
    map_res  = map_in->info.resolution;
    
    startxpos = (width/2 - 1);
    
    for (int i=0;i<(width*height);i++)
    {
      if (map_in->data[i] >0)
        map_data[i] = 1;
      else map_data[i] = 0;
    }
      map_received = true;
  }
}

void copy_cordinates(const std_msgs::Float64::ConstPtr& array,float *ar)
{
  array=ar[0];
}

bool initiate=false;

void initiate_astar(const std_msgs::Bool::ConstPtr& call)
{
	initiate=call->data;
}


float long_err=0,lat_err=0; // please set some threshold here 

int goal=stop_xpos+stop_ypos*width;

int main(int argc, char** argv)
{
  std_msgs::Bool final_goal_found;
  final_goal_found.data=false;

  btree opentree_f,closedtree,opentree_keyval,master_tree;
   
  node first,faker,faker2,faker3; // initialization nodes for all the trees

   
  first.key_value=width-1;
  faker.key_value=width/2;
  faker2.key_value=width/2;   // make necessary corrections here according to origin references
  faker3.key_value=width/2;
  
  opentree_f.root=&faker;
  opentree_keyval.root=&faker2;
  master_tree.root=&faker3;
  closedtree.root=&first;


  node cur;
  cur.key_value=width/2;
  
  ros::init(argc, argv, "astar");
  ros::NodeHandle n;

  ros::Subscriber sub2 = n.subscribe("/mavros/imu/data", 10, current_loc);
  ros::Subscriber sub3 = n.subscribe("/scan/blown_local_map", 10, map_to_plan); // from yashwant code 
  ros::Subscriber sub  = n.subscribe("/mavros/global_position/raw/fix", 1, current_pos); // check what must be the buffer size 
  ros::Subscriber subpos =n.subscribe("mavros/imu/mag",10,current_dir);
  ros::Subscriber boolros =n.subscribe("bool_pbsh",10,initiate_astar);

  distance_to_plan_real =Re*sqtr( pow(cos(currentgpsNS)*currentgpsEW,2) + pow(currentgpsEW-goalgpsEW ,2) );
  distance_to_plan=20*distance_to_plan_real;

  ros::Subscriber sub4 = n.subscribe("/waypoint_data", 10, set_goal);
  
  ros::Publisher pub = n.advertise<std_msgs::Float64>("lat_wp", 100);
  ros::Publisher pub2 = n.advertise<std_msgs::Float64>("lng_wp", 100);
  //ros::Publisher pub3 =n.advertise<std_msgs::Bool>("rotate_quad", 100);
  ros::Rate loop_rate(10);

  while(ros::ok() && !final_goal_found.data)
  {
    if(new_path_required == true && goal_set == true && map_received == true && initiate )
      astar(&cur,stop_xpos+stop_ypos*width,opentree_f,opentree_keyval,closedtree,master_tree);//   <=add relevant arguments here 
    
    else 
      goto loop_end;		

    int pathvec[],size=0,curpos;
    node *us;
    us=new node;
    us=master_tree.search_node(goalx+goaly*width,master_tree.root);
    curpos=us->route_parent_key;

    while(curpos!=stop_xpos+stop_ypos*width)
    {
      curpos=us->route_parent_key;
      pathvec[++size]=us->key_value;
      us=master_tree.search_node(curpos,master_tree.root);
    }

    int pathvecfliped[size];

    for(int i=0;i<size;i++)
      pathvecfliped[i]=pathvec[size-1-i];

    float goalgpsNS=returngpsNS(pathvecfliped,size);
    float goalgpsEW=returngpsEW(pathvecfliped,size);

    std_msgs::Float64 gps_vector_ew;
    std_msgs::Float64 gps_vector_ns;
    std_msgs::Bool rotate_quad_msgs;    
  
    copy_cordinates(gps_vector_ns,goalgpsNS);
    copy_cordinates(gps_vector_ew,goalgpsEW);
    rotate_quad_msgs=rotate_quad;

    pub.publish(gps_vector_ns);
    pub2.publish(gps_vector_ew);
    pub3.publish(rotate_quad_msgs);

    

    opentree_keyval.destroy_whole(opentree_keyval.root);
    opentree_f.destroy_whole(opentree_f.root);
    master_tree.destroy_whole(master_tree.root);
    closedtree.destroy_whole(closedtree.root);  
    if(abs(goalgpsEW-currentgpsEW)<long_err && abs(goalgpsNS-currentgpsNS)<lat_err)
    {
    	final_goal_found.data=true;
    }

    loop_end:
    map_received = false;
    goal_set = false;
    rotate_quad=false;  

    ros::spinOnce();
    loop_rate.sleep();
  }
}


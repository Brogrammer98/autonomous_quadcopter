#include <unistd.h>-
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;
float currentgpsNS=0;
float currentgpsEW=0;
float goalgpsEW=0;
float goalgpsNS=0;

int    width      = 500;
int    height     = 500;
float Re=6400000;
float distance_to_plan_real =10;
float distance_to_plan=distance_to_plan_real*20;

int map_data[250000];

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
`
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

void astar(node *current_node,int goal,btree &opentree_f,btree &opentree_keyval,btree &closedtree,btree &master_tree)
{   //usleep(100);
    int curr_xpos=(int)current_node->key_value%width;
    int curr_ypos=(int)(current_node->key_value-curr_xpos)/width;
    int t=current_node->key_value;
   // cout<<"*("<<curr_xpos<<","<<curr_ypos<<")    "<<current_node->f<<endl<<endl;

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
                { int key;
                  float fl,gl,hl;
                  int x,y,goalx,goaly;
                  key=t+i+width*j;
                  goalx=goal%width;
                  goaly=goal/width;
                  x=(int)t%width +i;
                  y=(int)t/width +j;

                  gl =i*i +j*j;// distance from sucesso
                  hl =sqrt(pow(goalx-x,2) + pow(goaly-y,2));
                  fl =0.01*gl+hl;
                  if(x>=0&&y>=0&&x<width&&y<height&&key!=t)
                   {
                   if(opentree_keyval.search_keyval_fval_comp(key,fl,opentree_keyval.root)&&!closedtree.search_bool(key,closedtree.root))
                    {//cout<<"working on   ("<<key%width<<","<<key/width<<")"<<endl;
                     if( opentree_keyval.search_bool(key,opentree_keyval.root) )
                      {
                 //      cout<<"gotcha   ("<<key%width<<","<<key/width<<")"<<endl;
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
   //cout<<"--------------------------------"<<endl;
   node *temps;
   temps=new node;
   temps=opentree_f.minnode(opentree_f.root);
   if(!temps)
    {
      return;
    }
    else
    {
     astar(temps,goal,opentree_f,opentree_keyval,closedtree,master_tree);
     return;
    }
   }
}
bool rotate_quad=false;

void set_goal(const a_star::waypoint_data::ConstPtr& waypointdata)  // <----- add some aruments here    optimise this by exact local destination within lical masomp
{  
  float des_head=0; 
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
        goalfound=false;
       } // fixed it at the top if it is outta bounds 
    }
        goalfound=true;  // fixed it at the top if it is outta bounds 
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
    { int flag=0;
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

float curr_head=0;
bool currhead_received=false;

void current_heading(const geometry_msgs::Pose2D::ConstPtr& msg)
{
  curr_head = msg->theta;
  currhead_received = true;
  currentgpsEW=msg->y;
  currentgpsNS=msg->x;
}

float* returngpsNS(int vec[],int size)
{
  float beta=0.05*1/6400000;
  float beatap=beta/cos(current_heading);

  float ns[size];

  for(int i=0l i<size;i++ )
    {
      int xa,ya;
      xa=vec[i]%width -curr_xpos;
      ya=vec[i]/width; 
      ns[i]=currentgpsNS +beta*(-1*xa*sin(current_heading)+ya*cos(current_heading));

    }  
return ns;
}
float* returngpsEW(int vec[],int size)
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



bool map_received=false;

int startxpos=0;
int startypos=0;

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

int main(int argc, char** argv)
{
  btree opentree_f,closedtree,opentree_keyval,master_tree;
  int goal=stop_xpos+stop_ypos*width;
  
  node first;
  first.key_value=100000;
  closedtree.root=&first;

  node faker,faker2,faker3;
  faker.key_value=0;
  faker2.key_value=0;   // make necessary corrections here 
  faker3.key_value=0;
  opentree_f.root=&faker;
  opentree_keyval.root=&faker2;
  master_tree.root=&faker3;
  
  ros::init(argc, argv, "astar");
  ros::NodeHandle n;

  ros::Subscriber sub2 = n.subscribe("/imu/HeadingTrue_degree", 10, current_heading);
  ros::Subscriber sub3 = n.subscribe("/scan/blown_local_map", 10, map_to_plan); // For sandeep code

  distance_to_plan_real =Re*sqtr( pow(cos(currentgpsNS)*currentgpsEW,2) + pow(currentgpsEW-goalgpsEW ,2) );
  distance_to_plan=20*distance_to_plan_real;

  ros::Subscriber sub4 = n.subscribe("/waypoint_data", 10, set_goal);
  
  ros::Publisher pub = n.advertise<std_msgs::Float64MultiArray>("NS_gps", 100);
  ros::Publisher pub2 = n.advertise<std_msgs::Float64MultiArray>("EW_gps", 100);
  
  ros::Rate loop_rate(10);

  while(ros::ok())
  {



    if (new_path_required == true && goal_set == true && map_received == true)
    {
      astar()//   <=add relevant arguments here 
      pub1.publish(path_stat);            
      pub.publish(plannedpath);
      map_received = false;
      goal_set = false;
    } 
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

    std_msgs::Float64MultiArray gps_vector_ew;
    std_msgs::Float64MultiArray gps_vector_ns;
        
   for(int i=0;i<size;i++)
   {
    gps_vector_ns.data.push_back(goalgpsNS[i]);
    gps_vector_ew.data.push_back(goalgpsEW[i]);
   }
  
  pub.publish(gps_vector_ns);
  pub1.publish(gps_vector_ew);

  ros::spinOnce();
  loop_rate.sleep();
  }
}


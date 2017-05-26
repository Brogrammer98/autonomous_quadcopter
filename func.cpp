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


float  map_data[] = NULL;

int w1 = 10;
int w2 = 0;
int w3 =35;
struct node
{
  int key_value;
  node *left=NULL;
  node *right=NULL;
  node *route_parent=NULL;
  node *tree_parent=NULL;
  int f=0,g=0,h=0;

};

class btree
{
    public:
        btree();
        ~btree();

        void insert(node *leaf, node *root);
        bool search_bool(int key_val,node searchnode);
        node *search_node(int key_val,node root);
        node *minnode(node *root);
        node *restructure_tree(int key,node *root);
        void destroy_leaf();

        node *root;
};

btree::btree()
{
  root=NULL;
}	

node *btree::minnode(node *rot=this->root)
{
	if(rot->left==NULL)
		return rot;
	else
		return minnode(rot->left);
}




void btree::insert_f(node *leaf,node *rot=this->root)
{
  if(leaf->f<rot->f)
  {
    if(rot->left!=NULL)
     insert(leaf, rot->left);
    else
    {
      rot->left=leaf;
      leaf->tree_parent=rot;
    }  
  }
  
  if(leaf->f>=rot->f)
  {
    if(rot->right!=NULL)
      insert(leaf, rot->right);
    else
    {
      rot->right=leaf;
      leaf->tree_parent=rot;
    }
  }
}

void btree::insert_keyval(node *leaf,node *rot=this->root)
{
  if(leaf->keyval<rot->keyval)
  {
    if(rot->left!=NULL)
     insert(leaf, rot->left);
    else
    {
      rot->left=leaf;
      leaf->tree_parent=rot;
    }  
  }
  
  if(leaf->keyval>rot->keyval)
  {
    if(rot->right!=NULL)
      insert(leaf, rot->right);
    else
    {
      rot->right=leaf;
      leaf->tree_parent=rot;
    }
  }

  if(leaf->keyval==rot->keyval)
  {
  	rot->f=leaf->f;
  	rot->g=leaf->g;
  	rot->h=leaf->h;
  	
  }

}



bool btree::search_bool(node leaf,node *rot=this->root)
{
	if(leaf->key_value==rot->key_value)
		return true;
	
	else
	{
		if(rot->right==rot->left==NULL)
		{
			return false;
		}
     	else 
     	{
     		if(leaf->key_value>rot->key_value)
     		{
     			if(rot->right!=NULL)
     				return search_bool(leaf,rot->right);
     			
     		}
     		else 
     		{
     			if(rot->left!=NULL)
     				return search_bool(leaf,rot->left);
     			
     		}
     	}     	
	}
}

node *btree::search_return_node(node *leaf,node *rot=this->root)
{
	if(leaf->key_value==rot->key_value)
	{
		return rot;
	}
	else
	{
		if(rot->right==rot->left==NULL)
		{
			return NULL;
		}
     	else 
     	{
     		if(leaf->key_value>rot->key_value)
     		{
     			if(rot->right!=NULL)
     			{
     				return search_node(leaf,rot->right);
     			}
     		}
     		else 
     		{
     			if(rot->left!=NULL)
     			{
     				return search_node(leaf,rot->left);
     			}
     		}
     	}     	
	}
}

/*

************************************************************************************

the following some lines are some really crazy ass lines of function please look into it 
i am assuming that the tree is not self balancing 

***********************************************************************************

*/






void btree::destroy_leaf(node *leaf , node *rot=this->root)   
{
	if(leaf->f<rot->f)
	{
		if(rot->left!=NULL)
		{
			destroy_leaf(leaf,rot->left);
			return;
		}

		else 
			return;
	}
	

	else 
	{
		if(leaf->f>rot->f)
		{
			if(rot->right!=NULL)
			{
				destroy_leaf(leaf,rot->right);
				return;
			}

			else 
			return;
        }
	
	}

	if((leaf->keyval==rot->keyval)&&(leaf->f==rot->f))
	{
		if(rot->left&&rot->right)
		{
			node *seed;
			seed=minnode(rot->right);
			rot->f=seed->f;
			rot->keyval=seed->keyval;
			rot->g=seed->g;
			rot->h=seed->h;
			destroy_leaf(seed,rot->right);
			return;
		}
		else
		{
			if(!rot->left&&!rot->right)
			{
				delete rot;
				this->root=NULL;
				return;
			}
			else
			{
				if(!rot->left&&rot->right)
				{
					if(!rot->parent)
					{
						this->root=this->root->right;
						delete rot;
						return;
					}
					if(rot==rot->tree_parent->left)
					{
						node *temps;
						temps=rot;
						rot->tree_parent->left=rot->right;
						delete temps;
						return; 	
					}
					if(rot==rot->tree_parent->right)
					{
						node *temps;
						temps=rot;
						rot->tree_parent->right=rot->right;
						delete temps;
						return;	
					}

				}
				else
				{
					if(rot==rot->tree_parent->left)
					{
						node *temps;
						temps=rot;
						rot->tree_parent->left=rot->left;
						delete temps;
						return; 	
					}
					else
					{
						node *temps;
						temps=rot;
						rot->tree_parent->right=rot->left;
						delete temps;
						return;	
					}	
				}
			}
		}
	}
}

node *current_node,*location_node;
current_node->key_value=curr_xpos+curr_ypos*width;
location_node->key_value=curr_xpos+curr_ypos*width;

btree opentree_f,closedtree,opentree_keyval;

opentree_f.root=location_node;
opentree_keyval.root=location_node;
closedtree.root=location_node;

float path_vector[height*width];
int path_vector_count=0;

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

void astar(int curr_xpos,int curr_ypos,int goal)
{
 
	node *temp;	
	temp->key_value=curr_xpos+curr_ypos*width;
	
	closedtree.insert(location_node);

    
	if(!current_node)
		return;
	else
	{

		curr_xpos=current_node->keyval%width;
		curr_ypos=(int)(current_node->keyval/width);

	for(int i=-1;i<=1;i++)
	{
		for(int j=-1;j<=1;j++)
		{
			if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
			{ 
				node *tempo,*tempo_mirror;

				tempo->keyval=curr_xpos+curr_ypos*width;
				tempo_mirror->keyval=curr_xpos+curr_ypos*width;
				
				tempo->g =temp->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ;
				tempo_mirror->g =temp->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ; // add distance here 
				
				tempo->h = astar_w2*( abs(curr_xpos+i-x,2) + abs(curr_ypos+j-y,2) ); // distance from sucesso 
				tempo_mirror->h = astar_w2*( abs(curr_xpos+i-x,2) + abs(curr_ypos+j-y,2) ); // distance from sucesso 
				
				tempo->f=tempo->g+tempo->h;
				tempo_mirror->f=tempo->g+tempo->h;
				

				node *searchresopen = opentree_keyval.search_node(tempo->keyval);
				node *searchresclosed = closedtree.search_node(tempo->keyval);

				
				if(curr_xpos+curr_ypos*width==goal)
				{
					tempo->route_parent=current_node;
					closedtree.insert(tempo);

					node *tempx;
					tempx=tempo;
					do
					{
						path_vector[path_vector_count++]=tempx->keyval;

					}while(tempx->route_parent!=NULL)	

					for(int i=0;i<path_vector_count;i++)
					{
						cout<path_vector[i]<<"   ";
					}

					return;
				}	

				if(opentree_keyval.search_bool(tempo)&& (searchresopen->f<tempo->f))
					goto ifexit; // add jump here
				if(closedtree.search_bool(tempo)&&(searchresopen->f<tempo->f))
					goto ifexit; // add jump here
				tempo->route_parent=current_node;
				tempo_mirror->route_parent=current_node;
							
				opentree_keyval.insert_keyval(tempo_mirror);
				opentree_f.insert_f(tempo);

				ifexit:;
			}
		}
	} 
	
	current_node=opentree.minnode();
	
	astar(   current_node->keyval%width, current_node->keyval-(current->keyval)%(width) ,goal  ); 				   	

    }
}

int main()
{
	for(int i=0;i<250000;i++)
	{
		map_data[i]=0;
	}

	for (int i = 0; i < 250; ++i)
	{
		map_data[i+100000]=1;
	}
	
astar(0,0,200250);

} 	 		 

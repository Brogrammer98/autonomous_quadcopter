#include <cstddef>
#include <functional>
#include <queue>
#include <vector>
#include <list>
#include <deque>

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>


using namespace std;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;

int    width      = 500;
int    height     = 500;


float  map_data[250000];

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
        void insert_f(node *leaf,node *rot);
        void insert_keyval(node *leaf,node *rot);
        bool search_bool(node *leaf,node *root);
        node *search_node(node *leaf,node *root);
        node *minnode(node *root);
        void destroy_leaf(node *leaf,node *root);

        node *root;
};

btree::btree()
{
  root=NULL;
}

node *btree::minnode(node *rot)
{
  if(rot->left==NULL)
    return rot;
  else
    return minnode(rot->left);
}




void btree::insert_f(node *leaf,node *rot)
{
  if(leaf->f<rot->f)
  {
    if(rot->left!=NULL)
     insert_f(leaf, rot->left);
    else
    {
      rot->left=leaf;
      leaf->tree_parent=rot;
    }
  }

  if(leaf->f>=rot->f)
  {
    if(rot->right!=NULL)
      insert_f(leaf, rot->right);
    else
    {
      rot->right=leaf;
      leaf->tree_parent=rot;
    }
  }
}

void btree::insert_keyval(node *leaf,node *rot)
{
  if(leaf->key_value<rot->key_value)
  {
    if(rot->left!=NULL)
     insert_keyval(leaf, rot->left);
    else
    {
      rot->left=leaf;
      leaf->tree_parent=rot;
    }
  }

  if(leaf->key_value>rot->key_value)
  {
    if(rot->right!=NULL)
      insert_keyval(leaf, rot->right);
    else
    {
      rot->right=leaf;
      leaf->tree_parent=rot;
    }
  }

  if(leaf->key_value==rot->key_value)
  {
    rot->f=leaf->f;
    rot->g=leaf->g;
    rot->h=leaf->h;

  }

}



bool btree::search_bool(node *leaf,node *rot)
{
  if(leaf->key_value==rot->key_value)
    return true;

  else
  {
    if(rot->right==NULL&&rot->left==NULL)
    {
      return false;
    }
      else
      {
        if(leaf->key_value>rot->key_value)
        {
          if(rot->right!=NULL)
            return search_bool(leaf,rot->right);
          else
                    return false;

        }
        else
        {
          if(rot->left!=NULL)
            return search_bool(leaf,rot->left);
          else
          return false;

        }
      }
  }
}

node *btree::search_node(node *leaf,node *rot)
{
  if(leaf->key_value==rot->key_value)
  {
    return rot;
  }
  else
  {
    if(rot->right==NULL&&rot->left==NULL)
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
          else
          return NULL;
        }
        else
        {
          if(rot->left!=NULL)
          {
            return search_node(leaf,rot->left);
          }
          else
          return NULL;
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








void btree::destroy_leaf(node *leaf , node *rot)
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

  if((leaf->key_value==rot->key_value)&&(leaf->f==rot->f))
  {
    if(rot->left&&rot->right)
    {
      node *seed;
      seed=minnode(rot->right);
      rot->f=seed->f;
      rot->key_value=seed->key_value;
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
          if(!rot->tree_parent)
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

btree opentree_f,closedtree,opentree_keyval;



float path_vector[250000];
int path_vector_count=0;
int i=0;



float astar_w1=0,astar_w2=0;

void astar(node *current_node,int goal)
{
    int  curr_xpos=current_node->key_value%width;
    int curr_ypos=current_node->key_value/width;

  if(!current_node)
    return;
  else
  { for(int i=-1;i<=1;i++)
      {
    for(int j=-1;j<=1;j++)
    {
      if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
      {
        node tempo,tempo_mirror;

        tempo.key_value=curr_xpos+curr_ypos*width;
        tempo_mirror.key_value=curr_xpos+curr_ypos*width;

        tempo.g =current_node->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ;
        tempo_mirror.g =current_node->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ; // add distance here

        tempo.h = astar_w2*( abs(i) + abs(j) ); // distance from sucesso
        tempo_mirror.h = astar_w2*( abs(i) + abs(j) ); // distance from sucesso

        tempo.f=tempo.g+tempo.h;
        tempo_mirror.f=tempo.g+tempo.h;


        node *searchresopen = opentree_keyval.search_node(&tempo,opentree_keyval.root);
        node *searchresclosed = closedtree.search_node(&tempo,closedtree.root);


        if(curr_xpos+curr_ypos*width==goal)
        {
          tempo.route_parent=current_node;
          closedtree.insert_keyval(&tempo,closedtree.root);


          node tempx;
          tempx=tempo;


          do
          {
            path_vector[path_vector_count++]=tempx.key_value;

          }while(tempx.route_parent!=NULL);

          for(int i=0;i<path_vector_count;i++)
          {
            cout<<path_vector[i]<<"   ";
          }

          return;
        }

        if(opentree_keyval.search_bool(&tempo,opentree_keyval.root)&& (searchresopen->f<tempo.f))
          goto ifexit; // add jump here
        if(closedtree.search_bool(&tempo,closedtree.root)&&(searchresclosed->f<tempo.f))
          goto ifexit; // add jump here
        tempo.route_parent=current_node;
        tempo_mirror.route_parent=current_node;

        opentree_keyval.insert_keyval(&tempo_mirror,opentree_keyval.root);
        opentree_f.insert_f(&tempo,opentree_f.root);

        ifexit:;
      }
    }
  }

  current_node=opentree_f.minnode(opentree_f.root);

  astar(current_node,goal);

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

int t=0;
    while(t<250000)
    {
   if(map_data[t]!=0)
    {
    node temp;
    temp.key_value=t;
    closedtree.insert_keyval(&temp,closedtree.root);
      }
     t+=1;
    }




    node curr_node;
    curr_node.key_value=0;
    opentree_f.insert_f(&curr_node,opentree_f.root);
    opentree_keyval.insert_keyval(&curr_node,opentree_keyval.root);
astar(&curr_node,0);

}
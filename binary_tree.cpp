#include <cstddef>
#include <functional>
#include <queue>
#include <vector>
#include <list>
#include <deque>
#include <console_bridge/console.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <memory.h>


int INT_MAX =9999999;

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
  int64_t key_value;
  int route_parent_key;
  int f=0,g=0,h=0;
  node *tree_parent=nullptr;
  node *left=nullptr;
  node *right=nullptr;
node()
  {
    left=nullptr;
    right=nullptr;
    tree_parent=nullptr;
    f=0;
    g=0;
    h=0;
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
          void insert_keyval(int key,int fval,int gval,int hval,node *rot);
          void insert_keyval(int key,int fval,int gval,int hval,int rp,node *rot);
          void destroy_leaf_keyval(int key , node *rot);
          void insert_f(int key,int fval,int gval,int hval,node *rot);
          void destroy_leaf_f(int key,int fval , node *rot);

          bool search_bool(int key,node *rot);
          bool search_keyval_fval_comp(int key,int fval,node *rot);
          node *search_node(int key,node *rot);
          node *minnode(node *rot);
          node *root;
};

btree::btree()
{
  root=(node*) malloc(sizeof(node));
  root=nullptr;
}

node *btree::minnode(node *rot)
{
  if(rot->left==nullptr)
    return rot;
  else
    return minnode(rot->left);
}
void btree::insert_keyval(int key,int fval,int gval,int hval,int rp,node *rot)
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

void btree::insert_keyval(int key,int fval,int gval,int hval,node *rot)
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
      {
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
      {
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
    {
     rot->f=fval;
     rot->g=gval;
     rot->h=hval;
     return;
    }
  }
}

void btree::insert_f(int key,int fval,int gval,int hval,node *rot)
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
       return;
      }
    }
   if(fval==rot->f)
    {
     insert_f(key,fval,gval,hval,rot->right);
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
    {
      return false;
    }
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
        return false;
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
    destroy_leaf_keyval(seed->key_value,rot->right);

    goto pop;

    jumpo:
    if(rot->tree_parent->right=rot)
    rot->tree_parent->right=nullptr;
    else
    rot->tree_parent->left=nullptr;
    pop:
   return;
  }
}


void btree::destroy_leaf_f(int key,int fval , node *rot)
{
cout<<endl<<"entering func"<<endl;
 if(fval<rot->f)
  {
  cout<<"chicking for left child"<<endl;
   if(rot->left!=nullptr)
    {
     cout<<"checking left child existence"<<endl;
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
      cout<<"checking for right child"<<endl;
      destroy_leaf_f(key,fval,rot->right);
      return;
     }
     else
      return;
    }
  }
 if(( fval==rot->f ))
  {
    cout<<"a match found by only fval"<<endl;
   if(key!=rot->key_value)
    {
     cout<<"going to right child as key val mismatch"<<endl;
     destroy_leaf_f(key,fval,rot->right);
     return;
    }
   else
    {
     if(( key==rot->key_value ))
      {
       cout<<"an exact hit found "<<key<<endl;
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
       if(rot->tree_parent->right=rot)
        rot->tree_parent->right=nullptr;
       else
        rot->tree_parent->left=nullptr;
       pop:
      return;
      }
    }
  }
}

bool btree::search_keyval_fval_comp(int key,int fval, node *rot)
{
 if(key==rot->key_value)
  {
   if(fval>rot->f)
    return true;
   else
    return false;
  }
 else
  {
   if(rot->right==nullptr&&rot->left==nullptr)
    return false;
   else
    {
     if(key>rot->key_value)
      {
       if(rot->right!=nullptr)
        return search_keyval_fval_comp(key,fval,rot->right);
       else
        return false;
      }
     else
      {
       if(rot->left!=nullptr)
        return search_keyval_fval_comp(key,fval,rot->left);
       else
        return false;
      }
    }
  }
}


void astar(node *current_node,int goal,btree &opentree_f,btree &opentree_keyval,btree &closedtree,btree &master_tree)
{
 int curr_xpos=current_node->key_value%width;
 int curr_ypos=current_node->key_value/width;
 cout<<"x and y "<<curr_xpos<<"    "<<curr_ypos<<endl;
 if(current_node->key_value==goal)
  {
   cout<<"goal has been found ";
   return;
  }
 else
  {
  cout<<endl<<"now we are inside"<<endl;
   for(int i=-1;i<=1;i++)
   {
    for(int j=-1;j<=1;j++)
     {

       int key,fl,gl,hl;
       key=(curr_xpos+i)+width*(curr_ypos+j);
      //  cout<<i<<"  "<<j<<endl;
       if((curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)&&(i*i+j*j))
        {
         gl =current_node->g +(sqrt(pow(i,2)+pow(j,2))) ;
         hl = ( abs(i) + abs(j) ); // distance from sucesso
         fl =gl+hl;
         if(!opentree_keyval.search_keyval_fval_comp(key,fl,opentree_keyval.root)&&!closedtree.search_keyval_fval_comp(key,fl,closedtree.root));
           {
           opentree_keyval.insert_keyval(key,fl,gl,hl,opentree_keyval.root);
           opentree_f.insert_f(key,fl,gl,hl,opentree_keyval.root);
           master_tree.insert_keyval(key,fl,gl,hl,current_node->key_value,master_tree.root);
           }
        }
     }
   }
   //closedtree.insert_keyval(current_node->key_value,current_node->f,current_node->g,current_node->h,current_node->route_parent_key,closedtree.root);
   //opentree_keyval.destroy_leaf_keyval(current_node->key_value,opentree_keyval.root);
  opentree_f.destroy_leaf_f(current_node->key_value,current_node->f,opentree_f.root);

   //node *temps;

   //temps=new node;
 //  temps=opentree_f.minnode(opentree_f.root);

   //if(!temps)
  //   {cout<<"node not found"<<endl;
  //    return;
  //    }
  //  else
  //  {
  //   cout<<"next node is"<<temps->key_value<<endl;
  //   astar(temps,goal,opentree_f,opentree_keyval,closedtree,master_tree);
  //   return;
  //  }


}
}

int main()
{

btree opentree_f,closedtree,opentree_keyval,master_tree;

 node first,second;
 first.key_value=0;
 second.key_value=0;
 closedtree.root=&first;
 master_tree.root=&second;

 for(int i=0;i<250000;i++)
    map_data[i]=0;

  for (int i = 0; i < 250; ++i)
    map_data[i+100000]=1;

    int i=0;
    while(i<250000)
    {

    if(map_data[i]!=0)
    {
    // cout<<"inititationg insertions";
    closedtree.insert_f(i,i,i,i,closedtree.root);

    }
    i+=1;
    }
cout<<"reached insertions end";
closedtree.destroy_leaf_f(100200,100200,closedtree.root);

 node temp[1000];
 long long t=0;
 long long p=0;


/*
node faker,faker2,faker3;
faker.key_value=0;
faker2.key_value=0;
faker3.key_value=0;
opentree_f.root=&faker;
opentree_keyval.root=&faker2;
master_tree.root=&faker3;
cout<<"we are here ";
astar(&faker,200000,opentree_f,opentree_keyval,closedtree,master_tree);
*/
}


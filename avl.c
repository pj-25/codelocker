#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct tree tnode;

struct tree
{
tnode *l;
int n;
int f;
int bf;
tnode *r;
};

void operation_menu();
tnode *insert(tnode *,tnode *);
void display(tnode *);
void create(tnode **);
tnode *del(tnode *,int );
tnode *newalloc(tnode *,int);

void main()
{
clrscr();
operation_menu();
}

tnode* newalloc(tnode *newnode,int v)
{
 newnode=(tnode *)malloc(sizeof(tnode));
 newnode->n=v;
 newnode->f=1;
 newnode->l=newnode->r=0;
 newnode->bf=0;
return newnode;
}

void operation_menu()
{
int ch,v,t;
tnode *root=0,*x;
printf("Select an operation to be performed on AVL tree:\n");
printf("1:Create\n2:Insert\n3:Delete\n4:Display\n5:End operation\n");
do
{
printf("\nEnter your choice:");
scanf("%d",&ch);
 switch(ch)
 {
 case 1:
 create(&root);
 break;

 case 2:
 printf("Enter value to be inserted:");
 scanf("%d",&v);
 x=newalloc(x,v);
 root=insert(root,x);
 break;

 case 3:
 printf("Enter value to be deleted:\n");
 scanf("%d",&v);
 root=del(root,v);
 break;

 case 4:
 printf("Values in AVL tree:\n");
 display(root);
 printf("\n");
 break;

 case 5:
 break;

 default:
 printf("Enter valid choice!!\n");
 }
}while(ch!=5);
return;
}

void create(tnode** root)
{
int t,v;
tnode *x;
printf("Enter total number of values to be inserted in AVL tree:");
scanf("%d",&t);
printf("Enter %d numbers:\n",t);
for(int i=0;i<t;i++)
{
scanf("%d",&v);
x=newalloc(x,v);
*root=insert(*root,x);
}
return;
}

tnode *del(tnode * root,int v)
{
int d;
tnode *temp;
if(root->n==v)
{
if(root->f==1)
{
 if(root->l==0 && root->r==0)
 return 0;
 else if(root->l!=0)
 {
 root->bf++;
 return root->l;
 }
 else if(root->r!=0)
 {
 root->bf--;
 return root->r;
 }
 else
 {
 temp=root->r;
  while(temp->l!=0)
  {
  temp=temp->l;
  }
  root->n=temp->n;
  root->r=del(root->r,temp->n);
 }
}
else
root->f--;
}
else if(root->n > v && root->l!=0)
{
root->l=del(root->l,v);
 if(root->l==0 || root->l->bf==0)
 root->bf--;
  if(root->bf==-2)
  {
    if(root->r->bf==1)
    {
    temp=root->r;
    root->r=temp->l;
    temp->l=root->r->r;
    root->r->r=temp;
    }
  temp=root;
  root=root->r;
  temp->r=root->l;
  root->l=temp;
   if(root->l->l!=0 && root->l->r==0)
   root->l->bf=1;
   else if(root->l->l!=0 && root->l->r->bf!=0)
   root->l->bf=-1;
   else
   root->l->bf=0;
     if(root->r->r!=0 && root->r->l==0)
     root->r->bf=-1;
     else
     root->r->bf=0;
   if(root->l->bf==-1)
   root->bf=1;
   else
   root->bf=0;
   }
}
else if(root->n < v && root->r!=0)
{
root->r=del(root->r,v);
 if(root->r==0 || root->r->bf==0)
 root->bf++;
  if(root->bf==2)
  {
   if(root->l->bf==-1)
    {
    temp=root->l;
    root->l=temp->r;
    temp->r=root->l->l;
    root->l->l=temp;
    }
  temp=root;
  root=root->l;
  temp->l=root->r;
  root->r=temp;
   if(root->r->r!=0 && root->r->l==0)
   root->r->bf=-1;
   else if(root->r->r!=0 && root->r->l->bf!=0)
   root->r->bf=1;
   else
   root->r->bf=0;
    if(root->l->l!=0 && root->l->r==0)
    root->l->bf=1;
    else
    root->l->bf=0;
   if(root->r->bf==1)
   root->bf=-1;
   else
   root->bf=0;
  }
}
else
printf("Value not found!\n");
return root;
}

tnode *insert(tnode *root,tnode *newnode)
{
tnode *temp;
if(root==0)
return newnode;
else if(newnode->n < root->n)
{
root->l=insert(root->l,newnode);
  if(root->l->n==newnode->n || root->l->bf!=0)
  root->bf++;
  if(root->bf > 1)
  {
   if(root->l->bf==-1)
    {
    temp=root->l;
    root->l=temp->r;
    temp->r=root->l->l;
    root->l->l=temp;
    }
  temp=root;
  root=root->l;
  temp->l=root->r;
  root->r=temp;
  if(root->r->l==0 && root->r->r!=0)
  root->r->bf=-1;
  else
  root->r->bf=0;
     if(root->l->r==0 && root->l->l!=0)
     root->l->bf=1;
     else
     root->l->bf=0;
  root->bf=0;
  }
}
else if(newnode->n > root->n)
{
root->r=insert(root->r,newnode);
 if(root->r->n==newnode->n || root->r->bf!=0)
 root->bf--;
 if(root->bf < -1)
  {
   if(root->r->bf==1)
    {
    temp=root->r;
    root->r=temp->l;
    temp->l=root->r->r;
    root->r->r=temp;
    }
  temp=root;
  root=root->r;
  temp->r=root->l;
  root->l=temp;
  if(root->l->r==0 && root->l->l!=0)
  root->l->bf=1;
  else
  root->l->bf=0;
    if(root->r->l==0 && root->r->r!=0)
    root->r->bf=-1;
    else
    root->r->bf=0;
  root->bf=0;
  }
}
else
{
root->f++;
}
return root;
}

void display(tnode *root)
{
if(root->l!=0)
display(root->l);
for(int i=0;i<root->f;i++)
printf("%d(%d) ",root->n,root->bf);
printf("\t");
if(root->r!=0)
display(root->r);
return;
}

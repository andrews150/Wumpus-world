// Agent.cc
//By: Andrew Springer
//Class: CptS 440 Artificial Intelligence

#include <iostream>
#include <math.h>
#include "Agent.h"
#include <stdio.h>

using namespace std;

void combinationUtil(int arr[], int n, int r, int index, int data[], int i);  
 void printCombination(int prob[], int n, int r);

int re, stck1, stck2;
int prevx, prevy;
int length;
int wallr, walll, goldf, firsts;
int found;
int width;
int stenches;
bool wumpprob;
bool scream, discover, tsize, optim;
int rem_len, space_len;
Location wump;
Action* goback;
Orientation* ori_goback;

struct Locations
{
	int X;
	int Y;
};
struct Tile_state
{
	// int X;
	// int Y;
	bool Known = false;
	bool temp12 = false;
	bool pKnown = false;
	double Pit = 0.2;
	int p = 0;
	int Wumpus = 0;
	bool wumpguess = false;
	bool Stench = false;
	bool Breeze = false;
	bool Frontier = false;
	int recent = 0;
	int visited = 0;
	bool bq = false;
};

// struct node
// {
// 	int name;
// 	struct node *left;
// 	struct node *right;
// };

// struct node* newNode(int name)
// {
// 	struct node* node = (struct node*)malloc(sizeof(struct node));

// 	node->name = name;

// 	node->left = NULL;
// 	node->right = NULL;
// 	return(node);
// }

// int u = 0;
// node* insertlvlordr(int arr[], node* root, int i, int n)
// {
// 	int layer = 1;
// 	if(i < n)
// 	{
// 		node* temp = newNode(arr[i % 2]);
// 		root = temp;
// 		root->name = 1;

// 		root->left = insertlvlordr(arr, root->left, 2 * i + 1, n);

// 		root->right = insertlvlordr(arr, root->right, 2 * i + 2, n);
// 	}
// 	return root;
// }

// void inOrder(node* root)
// {
// 	if(root != NULL)
// 	{
// 		inOrder(root->left);
// 		cout << root->name << " ";
// 		inOrder(root->right);
// 	}
// }

// bool printkdist(node* root, int k)
// {

// }

//struct Tile_state space[100][100]; // now dynamically increasing
Tile_state** space;
Locations *locations;

Agent::Agent ()
{

}

Agent::~Agent ()
{

}

void Agent::Initialize ()
{
	prevx = 0;
	prevy = 0;
	wstate.agentHasGold = false;
	wstate.agentHasArrow = true;
	alocal.X = 1;
	alocal.Y = 1;
	length = 2;
	width = 2;
	ori = RIGHT;
	wump.X = 0;
	wump.Y = 0;
	wumpprob = false;
	stenches = 0;
	found = 0;
	re = 0;
	firsts = 0;
	tsize = false;
	stck1 = 0, stck2 = 0;
	scream = false;
	discover = false;
	optim = false;
	rem_len = 10000;// fix these to also resize properly
	space_len = 1000; // fix resizing double array later plz
	space = new Tile_state*[space_len];
	for(int t = 0; t < space_len; ++t){
	space[t] = new Tile_state[space_len];
	}
	goback = new Action[rem_len];
	while(re < rem_len){
		goback[re] = CLIMB;
		re++;
	}
	re = 0;
	locations = new Locations[rem_len];
	while(re < rem_len){
		locations[re].X = 1;
		locations[re].Y = 1;
		re++;
	}
	re = 0;
	ori_goback = new Orientation[rem_len];
	while(re < rem_len){
		ori_goback[re] = RIGHT;
		re++;
	}
	re = 1;
}

Action Agent::Process (Percept& percept)
{
	int temp1 = 0, temp2 = 0;
	Action action;
	Action wallrt[3] = {TURNLEFT, GOFORWARD, TURNLEFT};
	Action walllt[3] = {TURNRIGHT, GOFORWARD, TURNRIGHT};
	Action goldfr[3] = {GRAB, TURNLEFT, TURNLEFT};
	Action firststuck[2] = {SHOOT, GOFORWARD};

	if(percept.Scream == 1)
	{
		scream = true;
		for(int i = 0; i < space_len; i++){
			for(int j = 0; j < space_len; j++){
					space[i][j].Wumpus = 0;
					space[i][j].Known = true;
			}
		}
	}
	if(wumpprob == true)
	{
		for(int i = 0; i < space_len; i++){
			for(int j = 0; j < space_len; j++){
				if(space[i][j].wumpguess == false)
				{
					space[i][j].Wumpus = 0;
					space[i][j].Known = true;
				}
			}
		}
	}

	if (percept.Bump == true){
		if(ori == RIGHT){
			alocal.X = alocal.X - 1;
			width = alocal.X;
			re--;
			length = width;
			tsize = true;
		} else if(ori == UP){
			alocal.Y = alocal.Y - 1;
			length = alocal.Y;
			re--;
			discover = true;
			width = length;
			tsize = true;
		} else if(ori == LEFT){
			alocal.X = alocal.X + 1;
			re--;
		} else if(ori == DOWN){
			alocal.Y = alocal.Y + 1;
			re--;
			//discover = false;
		}

		for(int i = 0; i < space_len; i++){
			for(int j = 0; j < space_len; j++){
				if((i == 0 || j == 0 || i > width + 1 || j > length + 1) && discover == false)
				{
					space[i][j].Breeze = false;
					space[i][j].Frontier = false;
					space[i][j].Known = false;
					space[i][j].Pit = 2;
					space[i][j].p = 2;
					space[i][j].pKnown = true;
					space[i][j].Stench = false;
					space[i][j].wumpguess = false;
					space[i][j].Wumpus = 0;
					space[i][j].visited = 0;
				}
				else if((i == 0 || j == 0 || i > width || j > length) && discover == true)
				{
					space[i][j].Breeze = false;
					space[i][j].Frontier = false;
					space[i][j].Known = false;
					space[i][j].Pit = 2;
					space[i][j].p = 2;
					space[i][j].pKnown = true;
					space[i][j].Stench = false;
					space[i][j].wumpguess = false;
					space[i][j].Wumpus = 0;
					space[i][j].visited = 0;
				}
				if(tsize == true && space[i][j].Pit == 2 && (j > 0 && i > 0 && j <= length && i <= width))
				{
					space[i][j].Breeze = false;
					space[i][j].Frontier = false;
					space[i][j].Known = false;
					space[i][j].Pit = 0;
					space[i][j].p = 0;
					space[i][j].pKnown = false;
					space[i][j].Stench = false;
					space[i][j].wumpguess = false;
					space[i][j].Wumpus = 0;
					space[i][j].visited = 0;
				}
			}
		}
	}	

	//printf("Tsize : %d\n", tsize);

	// print locations
	//printf("x axis : %d\ny axis : %d\n", alocal.X, alocal.Y);
	if(alocal.X + 1 == space_len || alocal.Y + 1 == space_len)
	{
		int old_len = space_len;
		space_len = space_len * 2;
		Tile_state** newarray = new Tile_state*[space_len];
		for(int t = 0; t < space_len; ++t){
		newarray[t] = new Tile_state[space_len];
		}

		for(int y = 0; y < old_len; y++)
		{
			for(int u = 0; u < old_len; u++)
			newarray[y][u] = space[y][u];
		}
		for(int y = old_len; y < space_len; y++)
		{
			for(int u = old_len; u < space_len; u++){
			newarray[y][u].Known = false;
			newarray[y][u].pKnown = false;
			newarray[y][u].Pit = 0;
			newarray[y][u].Wumpus = 0;
			newarray[y][u].Stench = false;
			newarray[y][u].Breeze = false;
			}
		}
		for(int y = 0; y < old_len; y++)
		{
			delete[] space[y];
		}
		delete[] space;
		space = newarray;
		for(int y = 0; y < old_len; y++)
		{
			delete[] newarray[y];
		}
		delete[] newarray;
		printf("Map array has been resized\nNew map array size : %d\n", space_len);
		// printf("Press any key to continue.\n");
		// getchar();
	}
	space[alocal.X][alocal.Y].Known = true;
	space[alocal.X][alocal.Y].Wumpus = 0;
	space[alocal.X][alocal.Y].recent = 50;
	space[alocal.X][alocal.Y].visited++;
	
	for(int i = 1; i < space_len; i++){
		for(int j = 1; j < space_len; j++){
			if(space[i][j].recent > 0)
			{
				space[i][j].recent--;
			}
		}
	}
		
	printf("Estimated size of map : %dx%d\n", width, length);
	if(width > length)
		length = width;
	else
		width = length;

	//printf("X : %d, Y : %d\n", alocal.X, alocal.Y);
	//printf("prevX : %d, prevY : %d\n", prevx, prevy);
	if(percept.Breeze == true)
	{
		space[alocal.X][alocal.Y].bq = true;
		space[alocal.X][alocal.Y].Frontier = false;
		// printf("Breeze!\n");
		if(ori == RIGHT && !(prevx == alocal.X && prevy == alocal.Y)){
			if(space[alocal.X+1][alocal.Y].pKnown == false){
				space[alocal.X+1][alocal.Y].p++;
				space[alocal.X+1][alocal.Y].Frontier = true;
			}
			if(space[alocal.X][alocal.Y+1].pKnown == false){
				space[alocal.X][alocal.Y+1].p++;
				space[alocal.X][alocal.Y+1].Frontier = true;
			}
			if(space[alocal.X][alocal.Y-1].pKnown == false){
				space[alocal.X][alocal.Y-1].p++;
				space[alocal.X][alocal.Y-1].Frontier = true;
			}
		} else if(ori == UP && !(prevx == alocal.X && prevy == alocal.Y)){
			if(space[alocal.X+1][alocal.Y].pKnown == false){
				space[alocal.X+1][alocal.Y].p++;
				space[alocal.X+1][alocal.Y].Frontier = true;
			}
			if(space[alocal.X][alocal.Y+1].pKnown == false){
				space[alocal.X][alocal.Y+1].p++;
				space[alocal.X][alocal.Y+1].Frontier = true;
			}
			if(space[alocal.X-1][alocal.Y].pKnown == false){
				space[alocal.X-1][alocal.Y].p++;
				space[alocal.X-1][alocal.Y].Frontier = true;
			}
		} else if(ori == LEFT && !(prevx == alocal.X && prevy == alocal.Y)){
			if(space[alocal.X-1][alocal.Y].pKnown == false){
				space[alocal.X-1][alocal.Y].p++;
				space[alocal.X-1][alocal.Y].Frontier = true;
			}
			if(space[alocal.X][alocal.Y+1].pKnown == false){
				space[alocal.X][alocal.Y+1].p++;
				space[alocal.X][alocal.Y+1].Frontier = true;
			}
			if(space[alocal.X][alocal.Y-1].pKnown == false){
				space[alocal.X][alocal.Y-1].p++;
				space[alocal.X][alocal.Y-1].Frontier = true;
			}
		} else if(ori == DOWN && !(prevx == alocal.X && prevy == alocal.Y)){
			if(space[alocal.X+1][alocal.Y].pKnown == false){
				space[alocal.X+1][alocal.Y].p++;
				space[alocal.X+1][alocal.Y].Frontier = true;
			}
			if(space[alocal.X-1][alocal.Y].pKnown == false){
				space[alocal.X-1][alocal.Y].p++;
				space[alocal.X-1][alocal.Y].Frontier = true;
			}
			if(space[alocal.X][alocal.Y-1].pKnown == false){
				space[alocal.X][alocal.Y-1].p++;
				space[alocal.X][alocal.Y-1].Frontier = true;
			}
		}

		// if(space[alocal.X-1][alocal.Y].p > 1 && space[alocal.X][alocal.Y].visited == 1){
		// 	space[alocal.X-1][alocal.Y].Frontier = false;
		// 	space[alocal.X-1][alocal.Y].pKnown == true;
		// 	space[alocal.X-1][alocal.Y].Pit = 1.00;
		// }
		// if(space[alocal.X+1][alocal.Y].p > 1 && space[alocal.X][alocal.Y].visited == 1){
		// 	space[alocal.X+1][alocal.Y].Frontier = false;
		// 	space[alocal.X+1][alocal.Y].pKnown == true;
		// 	space[alocal.X+1][alocal.Y].Pit = 1.00;
		// }
		// if(space[alocal.X][alocal.Y-1].p > 1 && space[alocal.X][alocal.Y].visited == 1){
		// 	space[alocal.X][alocal.Y-1].Frontier = false;
		// 	space[alocal.X][alocal.Y-1].pKnown == true;
		// 	space[alocal.X][alocal.Y-1].Pit = 1.00;
		// }
		// if(space[alocal.X][alocal.Y+1].p > 1 && space[alocal.X][alocal.Y].visited == 1){
		// 	space[alocal.X][alocal.Y+1].Frontier = false;
		// 	space[alocal.X][alocal.Y+1].pKnown == true;
		// 	space[alocal.X][alocal.Y+1].Pit = 1.00;
		// }
		//printf("here maybe\n");
	}
	else
	{
		space[alocal.X][alocal.Y].Pit = 0;
		space[alocal.X][alocal.Y].p = 0;
		space[alocal.X][alocal.Y].pKnown = true;
		space[alocal.X][alocal.Y].Frontier = false;
		//printf("ori : %d\n", ori);
		//printf("pit? in (%d,%d) : %.2f percent\n", alocal.X, alocal.Y, space[alocal.X][alocal.Y-1].Pit * 100);
		if(ori == RIGHT){
				space[alocal.X+1][alocal.Y].Pit = 0;
				space[alocal.X+1][alocal.Y].p = 0;
				space[alocal.X+1][alocal.Y].pKnown = true;
				space[alocal.X+1][alocal.Y].Frontier = false;
				// space[alocal.X-1][alocal.Y].bq = false;
				// space[alocal.X][alocal.Y+1].bq = false;
				// space[alocal.X][alocal.Y-1].bq = false;
				// space[alocal.X+1][alocal.Y].bq = false;
				space[alocal.X][alocal.Y+1].Pit = 0;
				space[alocal.X][alocal.Y+1].p = 0;
				space[alocal.X][alocal.Y+1].pKnown = true;
				space[alocal.X][alocal.Y+1].Frontier = false;
				space[alocal.X][alocal.Y-1].Pit = 0;
				space[alocal.X][alocal.Y-1].p = 0;
				space[alocal.X][alocal.Y-1].pKnown = true;
				space[alocal.X][alocal.Y-1].Frontier = false;
		} else if(ori == UP){
				space[alocal.X+1][alocal.Y].Pit = 0;
				space[alocal.X+1][alocal.Y].p = 0;
				space[alocal.X+1][alocal.Y].pKnown = true;
				space[alocal.X+1][alocal.Y].Frontier = false;
				space[alocal.X][alocal.Y+1].Pit = 0;
				space[alocal.X][alocal.Y+1].p = 0;
				space[alocal.X][alocal.Y+1].pKnown = true;
				space[alocal.X][alocal.Y+1].Frontier = false;
				space[alocal.X-1][alocal.Y].Pit = 0;
				space[alocal.X-1][alocal.Y].p = 0;
				space[alocal.X-1][alocal.Y].pKnown = true;
				space[alocal.X-1][alocal.Y].Frontier = false;
		} else if(ori == LEFT){
				space[alocal.X-1][alocal.Y].Pit = 0;
				space[alocal.X-1][alocal.Y].p = 0;
				space[alocal.X-1][alocal.Y].pKnown = true;
				space[alocal.X-1][alocal.Y].Frontier = false;
				space[alocal.X][alocal.Y+1].Pit = 0;
				space[alocal.X][alocal.Y+1].p = 0;
				space[alocal.X][alocal.Y+1].pKnown = true;
				space[alocal.X][alocal.Y+1].Frontier = false;
				space[alocal.X][alocal.Y-1].Pit = 0;
				space[alocal.X][alocal.Y-1].p = 0;
				space[alocal.X][alocal.Y-1].pKnown = true;
				space[alocal.X][alocal.Y-1].Frontier = false;
		} else if(ori == DOWN){
				space[alocal.X+1][alocal.Y].Pit = 0;
				space[alocal.X+1][alocal.Y].p = 0;
				space[alocal.X+1][alocal.Y].pKnown = true;
				space[alocal.X+1][alocal.Y].Frontier = false;
				space[alocal.X-1][alocal.Y].Pit = 0;
				space[alocal.X-1][alocal.Y].p = 0;
				space[alocal.X-1][alocal.Y].pKnown = true;
				space[alocal.X-1][alocal.Y].Frontier = false;
				space[alocal.X][alocal.Y-1].Pit = 0;
				space[alocal.X][alocal.Y-1].p = 0;
				space[alocal.X][alocal.Y-1].pKnown = true;
				space[alocal.X][alocal.Y-1].Frontier = false;
		}
		//printf("Did we do something here?\n");
	}

	int nopit = 0;
	int yespit = 0;
	//prints no pit locations
	printf("width = %d, length = %d\n", width, length);
	// for(int i = 1; i < space_len; i++){
	// 	for(int j = 1; j < space_len; j++){
	// 		if(space[i][j].pKnown == true && space[i][j].Pit >= 0)
	// 		{
	// 			printf("No pit loc (%d,%d)\n", i, j);
	// 		}
	// 	}
	// }

	int breque = 0;
	//prints known pits
	for(int i = 1; i < space_len; i++){
		for(int j = 1; j < space_len; j++){
			if(space[i][j].Pit >= 1.00 && i < width + 1 && j < length + 1 && discover == false)
			{
				printf("Known pit (%d,%d) : %0.2f\n", i, j, space[i][j].Pit);
				space[i][j].Frontier = false;
				space[i+1][j].bq = false;
				space[i-1][j].bq = false;
				space[i][j-1].bq = false;
				space[i][j+1].bq = false;
				//yespit++;
			}
			else if(space[i][j].Pit >= 1.00 && i < width && j < length && discover == true)
			{
				printf("Known pit (%d,%d) : %0.2f\n", i, j, space[i][j].Pit);
				space[i][j].Frontier = false;
				space[i+1][j].bq = false;
				space[i-1][j].bq = false;
				space[i][j-1].bq = false;
				space[i][j+1].bq = false;
				//yespit++;
			}
			
			//if(space[i][j].Frontier == true){
			//printf("i : %d, j : %d\nX : %d, Y : %d\n", i, j, alocal.X, alocal.Y);
			//printf("i - X : %d, j - Y : %d\n\n", i - alocal.X, j - alocal.Y);
			//}
			if(space[i][j].bq == true && (space[i][j-1].pKnown == true && space[i][j-1].p == 0) && (space[i][j+1].pKnown == true && space[i][j+1].p == 0) && (space[i+1][j].Known == true && space[i+1][j].p == 0) && (space[i-1][j].p > 0))
			{
				space[i][j].bq = false;
				space[i-1][j].Pit = 1.00;
				space[i-1][j].Frontier = false;
				printf("Did we get here111!\n");
			}
			if(space[i][j].bq == true && (space[i][j-1].pKnown == true && space[i][j-1].p == 0) && (space[i][j+1].pKnown == true && space[i][j+1].p == 0) && (space[i-1][j].Known == true && space[i-1][j].p == 0) && (space[i+1][j].p > 0))
			{
				space[i][j].bq = false;
				space[i+1][j].Pit = 1.00;
				space[i+1][j].Frontier = false;
				printf("Did we get here222!\n");
			}
			if(space[i][j].bq == true && ((space[i][j-1].pKnown == true && space[i][j-1].p == 0) || (j-1 >= 1)) && (space[i+1][j].pKnown == true && space[i+1][j].p == 0) && (space[i-1][j].Known == true && space[i-1][j].p == 0) && (space[i][j+1].p > 0))
			{
				space[i][j].bq = false;
				space[i][j+1].Pit = 1.00;
				space[i][j+1].Frontier = false;
				printf("Did we get here333!\n");
			}
			//currently working on this setting up so its either that square or its a wall like the bottom one!
			if(space[i][j].bq == true && ((space[i][j+1].pKnown == true && space[i][j+1].p == 0) || (j+1 <= length)) && ((space[i+1][j].pKnown == true && space[i+1][j].p == 0) || (i+1 <= width)) && ((space[i-1][j].Known == true && space[i-1][j].p == 0) || (i-1 >= 1)) && (space[i][j-1].p > 0))
			{
				space[i][j].bq = false;
				space[i][j-1].Pit = 1.00;
				space[i][j-1].Frontier = false;
				printf("Did we get here444!\n");
			}
			if(space[i][j].Frontier == true && ((i - alocal.X > 2 || j - alocal.Y > 2) || ((i - alocal.X == 2 || i - alocal.X == -2) && (j - alocal.Y == 2 || j - alocal.Y == -2))))
			{
				space[i][j].Frontier = false;
				space[i][j].temp12 = true;
				if(breque > 2)
					breque = 2;
				else if (breque == 2)
				 	breque = 1;
			}

			//if(){} if adjacent then be in frontier

			//Print frontier
			if(space[i][j].Frontier == true )//&& ((i == alocal.X && j == alocal.Y + 1) || (i == alocal.X+1 && j == alocal.Y) || (i == alocal.X && j == alocal.Y - 1) ||(i == alocal.X-1 && j == alocal.Y + 1)))
			{
				//printf("Frontier = (%d,%d) : %d\n", i, j, space[i][j].p);
				yespit++;
				//yespit++;
			}

			if(space[i][j].bq == true && space[i][j].p < 1)
			{
				printf("bq = (%d,%d) : %d\n", i, j, space[i][j].p);
				breque++;
				//yespit++;
			}
		}
	}

	//printf("yespit : %d\n", yespit);
	//double t[4] = {0, 1, 0, 1}, f[4] = {1, 0, 0, 1};
	double t = 0.0, f = 0.0;
	double Ppitt = 0.0, Ppitf = 0.0, Pfrontier = 0.0, jt = 0.0, lt = 0.0;
	int C = 0, D = 0, ypit = 0, npit = 0;
	//struct node *root = newNode(1);
	int n = yespit - 1;
	int list[11] = {1, 0, 10, 11, 01, 101, 111, 110, 11, 10, 100};
	int list1[5] = {111, 110, 101, 100, 11};
	//int list2[6] = {1111, 1010, 1011, 1001, 1110, 1101};
	//int list3[6] = {111, 110, 101, 1111, 1110, 1101};
	int ts = 0, fs = 0;
	
	//printf("yespit : %d\n", yespit);
	C = 0;

	//int n = sizeof(prob)/sizeof(prob[0]);

	//printf("yespit : %d\n", yespit);
	if(yespit > 0){
		C = pow(2, yespit);
		
		//printf("C : %d\n", C);

		breque = breque - 1;
		if(breque > 0){
		//printf("Other breezes : %d\n", breque);
		t = C/2 - breque;
		}
		else 
		t = C/2;

		f = t - 1;

		// if(breque > 1)
		// f = t;

		//printf("t : %0.2f\nf : %0.2f\n", t, f);
		// node* root = insertlvlordr(prob, root, 0, n);
		// inOrder(root);


		//printf("C : %d\n", C);
		//int prob[2] = {0, 1};
		//int n = sizeof(prob)/sizeof(prob[0]);
		//printCombination(prob, n, D);
	}

	// for(int j = 0; j < C; j++)
	// {
	// 	for(int y = 0; y < D; y++)
	// 	{
	// 		l = y % 2;
	// 		data[j][y] = opt[l];			
	// 	}
	// }

	//printf("t : %0.2f\nf : %0.2f\n", t, f);

	int o = 0;
	int runs = f;
	//Do some maths
	int index = 0;

	for(int i = 1; i < space_len; i++){
		for(int j = 1; j < space_len; j++){
			Ppitt = 0.0;
			Ppitf = 0.0;
			if(space[i][j].Frontier == true)
			{
				//printf("space[%d][%d].p : %d\n", i, j, space[i][j].p);
				if(space[i][j].p == 2 && breque == 1){
						t = t+1;
						f = f - 1;
						o = 1;
				}
				else if(space[i][j].p == 1 && o == 1){
						t = t - 1;
						f = f + 1;
						o = 0;
				}

				//printf("real t : %0.2f\nreal f : %0.2f\n", t, f);
				int one = 0;
				for(int y = 0; y < t; y++)
				{
					//if(data[y] == .2)
					if(t == 3)
						index = 5;
					if(t == 2)
						index = 2;
					int u = 0;
					index = index + y;

					// if(breque > 1)
					// {
					// 	u = list2[index];
					// }
					// else
					if(space[i][j].p == 1){
					u = list[index];
					//printf("index : %d\n", index);
					}else if (space[i][j].p == 2){
					u = list1[one];
					//printf("one : %d\n", one);
					}

					one++;

					int h = 0;
					//printf("u : %d\n", u);
					double tmp = 0;
					ts = 0;
					fs = 0;

					
					for(int r = 0; r < runs; r++)
					{
						if(u > 10){
						tmp = double(u)/10;
						u = u/10;
						}
						//printf("tmp before : %0.2f\n", tmp);
						tmp = tmp - double(u);
						//printf("tmp : %0.2f\n", tmp);
						//printf("U before : %d\n", u);
						if(tmp > 0)
							tmp = 1;
						else
							tmp = 0;

						//printf("u : %d\n", u);

						h = int(tmp) % 2;
						if (h == 1)
							ts++;
						if (h == 0)
							fs++;					
					}

					//printf("ts : %d\nfs : %d\n", ts, fs);

					Pfrontier = pow(.2, ts) * pow(.8 , fs);
					

					Ppitt += Pfrontier;
					//printf("Ppitt : %0.2f\n", Ppitt);
					//Ppitf += Pfrontier;
				}

			 	index++;

				for(int y = 0; y < f; y++)
				{
					int u = 0;
					index = index + y;

					// if(breque > 1)
					// {
					// 	u = list3[index];
					// }
					// else 
					if(space[i][j].p == 1){
					u = list[index];
					//printf("pitf index : %d\n", index);
					}else if (space[i][j].p == 2){
					u = list1[one];
					//printf("pitf one : %d\n", one);
					}

					one++;

					int h = 0;
					//printf("pitf u : %d\n", u);
					double tmp = 0;
					ts = 0;
					fs = 0;

					
					for(int r = 0; r < runs; r++)
					{
						if(u > 10){
						tmp = double(u)/10;
						u = u/10;
						}
						else if(u == 10)
							u = 0;
						else if(u == 0)
							u = 1;
						//printf("tmp before : %0.2f\n", tmp);
						tmp = tmp - double(u);
						//printf("pitf tmp : %0.2f\n", tmp);
						//printf("pitf U before : %d\n", u);
						if(tmp > 0)
							tmp = 1;
						else
							tmp = 0;

						if(u == 1)
							tmp = 1;
						if(u == 0)
							tmp = 0;
						//printf("u : %d\n", u);

						h = int(tmp) % 2;
						if (h == 1)
							ts++;
						if (h == 0)
							fs++;					
					}

					//printf("pitf ts : %d\nfs : %d\n", ts, fs);
					// index = index + y;					
					// printf("index : %d\n", index);
					// int u = list[index];
					// printf("u : %d\n", u);
					// double tmp = 0;
					// ts = 0;
					// fs = 0;
					// int h = 0;

					// for(int r = 0; r < f; r++)
					// {
					// 	printf("uuuu : %d\n", u);
					// 	tmp = double(u)/10;
					// 	u = u/10;
					// 	if(tmp < 1 && u == 0)
					// 		u == 1;
					// 	printf("tmp before : %0.2f\n", tmp);
					// 	//printf("u after : %d\n", u);
					// 	tmp = tmp - double(u);
					// 	//printf("tmp : %0.2f\n", tmp);
					// 	printf("u before : %d\n", u);
					// 	printf("tmp : %0.2f\n", tmp);

					// 	if(tmp > 0 || h == 0)
					// 		tmp = 0;
					// 	else if(tmp <= 0 || h == 1)
					// 		tmp = 1;


					// 	printf("tmp : %0.2f\n", tmp);

					// 	h = int(tmp) % 2;
					// 	if (h == 0)
					// 		ts++;
					// 	if (h == 1)
					// 		fs++;
						
					// 	printf("ts : %d\nfs : %d\n", ts, fs);

					// 	tmp = double(u)/10;
					// 	printf("tmp again : %0.2f\n", tmp);
					// 	u = u/10;

					// 	if(tmp < 1 && u == 0)
					// 		u == 1;							
					// }

					Pfrontier = pow(.2, ts) * pow(.8, fs);
					

					//Ppitt += Pfrontier;
					Ppitf += Pfrontier;
					//printf("Ppitf : %0.2f\n", Ppitf);
				}

				//printf("Ppitt : %0.2f\n", Ppitt);
				//printf("Ppitf : %0.2f\n", Ppitf);
				Ppitt *= .2;
				Ppitf *= .8;
				double pitl = Ppitt + Ppitf;
				//printf("Pitl : %0.2f\n", pitl);
				//printf("Ppitt : %0.2f\n", Ppitt);
				//printf("Ppitf : %0.2f\n", Ppitf);
				Ppitt = Ppitt / (Ppitt + Ppitf);
				//printf("Ppitt again : %0.2f\n", Ppitt);
				// Ppitf = 0.0;
				// Ppitt = 0.0;
				//Ppitt = (1 / double(yespit));
				// t = (1 + pow(2, yespit - 1));
				// f = (pow(2, yespit - 1));
				// jt = pow(.2 , t);
				// lt = pow(.8, f);
				// Pfrontier = jt * lt;
				// //printf("Prob Frontier : %.2f\n", Pfrontier);
				// Ppitt += Pfrontier;
				// Ppitf += Pfrontier;
				// Ppitt *= .2;
				// Ppitf *= .8;
				// Ppitt = (Ppitt / (Ppitt + Ppitf));
				space[i][j].Pit = Ppitt;
				//printf("Frontier = (%d,%d) : %0.2f\n", i, j, space[i][j].Pit);
			}
		}
	}

	for(int i = 1; i < space_len; i++){
		for(int j = 1; j < space_len; j++){

			if(space[i][j].Frontier == true)
				printf("Frontier = (%d,%d) : %0.2f\n", i, j, space[i][j].Pit);

			if(space[i][j].temp12 == true){
				space[i][j].Frontier = true;
				space[i][j].temp12 = false;
			}

		}
	}

	if(((alocal.X == 1 && alocal.Y == 1) && percept.Stench == true) && wstate.agentHasGold == false)
	{
		space[alocal.X][alocal.Y].Stench = true;
		//printf("firsts : %d\n", firsts);
		action = firststuck[firsts];
		wstate.agentHasArrow = false;
		firsts++;
		if(firsts == 1){
			if(percept.Scream == true)
			space[alocal.X+1][alocal.Y].Wumpus = 2;
			else 
			space[alocal.X][alocal.Y+1].Wumpus = 2;
		}
	}
	if(percept.Stench == true && space[alocal.X][alocal.Y].Stench == false && scream == false && wumpprob == false){
		space[alocal.X][alocal.Y].Stench = true;
		//printf("Stench %d,%d\n", alocal.X, alocal.Y);
		if(ori == RIGHT){
			if(space[alocal.X+1][alocal.Y].Known == false)
				space[alocal.X+1][alocal.Y].Wumpus++;
			if(space[alocal.X][alocal.Y+1].Known == false)
				space[alocal.X][alocal.Y+1].Wumpus++;
			if(space[alocal.X][alocal.Y-1].Known == false)
				space[alocal.X][alocal.Y-1].Wumpus++;
		} else if(ori == UP){
			if(space[alocal.X+1][alocal.Y].Known == false)
				space[alocal.X+1][alocal.Y].Wumpus++;
			if(space[alocal.X][alocal.Y+1].Known == false)
				space[alocal.X][alocal.Y+1].Wumpus++;
			if(space[alocal.X-1][alocal.Y].Known == false)
				space[alocal.X-1][alocal.Y].Wumpus++;
		} else if(ori == LEFT){
			if(space[alocal.X-1][alocal.Y].Known == false)
				space[alocal.X-1][alocal.Y].Wumpus++;
			if(space[alocal.X][alocal.Y+1].Known == false)
				space[alocal.X][alocal.Y+1].Wumpus++;
			if(space[alocal.X][alocal.Y-1].Known == false)
				space[alocal.X][alocal.Y-1].Wumpus++;
		} else if(ori == DOWN){
			if(space[alocal.X+1][alocal.Y].Known == false)
				space[alocal.X+1][alocal.Y].Wumpus++;
			if(space[alocal.X-1][alocal.Y].Known == false)
				space[alocal.X-1][alocal.Y].Wumpus++;
			if(space[alocal.X][alocal.Y-1].Known == false)
				space[alocal.X][alocal.Y-1].Wumpus++;
		}
	} 
    //printf("did we get here1\n\n");
	if(percept.Stench == false){
		space[alocal.X+1][alocal.Y].Wumpus = 0;
		space[alocal.X+1][alocal.Y].Known = true;
		if(alocal.X - 1 > 0)
		{
		space[alocal.X-1][alocal.Y].Wumpus = 0;
		space[alocal.X-1][alocal.Y].Known = true;
		}
		space[alocal.X][alocal.Y+1].Wumpus = 0;
		space[alocal.X][alocal.Y+1].Known = true;
		if(alocal.Y - 1 > 0)
		{
		space[alocal.X][alocal.Y-1].Wumpus = 0;
		space[alocal.X][alocal.Y-1].Known = true;
		}
	} 
    //printf("did we get here2\n\n");  

	for(int i = 1; i < space_len; i++){
		for(int j = 1; j < space_len; j++){
			if(space[i][j].Wumpus > 0 && i < width + 1 && j < length + 1)
			{
				printf("Wumpus possible location: %dx%d, likely hood: %d\n", i, j, space[i][j].Wumpus);
				if(space[i][j].Wumpus > 2 || (j == 1 && space[i][j].Wumpus > 1) || (i == 1 && space[i][j].Wumpus > 1) || (i == width && space[i][j].Wumpus > 1) || (j == length && space[i][j].Wumpus > 1)){
					wumpprob = true;
					space[i][j].wumpguess = true;
					printf("Found the Wumpus!!\n");
				}
			}
		}
	}

	prevx = alocal.X;
	prevy = alocal.Y;
	//printf("ori = %d\n", ori);

//printf("did we get here3\n\n");  
if(firsts == 0){
	if(percept.Glitter == true || goldf > 0){ 
		action = goldfr[goldf];
		if(goldf > 1)
				re--;
		goldf++;
		wstate.agentHasGold = true;
	}else if(wstate.agentHasGold == true){
		//add in path optimazation function
		if(optim == false){
			//pathoptimization();
			optim = true;
		}
		if(alocal.X == 1 && alocal.Y == 1)
			action = CLIMB;
		else if(goback[re] == GOFORWARD)
			action = GOFORWARD;
		else if (goback[re] == TURNLEFT)
			action = TURNRIGHT;
		else if (goback[re] == TURNRIGHT)
			action = TURNLEFT;
		else if (goback[re] == CLIMB)
			action = CLIMB;
		printf("Location = %d,%d\n", locations[re].X, locations[re].Y);
		re--;
	}else if(ori == RIGHT && space[alocal.X+1][alocal.Y].Wumpus > 1 && wstate.agentHasArrow == true){
		action = SHOOT;
		wstate.agentHasArrow = false;
	}else if(ori == LEFT && space[alocal.X-1][alocal.Y].Wumpus > 1 && wstate.agentHasArrow == true){
		action = SHOOT;
		wstate.agentHasArrow = false;
	}else if(ori == UP && space[alocal.X][alocal.Y+1].Wumpus > 1 && wstate.agentHasArrow == true){
		action = SHOOT;
		wstate.agentHasArrow = false;
	}else if(ori == DOWN && space[alocal.X][alocal.Y-1].Wumpus > 1 && wstate.agentHasArrow == true){
		action = SHOOT;
		wstate.agentHasArrow = false;
	// }else if(space[alocal.X][alocal.Y].visited > 10){
	// 	action = GOFORWARD;
	}else if(ori == LEFT && space[alocal.X-1][alocal.Y].Wumpus > 0 && scream == false && alocal.X-1 > 0 && space[alocal.X][alocal.Y+1].Wumpus > 0){
		action = TURNLEFT;
		printf("This left!\n");
	}else if(ori == UP && discover == true && alocal.Y + 1 < length + 1 && space[alocal.X][alocal.Y+1].p > .5){
		action = TURNLEFT;
		printf("This left upty!\n");
	}else if(ori == LEFT && space[alocal.X][alocal.Y].visited > 3 && space[alocal.X][alocal.Y+1].visited < space[alocal.X-1][alocal.Y].visited && space[alocal.X][alocal.Y+1].Pit < .5){
		action = TURNRIGHT;
		printf("This rightyuji upty!\n");
	}else if(ori == RIGHT && space[alocal.X][alocal.Y].visited > 3 && space[alocal.X][alocal.Y+1].visited > space[alocal.X+1][alocal.Y].visited && space[alocal.X][alocal.Y+1].Pit < .5){
		action = TURNLEFT;
		printf("This leftrtrtr upty!\n");
	}else if(ori == LEFT && space[alocal.X][alocal.Y].visited > 3 && (space[alocal.X][alocal.Y-1].visited < 3 || space[alocal.X][alocal.Y+1].visited < 3) && space[alocal.X][alocal.Y+1].Pit < .5 && space[alocal.X][alocal.Y+1].recent < space[alocal.X-1][alocal.Y].recent){
		if(space[alocal.X][alocal.Y-1].visited <= space[alocal.X][alocal.Y+1].visited && alocal.Y-1 > 0){
			action = TURNLEFT;
			printf("This leffffft!\n");
		}else {
			action = TURNRIGHT;
			printf("This Riggggght!\n");
		}
	}else if(ori == RIGHT && space[alocal.X][alocal.Y].visited > 3 && (space[alocal.X][alocal.Y-1].visited < 3 || space[alocal.X][alocal.Y+1].visited < 3) && space[alocal.X][alocal.Y+1].Pit < .5 && space[alocal.X][alocal.Y+1].recent < space[alocal.X+1][alocal.Y].recent){
		if(space[alocal.X][alocal.Y-1].visited <= space[alocal.X][alocal.Y+1].visited && alocal.Y-1 > 0){
			action = TURNRIGHT;
			printf("This ritzzzzt!\n");
		}else {
			action = TURNLEFT;
			printf("This leftttttttfd!\n");
		}
	// }else if(ori == LEFT && space[alocal.X-1][alocal.Y].recent > space[alocal.X][alocal.Y-1].recent && alocal.Y-1 > 0){
	// 	action = TURNLEFT;
	// 	printf("This lefty mc lefter!\n");
	// }else if(ori == RIGHT && space[alocal.X+1][alocal.Y].recent > space[alocal.X][alocal.Y-1].recent && alocal.Y-1 > 0){
	// 	action = TURNRIGHT;
	// 	printf("This righty mc righter!\n");
	}else if(ori == RIGHT && ((space[alocal.X+1][alocal.Y].Wumpus > 0 && scream == false) || space[alocal.X+1][alocal.Y].Pit > .5)){
		if(space[alocal.X][alocal.Y+1].Pit <= space[alocal.X][alocal.Y-1].Pit || space[alocal.X][alocal.Y+1].Wumpus <= space[alocal.X][alocal.Y-1].Wumpus){
			action = TURNLEFT;
			printf("motto left t!\n");
			printf("pit of x+1 : %0.2f, wump : %d\n", space[alocal.X+1][alocal.Y].Pit, space[alocal.X+1][alocal.Y].Wumpus);
		}
		else{
			action = TURNRIGHT;
			printf("motto right t!\n");
		}
	}else if(ori == LEFT && ((space[alocal.X-1][alocal.Y].Wumpus > 0 && scream == false) || space[alocal.X-1][alocal.Y].Pit > .5)){
		if(space[alocal.X][alocal.Y+1].Pit <= space[alocal.X][alocal.Y-1].Pit || space[alocal.X][alocal.Y+1].Wumpus <= space[alocal.X][alocal.Y-1].Wumpus){
			action = TURNRIGHT;
			printf("gotto left t!\n");
		}
		else{
			action = TURNLEFT;
			printf("gotto right t!\n");
		}
	}else if(ori == RIGHT && space[alocal.X+1][alocal.Y].Wumpus > 0 && scream == false && alocal.X-1 > 0 && space[alocal.X][alocal.Y+1].Wumpus > 0){
		action = TURNRIGHT;
		printf("This lef right t!\n");
	}else if(ori == LEFT && space[alocal.X-1][alocal.Y].recent > space[alocal.X][alocal.Y-1].recent && space[alocal.X][alocal.Y-1].Pit < .5 && alocal.Y-1 > 0){
		action = TURNLEFT;
		printf("Left1!\n");
	}else if(ori == RIGHT && space[alocal.X+1][alocal.Y].recent > space[alocal.X][alocal.Y-1].recent && space[alocal.X][alocal.Y-1].Pit < .5 && alocal.Y-1 > 0){
		action = TURNRIGHT;
		printf("Left1!\n");
	}else if(ori == LEFT && space[alocal.X-1][alocal.Y].recent > space[alocal.X][alocal.Y+1].recent && space[alocal.X][alocal.Y+1].Pit < .5 && alocal.Y+1 < length + 1){
		action = TURNRIGHT;
		printf("Left1!\n");
	}else if(ori == RIGHT && space[alocal.X+1][alocal.Y].recent > space[alocal.X][alocal.Y+1].recent && space[alocal.X][alocal.Y+1].Pit < .5 && alocal.Y+1 < length + 1){
		action = TURNLEFT;
		printf("Left1!\n");
	}else if(ori == UP && tsize == true && alocal.Y <= length/2){
		action = TURNRIGHT;
		printf("This up right turn t!\n");
	// }else if(ori == UP && tsize == true && alocal.Y > 1){
	// 	action = TURNLEFT;
	// 	printf("This up left turn t!\n");
	// }
	}else if(ori == DOWN && space[alocal.X][alocal.Y-1].Wumpus > 0 && space[alocal.X-1][alocal.Y].Wumpus <= space[alocal.X][alocal.Y-1].Wumpus && scream == false){
		action = TURNRIGHT;
		walll = 0;
		wallr = 0;
		stck1 = 0;
		stck2 = 0;
		printf("is it this right999999\n");
	}else if(ori == DOWN && space[alocal.X][alocal.Y-1].Wumpus > 0 && space[alocal.X+1][alocal.Y].Wumpus <= space[alocal.X][alocal.Y-1].Wumpus && scream == false){
		action = TURNLEFT;
		walll = 0;
		wallr = 0;
		stck1 = 0;
		stck2 = 0;
		printf("Maybe its here! left\n");
	}else if(ori == DOWN && space[alocal.X][alocal.Y-1].Pit > .5 && space[alocal.X+1][alocal.Y].Pit <= space[alocal.X][alocal.Y-1].Pit && alocal.X + 1 < width + 1){
		action = TURNLEFT;
		walll = 0;
		wallr = 0;
		stck1 = 0;
		stck2 = 0;
		printf("This is new! left\n");
	}else if(ori == DOWN && space[alocal.X][alocal.Y-1].Pit > .5 && space[alocal.X-1][alocal.Y].Pit <= space[alocal.X][alocal.Y-1].Pit){
		action = TURNRIGHT;
		walll = 0;
		wallr = 0;
		stck1 = 0;
		stck2 = 0;
		printf("This is new! Right\n");
	}else if(ori == RIGHT && ((space[alocal.X+1][alocal.Y].Wumpus > 0 && scream == false) || (space[alocal.X+1][alocal.Y].Pit >= .5))){
		if(space[alocal.X][alocal.Y+1].recent <= space[alocal.X][alocal.Y-1].recent){
			action = TURNLEFT;
			printf("Your Left for whatever reason!\n");
		}
		else{
			action = TURNRIGHT;
			printf("Your Right for whatever reason!\n");
		}
	}else if(ori == LEFT && ((space[alocal.X-1][alocal.Y].Wumpus > 0 && scream == false) || space[alocal.X-1][alocal.Y].Pit >= .5) && alocal.X-1 > 0){
		if(discover == true){
			action = TURNLEFT;
			printf("Or this left!\n");
		}else{
			action = TURNRIGHT;
			printf("is it this right!\n");
		}	

	}else if(ori == UP && space[alocal.X][alocal.Y+1].Wumpus > 0 && scream == false){
		if((space[alocal.X+1][alocal.Y].Wumpus < space[alocal.X-1][alocal.Y].Wumpus && alocal.X + 1 <= width) || (space[alocal.X-1][alocal.Y].Wumpus < space[alocal.X+1][alocal.Y].Wumpus && alocal.X - 1 == 0))
			{action = TURNRIGHT;
			printf("this rightios\n");
		}else
			action = TURNLEFT;
		
		walll = 0;
		wallr = 0;
		stck1 = 0;
		stck2 = 0;
	}else if(ori == UP && space[alocal.X][alocal.Y+1].Pit >= .5 && (space[alocal.X+1][alocal.Y].Pit < .5 || space[alocal.X-1][alocal.Y].Pit < .5)){
		if(space[alocal.X+1][alocal.Y].Pit < space[alocal.X-1][alocal.Y].Pit){
			action = TURNRIGHT;
			printf("magically this one\n");
		}
		else{
			action = TURNLEFT;
			printf("magically other left\n");
		}
		
		walll = 0;
		wallr = 0;
		stck1 = 0;
		stck2 = 0;
	}else if(ori == DOWN && space[alocal.X][alocal.Y-1].Wumpus > 0 && scream == false){
		action = TURNLEFT;
		printf("hello\n");
	}else if (((percept.Bump == true || wallr > 0 || walll > 0) && discover == false) || stck2 > 0){
			if(ori == RIGHT || wallr > 0){
				action = wallrt[wallr];
				wallr++;
				stck2++;
			} else if(ori == LEFT || walll > 0){
				action = walllt[walll];
				walll++;
				stck2++;
			} else if (ori == UP){
				action = TURNRIGHT;
				printf("right?\n");
			} else if (ori == DOWN){
				action = TURNLEFT;
			}
			
	}else if (((percept.Bump == true || wallr > 0 || walll > 0) && discover == true) || stck1 > 0){
			if(ori == RIGHT || walll > 0){
				action = walllt[walll];
				walll++;
				stck1++;
			} else if(ori == LEFT || wallr > 0){
				action = wallrt[wallr];
				wallr++;
				stck1++;
			} else if (ori == UP){
				action = TURNRIGHT;
				printf("Left jk right\n");
			} else if (ori == DOWN){
				action = TURNLEFT;
			}
			
	}else{
		action = GOFORWARD;
	}
}
//printf("did we get here4\n\n");

	if(tsize == false){
		if(alocal.X == width)
			width = alocal.X + 1;
		if(alocal.Y == length)
			length = alocal.Y + 1;
	}

	if(wallr > 2) wallr = 0;
	if(walll > 2) walll = 0;
	if(goldf > 2) goldf = 0;
	if(firsts > 1) firsts = 0;
	if(stck1 > 2) stck1 = 0;
	if(stck2 > 2) stck2 = 0;
	//printf("orientation : %x\n", ori);

if(wstate.agentHasGold == false){
	if(re >= rem_len){
		int old_len = rem_len;
		rem_len = rem_len * 2;
		Action* newarray = new Action[rem_len];
		for(int y = 0; y < old_len; y++)
		{
			newarray[y] = goback[y];
		}
		for(int y = old_len; y < rem_len; y++)
		{
			newarray[y] = CLIMB;
		}
		delete [] goback;
		goback = newarray;
		delete [] newarray;
		Locations* newarray2 = new Locations[rem_len];
		for(int y = 0; y < old_len; y++)
		{
			newarray2[y].X = locations[y].X;
			newarray2[y].Y = locations[y].Y;
		}
		for(int y = old_len; y < rem_len; y++)
		{
			newarray2[y].X = 0;
			newarray2[y].Y = 0;
		}
		delete [] locations;
		locations = newarray2;
		delete [] newarray2;
		Orientation* newarray3 = new Orientation[rem_len];
		for(int y = 0; y < old_len; y++)
		{
			newarray3[y] = ori_goback[y];
		}
		for(int y = old_len; y < rem_len; y++)
		{
			newarray3[y] = RIGHT;
		}
		delete [] ori_goback;
		ori_goback = newarray3;
		delete [] newarray3;
		printf("Array's have been resized\nLast stored value in re : %d\nNew array size : %d\n", re, rem_len);
		// printf("Press any key to continue.\n");
		// getchar();
	}
	ori_goback[re] = ori;
	locations[re].X = alocal.X;
	locations[re].Y = alocal.Y;
	printf("Location (X : %d) (Y : %d)\n", locations[re].X, locations[re].Y);
	goback[re] = action;
	if(action == GOFORWARD)
	printf("Added to stack: GOFORWARD\n");
	if(action == TURNLEFT)
	printf("Added to stack: TURNLEFT\n");
	if(action == TURNRIGHT)
	printf("Added to stack: TURNRIGHT\n");
	if(action == SHOOT){
	printf("Nothing added to stack\n");
	re--;
	}
	if(action == GRAB)
	printf("Added to stack: GRAB\n");
	re++;
}
//printf("did we get here5\n\n");
	// int i = 0;
	// while(i < 10){
	// 	printf("goback[%d] entry : %x\n", i, goback[i]);
	// 	i++;
	// }

	//printf("re = %d\n", re);
	printf("Press any key to continue.\n");
	getchar();

	orientationupdater(action);
	return action;
}

void Agent::GameOver (int score)
{

}

void Agent::orientationupdater(Action& action)
{

	if(ori == RIGHT && action == TURNLEFT){
		ori = UP;
	} else if(ori == UP && action == TURNLEFT){
		ori = LEFT;
	} else if(ori == LEFT && action == TURNLEFT){
		ori = DOWN;
	} else if(ori == DOWN && action == TURNLEFT){
		ori = RIGHT;
	} else if(ori == RIGHT && action == TURNRIGHT){
		ori = DOWN;
	} else if(ori == UP && action == TURNRIGHT){
		ori = RIGHT;
	} else if(ori == LEFT && action == TURNRIGHT){
		ori = UP;
	} else if(ori == DOWN && action == TURNRIGHT){
		ori = LEFT;
	} else if(ori == RIGHT && action == GOFORWARD){
		alocal.X = alocal.X + 1;
	} else if(ori == UP && action == GOFORWARD){
		alocal.Y = alocal.Y + 1;
	} else if(ori == LEFT && action == GOFORWARD){
		alocal.X = alocal.X - 1;
	} else if(ori == DOWN && action == GOFORWARD){
		alocal.Y = alocal.Y - 1;
	}
}


void Agent::pathoptimization() // currently not working
{
	Locations temp;
	re = re + 1;

	for(int i = 0; i < re; i++)
	{
		Action action = goback[i];
		temp = locations[i];

		printf("Spot %d, Location %d,%d, Orientation : ", i, temp.X, temp.Y);

		if(ori_goback[i] == RIGHT)
		printf("RIGHT, Action : ");
		if(ori_goback[i] == LEFT)
		printf("LEFT, Action : ");
		if(ori_goback[i] == UP)
		printf("UP, Action : ");
		if(ori_goback[i] == DOWN)
		printf("DOWN, Action : ");

		if(action == GOFORWARD)
		printf("GOFORWARD\n");
		if(action == TURNLEFT)
		printf("TURNLEFT\n");
		if(action == TURNRIGHT)
		printf("TURNRIGHT\n");
		if(action == SHOOT)
		printf("SHOOT\n");
		if(action == CLIMB)
		printf("CLIMB\n");
	}

	for(int i = 0; i < re + 1; i++)
	{
		Action action = goback[i];
		temp = locations[i];

		for(int y = i + 1; y < re; y++)
		{
			//printf("Checking spot %d with spot %d\n", i, y);
			if(locations[y].X == temp.X && locations[y].Y == temp.Y && goback[y] != TURNLEFT && goback[y] != TURNRIGHT)
			{
				printf("Found short cut from %d,%d to %d,%d\n", temp.X, temp.Y, locations[y].X, locations[y].Y);
				printf("First number location = %d\n", i);
				printf("Second number location = %d\n", y);
				int shiftamt = (y) - (i + 1);
				printf("Shift amount = %d\n", shiftamt);
				for(int u = i + 1; u < re; u++)
				{
					locations[u] = locations[u + shiftamt];
					printf("Changed Locations : %d,%d with Locations : %d,%d\n", locations[u].X, locations[u].Y, locations[u + shiftamt].X, locations[u + shiftamt].Y);
				 	goback[u] = goback[u + shiftamt];
					ori_goback[u] = ori_goback[u + shiftamt];
				}
				if(ori_goback[i + 1] == RIGHT)
					goback[i + 1] = TURNRIGHT;
				else if(ori_goback[i + 1] == LEFT)
					goback[i + 1] = TURNLEFT;

					
				// if(goback[i + 1] == TURNRIGHT && goback[i + 2] == TURNLEFT){
				// 	for(int u = i; u < re; u++)
				// 	{
				// 		locations[u] = locations[u + 2];
				//  		goback[u] = goback[u + 2];
				// 		ori_goback[u] = ori_goback[u + 2];
				// 	}
				// }
				// else if(goback[i + 1] == TURNLEFT && goback[i + 2] == TURNRIGHT){
				// 	for(int u = i; u < re; u++)
				// 	{
				// 		locations[u] = locations[u + 2];
				//  		goback[u] = goback[u + 2];
				// 		ori_goback[u] = ori_goback[u + 2];
				// 	}
				// }

				printf(" I = %d \n", i);
				re = re - shiftamt;

				for(int i = 0; i < re; i++)
				{

				printf("Spot %d, Location %d,%d, Orientation : ", i, temp.X, temp.Y);

				if(ori_goback[i] == RIGHT)
				printf("RIGHT, Action : ");
				if(ori_goback[i] == LEFT)
				printf("LEFT, Action : ");
				if(ori_goback[i] == UP)
				printf("UP, Action : ");
				if(ori_goback[i] == DOWN)
				printf("DOWN, Action : ");

				if(action == GOFORWARD)
				printf("GOFORWARD\n");
				if(action == TURNLEFT)
				printf("TURNLEFT\n");
				if(action == TURNRIGHT)
				printf("TURNRIGHT\n");
				if(action == SHOOT)
				printf("SHOOT\n");
				if(action == CLIMB)
				printf("CLIMB\n");
				}
				i++;
				break;
			}
		}
	} // end of the optimization for loops

	// for(int i = 0; i < re; i++)
	// {
	// Action action = goback[i];

	// if(goback[i] == TURNRIGHT && goback[i + 1] == TURNLEFT){
	// 	for(int u = i; u < re; u++)
	// 	{
	// 		locations[u] = locations[u + 2];
	//  		goback[u] = goback[u + 2];
	// 		ori_goback[u] = ori_goback[u + 2];
	// 	}
	// }
	// else if(goback[i] == TURNLEFT && goback[i + 1] == TURNRIGHT){
	// for(int u = i; u < re; u++)
	// 	{
	// 		locations[u] = locations[u + 2];
	//  		goback[u] = goback[u + 2];
	// 		ori_goback[u] = ori_goback[u + 2];
	// 	}
	// }

	// printf("Spot %d, Location %d,%d : %d, Orientation : ", i, temp/10, temp%10, locations[i]);
	
	// if(ori_goback[i] == RIGHT)
	// 	printf("RIGHT, Action : ");
	// if(ori_goback[i] == LEFT)
	// 	printf("LEFT, Action : ");
	// if(ori_goback[i] == UP)
	// 	printf("UP, Action : ");
	// if(ori_goback[i] == DOWN)
	// 	printf("DOWN, Action : ");

	// if(action == GOFORWARD)
	// 	printf("GOFORWARD\n");
	// if(action == TURNLEFT)
	// 	printf("TURNLEFT\n");
	// if(action == TURNRIGHT)
	// 	printf("TURNRIGHT\n");
	// if(action == SHOOT)
	// 	printf("SHOOT\n");
	// if(action == CLIMB)
	// 	printf("CLIMB\n");

	// i++;
	// }


	// printf("re = %d\n", re);

	// for(int i = 0; i < re; i++)
	// {
	// 	Action action = goback[i];
	// 	temp = locations[i];

	// 	for(int y = i + 1; y < re; y++)
	// 	{
	// 		printf("Checking spot %d with spot %d\n", i, y);
	// 		if(locations[y] == temp + 1 && ori_goback[y] != UP)
	// 		{
	// 			printf("Found short cut from %d,%d to %d,%d\n", temp/10, temp%10, locations[y]/10, locations[y]%10);
	// 			printf("First number location = %d\n", i);
	// 			printf("Second number location = %d\n", y);
	// 			int shiftamt = (y - 1) - (i + 1);
	// 			printf("Shift amount = %d\n", shiftamt);
	// 			for(int u = i + 1; u < re; u++)
	// 			{
	// 				locations[u] = locations[u + shiftamt];
	// 			 	goback[u] = goback[u + shiftamt];
	// 				ori_goback[u] = ori_goback[u + shiftamt];
	// 			}
	// 			printf(" I = %d \n", i);
	// 			if(ori_goback[i] == RIGHT){
	// 				locations[i + 1] = locations[i];
	// 				goback[i + 1] = TURNLEFT;
	// 				ori_goback[i + 1] = UP;
	// 				ori_goback[i + 2] = UP;
	// 			} else if(ori_goback[i] == LEFT){
	// 				locations[i + 1] = locations[i];
	// 				goback[i + 1] = TURNRIGHT;
	// 				ori_goback[i + 1] = UP;
	// 				ori_goback[i + 2] = UP;
	// 			} else if(ori_goback[i] == UP){
	// 				for(int u = i + 1; u < re; u++)
	// 				{
	// 					locations[u] = locations[u + 1];
	// 			 		goback[u] = goback[u + 1];
	// 					ori_goback[u] = ori_goback[u + 1];
	// 				}
					
	// 				re--;
	// 				ori_goback[i + 1] = UP;
	// 			}

	// 			re = re - shiftamt;

	// 			// if(locations[i + 2] == locations[i + 1] - 10 && goback[i + 2] == GOFORWARD){
	// 			// 	for(int u = re + 1; u > i + 1; u--)
	// 			// 	{
	// 			// 		locations[u] = locations[u - 1];
	// 			//  		goback[u] = goback[u - 1];
	// 			// 		ori_goback[u] = ori_goback[u - 1];
	// 			// 	}
	// 			// 	re++;
	// 			// 	locations[i + 2] = locations[i + 1];
	// 			// 	goback[i + 2] = TURNLEFT;
	// 			// 	ori_goback[i + 2] = LEFT;
	// 			// }
	// 			// else if(locations[i + 2] == locations[i + 1] + 10 && goback[i + 2 == GOFORWARD]){
	// 			// 	for(int u = re + 1; u > i + 1; u--)
	// 			// 	{
	// 			// 		locations[u] = locations[u - 1];
	// 			//  		goback[u] = goback[u - 1];
	// 			// 		ori_goback[u] = ori_goback[u - 1];
	// 			// 	}
	// 			// 	re++;
	// 			// 	locations[i + 2] = locations[i + 1];
	// 			// 	goback[i + 2] = TURNRIGHT;
	// 			// 	ori_goback[i + 2] = RIGHT;
	// 			// }
	// 			// if((locations[i + 2] == locations[i + 1] + 1 || locations[i + 2] == locations[i + 1] - 1) && (goback[i + 2] == TURNLEFT || goback[i + 2] == TURNRIGHT))
	// 			// 	goback[i+2] = GOFORWARD;


	// 			for(int i = 0; i < re; i++)
	// 			{
	// 			Action action = goback[i];
	// 			temp = locations[i];

	// 			printf("Spot %d, Location %d,%d : %d, Orientation : ", i, temp/10, temp%10, locations[i]);

	// 			if(ori_goback[i] == RIGHT)
	// 			printf("RIGHT, Action : ");
	// 			if(ori_goback[i] == LEFT)
	// 			printf("LEFT, Action : ");
	// 			if(ori_goback[i] == UP)
	// 			printf("UP, Action : ");
	// 			if(ori_goback[i] == DOWN)
	// 			printf("DOWN, Action : ");

	// 			if(action == GOFORWARD)
	// 			printf("GOFORWARD\n");
	// 			if(action == TURNLEFT)
	// 			printf("TURNLEFT\n");
	// 			if(action == TURNRIGHT)
	// 			printf("TURNRIGHT\n");
	// 			if(action == SHOOT)
	// 			printf("SHOOT\n");
	// 			if(action == CLIMB)
	// 			printf("CLIMB\n");
	// 			}
	// 			i++;
	// 			break;
	// 		}
	// 		// if(locations[y] == temp && ori_goback[i] == RIGHT && ori_goback[y] == LEFT)
	// 		// {

	// 		// }
	// 		// if(locations[y] == temp && ori_goback[i] == LEFT && ori_goback[y] == RIGHT)
	// 		// {

	// 		// }
	// 	}
	//}
	// printf("re = %d\n", re);
	//re++;
}

// void printCombination(int prob[], int n, int C)
// {
// 	int data[C];

// 	combinUtil(prob, data, n, C, 0, 0);
// 	printf("prob, data, 0 , n-1 : %d, 0 , C : %d\n", n-1, C);
// }

// void combinUtil(int prob[], int data[], int n, int C, int index, int i)
// {
// 	printf("index : %d\n", index);
// 	printf("I : %d\n", i);
// 	printf("N : %d\n", n);
// 	printf("end : %d\n", C);
// 	if(index == C)
// 	{
// 		for(int j = 0; j < C; j++)
// 			printf("%d ", data[j]);
// 		printf("\n");
// 		return;
// 	}

// 	int start = n;
// 	int end = n-1;
// 	// if (i >= n)
// 	// 	return;

// 	// data[index] = prob[i];
// 	// combinUtil(prob, data, n, C, index + 1, i + 1);
// 	for(int y = start; y <= end && end - y + 1 >= C - index; y++)
// 	{
// 		data[index] = prob[i];
// 		combinUtil(prob, data, i+1, end, index+1, C);
// 	}
// }

// void printCombination(int arr[], int n, int r)  
// {  
//     // A temporary array to store  
//     // all combination one by one  
//     int data[r];  
  
//     // Print all combination using  
//     // temprary array 'data[]'  
//     combinationUtil(arr, n, r, 0, data, 0);  
// }  
  
/* arr[] ---> Input Array  
n ---> Size of input array  
r ---> Size of a combination to be printed  
index ---> Current index in data[]  
data[] ---> Temporary array to store current combination  
i ---> index of current element in arr[] */
// void combinationUtil(int arr[], int n, int r,  
//                     int index, int data[], int i)  
// {  
//     // Current cobination is ready, print it  
//     if (index == r)  
//     {  
//         for (int j = 0; j < r; j++)  
//             cout << data[j] << " ";  
//         cout << endl;  
//         return;  
//     }  
  
//     // When no more elements are there to put in data[]  
//     if (i >= n)  
//         return;  
  
//     // current is included, put next at next location  
//     data[index] = arr[i];  
//     combinationUtil(arr, n, r, index + 1, data, i + 1);  
  
//     // current is excluded, replace it with next (Note that  
//     // i+1 is passed, but index is not changed)  
//     combinationUtil(arr, n, r, index, data, i+1);  
// }  
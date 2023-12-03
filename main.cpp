/********* CGM Project  **********

12/12/2021

2D Game - Snakes & Ladders

Group Members:

Romy Savin Peter - S20190010153
Emma Mary Cyriac - S20190010048
Riya Rajesh - S20190010152
Sai Krishna Kotina - S20190010105

*********************************/

/***** All header files needed by the program are defined below  *****/

//Required OpenGL header files
#include <GL/glut.h>
#include <GL/glext.h>

//Required standard C/C++ header files
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>

//Custom user defined header file required to load images (PNG)
#include "lodepng.h"

using namespace std;

/***** All variables that are used by the program are defined below  *****/

//The below defined variables are used for window transitions and rendering
int windowWidth;
int windowHeight;
bool window1 = false;
bool window2 = false;
bool window3 = false;
bool window4 = false;

//The below defined variables are used for gameplay
int flag;	                                //Observes if the mouse input is a right click or not
int n = 0;	                                //Stores the image loading flag
float spin;	                                //Stores the spinning factor of the cube
int winner;	                                //Stores the winning player number
int dice[4];	                            //Stores the dice values of each player
int dicenum;	                            //Stores the dice throw value
int numplayers = 0;	                        //Stores the number of players
int pc_counter = 1;	                        //Stores chances condition factor
void *currentfont;	                        //Stores the address of the font
int set_pointer = 0;	                    //Set program counter
int select_flag = 0;	                    //Stores the number of places as specified by the user
int snake_pos[101];	                        //Stores the count of snake heads in the mesh
int stair_pos[101];	                        //Stores bottom of the ladders in the mesh
int currentplayer = 1;	                    //Stores the value of current player flag
int dice_position = -1;	                    //Stores the value of dice movement
int player_sum[4] = { 0 };	                //Stores the current dice sum for every player
float dice_dimension = 50;	                //Stores the dice dimension
int player_flag[4] = { 1, 0, 0, 0 };	    //Stores the player which has current chance
float start[4] = { -70, -70, -70, -70 };	//Start positions of the players
float right_movement[4] = { 0 };	        //Monitors the player's horizontal position
float up_movement[4] = { 0 };	            //Monitors player's vertical position

/***** All image rendering declarations the program requires are defined below  *****/

//The below defined variables are required for viewport and window
int pixelwidth = 700;	//Stores the width of the mesh
int pixelheight = 850;	//Stores the height of the mesh
int WIDTH = 500;
int HEIGHT = 500;

//The below defined vectors are required for storage of images

//Stores the logo image and it's attributes
vector < unsigned char > image_logo;
unsigned logowidth;
unsigned logoheight;

//Stores the board image and it's attributes
vector < unsigned char > image_board;
unsigned boardwidth;
unsigned boardheight;

//Stores the image of the first dice and it's attributes
vector < unsigned char > image_dice1;
unsigned dice1width;
unsigned dice1height;

//Stores the image of the second dice and it's attributes
vector < unsigned char > image_dice2;
unsigned dice2width;
unsigned dice2height;

//Stores the image of the third dice and it's attributes
vector < unsigned char > image_dice3;
unsigned dice3width;
unsigned dice3height;

//Stores the image of the fourth dice and it's attributes
vector < unsigned char > image_dice4;
unsigned dice4width;
unsigned dice4height;

//Stores the image of the fifth dice and it's attributes
vector < unsigned char > image_dice5;
unsigned dice5width;
unsigned dice5height;

//Stores the image of the sixth dice and it's attributes
vector < unsigned char > image_dice6;
unsigned dice6width;
unsigned dice6height;

//Defines the texture to be used
GLuint texname;

/***** All function prototypes involving images are defined below *****/

//The below defined functions are responsible for loading images
void setTexture(vector < unsigned char > img, unsigned width, unsigned height);
void invert(vector < unsigned char > &img, const unsigned width, const unsigned height);
void loadImage(const char *name, int n);

//The below defined functions are responsible for stroke drawing
void drawStrokeText(const char str[250], int x, int y, int z, float p1, float p2);
void setFont(void *font);
void drawstring(float x, float y, char *str);

/***** All user defined function prototypes are defined below *****/

//The below defined functions are required by the first output window (player selection menu)
void windowOne();
void drawoptions();
void selectoptions();

//The below defined function is required by the second output window (rules)
void windowTwo();

//The below defined functions are required by the third output window (gameplay area)
void windowThree();
void drawMesh();
void drawplayer();
void drawdice();
void spinDice();
void gameplay();
void diceimages();
void diceposition();
void check_ladder();
void check_snake();

//The below defined function is required by the fourth output window (displaying the winner)
void windowFour();

/***** All GLUT functions with altered definitions are defined below *****/

static void init(void);
static void idle(void);
static void display(void);
static void key(unsigned char key, int x, int y);
static void specialkeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);

/***** The beginning of the program (a.k.a the main function) is defined below *****/

int main(int argc, char *argv[])
{
	//Responsible for loading the image into memory
	loadImage("logo.png", n);
	n = 1;
	loadImage("board.png", n);

	//Responsible for generating textures
	glGenTextures(1, &texname);

	//GLUT initialization call
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Defines the window attributes
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Snake and Ladders");
	windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	//Other initialization calls
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//Function calls
	init();
	glutFullScreen();
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialkeys);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);

	//Responsible for continuously rendering the buffer
	glutMainLoop();

	return EXIT_SUCCESS;
}

/***** GLUT functions and their user defined definitions are defined below *****/

//This function sets the viewport and camera projection along with co-ordinate system
static void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1000, 0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Responsible for display control of the window and content in it
static void display(void)
{
	if (!window2)
		windowOne();
	else if (!window3)
		windowTwo();
	else if (!window4)
		windowThree();
	else
		windowFour();
}

//Defines the various control keys used by the game
static void key(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q' || key == 27)	//This is the key to quit the game, which can be modified (default: Q)
	{
		exit(1);
	}
	else if (key == 13)	//This key is used to select the number of players, which can be modified (default: Enter)
	{
		window2 = true;
	}
	else if (key == 'p' || key == 'P')	//This is the key to start the game, which can be modified (default: P)
	{
		window3 = true;
	}
}

//Function to select the number of players using left/right key
static void specialkeys(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
	{
		select_flag = (select_flag + 1) % 3;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		select_flag--;
		if (select_flag < 0)
			select_flag = 2;
	}

	printf("\nselect_flag: %d\n", select_flag);
}

//Idle function for when no changes are made in the buffer
static void idle(void)
{
	glutPostRedisplay();
}

//Defines the mouse click buttons, with left click for spinning the dice and right click for stopping it, by default
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			flag = 0;
			if (state == GLUT_DOWN)
			{
				dice_position = -1;
				glutIdleFunc(spinDice);

				if (set_pointer == 0)
				{
					pc_counter = (numplayers - 1);
					set_pointer++;
				}
			}

			break;

		case GLUT_RIGHT_BUTTON:
			flag = 1;
			if (state == GLUT_DOWN)
			{
				dice_position = 2;
				glutIdleFunc(diceposition);
				pc_counter++;
				gameplay();
			}

			break;

		default:
			break;
	}
}

/***** All function definitions involving images are defined below  *****/

/** Function that sets current texture to given image

Parameters:

@param img is the image vector that has already been loaded
@param width is the width of the image
@param height is the  height of image

**/
void setTexture(vector < unsigned char > img, unsigned width, unsigned height)
{
	glBindTexture(GL_TEXTURE_2D, texname);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// This ensures textures have correct brightness. Else, they tend to appear dark
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);
}

/** OpenGL tends to draw images that are flipped vertically. This function inverts the data so that it is displayed correctly.

Parameters:

@param img is the image data vector
@param width is the image width
@param height is the image height

**/
void invert(vector < unsigned char > &img, const unsigned width, const unsigned height)
{
	unsigned char *imageptr = &img[0];
	unsigned char *first = NULL;
	unsigned char *last = NULL;
	unsigned char temp = 0;
	for (int h = 0; h < (int) height / 2; ++h)
	{
		first = imageptr + h *width * 4;
		last = imageptr + (height - h - 1) *width * 4;
		for (int i = 0; i < (int) width * 4; ++i)
		{
			temp = *first;
			*first = *last;
			*last = temp;
			++first;
			++last;
		}
	}
}


//Function responsible for loading input PNG images into memory
void loadImage(const char *name, int n)
{
	//LodePNG Decode is used to decode input PNG images
	int error;
	if (n == 0)
	{
		if ((error = lodepng::decode(image_logo, logowidth, logoheight, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_logo, logowidth, logoheight);
			cout << "\nLogo was loaded successfully.\n";
		}
	}
	else if (n == 1)
	{
		if ((error = lodepng::decode(image_board, boardwidth, boardheight, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
			exit(1);
		}
		else
		{
			invert(image_board, boardwidth, boardheight);
			cout << "\nBoard image was loaded successfully.\n";
		}
	}
	else if (n == 11)
	{
		if ((error = lodepng::decode(image_dice1, dice1width, dice1height, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_dice1, dice1width, dice1height);
			cout << "\nImage of first dice was loaded succesfully.\n";
		}
	}
	else if (n == 12)
	{
		if ((error = lodepng::decode(image_dice2, dice2width, dice2height, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_dice2, dice2width, dice2height);
			cout << "\nImage of second dice was loaded succesfully.\n";
		}
	}
	else if (n == 13)
	{
		if ((error = lodepng::decode(image_dice3, dice3width, dice3height, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_dice3, dice3width, dice3height);
			cout << "\nImage of third dice was loaded succesfully.\n";
		}
	}
	else if (n == 14)
	{
		if ((error = lodepng::decode(image_dice4, dice4width, dice4height, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_dice4, dice4width, dice4height);
			cout << "\nImage of fourth dice was loaded succesfully.\n";
		}
	}
	else if (n == 15)
	{
		if ((error = lodepng::decode(image_dice5, dice5width, dice5height, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_dice5, dice5width, dice5height);
			cout << "\nImage of fifth dice was loaded succesfully.\n";
		}
	}
	else if (n == 16)
	{
		if ((error = lodepng::decode(image_dice6, dice6width, dice6height, name)))
		{
			cout << name << ":" << lodepng_error_text(error) << endl;
		}
		else
		{
			invert(image_dice6, dice6width, dice6height);
			cout << "\nImage of sixth dice was loaded succesfully.\n";
		}
	}
}

/***** The below defined functions are responsible for implementing strokes *****/

//Function responsible for drawing strokes
void drawStrokeText(const char str[250], int x, int y, int z, float p1, float p2)
{
	int i;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(p1, p2, z);

	for (i = 0; str[i] != '\0'; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}

	glPopMatrix();
}

//Function responsible for setting the font
void setFont(void *font)
{
	currentfont = font;
}

//Function responsible for drawing strings
void drawstring(float x, float y, char *str)
{
	char *c;
	glRasterPos2f(x, y);
	for (c = str; *c != '\0'; c++)
		glutBitmapCharacter(currentfont, *c);

}

/***** Given below are various user defined functions related to the gameplay *****/

/***** Functions related to the first window (player selection menu) *****/

//Function responsible for drawing options to select the number of players
void drawoptions()
{
	float cn = windowWidth / 2;
	float transx = windowWidth / 2;

	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(transx, 100, 0);
	glRectf(cn - 75, 50.0, cn + 75, 150.0);
	glPopMatrix();

	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(transx, 100, 0);
	glRectf(cn - 350, 50, cn - 200, 150);
	glPopMatrix();

	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(transx, 100, 0);
	glRectf(cn + 200, 50, cn + 350, 150);
	glPopMatrix();
}

//Function responsible for displaying player selection buttons
void selectoptions()
{
	float cn = windowWidth / 2;
	float fontsize = 0.13;
	float transx = windowWidth / 2;

	//Text for player selection buttons are defined here
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslatef(transx, 100, 0);
	drawStrokeText("2 Players", cn - 335.0, 100.0, 0.0, fontsize, fontsize);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(transx, 100, 0);
	drawStrokeText("3 Players", cn - 60.0, 100.0, 0.0, fontsize, fontsize);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(transx, 100, 0);
	drawStrokeText("4 Players", cn + 215.0, 100.0, 0.0, fontsize, fontsize);
	glPopMatrix();

	if (select_flag == 0)
	{
		glPushMatrix();
		glTranslatef(transx, 100, 0);
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(cn - 355, 45.0);
		glVertex2f(cn - 355, 155.0);
		glVertex2f(cn - 195, 155.0);
		glVertex2f(cn - 195, 45.0);
		glEnd();
		glPopMatrix();
		numplayers = select_flag + 2;
		printf("\nThe number of players: %d\n", numplayers);
	}
	else if (select_flag == 1)
	{
		glPushMatrix();
		glTranslatef(transx, 100, 0);
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(cn - 80, 45.0);
		glVertex2f(cn - 80, 155.0);
		glVertex2f(cn + 80, 155.0);
		glVertex2f(cn + 80, 45.0);
		glEnd();
		glPopMatrix();
		numplayers = select_flag + 2;
		printf("\nThe number of players: %d\n", numplayers);
	}
	else if (select_flag == 2)
	{
		glPushMatrix();
		glTranslatef(transx, 100, 0);
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(cn + 355, 45.0);
		glVertex2f(cn + 355, 155.0);
		glVertex2f(cn + 195, 155.0);
		glVertex2f(cn + 195, 45.0);
		glEnd();
		glPopMatrix();
		numplayers = select_flag + 2;
		printf("\nThe number of players: %d\n", numplayers);
	}
}

//Display function for the first window (player selection menu)
void windowOne()
{
	glClear(GL_COLOR_BUFFER_BIT);
	float scale = 0.70;
	drawoptions();
	selectoptions();
	glPushMatrix();
	//Image begins here
	glEnable(GL_TEXTURE_2D);
	setTexture(image_logo, logowidth, logoheight);
	glPushMatrix();
	glTranslatef(300, 500, 0);
	glScalef(scale, scale, 1);
	glBegin(GL_POLYGON);
	glTexCoord2d(0, 0);
	glVertex2f(0, 0);
	glTexCoord2d(0, 1);
	glVertex2f(0, logoheight);
	glTexCoord2d(1, 1);
	glVertex2f(logowidth, logoheight);
	glTexCoord2d(1, 0);
	glVertex2f(logowidth, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//Image ends here
	glPopMatrix();
	glutSwapBuffers();
}

/*****  Functions related to the second window (rules) *****/

//Display function for the second window (rules)
void windowTwo()
{
	glClear(GL_COLOR_BUFFER_BIT);
	float xpos = windowWidth / 5;
	float ypos = windowHeight *3 / 4;
	float xtrans = windowWidth / 6;
	float ytrans = windowHeight;
	float fontsize = 0.13;

	glPushMatrix();
	glTranslatef(xtrans, ytrans, 0);
	glLineWidth(3.0);
	glColor3f(0.0, 1.0, 0.0);
	drawStrokeText("Snakes & Ladders - The Game of Chance", xpos, ypos, 0, 0.210, 0.210);
	glBegin(GL_LINES);
	glVertex2f(xpos, ypos - 15);
	glVertex2f(xpos + 620, ypos - 15);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xtrans, ytrans, 0);
	glColor3f(0.698, 0.133, 0.133);
	glLineWidth(2.0);
	drawStrokeText("Here's a Snakes & Ladders board game to play with your friends and family. ", xpos - 150, ypos - 100, 0, 0.17, 0.17);
	glColor3f(1.0, 1.0, 0.0);
	drawStrokeText("RULES:", xpos *2.50 + 80, ypos - 180, 0, 0.17, 0.17);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xtrans, ytrans, 0);
	glColor3f(0.0, 0.8, 1.0);
	glLineWidth(1.3);
	drawStrokeText("1. The objective of the game is to get to the number 100, which is the final destination.", 0, ypos - 250, 0, fontsize, fontsize);
	drawStrokeText("2. Each player puts their coin on the space near the arrow. Dice roll of 1 or 6 starts the game.", 0, ypos - 300, 0, fontsize, fontsize);
	drawStrokeText("3. Take turns to roll the dice. The coin moves the number of spaces as shown on the dice.", 0, ypos - 350, 0, fontsize, fontsize);
	drawStrokeText("4. Left click starts the dice roll and right click stops it.", 0, ypos - 400, 0, fontsize, fontsize);
	drawStrokeText("5. If your coin lands at the bottom of a ladder, you can move up to the top of the ladder.", 0, ypos - 450, 0, fontsize, fontsize);
	drawStrokeText("6. If your coin lands on the head of a snake, you must slide down to the bottom of the snake.", 0, ypos - 500, 0, fontsize, fontsize);
	drawStrokeText("7. The first player to get to the space that says '100' is the winner!", 0, ypos - 550, 0, fontsize, fontsize);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xtrans, ytrans, 0);
	glColor3f(1.000, 0.843, 0.000);
	drawStrokeText("Press P to Play.                  Press Q to Quit.", xpos *1.1, ypos - 750, 0, fontsize + 0.03, fontsize + 0.03);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

/***** Functions related to the third window (gameplay area) *****/

//The below defined function is responsible for drawing meshes over the given board
void drawMesh()
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(31, 81, -50.0);
	glPointSize(10.0);
	glScalef(0.9, 1, 1);
	for (int i = 0; i < pixelwidth; i += 70)
		for (int j = 0; j < pixelheight; j += 85)
		{
			glPointSize(4.0);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINE_LOOP);
			glVertex3f(i, j, 50);
			glVertex3f(i, j + 85, 50);
			glVertex3f(i + 70, j + 85, 50);
			glVertex3f(i + 70, j, 50);
			glEnd();
		}
}

//The below defined function is responsible for randomnly generating the dice number for each throw
int generate_num()
{
	int chancenum;
	srand(time(NULL));
	chancenum = ((rand() % 6) + 1);

	if (chancenum == 0)
		dicenum = generate_num();
	else
		dicenum = chancenum;
	printf("\nDice number: %d\n", dicenum);
	return dicenum;
}

//The below defined function is responsible for drawing each player's coin (circular in shape)
void drawplayer()
{
	int pi = 3.14;
	float theta = 0, radius = 25;
	glPointSize(200.0);

	//Player 1
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; i++)
	{
		glVertex3f((radius* cos((pi / (float) 180) *theta)) + 35 + right_movement[0] + start[0], (radius* sin((pi / (float) 180) *theta)) + 42.5 + up_movement[0], -100);
		theta = theta + 1;
	}

	glEnd();

	//Player 2
	glPointSize(200.0);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; i++)
	{
		glVertex3f((radius* cos((pi / (float) 180) *theta)) + 35 + right_movement[1] + start[1], (radius* sin((pi / (float) 180) *theta)) + 42.5 + up_movement[1], -100);
		theta = theta + 1;
	}

	glEnd();

	//Player 3
	glPointSize(200.0);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; i++)
	{
		glVertex3f((radius* cos((pi / (float) 180) *theta)) + 35 + right_movement[2] + start[2], (radius* sin((pi / (float) 180) *theta)) + 42.5 + up_movement[2], -100);
		theta = theta + 1;
	}

	glEnd();

	//Player 4
	glPointSize(200.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; i++)
	{
		glVertex3f((radius* cos((pi / (float) 180) *theta)) + 35 + right_movement[3] + start[3], (radius* sin((pi / (float) 180) *theta)) + 42.5 + up_movement[3], -100);
		theta = theta + 1;
	}

	glEnd();
}

//The below defined function is responsible for drawing each face of the dice
void drawdice()
{
	glColor3f(1, 0, 1);

	glBegin(GL_QUADS);
	//Top face
	glColor3f(1, 0, 1);
	glVertex3f(-dice_dimension, dice_dimension, +dice_dimension);
	glVertex3f(dice_dimension, dice_dimension, +dice_dimension);
	glVertex3f(dice_dimension, dice_dimension, -dice_dimension);
	glVertex3f(-dice_dimension, dice_dimension, -dice_dimension);

	//Bottom face
	glColor3f(1, 0, 0);
	glVertex3f(-dice_dimension, -dice_dimension, dice_dimension);
	glVertex3f(dice_dimension, -dice_dimension, dice_dimension);
	glVertex3f(dice_dimension, -dice_dimension, -dice_dimension);
	glVertex3f(-dice_dimension, -dice_dimension, -dice_dimension);

	//Left face
	glColor3f(0, 1, 0);
	glVertex3f(-dice_dimension, -dice_dimension, dice_dimension);
	glVertex3f(-dice_dimension, -dice_dimension, -dice_dimension);
	glVertex3f(-dice_dimension, dice_dimension, -dice_dimension);
	glVertex3f(-dice_dimension, dice_dimension, dice_dimension);

	//Right face
	glColor3f(1, 1, 0);
	glVertex3f(dice_dimension, -dice_dimension, dice_dimension);
	glVertex3f(dice_dimension, -dice_dimension, -dice_dimension);
	glVertex3f(dice_dimension, dice_dimension, -dice_dimension);
	glVertex3f(dice_dimension, dice_dimension, dice_dimension);

	//Front face
	glColor3f(0, 1, 1);
	glVertex3f(dice_dimension, -dice_dimension, dice_dimension);
	glVertex3f(dice_dimension, dice_dimension, dice_dimension);
	glVertex3f(-dice_dimension, dice_dimension, dice_dimension);
	glVertex3f(-dice_dimension, -dice_dimension, dice_dimension);

	//Back face
	glColor3f(0, 0, 1);
	glVertex3f(dice_dimension, -dice_dimension, -dice_dimension);
	glVertex3f(dice_dimension, dice_dimension, -dice_dimension);
	glVertex3f(-dice_dimension, dice_dimension, -dice_dimension);
	glVertex3f(-dice_dimension, -dice_dimension, -dice_dimension);

	glEnd();
}

//The below defined function is responsible for spinning the dice
void spinDice()
{
	spin = spin + 50.0;
	if (spin > 360)
		spin -= 359;
	glutPostRedisplay();
}

void gameplay()
{
	//Definition position co-ordinates for snakes & ladders.

	//Co-ordinates of snake positions = ({16,6},{47,26},{49,30},{56,53},{62,19},{63,60},{87,24},{93,73},{95,75},{98,75})

	//Co-ordinate of ladder positions= ({1,38},{4,14},{9,31},{21,42},{28,84},{36,44},{51,67},{71,91},{80,100})

	stair_pos[1] = 38;
	stair_pos[4] = 14;
	stair_pos[9] = 31;
	stair_pos[21] = 42;
	stair_pos[28] = 84;
	stair_pos[36] = 44;
	stair_pos[51] = 67;
	stair_pos[71] = 91;
	stair_pos[80] = 100;

	snake_pos[16] = 6;
	snake_pos[47] = 26;
	snake_pos[49] = 30;
	snake_pos[56] = 53;
	snake_pos[62] = 19;
	snake_pos[63] = 60;
	snake_pos[87] = 24;
	snake_pos[93] = 73;
	snake_pos[95] = 75;
	snake_pos[98] = 78;

	if (player_flag[((pc_counter) % numplayers)] == 1)
	{
		printf("%d-->", player_sum[pc_counter % numplayers]);
		dice[((pc_counter) % numplayers)] = generate_num();

		if ((player_sum[((pc_counter) % numplayers)] + dice[((pc_counter) % numplayers)]) > 100)
		{
			player_flag[((pc_counter) % numplayers)] = 0;
			player_flag[((pc_counter + 1) % numplayers)] = 1;
		}

		if ((player_sum[((pc_counter) % numplayers)] + dice[((pc_counter) % numplayers)]) < 100 && (start[((pc_counter) % numplayers)] == 0))
		{
			player_sum[((pc_counter) % numplayers)] += dice[((pc_counter) % numplayers)];

			if (stair_pos[(player_sum[((pc_counter) % numplayers)] + 1)] != 0)
			{
				//Condition if ladder is found
				player_sum[((pc_counter) % numplayers)] = stair_pos[player_sum[((pc_counter) % numplayers)] + 1] - 1;

				if (((player_sum[((pc_counter) % numplayers)] / 10) % 2) != 0)
				{
					right_movement[((pc_counter) % numplayers)] = 70 *(9 - (player_sum[((pc_counter) % numplayers)] % 10));
				}
				else
				{
					right_movement[((pc_counter) % numplayers)] = 70 *(player_sum[((pc_counter) % numplayers)] % 10);
				}

				up_movement[((pc_counter) % numplayers)] = 85 *(player_sum[((pc_counter) % numplayers)] / 10);
				player_flag[((pc_counter) % numplayers)] = 0;
				player_flag[((pc_counter + 1) % numplayers)] = 1;
			}
			else
			{
				if (((player_sum[((pc_counter) % numplayers)] / 10) % 2) != 0)
				{
					right_movement[((pc_counter) % numplayers)] = 70 *(9 - (player_sum[((pc_counter) % numplayers)] % 10));
				}
				else
				{
					right_movement[((pc_counter) % numplayers)] = 70 *(player_sum[((pc_counter) % numplayers)] % 10);
				}

				up_movement[((pc_counter) % numplayers)] = 85 *(player_sum[((pc_counter) % numplayers)] / 10);
				player_flag[((pc_counter) % numplayers)] = 0;
				player_flag[((pc_counter + 1) % numplayers)] = 1;
			}

			if (snake_pos[player_sum[((pc_counter) % numplayers)] + 1] != 0)
			{
				//Condition if snake is found
				player_sum[((pc_counter) % numplayers)] = snake_pos[player_sum[((pc_counter) % numplayers)] + 1] - 1;

				if (((player_sum[((pc_counter) % numplayers)] / 10) % 2) != 0)
				{
					right_movement[((pc_counter) % numplayers)] = 70 *(9 - (player_sum[((pc_counter) % numplayers)] % 10));
				}
				else
				{
					right_movement[((pc_counter) % numplayers)] = 70 *(player_sum[((pc_counter) % numplayers)] % 10);
				}

				up_movement[((pc_counter) % numplayers)] = 85 *(player_sum[((pc_counter) % numplayers)] / 10);
			}

			if (player_sum[((pc_counter) % numplayers)] == 99)
			{
				printf("\nThe winner has been decided!\n");
				window4 = true;
				winner = pc_counter % numplayers;
			}
		}

		if (start[((pc_counter) % numplayers)] == -70)
		{
			if (dice[((pc_counter) % numplayers)] == 6 || dice[((pc_counter) % numplayers)] == 1)
			{
				start[((pc_counter) % numplayers)] = 0;
				player_flag[((pc_counter) % numplayers)] = 0;
				player_flag[((pc_counter + 1) % numplayers)] = 1;
			}
			else
			{
				player_flag[((pc_counter) % numplayers)] = 0;
				player_flag[((pc_counter + 1) % numplayers)] = 1;
			}
		}

		printf("\nProgram counter: %d\n", pc_counter);
	}
}

void diceimages()
{
	if (dicenum == 1)
	{
		n = 11;
		loadImage("dice1.png", n);
	}

	if (dicenum == 2)
	{
		n = 12;
		loadImage("dice2.png", n);
	}

	if (dicenum == 3)
	{
		n = 13;
		loadImage("dice3.png", n);
	}

	if (dicenum == 4)
	{
		n = 14;
		loadImage("dice4.png", n);
	}

	if (dicenum == 5)
	{
		n = 15;
		loadImage("dice5.png", n);
	}

	if (dicenum == 6)
	{
		n = 16;
		loadImage("dice6.png", n);
	}
}

//The below function show the dice face for each player
void diceposition()
{
	spin = 0;

	//Player 1
	if ((pc_counter % numplayers) == 0)
	{
		if (dice[0] == 1)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice1, dice1width, dice1height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[0] == 2)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice2, dice2width, dice2height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[0] == 3)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice3, dice3width, dice3height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[0] == 4)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice4, dice4width, dice4height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[0] == 5)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice5, dice5width, dice5height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[0] == 6)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice6, dice6width, dice6height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}
	}

	//Player 2
	if ((pc_counter % numplayers) == 1)
	{
		if (dice[1] == 1)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice1, dice1width, dice1height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[1] == 2)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice2, dice2width, dice2height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[1] == 3)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice3, dice3width, dice3height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[1] == 4)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice4, dice4width, dice4height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[1] == 5)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice5, dice5width, dice5height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[1] == 6)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice6, dice6width, dice6height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}
	}

	//Player 3
	if ((pc_counter % numplayers) == 2)
	{
		if (dice[2] == 1)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice1, dice1width, dice1height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[2] == 2)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice2, dice2width, dice2height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[2] == 3)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice3, dice3width, dice3height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[2] == 4)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice4, dice4width, dice4height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[2] == 5)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice5, dice5width, dice5height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[2] == 6)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice6, dice6width, dice6height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}
	}

	//Player 4
	if ((pc_counter % numplayers) == 3)
	{
		if (dice[3] == 1)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice1, dice1width, dice1height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[3] == 2)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice2, dice2width, dice2height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[3] == 3)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice3, dice3width, dice3height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[3] == 4)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice4, dice4width, dice4height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[3] == 5)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice5, dice5width, dice5height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

		if (dice[3] == 6)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			setTexture(image_dice6, dice6width, dice6height);
			glPushMatrix();
			glTranslatef(850, 200, 0);
			glScalef(0.9, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2f(60, -60);
			glTexCoord2d(0, 1);
			glVertex2f(60, 60);
			glTexCoord2d(1, 1);
			glVertex2f(-60, 60);
			glTexCoord2d(1, 0);
			glVertex2f(-60, -60);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}
	}
}

//Display function for the third window (gameplay area)
void windowThree()
{
	currentplayer = ((pc_counter + 1) % numplayers) + 1;
	int prev_player = currentplayer - 1;
	if (prev_player == 0)
	{
		if (numplayers == 2)
		{
			prev_player = 2;
		}
		else if (numplayers == 3)
		{
			prev_player = 3;
		}
		else
		{
			prev_player = 4;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	setTexture(image_board, boardwidth, boardheight);
	glPushMatrix();
	glTranslatef(30, 80, 0);
	glScalef(0.9, 1, 1);
	glBegin(GL_POLYGON);
	glTexCoord2d(0, 0);
	glVertex2f(0, 0);
	glTexCoord2d(0, 1);
	glVertex2f(0, pixelheight);
	glTexCoord2d(1, 1);
	glVertex2f(pixelwidth, pixelheight);
	glTexCoord2d(1, 0);
	glVertex2f(pixelwidth, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	drawMesh();
	drawplayer();
	diceimages();

	setFont(GLUT_BITMAP_HELVETICA_18);
	char name[50] = { " TURN OF PLAYER  ------>   " };
	char buffer[10] = { '\0' };
	drawstring(785, 650, name);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(780, 575);
	glVertex2f(1000, 575);
	glVertex2f(1000, 725);
	glVertex2f(780, 725);
	glEnd();

	glPointSize(10.0);
	glColor3f(1.0, 1.0, 0.0);
	sprintf(buffer, "%d", currentplayer);
	drawstring(985, 650, buffer);

	if (flag)
	{
		glColor3f(0.0, 1.0, 0.0);
		setFont(GLUT_BITMAP_HELVETICA_18);
		char names[50] = { "       PLAYER      GOT DICE ROLL OF   " };
		char buffers[10] = { '\0' };
		drawstring(785, 400, names);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(780, 325);
		glVertex2f(1000, 325);
		glVertex2f(1000, 475);
		glVertex2f(780, 475);
		glEnd();

		glPointSize(10.0);
		glColor3f(1.0, 1.0, 0.0);
		sprintf(buffers, "%d", prev_player);
		drawstring(855, 400, buffers);

		sprintf(buffers, "%d", dicenum);
		drawstring(970, 400, buffers);
	}

	glPushMatrix();
	glTranslatef(900.0, 400.0, 0.0);
	glRotatef(spin, 1.0, 0.5, 1.0);
	if (dice_position < 0)
		drawdice();
	if (dice_position > 0)
		diceposition();

	glPopMatrix();

	glutSwapBuffers();
}

//Display function for the fourth window (displaying the winner)
void windowFour()
{
	int num = 0;
	float cn = 500;
	num = (winner + 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutIdleFunc(idle);

	glColor3f(1.0 *((rand() % 100) / 100.0), 1.0 *((rand() % 100) / 100.0), 1.0 *((rand() % 100) / 100.0));
	setFont(GLUT_BITMAP_HELVETICA_18);
	char name[100] = { "CONGRATULATIONS! THE WINNER IS PLAYER -->   " };
	char buffer[10] = { '\0' };
	drawstring(cn - 165, 500, name);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(cn - 170, 400);
	glVertex2f(cn + 170, 400);
	glVertex2f(cn + 170, 600);
	glVertex2f(cn - 170, 600);
	glEnd();

	glPointSize(10.0);
	glColor3f(1.0, 1.0, 0.0);
	sprintf(buffer, "%d", num);
	drawstring(cn + 150, 500, buffer);

	glFlush();
	glutSwapBuffers();

}

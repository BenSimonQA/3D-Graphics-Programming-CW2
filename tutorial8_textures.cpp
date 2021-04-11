//Benjamin Simon
//W1634216

#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

//  Created by Philip Trwoga on 09/11/2012. Modified on 20/1/16 to include the many VS2015 changes
//  Copyright (c) 2012 __Philip Trwoga__. All rights reserved.
// DO NOT DISTRIBUTE WITHOUT THE AUTHOR'S PERMISSION


//note that this needs gltools.h and gltools.cpp to be included in the shared/library directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such
#define TEXTURE_YELLOW 0 //flower yellow
#define IMAGE2      1
#define IMAGE3      2
#define TEXTURE_BALL 3    //football map
#define TEXTURE_SKYBOX 4  //sky box
#define TEXTURE_ORANGE 5 //flower orange
#define TEXTURE_TREE 6 //tree image
#define TEXTURE_TRED 7 //red target
#define TEXTURE_TGREEN 8 //green target
#define TEXTURE_TBLUE 9 //blue target
#define TEXTURE_TDULL 10 //dull target
#define TEXTURE_HORIZONTAL 11
#define TEXTURE_VERTICAL 12
#define TEXTURE_COUNT 13
GLuint  textures[TEXTURE_COUNT];

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char *textureFiles[TEXTURE_COUNT] = { "yellowFlowerFinal.tga","grass_diff.tga","old_wall_texture_TGA.tga","FootballCompleteMap.tga", "stormydays_large.tga","orangeFlowerFinal5.tga","palmBranchA.tga", "targetRed.tga",  "targetGreen.tga" , "targetBlue.tga" , "targetDull.tga", "fillBarHorizontal.tga", "fillBarVerticalR.tga" };


//for lighting if you want to experiment with these
GLfloat mKa[4] = { 0.11f,0.06f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.47f,0.54f,1.0f }; //diffuse
GLfloat mKs[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission

//spot position and direction
GLfloat	 lightPos[] = { 0.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir[] = { 50.0, 25.0, 0.0 };

GLfloat	 lightPos2[] = { 50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir2[] = { 50.0, 15.0, 0.0 };

GLfloat	 lightPos3[] = { -50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir3[] = { 50.0, 15.0, 0.0 };


// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  greenLight[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat  blueLight[] = { 0.0f, 1.0f, 1.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

//Skybox Rotation
GLfloat fEarthRot = 0.0f;

//camera
GLfloat cameraX = 0.0;
GLfloat cameraY = 100.0;
GLfloat cameraZ = 500.0;

//Movement
GLfloat moveX = 0.0f;
GLfloat moveY = 25.0f;
GLfloat moveZ = 280.0f;

//Projection
GLint Pwidth;
GLint Pheight;

GLint Wwidth;
GLint Wheight;

//Coordination
GLfloat xCom = 1.0f;
GLfloat yCom = 1.0f;

GLfloat tempYcom = 1.0f;
GLfloat tempXcom = 1.0f;

int totalNum = 0;

bool move = false;

void setBallProjection();
void resetPerspectiveProjection();

//Sky box
// here we declare an array of vertices for the cube
GLfloat cubeVertexData[108] =
{
	//object vertex coordinates for cube made from triangles
	// Data layout for each line below is:
	// positionX, positionY, positionZ
	//wind counter-clockwise

	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,

	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,

	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	//4
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	//5
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	//6
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f

};

GLfloat textureCoordsSkyBox[72] = {
	//face 1
	0.75,0.33,      //    0,1,
	0.75,0.67,     //    1,1,
	0.5,0.33,     //    0,0,
	0.5,0.33,     //    0,0,
	0.75,0.67,    //    1,0,
	0.5,0.67,   //    1,1,

	//face 2
	0.5,1.0, //    1,1,
	0.25,1, //    0,1,
	0.5,0.67, //    1,0,
	0.5,0.67, //    1,0,
	0.25,1.0, //    0,1,
	0.25,0.67, //    1,1,
	//face 3
	0,0.67,//    1,1,
	0,0.33,//    0,1,
	0.25,0.67,//    1,0,
	0.25,0.67,//    1,0,
	0,0.33,//    0,1,
	0.25,0.33,//    0,0,
	//face 4
	0.25,0.0,//    0,1,
	0.5,0.0,//    1,1,
	0.25,0.33,//    0,0,
	0.25,0.33,//    0,0,
	0.5,0.0,//    1,1,
	0.5,0.33,//    0,0,
	//face 5
	0.5,0.67,//    1,0,
	0.25,0.67,//    0,0,
	0.5,0.33,//    1,1,
	0.5,0.33,//    1,1,
	0.25,0.67,//    0,0,
	0.25,0.33,//    0,1,
	//face 6
	0.75,0.33,//    1,1,
	1.0,0.33,//    0,1,
	0.75,0.67,//    1,0,
	0.75,0.67,//    1,0,
	1.0,0.33,//    0,1,
	1.0,0.67//    0,0


};

//the lighting normals - all facing out from each face
GLfloat gCubeVertexdataNormals[108] =
{
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f

};

bool repeatOn = false;
bool repeatWallOn = false;
bool moveCamera = false;

//end of intialisation 

//---Text---//
void drawText(char *string,float x, float y, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);

	char * c;
	glRasterPos2f(x, y);

	for(c = string; *c !='\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

void drawNum(int num, float x, float y, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);

	int tNum;
	glRasterPos2f(x, y);

	for (tNum = num; tNum != '\0'; tNum++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, tNum);
	}
}

//---------------This function is used for Images 1 and 2---------------------//
void drawTexturedQuad(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-50.0, 0.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(50.0, 0.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(50.0, 100.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-50.0, 100.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//---------------The following two function are used for Images 3 and 4---------------------//
//----This is with tilling-----//
void drawTexturedSurface(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	//draw face up and then rotated
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(-4.0, -4.0);//repeated texture
	glVertex3f(-2.0, -400.0, 0.0);
	glTexCoord2f(4.0, -4.0);
	glVertex3f(2.0, -400.0, 0.0);
	glTexCoord2f(4.0, 4.0);
	glVertex3f(2.0, 400.0, 0.0);
	glTexCoord2f(-4.0, 4.0);
	glVertex3f(-2.0, 400.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

//----This is without tilling-----//
void drawTexturedSurfaceNoTiling(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	//draw face up and then rotated
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);//repeated texture
	glVertex3f(-800.0, -800.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(800.0, -800.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(800.0, 800.0, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(-800.0, 800.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//----This is for the back walls----//
void drawWalls(int image, float x, float y)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);//repeated texture
	glVertex3f(-x, -y, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(x, -y, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(x, y, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(-x, y, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawEnvironment()
{
	glPushMatrix();
	glTranslatef(-400.0, 0.0, -200.0);
	glRotatef(-3.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawWalls(IMAGE3, 300, 200);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(400.0, 0.0, -200.0);
	glRotatef(-3.0, 1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawWalls(IMAGE3, 300, 200);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -250.0);
	drawWalls(IMAGE3, 410, 200);
	glPopMatrix();

}

//---Draw FLowers---//
void drawFlowers()
{
	glPushMatrix();
	glTranslatef(0.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-300.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-230.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(230.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100.0, 30.0, -240.0);
	drawWalls(TEXTURE_YELLOW, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-350.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-320.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-250.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-150.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(350.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef (150.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(320.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50.0, 30.0, -240.0);
	drawWalls(TEXTURE_ORANGE, 20, 30);
	glPopMatrix();
}

//---Draw Tree---//

void drawTree()
{
	glPushMatrix();
	glTranslatef(-380.0, 50.0, -100.0);
	glRotatef(-3.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawWalls(TEXTURE_TREE, 80, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-380.0, 50.0, 50.0);
	glRotatef(-3.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawWalls(TEXTURE_TREE, 80, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(380.0, 50.0, -100.0);
	glRotatef(-3.0, 1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawWalls(TEXTURE_TREE, 80, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(380.0, 50.0, 50.0);
	glRotatef(-3.0, 1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawWalls(TEXTURE_TREE, 80, 100);
	glPopMatrix();
}

//---This is used to draw Targets---//
void drawTargets()
{
	//Draws the Red 30 point Targets
	glPushMatrix();
	glTranslatef(-350.0, 80.0, -240.0);
	if (moveX >= -370.0 && moveX <= -330.0 && moveY >= 60.0 && moveY <= 100.0 && moveZ >= -260.0 && moveZ <= -220.0)
	{
		drawWalls(TEXTURE_TDULL, 20, 20);
	}
	else
	{
		drawWalls(TEXTURE_TRED, 20, 20);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-150.0, 150.0, -240.0);
	if (moveX >= -170.0 && moveX <= -130.0 && moveY >= 130.0 && moveY <= 170.0 && moveZ >= -260.0 && moveZ <= -220.0)
	{
		drawWalls(TEXTURE_TDULL, 20, 20);
	}
	else
	{
		drawWalls(TEXTURE_TRED, 20, 20);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250.0, 100.0, -240.0);
	if (moveX >= 230.0 && moveX <= 280.0 && moveY >= 80.0 && moveY <= 120.0 && moveZ >= -260.0 && moveZ <= -220.0)
	{
		drawWalls(TEXTURE_TDULL, 20, 20);
	}
	else
	{
		drawWalls(TEXTURE_TRED, 20, 20);
	}
	glPopMatrix();

	//Draws the Blue 20 point Targets
	glPushMatrix();
	glTranslatef(80.0, 120.0, -240.0);
	if (moveX >= 50.0 && moveX <= 110.0 && moveY >= 90.0 && moveY <= 150.0 && moveZ >= -270.0 && moveZ <= -210.0)
	{
		drawWalls(TEXTURE_TDULL, 30, 30);
	}
	else
	{
		drawWalls(TEXTURE_TBLUE, 30, 30);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-210.0, 100.0, -240.0);
	if (moveX >= -240.0 && moveX <= -180.0 && moveY >= 70.0 && moveY <= 130.0 && moveZ >= -270.0 && moveZ <= -210.0)
	{
		drawWalls(TEXTURE_TDULL, 30, 30);
	}
	else
	{
		drawWalls(TEXTURE_TBLUE, 30, 30);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(320.0, 160.0, -240.0);
	if (moveX >= 290.0 && moveX <= 350.0 && moveY >= 130.0 && moveY <= 190.0 && moveZ >= -270.0 && moveZ <= -210.0)
	{
		drawWalls(TEXTURE_TDULL, 30, 30);
	}
	else
	{
		drawWalls(TEXTURE_TBLUE, 30, 30);
	}
	glPopMatrix();

	//Draws the Green 10 points Targets
	glPushMatrix();
	glTranslatef(-290.0, 150.0, -240.0);
	if (moveX >= -330.0 && moveX <= -250.0 && moveY >= 110.0 && moveY <= 190.0 && moveZ >= -280.0 && moveZ <= -200.0)
	{
		drawWalls(TEXTURE_TDULL, 40, 40);
	}
	else
	{
		drawWalls(TEXTURE_TGREEN, 40, 40);
	}	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-40.0, 110.0, -240.0);
	if (moveX >= -80.0 && moveX <= 0.0 && moveY >= 70.0 && moveY <= 150.0 && moveZ >= -280.0 && moveZ <= -200.0)
	{
		drawWalls(TEXTURE_TDULL, 40, 40);
	}
	else
	{
		drawWalls(TEXTURE_TGREEN, 40, 40);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(180.0, 130.0, -240.0);
	if (moveX >= 140.0 && moveX <= 220.0 && moveY >= 90 && moveY <= 170.0 && moveZ >= -280.0 && moveZ <= -200.0)
	{
		drawWalls(TEXTURE_TDULL, 40, 40);
	}
	else
	{
		drawWalls(TEXTURE_TGREEN, 40, 40);
	}
	glPopMatrix();
}

void drawDullTarget()
{
	glPushMatrix();
	glTranslatef(180.0, 130.0, -240.0);
	drawWalls(TEXTURE_TDULL, 40, 40);
	glPopMatrix();
}

//---------This is used for the football ball-------//
GLUquadricObj *quadricFootball;
void drawFootBall(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
	glPushMatrix();
	glFrontFace(GL_CCW);
	glTranslatef(moveX, moveY, moveZ);
	// Create and texture the ball
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BALL]);
	// glDisable(GL_LIGHTING);
	glColor3f(0.5, 0.5, 0.5);
	quadricFootball = gluNewQuadric();
	gluQuadricDrawStyle(quadricFootball, GLU_FILL);
	gluQuadricNormals(quadricFootball, GLU_SMOOTH);
	gluQuadricOrientation(quadricFootball, GLU_OUTSIDE);
	gluQuadricTexture(quadricFootball, GL_TRUE);
	gluSphere(quadricFootball, r, 50, 35);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawFillBars(int image, float x, float y)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	//draw face up and then rotated
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, textures[image]);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);//repeated texture
	glVertex3f(-x, -y, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(x, -y, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(x, y, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(-x, y, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBars()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.9, 0.6, 0.0);
	drawFillBars(TEXTURE_VERTICAL, 0.03, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0.1, 0.0);
	drawFillBars(TEXTURE_HORIZONTAL, 0.1, 0.05);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawBars2()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.2, 0.1, 0.0);
	drawFillBars(TEXTURE_HORIZONTAL, 0.1, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.6, 0.0);
	drawFillBars(TEXTURE_VERTICAL, 0.03, 0.2);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


//----This is for the Sky Box----//
void drawSkybox(int image)
{
	//add some lighting normals for each vertex
//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);

	glFrontFace(GL_CW);

	glNormal3f(0.0f, 0.0f, 1.0f);

	//--//--//--//--//--//--//--//



	glTexCoord2f(0.0, 0.3);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord3f(0.6, 0.3, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.6, 0.6);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.3, 0.6);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//--//--//--//--//--//--//--//



	glTexCoord2f(0.0, 0.3);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord3f(0.6, 0.3, 0.0);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.6, 0.6);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.3, 0.6);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//--//--//--//--//--//--//--//


	glTexCoord2f(0.0, 0.3);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord3f(0.25, 0.3, 0.0);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.25, 0.6);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0, 0.6);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	//--//--//--//--//--//--//--//


	glTexCoord2f(0.75, 0.6);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord3f(0.5, 0.6, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.5, 0.3);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.75, 0.3);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//--//--//--//--//--//--//--//



	glTexCoord2f(0.25, 0.3);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord3f(0.5, 0.3, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.5, 0.6);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.25, 0.6);
	glVertex3f(1.0f, 1.0f, -1.0f);

	//--//--//--//--//--//--//--//


	glTexCoord2f(0.75, 0.3);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord3f(1.0, 0.3, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0, 0.6);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.75, 0.6);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//--//--//--//--//--//--//--//

	glFrontFace(GL_CCW);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

// Called to draw scene
void RenderScene(void)
{

	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// view the scene
	gluLookAt(cameraX, cameraY, cameraZ,//eye
		0.00, 0.00, 0.00,//centre
		0.00, 1.00, 0.00);//up
	
	drawBars();
	drawBars();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -400.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	//drawTexturedSurfaceNoTiling(IMAGE2);
	if (repeatOn) {
		drawTexturedSurface(IMAGE2);
	}
	else {
		drawTexturedSurfaceNoTiling(IMAGE2);
	}
	glPopMatrix();

	drawFootBall(0.0, 25.0, 280.0, 20.0);
	drawEnvironment();
	drawFlowers();
	drawTree();
	drawTargets();
	drawText("Score :", 2.5, 200.0, 30.0, 10.0, 0.0);
	


	//Draw skybox
	glPushMatrix();
	glTranslatef(0.0, 200.0, 100.0);
	glScalef(1000.0, 1000.0, 400.0);
	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);
	drawSkybox(TEXTURE_SKYBOX);
	glPopMatrix();
	
	glutSwapBuffers();
}


// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	//textures

	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//try these too
	   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}

	//enable textures
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
 //	glEnable(GL_CULL_FACE);		// Do not calculate inside

//    glCullFace(GL_FRONT_AND_BACK);

// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
	//glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
	//glEnable(GL_LIGHT2);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Black blue background clear colour
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}

void setBallProjection()
{
	//Projecion mode
	glMatrixMode(GL_PROJECTION);
	//Save previouse matrix
	glPushMatrix();
	//Reset
	glLoadIdentity();
	//Sets a 2D Projection
	gluOrtho2D(0, Pwidth, 0, Pheight);
	glScalef(1, -1, 1);
	glTranslatef(0, -Pheight, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, Wwidth, 0, Wheight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -Wheight, 0);
	//set for drawing again
	glMatrixMode(GL_MODELVIEW);
}

void TimerFunc(int value)
{
	//Ball Movement Projection
	if (move)
	{
		moveX += xCom;
		if (moveY >= 23)
			moveY += yCom;

		yCom -= 0.25f;

		if (moveZ >= -600)
		{
			moveZ -= 10.0f;
		}
		else
		{
			moveZ = 280.0f;
			move = false;
			moveX = 0.0f;
			moveY = 25.0f;
			yCom = tempYcom;
			xCom = tempXcom;
		}
	}

	//rotate skybox
	fEarthRot += 0.05f;
	if (fEarthRot > 360.0f)
	{
		fEarthRot = 0.0f;
	}

	//move camera
	if (moveCamera)
	{
		cameraZ = cameraZ - 2;
		if (cameraZ > 300.0) {
			cameraY = cameraY + 0.3;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);

}

void keyInputs(unsigned char key, int x, int y)
{
	if (key == 27)
	{

	}
	else  if (key == 32)
	{

		if (move == false)
		{
			move = true;
			tempYcom = yCom;
			tempXcom = xCom;
			tempYcom = 0;
			tempXcom = 0;
		}
	}
	if (key == 'w' || key == 'W')
	{
		if (move == false)
		{
			yCom++;
		}
	}

	if (key == 's' || key == 'S')
	{
		if (move == false)
		{
			yCom--;
		}
	}

	if (key == 'a' || key == 'A')
	{
		if (move == false)
		{
			xCom--;
		}
	}

	if (key == 'd' || key == 'D')
	{
		if (move == false)
		{
			xCom++;
		}
	}

	//Executing the State 
	glutPostRedisplay();
}

void ProcessMenu(int choice) {

	switch (choice)
	{
	case 1:
		repeatOn = !repeatOn;
		break;
	case 2:
		repeatWallOn = !repeatWallOn;
		break;
	case 3:
		moveCamera = !moveCamera;
		if (!moveCamera) {
			cameraX = 0.0;
			cameraY = 100.0;
			cameraZ = 500.0;
		}
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 1000
	//that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
	gluPerspective(55.0f, fAspect, 1.0, 1000.0);

	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);// a 4:3 ratio
	glutCreateWindow("FootBall Shooter");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyInputs);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);
	// Create the Menu (right click on window)
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Toggle repeated texture", 1);
	glutAddMenuEntry("Toggle repeated wall", 2);
	glutAddMenuEntry("Toggle camera movement", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	SetupRC();
	glutMainLoop();
	return 0;
}


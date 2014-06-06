#include "stdafx.h"

#include <string.h>
#include <math.h>


#include <Windows.h>
#include <Ole2.h>

//GLUT and OpenGL things
#include <GL/glut.h>

//Kinect things
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <NuiSkeleton.h>

#include <math.h>
#include <iostream>
using namespace std;


const int TEXTURECOUNT = 1;
const double PI = 3.14159265358979323846;
const float screenWidthCm = 34.5;
const float screenHeightCm = 19.5;
//Collab 4K tv 142.3 cm Width 80.5 Height
//Collab Resolution 3840 x 2160

//Laptop 34.5 cm Width 19.5 Height
//Laptop Resolution 1920 x 1080
//In centimeters


// Camera Postition Vars
float xposOriginal = 0.0; 
float yposOriginal = 0.0;
float zposOriginal = 0.0;

float xpos = xposOriginal; 
float ypos = yposOriginal;
float zpos = zposOriginal;

float xrotOriginal = 0.0;
float yrotOriginal = 0.0;
float zrotOriginal = 0.0; 

float xrot = xrotOriginal;
float yrot = yrotOriginal;
float zrot = zrotOriginal; 

float scale = 1.0;
float rotScale = 1.0; //May need different scales for each direction
float posScale = 1.0;

//Textures and Normals Vars
GLUquadric* qobj;
GLuint* image[TEXTURECOUNT];
GLuint texture[TEXTURECOUNT];
int textureWidth[TEXTURECOUNT];
int textureHeight[TEXTURECOUNT];

//Lighting Vars
GLfloat lightPosition[]    = {20, 10, 4, 0.0};

GLfloat green[] = {0.0, 1.0, 0.0, 1.0}; //Green Color
GLfloat blue[] = {0.0, 0.0, 1.0, 1.0}; //Blue Color
GLfloat orange[] = {1.0, 0.5, 0.0, 1.0}; //Orange Color
GLfloat purple[] = {0.5, 0.0, 0.5, 1.0}; //Purple Color
GLfloat black[] = {0.0, 0.0, 0.0, 1.0}; //Black Color

GLfloat red[] = {1.0, 0.0, 0.0, 1.0}; //Red Color

GLfloat white[] = {1.0, 1.0, 1.0, 1.0}; //White Color

GLfloat ambientLight[]    = {0.2, 0.2, 0.2, 1.0};
GLfloat diffuseLight[]    = {0.8, 0.8, 0.8, 1.0};
GLfloat specularLight[]    = {1.0, 1.0, 1.0, 1.0};


//Kinect Vars
HANDLE depthStream;
HANDLE rgbStream;
INuiSensor* sensor;
Vector4 skeletonPosition[NUI_SKELETON_POSITION_COUNT];

//Generic Vars
bool infoToggle = false;


//Collab 4K tv 142.3 cm Width 80.5 Height
//Collab Resolution 3840 x 2160

//Laptop 34.5 cm Width 19.5 Height
//Laptop Resolution 1920 x 1080
//In centimeters
int pixelWidth;
int pixelHeight;
float pixelRatio;

//Originally in Meters once stored here they are in Centimeters
float worldHeadLocX = 0; //May need original values scaled to fit coords
float worldHeadLocY = 0;
float worldHeadLocZ = 0;

float virtualNearTopLeftLocX;
float virtualNearTopLeftLocY;

float virtualNearBottomRightLocX;
float virtualNearBottomRightLocY;


float worldFarPlane = 2000.0; //in cm
float virtualNearPlane = 5.0;
float virtualFarPlane;
/*
void doCameraUpdate()
{
	
	float worldScreenTopLeftLocX = -screenWidthCm/2; 
	float worldScreenTopLeftLocY = -screenHeightCm/2; 
	float worldScreenTopLeftLocZ = 0; 

	float worldScreenBottomRightLocX = screenWidthCm/2;
	float worldScreenBottomRightLocY = screenHeightCm/2;
	float worldScreenBottomRightLocZ = 0;

	float virtualScreenTopLeftLocX = worldScreenTopLeftLocX - worldHeadLocX;
	float virtualScreenTopLeftLocY = worldScreenTopLeftLocY - worldHeadLocY;
	float virtualScreenTopLeftLocZ = worldScreenTopLeftLocZ - worldHeadLocZ;

	float virtualScreenBottomRightLocX =  worldScreenBottomRightLocX - worldHeadLocX;
	float virtualScreenBottomRightLocY =  worldScreenBottomRightLocY - worldHeadLocY;
	float virtualScreenBottomRightLocZ =  worldScreenBottomRightLocZ - worldHeadLocZ;

	virtualFarPlane = worldFarPlane - worldHeadLocZ;

	virtualNearTopLeftLocX = virtualNearPlane/virtualScreenTopLeftLocZ*virtualScreenTopLeftLocX;
	virtualNearTopLeftLocY = virtualNearPlane/virtualScreenTopLeftLocZ*virtualScreenTopLeftLocY;
	float virtualNearTopLeftLocZ = virtualNearPlane/virtualScreenTopLeftLocZ*virtualScreenTopLeftLocZ;

	virtualNearBottomRightLocX = virtualScreenBottomRightLocX / virtualScreenBottomRightLocZ * virtualNearPlane;
	virtualNearBottomRightLocY = virtualScreenBottomRightLocY / virtualScreenBottomRightLocZ * virtualNearPlane;
	float virtualNearBottomRightLocZ = virtualScreenBottomRightLocZ / virtualScreenBottomRightLocZ * virtualNearPlane;
	
	

	glFrustum(virtualNearTopLeftLocX, virtualNearBottomRightLocX, -virtualNearBottomRightLocY, -virtualNearTopLeftLocY, virtualNearPlane, virtualFarPlane);
}

void testDoCameraUpdate()
{
	//Center of screen 0,0,0 this should translate to the center of slipping volume being at 0,0,0
	//Top of screen in reality (-height/2)
	//Bottom of screen in reality (height/2)
	//Left of screen in reality (-width/2)
	//Right of screen in reality (width/2)
	//Z Loc of screen in reality (0?)

	//Need to get coord systems matching for the kinect and OpenGL, a simple rotate(90, 1,0,0) should be it
	//Point X of head in reality
	//Point Y of head in reality
	//Point Z of head in reality

	//Near clipping field I think might need to be distance from you to screen (headPosZ - |screenLocationRealityZ| + kinectToScreenDist) (Unless objects in front of screen can happen: 
		//then it would require a lot of calulations to fake the near plane to still be at 0,0,0 but when the near plane is somewhere in fron of screen (likely multiplying everything by some scale based on how far in front of the screen that is, not sure though
	//Far clipping feild would be any point out to infinity

	//convert those real world coord into virtual world coords
	//Use ratio of w and h from viewport for this I think


}
*/
//Clean up kinect code some uneeded stuff in here
bool initKinect() 
{
    // Get a working kinect sensor
    int numSensors;
    if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
    if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

    // Initialize sensor
    sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
    sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
        0,        // Image stream flags, e.g. near mode
        2,        // Number of frames to buffer
        NULL,     // Event handle
        &depthStream);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
		&rgbStream);
	sensor->NuiSkeletonTrackingEnable(NULL, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT); // NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT for only upper body
    return sensor;
}

void getSkeletalData() 
{
	NUI_SKELETON_FRAME skeletonFrame = {0};
    if (sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame) >= 0) 
	{
		sensor->NuiTransformSmooth(&skeletonFrame, NULL);
		// Loop over all sensed skeletons
		for (int z = 0; z < NUI_SKELETON_COUNT; ++z) 
		{
			const NUI_SKELETON_DATA& skeleton = skeletonFrame.SkeletonData[z];
			// Check the state of the skeleton
			if (skeleton.eTrackingState == NUI_SKELETON_TRACKED) 
			{			
				// Copy the joint positions into our array
				for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) 
				{
					skeletonPosition[i] = skeleton.SkeletonPositions[i];
					if (skeleton.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_NOT_TRACKED) 
					{
						skeletonPosition[i].w = 0;
					}
				}
				return; // Only take the data for one skeleton
			}
		}
	}
}

void initTextures(char FileName[256])
{
	//Code from CPSC 456 class at SRU originally written in C and adopted to C++ 
   
	for(int count = 0; count < TEXTURECOUNT; count++)
	{
		float n = 0;
		float s;


		FILE *fd;
		int k, nm;
		int i;
		char b[256];
		for(int j = 0; j < 256; j++)
		{
			 b[j]= FileName[j];
		}
		int redPixel, greenPixel, bluePixel;
		char c;
		fd = fopen(FileName, "r");
		if(fd == NULL)
		{
			printf("%s is not in current directory\n", b);
			system("pause");
		}
		// check first line for P3
		fscanf(fd, "%[^\n]", b);
		
		if (b[0] != 'P' || b[1] != '3') {
				 printf("%s is not a PPM file\n", b);
				 system("pause");
				 exit(0);
				 }
		// skip comments
		fscanf(fd, "%c%c", &c, &c);
		while (c == '#') {
			  fscanf(fd, "%[^\n]", b);
			  fscanf(fd, "%c%c", &c, &c);
		}
    
		// put back first character of first non-comment line
		ungetc(c, fd);
		// read file info
		fscanf(fd, "%d %d %d", &textureWidth, &textureHeight, &k);
    
		nm = textureWidth[count] * textureHeight[count]; // overall size
    
		image[count] = (GLuint*) malloc(3*sizeof(GLuint)*nm);
    
		s = 255./k;
    
		for (i=0; i < (nm); i++) 
		{
			fscanf(fd, "%d %d %d", &redPixel, &greenPixel, &bluePixel);
			image[count][3*nm - 3*i - 3] = redPixel;
			image[count][3*nm - 3*i - 2] = greenPixel;
			image[count][3*nm - 3*i - 1] = bluePixel;
			//printf("(%d, %d, %d)\n", redPixel, greenPixel, bluePixel);
			//system("pause");
		}

		glGenTextures(1, &texture[count]);
		glBindTexture(GL_TEXTURE_2D, texture[count]);

		glPixelTransferf(GL_RED_SCALE, s);
		glPixelTransferf(GL_GREEN_SCALE, s);
		glPixelTransferf(GL_BLUE_SCALE, s);
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
}



void spheres()
{
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	// Set lighting intensity


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Set the light position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//Set light color
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
	glPushMatrix();
		glColor3f(0.0, 1.0, 0.0); //wanted center of projection green sphere
		glTranslatef (0.0, 0.0, 0.0);
		glutSolidSphere(1, 100, 100);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
	glPushMatrix();
		glColor3f(0.0, 0.0, 1.0); //blue
		glTranslatef (0.0, 0.0, 10.0);
		glutSolidSphere(1, 100, 100);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, orange);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange);
	glPushMatrix();
		glColor3f(1.0, 0.5, 0.0); //orange
		glTranslatef (.0, 0.0, -10.0);
		glutSolidSphere(1, 100, 100);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, purple);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, purple);
	glPushMatrix();
		glColor3f(0.5, 0.0, 0.5); //purple
		glTranslatef (10.0, 0.0, 0.0);
		glutSolidSphere(1, 100, 100);
	glPopMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //black
		glTranslatef (-10.0, 0.0, 0.0); 
		glutSolidSphere(1, 100, 100);
	glPopMatrix();   

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0); //red
		glTranslatef (0.0, 0.0, 18.0); 
		glutSolidSphere(1, 100, 100);
	glPopMatrix();   

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

}

void scene()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Set the light position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//Set light color
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

	glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWidth[0], textureHeight[0], 0, GL_RGB, GL_UNSIGNED_INT, image[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glNormal3d(0, 0, 1);
		glTexCoord2f(-5.0,-5.0);	glVertex3f(-screenWidthCm,-screenHeightCm,-50.0);
		glTexCoord2f(-5.0,5.0);		glVertex3f( screenWidthCm,-screenHeightCm,-50.0); 
		glTexCoord2f(5.0,5.0);		glVertex3f( screenWidthCm,-screenHeightCm, 50.0);  
		glTexCoord2f(5.0,-5.0);		glVertex3f(-screenWidthCm,-screenHeightCm, 50.0);
    glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glNormal3d(0, 0, 1);
		glTexCoord2f(-5.0,-5.0);	glVertex3f(-screenWidthCm,-screenHeightCm,-50.0);
		glTexCoord2f(-5.0,5.0);		glVertex3f(-screenWidthCm, screenHeightCm,-50.0); 
		glTexCoord2f(5.0,5.0);		glVertex3f(-screenWidthCm, screenHeightCm, 50.0);  
		glTexCoord2f(5.0,-5.0);		glVertex3f(-screenWidthCm,-screenHeightCm, 50.0);
    glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glNormal3d(0, 0, 1);
		glTexCoord2f(-5.0,-5.0);	glVertex3f(screenWidthCm,-screenHeightCm,-50.0);
		glTexCoord2f(-5.0,5.0);		glVertex3f(screenWidthCm, screenHeightCm,-50.0); 
		glTexCoord2f(5.0,5.0);		glVertex3f(screenWidthCm, screenHeightCm, 50.0);  
		glTexCoord2f(5.0,-5.0);		glVertex3f(screenWidthCm,-screenHeightCm, 50.0);
    glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glNormal3d(0, 0, 1);
		glTexCoord2f(-5.0,-5.0);	glVertex3f(-screenWidthCm,screenHeightCm,-50.0);
		glTexCoord2f(-5.0,5.0);		glVertex3f( screenWidthCm,screenHeightCm,-50.0); 
		glTexCoord2f(5.0,5.0);		glVertex3f( screenWidthCm,screenHeightCm, 50.0);  
		glTexCoord2f(5.0,-5.0);		glVertex3f(-screenWidthCm,screenHeightCm, 50.0);
    glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glNormal3d(0, 0, 1);
		glTexCoord2f(-5.0,-5.0);	glVertex3f(-screenWidthCm,-screenHeightCm,-50.0);
		glTexCoord2f(-5.0,5.0);		glVertex3f( screenWidthCm,-screenHeightCm,-50.0); 
		glTexCoord2f(5.0,5.0);		glVertex3f( screenWidthCm, screenHeightCm,-50.0);  
		glTexCoord2f(5.0,-5.0);		glVertex3f(-screenWidthCm, screenHeightCm,-50.0);
    glEnd();
	/*
	//Draw box out to infinity
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);
	glPushMatrix();



	
	glPopMatrix();
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	*/
	glDisable(GL_TEXTURE_2D);
}

void keyboard (unsigned char key, int x, int y) {
 
	switch (key) {
		case  27:  
			exit (0);
		case 'w': 
			zposOriginal += 1;
			glutPostRedisplay();  
			break;
		case 's': 
			zposOriginal -= 1;
			glutPostRedisplay();  
			break;
		case 'a': 
			xposOriginal += 1;
			glutPostRedisplay();  
			break;
		case 'd': 
			xposOriginal -= 1;
			glutPostRedisplay();  
			break;
		case 'q': 
			yposOriginal += 1;
			glutPostRedisplay();  
			break;
		case 'e': 
			yposOriginal -= 1;
			glutPostRedisplay();  
			break;
		case 'p':
			cout << "xposOriginal = " << xposOriginal << endl; 
			cout << "yposOriginal = " << yposOriginal << endl;
			cout << "zposOriginal = " << zposOriginal << endl;

			cout << "xpos = " << xpos << endl; 
			cout << "ypos = " << xpos << endl;
			cout << "zpos = " << xpos << endl;

			cout << "xrot = " << xrot << endl;
			cout << "yrot = " << yrot << endl;
			cout << "zrot = " << zrot << endl; 

			cout << "scale = " << scale << endl;
			cout << "rotScale = " << rotScale << endl;
			cout << "posScale = " << posScale << endl;
			break;
		case 'i':
			infoToggle = !infoToggle;
			break;
		case 'W': 
			xrotOriginal += 1;
			glutPostRedisplay();  
			break;
		case 'S': 
			xrotOriginal -= 1;
			glutPostRedisplay();  
			break;
		case 'A': 
			yrotOriginal -= 1;
			glutPostRedisplay();  
			break;
		case 'D': 
			yrotOriginal += 1;
			glutPostRedisplay();  
			break;
		case 'Q': 
			scale -= .25;
			glutPostRedisplay();  
			break;
		case 'E': 
			scale += .25;
			glutPostRedisplay();  
			break;
		case '8': 
			worldHeadLocZ += .5;
			glutPostRedisplay();  
			break;
		case '4': 
			glutPostRedisplay();  
			break;
		case '5': 
			worldHeadLocZ -= .5;
			glutPostRedisplay();  
			break;
		case '6': 
			glutPostRedisplay();  
			break; 
		case ' ':
			glutFullScreen();
			break;

		default :  printf ("   key = %c -> %d\n", key, key);
   }
}

void updatePosition(int value)
{
	glutPostRedisplay();  
	glutTimerFunc(16, updatePosition, 1);    
}


void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0*pixelRatio, 1.0*pixelRatio, -1.0, 1.0, 5.0 - worldHeadLocZ, 2000.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//doCameraUpdate();
    //gluLookAt(10.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    

	if(sensor)
	{
		//do head position update
		//getSkeletalData();//grab skeleton data

		//Old code each track type seperated by new line, rot was the most tested		
		//change based on pos
		//zpos =  skeletonPosition[NUI_SKELETON_POSITION_HEAD].x * posScale + zposOriginal;
		//ypos = -skeletonPosition[NUI_SKELETON_POSITION_HEAD].y * posScale + yposOriginal; //may not need later
		//xpos = -skeletonPosition[NUI_SKELETON_POSITION_HEAD].z * posScale + xposOriginal; //may not need later

		//change using rotates
		//yrot = ((atan(skeletonPosition[NUI_SKELETON_POSITION_HEAD].x/(-skeletonPosition[NUI_SKELETON_POSITION_HEAD].z)) * 180) / PI) * rotScale;
		//zrot = ((atan(-skeletonPosition[NUI_SKELETON_POSITION_HEAD].y/(-skeletonPosition[NUI_SKELETON_POSITION_HEAD].z)) * 180) / PI) * rotScale; //may not need later

		//change using gluLookAt
		//glRotatef(90, 0, 1, 0);
		//gluLookAt(skeletonPosition[NUI_SKELETON_POSITION_HEAD].z, skeletonPosition[NUI_SKELETON_POSITION_HEAD].y, -skeletonPosition[NUI_SKELETON_POSITION_HEAD].x, 0.0, 0.0, 0.0, 0, 1, 0);

		//get real world coords and store them appropriately 
		worldHeadLocX = skeletonPosition[NUI_SKELETON_POSITION_HEAD].x * 100; //grab head positions and convert to cmgu
		worldHeadLocY = skeletonPosition[NUI_SKELETON_POSITION_HEAD].y * 100;		
		worldHeadLocZ = skeletonPosition[NUI_SKELETON_POSITION_HEAD].z * 100;

		if(infoToggle)
		{
			cout << worldHeadLocX << "      " << worldHeadLocY << "      " << worldHeadLocZ << endl;
	
		}
	}
	glTranslatef (xposOriginal, yposOriginal, zposOriginal);  // Translations.

	glRotatef (zrotOriginal + zrot, 0,0,1);        // Rotations.
	glRotatef (yrotOriginal + yrot, 0,1,0);
	glRotatef (xrotOriginal, 1,0,0);


    glScalef (scale, scale, scale);
    spheres();
	scene();	
	//light position needs to rotate
	glutSwapBuffers();
    
}

// reshapes window, gives the actual size of the xzy coordinate units
void reshape(int w, int h)
{
	pixelWidth = w;
	pixelHeight = h;
	pixelRatio = (float)w/h;
	glViewport(0,0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 230.0); //Old parallel projection
	//gluPerspective(10, w/h, 10, 100);
	//glFrustum(-1.0*w/h, 1.0*w/h, -1.0, 1.0, 5.0, 2000.0); //first frustrum
	//doCameraUpdate();
}


//get it all up and ready at start
void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);      
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
}

//get window size, position, and start the functions to draw it all 
int main(int argc, char** argv)
{
	cout << "Loading Kinect \n";
	initKinect();
	if(!sensor)
	{
		cout << "Kinect not found, proceeding to just render scene\n";
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(800, 450);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("WarGames");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	

	cout << "Loading Textures \n";
	initTextures("checkerboard.ppm");

	init();


	updatePosition(1);
	glutMainLoop();   
}
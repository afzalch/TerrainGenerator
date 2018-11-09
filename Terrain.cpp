#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <cmath>
#include <time.h>

using namespace std;

enum wireframe {Triangle, Quad};

struct vertex{
    int x;
    int z;
};

struct normal {
	float x;
	float y;
	float z;
};


//Globals
float camPos[] = {70, 30, 40};	//where the camera is

// used reset camera position after moving camera
normal cameraPos = {70,30,40};

// positions of 2 lights
float lightPos1[] = {70,373, -150,1};
float lightPos2[] = {200,373, -150,1};

float pos[] = {10,15,35};
float rot[] = {0, 0, 0};
float headRot[] = {0, 0, 0};

float heightMap[300][300];

float normalVectors[300][300][3]; //declare the normal vector array of the same max size of 300 x 300

wireframe w = Triangle;
normal normalsMap[300][300];

int gridWidth = 0;
int gridHeight = 0;

int loop = 50;

// for hcanging wireframe representation
bool wireframe = false;
bool solid = true;

// for preventing the camera to be flipped upside down
bool flipMode = false; 

// for toggling the lights in the terrain
// true - means lighting is enabled, false - lighting is disabled
bool lightStatus = true;

// shading
bool shading = true;

// to decide whether to use triangle strips or quad strips
bool drawStyle = true;


void circlesAlgorithm(int iterations){
	//Populate with randoms
	for(int i = 0; i < gridWidth; i++){                                                                                                             
		for(int j = 0; j < gridHeight; j++){
			heightMap[i][j] = 5;
		}
	}
    srand(time(NULL));
	for(int p = 0; p < iterations; p++){
		int random_x = rand() % gridWidth;                         //randomly select x center point for circle
        int random_z = rand() % gridHeight;                         //randomly select z center point for circle
        int random_radius = (rand() % (gridHeight  / 2)) + 3;                   //randomly generate radius integer between 1 - 20 to be the terrainCircleSize
        float disp = (rand() % 10) + 3;    

		for(int i = 0; i < gridWidth; i++){
				for(int j = 0; j < gridHeight; j++){
					int dx = i - random_x;                      //get the value for tx to determine distance from circle center
                	int dz = j - random_z;  
					float distance = sqrtf((dx*dx) + (dz*dz));  //get the distance from circle center
                	float pd = (distance*2)/random_radius;
					if (fabs(pd) <= 1.0){                       //check if value is within the circle
                    	heightMap[i][j] += (disp/2.0) + (cos(pd*3.14)*(disp/2.0)); //apply circle alorithm to get the new height
                	}
					//float pd = (float)rand()/(float)(RAND_MAX/10) * 2 / 5;
					//if (fabs(pd) <= 1.0) heightMap[i][j] += disp/2 + cos(pd*3.14) * disp/2;
				}
				//printf("GG: %f \n", heightMap[i][1]);
			}
	}
}

// attempt to create normal algorithm for garaoud shading
void normalAlgo(int iterations){
    srand(time(NULL));
    for(int p = 0; p < iterations; p++){
        // randomly select 2 values which will represent x values of the 2 points that will create the line
        int randX1 = rand() % gridWidth;
        int randX2 = rand() % gridWidth;
        int randZ1 = rand() % gridHeight;
        int randZ2 = rand() % gridHeight;

        while (randX1 != 0 || randZ1 != 0){
            randX1 = rand() % gridWidth;
            randZ1 = rand() % gridHeight;
        } 
        while(randX2 != 0 || randZ2 != 0){
            randX1 = rand() % gridWidth;
            randZ1 = rand() % gridHeight;
        }


	}
}


void normalsAlgorithm(){
    //define vectors 
	normal v1;
	normal v2;
	normal v3;
	normal v;

    for(int x = 0; x < gridWidth; x++){
        for(int z = 0; z < gridWidth; z++){
            v1.x = x + 1;
            v1.y = heightMap[x+1][z] - heightMap[x][z];
            v1.z = z;
            
            v2.x = x+1;
            v2.y = heightMap[x+1][z+1] - heightMap[x][z];
            v2.z = z+1;
            
            //calculate cross product
            v.x = v1.y * v2.z - v1.z * v2.y;
            v.y = v1.z * v2.x - v1.x * v2.z;
            v.z = v1.x * v2.y - v1.x * v2.x;
            float l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
            
            //normal vectors
            normalsMap[x][z].x = v.x/l;
            normalsMap[x][z].y = v.y/l;
            normalsMap[x][z].z = v.z/l;
        }
    }
}


//Renders terrain after heightmap is filled
void renderTerrain(){
	for(int i = 0; i < gridWidth; i++){
		glColor3f(0.3f, 0.9f, 0.0f);
        if(drawStyle){
            glBegin(GL_TRIANGLE_STRIP);
        } else {
            glBegin(GL_QUAD_STRIP);
        }
			for(int j = 0; j < gridWidth; j++){
				glNormal3f(normalsMap[j][i].x, normalsMap[j][i].y, normalsMap[j][i].z);
				glVertex3f(float(j), heightMap[j][i], float(i));
				glNormal3f(normalsMap[j][i + 1].x, normalsMap[j][i + 1].y, normalsMap[j][i + 1].z);
				glVertex3f(float(j), heightMap[j][i + 1], float(i + 1));
				}
		glEnd();
	}
}

// attempted to create animating character
void DrawSnowman(float* pos, float* rot)
{
	glPushMatrix();

	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[1], 0, 1, 0);

	//draw body
	glColor3f(1,1,1);
	glutSolidSphere(1, 16, 16);

	//draw buttons
	glPushMatrix();
	glTranslatef(0, 0.35, 0.9);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.05, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();


	glPushMatrix();
	//translate relative to body, and draw head
	glTranslatef(0, 1.25, 0);
	glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
	glColor3f(1,1,1);
	glutSolidSphere(0.5, 16, 16);
	
	//translate and draw right eye
	glPushMatrix();
	glTranslatef(0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw left eye
	glPushMatrix();
	glTranslatef(-0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw nose
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1,0.4,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPopMatrix();//body
	glPopMatrix();//snowman
}




void display(void){
    //setup of material values for light
    float m_amb[] = {0.83, 0.52, 0.63, 1.0};
    float m_dif[] = {0.28, 0.87, 0.11, 1.0};
    float m_spec[] = {0.99, 0.91, 0.81, 1.0};
    float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float shiny = 0.0f;
    
    //enable material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glColorMaterial(GL_AMBIENT, GL_DIFFUSE);

    // setup light positions
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //create camera viewing transformations
    gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    glTranslatef(-1 * (gridWidth/2), 0, -1 * (gridWidth/2));
	renderTerrain();
    DrawSnowman(pos, rot);
    
    glutSwapBuffers();
}






//all keyboard functions
void keyboard(unsigned char key, int xIn, int yIn)
{
	switch(key){
		case 'q':
		case 27:
			exit(0);
			break;
		case 'w':
		case 'W':
			if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
			wireframe = !wireframe;
			break;
		case 'r':
		case 'R':
			circlesAlgorithm(100);
			normalsAlgorithm();
			renderTerrain();
			break;
		case 's':
		case 'S':
			if (shading){
                glShadeModel(GL_SMOOTH);
            } else {
                glShadeModel(GL_SMOOTH);
            }
			shading = !shading;
			break;
		case 'l':
		case 'L':
			if (lightStatus) {
				glDisable(GL_LIGHT0);
                glDisable(GL_LIGHTING);
			} else {
				glEnable(GL_LIGHT0);
                glEnable(GL_LIGHTING);
			}
			lightStatus = !lightStatus;
			break;
        case 't':
        case 'T':
            drawStyle = true;
            break;
        case 'y':
        case 'Y':
            drawStyle = false;
            break;
		case '1':
            printf("lightPos1[1] = %d\n",lightPos1[1]);
            lightPos1[1] -= 10;
            lightPos2[1] -= 10;
            break;
            
        case '2':
            printf("lightPos1[1] = %d\n",lightPos1[1]);
            lightPos1[1] += 10;
            lightPos2[1] += 10;
            break;
        
        case '3':
            printf("lightPos1[2] = %d\n",lightPos1[2]);
            lightPos1[2] -= 10;
            lightPos2[2] -= 10;
            break;
            
        case '4':
            printf("lightPos1[2] = %d\n",lightPos1[2]);
            lightPos1[1] += 10;
            lightPos2[1] += 10;
            break;
        case '5':
            printf("lightPos1[0] = %d\n",lightPos1[0]);
            lightPos1[0] -= 10;
            lightPos2[0] -= 10;
            break;
        case '6':
            printf("lightPos1[0] = %d\n",lightPos1[0]);
            lightPos1[0] += 10;
            lightPos2[0] += 10;
            break;
	}
}

void init(void)
{

	glClearColor(0, 0, 0, 0);
    glColor3f(0, 0, 1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-2, 2, -2, 2, -2, 2);
    gluPerspective(70, 1, 1, 400);
    
    //enable backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
     
}

void special(int key, int x, int y){
	switch(key)
    {
            //left keypress, translate left on the  axis
        case GLUT_KEY_LEFT:
            if (camPos[1] > 125){
                camPos[0] = cameraPos.x;
                camPos[1] = cameraPos.y;
                camPos[2] = cameraPos.z;
            }
            camPos[1] +=  2;
            break;
            //right keypress, translate right on the y axis
        case GLUT_KEY_RIGHT:
            if (camPos[1] < -20){
                camPos[0] = cameraPos.x;
                camPos[1] = cameraPos.y;
                camPos[2] = cameraPos.z;
            }
            camPos[1] -= 2;
            break;
            //up keypress, translate back on the y axis
        case GLUT_KEY_UP:
            if (camPos[0]>100){
                camPos[0] = cameraPos.x;
                camPos[1] = cameraPos.y;
                camPos[2] = cameraPos.z;
            }
            // if (flipMode){
                camPos[0] -= 2;
            // }
            // else{
            //     camPos[0] += 2;
            // }
            break;
            //down keypress, translate up on the y axis
        case GLUT_KEY_DOWN:
            //check to see if the camera has gone below the y plane
            //if so flip the orentation and direction of the camera so that it stays above the y plane 
            if (camPos[0]<-100){
                camPos[0] = cameraPos.x;
                camPos[1] = cameraPos.y;
                camPos[2] = cameraPos.z;
            }
            // if (!flipMode){
            //     camPos[0] -= 2;
            // }
            // else{
                camPos[0] += 2;
            // }
            break;
    }
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y){
	if(btn == GLUT_LEFT_BUTTON){
		printf("left button, %i, %i\n", x, y);
	}
}

void motion(int x, int y){
	printf("mouse click drag, %i, %i\n", x, y);
}


void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, w, 0, h);
	gluPerspective(100, ((w+0.0f)/h), 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

void FPS(int val){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}

void callBackInit(){
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, FPS, 0);
}


void menuProc(int value){
	if (value == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (value == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (value == 3)
    // dont know how to implement it
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (value == 4)
		printf("gahhhhhh item was clicked\n");
	if (value == 5)
		printf("????? item was clicked\n");
	if (value == 6){
		printf("Program was quit!\n");
		exit(0);
    }
}

//could use a seperate menu processor for submenu!
void menuProc2(int value){

}

void createOurMenu(){
	//int subMenu_id = glutCreateMenu(menuProc2);
	int subMenu_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Solid", 1);
	glutAddMenuEntry("Wireframe", 2);
    // dont know how to implement both
	glutAddMenuEntry("Both", 3);


	int main_id = glutCreateMenu(menuProc);
	glutAddSubMenu("Wireframe", subMenu_id);
    glutAddMenuEntry("Quit", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/* main function - program entry point */
int main(int argc, char** argv){
    cout << "-----------------------------------ReadMe------------------------------------------\n";
	cout << "First, please input the grid column and row to define grid size. Clicking the right mouse button will pop up the menu which has the\n"
			"following menu items:\n"
			"\"Terrain Algorithm\" - Show the algorithm sub menu to select algorithm to create them\n"
			"                        terrain.\n"
			"                        \"Fault\" - Select Fault Algorithm.\n"
			"                        \"Midpoint Displacement\" - Select Midpoint Displacement \n"
			"                                                    Algorithm.\n"
			"                        \"Circles\" - Select Circles Algorithm.\n"
			"\"Shading\" - Show the shading sub menu to select shading model.\n"
			"              \"Flat\" - Select Flat shading model.\n"
			"              \"Gouraud\" - Select Gouraud shading model.\n"
			"\"Quit\" - Quit the program.\n";
	cout << "Note: also implement and support three extra features as following:\n"
	        "1. Additional terrain algorithm(s):\n"
	        "In Terrain Algorithm submenu, select an additional terrain generation algorithm.\n"
	        "the fault algorithm and the midpoint displacement algorithm.\n"
	        "2. Display 2D terrain overview:\n"
            "Display a second GLUT window, and draw a 2D overhead representation of the 300 x 300\n"
            "terrain.\n"
            "3. Improved map colouring:\n"
            "Colour the terrain according to topographic maps. The lowest parts should be shades\n"
            "of green, and higher parts should become progressively more orange/red. The highest\n"
            "points (mountains) should be gray.\n"
	        "-----------------------------------------------------------------------------------\n";



	cout << "Please input grid size with minimum size 50 x 50 and maximum size 300 x 300. \n";

    // Ensuring size of grid remains in the current boundary between being 50x50 and 300x300
    cout << "Input width of grid: \n";
    cin >> gridWidth;
    while(gridWidth > 300 || gridWidth < 50){
        if(gridWidth > 300){
            cout << "The inputted width for grid is greater than 300! "; 
        } else if (gridWidth < 50){
            cout << "The inputted width is less than 50!";
        } else {
            break;
        }
        cout << "Input a value between 50 and 300: \n";
        cin >> gridWidth;
    }

    cout << "Input height of grid: \n";
    cin >> gridHeight;
    while(gridHeight > 300 || gridHeight < 50){
        if(gridHeight > 300){
            cout << "The inputted height for grid is greater than 300! ";
        } else if (gridHeight < 50){
            cout << "The inputted height is less than 50!";
        } else {
            break;
        }
        cout << "Input a value between 50 and 300: \n";
        cin >> gridHeight;
    }



	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(25, 25);
	glutCreateWindow("Terrain");

	callBackInit();

	init();

	glEnable(GL_DEPTH_TEST);

	createOurMenu();

	circlesAlgorithm(loop);
	normalsAlgorithm();

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}

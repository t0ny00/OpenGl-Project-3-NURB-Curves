// Cubica

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <math.h>
#include <functional>
#include <numeric>
#include <string>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0
#define PI 3.14159265


GLUnurbsObj *theNurb;

GLfloat ctrPointsNurbsSurf[21][21][3] ;

GLfloat knots[25] = {0,0,0,0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.9,1,1,1,1};

GLfloat t = 0;
bool play = false;
int selected;

struct Wave
{
	GLfloat length;
	GLfloat ampl;
	GLfloat spd;
	GLfloat dir_x;
	GLfloat dir_y;

}wave1,wave2;

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);

}

void animacion(int value) {
	
	if(play){

		t += 0.1;

		GLfloat w1 = 2*PI/wave1.length;
		GLfloat w2 = 2*PI/wave2.length;
		GLfloat delta1 = wave1.spd*2*PI/wave1.length;
		GLfloat delta2 = wave2.spd*2*PI/wave2.length;
		GLfloat length1 = sqrt(wave1.dir_x*wave1.dir_x + wave1.dir_y*wave1.dir_y);
		GLfloat length2 = sqrt(wave2.dir_x*wave2.dir_x + wave2.dir_y*wave2.dir_y);
		float normalize_direction1[2] = {wave1.dir_x/length1,wave1.dir_y/length1};
		float normalize_direction2[2] = {wave2.dir_x/length2,wave2.dir_y/length2};

		for (int i = 0; i<21;i++){
			for (int j = 0; j<21;j++){
				float temp[2] = {ctrPointsNurbsSurf[i][j][0],ctrPointsNurbsSurf[i][j][2]};
				ctrPointsNurbsSurf[i][j][1] = wave1.ampl*sin(  (normalize_direction1[0]*temp[0]+normalize_direction1[1]*temp[1]) * w1 + t*delta1 )
											+ wave2.ampl*sin(  (normalize_direction2[0]*temp[0]+normalize_direction2[1]*temp[1]) * w2 + t*delta2 );

			};
		};
	};
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	
}

void init_surface() {

	int x,y,z;
	z = 10;
	y = 0;
	for (int i = 0; i<21;i++){
		x = 10;
		for (int j = 0; j<21;j++){
			ctrPointsNurbsSurf[i][j][0] = x;
			ctrPointsNurbsSurf[i][j][1] = y;
			ctrPointsNurbsSurf[i][j][2] = z;
			x--;
		}
		z--;
	}
	
}

void printWaveState() {
	system("cls");
	printf("\n\n\n\n");

	if(selected == 1){printf("-> ");}

	printf("WAVE 1:\n\n");
	printf("\tLength = %f\n",wave1.length);
	printf("\tAmplitude = %f\n",wave1.ampl);
	printf("\tSpeed = %f\n",wave1.spd);
	printf("\tDirection x = %f\n",wave1.dir_x);
	printf("\tDirection y = %f\n",wave1.dir_y);

	printf("\n ================================= \n\n");

	if(selected == 2){printf("-> ");}

	printf("WAVE 2:\n\n");
	printf("\tLength = %f\n",wave2.length);
	printf("\tAmplitude = %f\n",wave2.ampl);
	printf("\tSpeed = %f\n",wave2.spd);
	printf("\tDirection x = %f\n",wave2.dir_x);
	printf("\tDirection y = %f\n",wave2.dir_y);

	printf("\n\n\n");
}

void init(){



	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	init_surface();
   
	wave1.length = 8.0;
	wave1.ampl = 0.4;
	wave1.spd = 2.0;
	wave1.dir_x = 0.0;
	wave1.dir_y = -1.0;

	wave2.length = 4.0;
	wave2.ampl = 0.0;
	wave2.spd = 0.0;
	wave2.dir_x = 1.0;
	wave2.dir_y = 1.0;

	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 15.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	printWaveState();
	glutTimerFunc(10,animacion,1);

}





void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;
	case 'r':
		play = true;
		break;
	case 'p':
		play = false;
		break;  
	case 'a':
		if (selected == 1) wave1.length +=0.1;
		if (selected == 2) wave2.length +=0.1;
		break;
	case 'z':
		if (selected == 1) wave1.length -=0.1;
		if (selected == 2) wave2.length -=0.1;
		break;
	case 's':
		if (selected == 1) wave1.ampl +=0.1;
		if (selected == 2) wave2.ampl +=0.1;
		break;
	case 'x':
		if (selected == 1) wave1.ampl -=0.1;
		if (selected == 2) wave2.ampl -=0.1;
		break;
	case 'd':
		if (selected == 1) wave1.spd +=0.1;
		if (selected == 2) wave2.spd +=0.1;
		break;
	case 'c':
		if (selected == 1) wave1.spd -=0.1;
		if (selected == 2) wave2.spd -=0.1;
		break;
	case 'f':
		if (selected == 1) wave1.dir_x +=0.1;
		if (selected == 2) wave2.dir_x +=0.1;
		break;
	case 'v':
		if (selected == 1) wave1.dir_x -=0.1;
		if (selected == 2) wave2.dir_x -=0.1;
		break;
	case 'g':
		if (selected == 1) wave1.dir_y +=0.1;
		if (selected == 2) wave2.dir_y +=0.1;
		break;
	case 'b':
		if (selected == 1) wave1.dir_y -=0.1;
		if (selected == 2) wave2.dir_y -=0.1;
		break;
	case '1':
		selected = 1;
		break;
	case '2':
		selected = 2;
		break;
  }
  printWaveState();
}



void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;
	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	

	// Luz y material

	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   


	
	// Render Grid 
	/*glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glPushMatrix();
	glRotatef(90,1.0,0.0,0.0);
    glColor3f( 0.0, 0.7, 0.7 );
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0 );
	glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0 );
	glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();
	ejesCoordenada();
    glPopMatrix();
	glEnable(GL_LIGHTING);*/
	// Fin Grid
	


	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	

	glPushMatrix();

	gluBeginSurface(theNurb);
    
	gluNurbsSurface(theNurb, 
                   25, knots, 25, knots,
                   21 * 3, 3, &ctrPointsNurbsSurf[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
	/*

		No cambien los numeros de la funcion, solo deben de poner los nombres de las variables correspondiente.
		
	*/


	gluEndSurface(theNurb);
	
	glPopMatrix();
	
	
	/* Muestra los puntos de control */
	
		/*int i,j;
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (i = 0; i <21; i++) {
			for (j = 0; j < 21; j++) {
	            glVertex3f(ctrPointsNurbsSurf[i][j][0], 	ctrPointsNurbsSurf[i][j][1], ctrPointsNurbsSurf[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);*/
	
	
	glutSwapBuffers();
	
}



int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Nurbs Proyecto - Ola");

	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}

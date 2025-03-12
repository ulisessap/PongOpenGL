//Example2_4.cpp : A bouncing ball 

//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <cstdio>

#define PI 3.1415926535898 

double xpos, ypos, ydir, xdir;         // x and y position for house to be drawn
double sx, sy, squash;          // xy scale factors
double rot, rdir;             // rotation 
int SPEED = 50;        // speed of timer call back in msecs

//Raquetas
float raquetaIzq_x = 10, raquetaIzq_y = 50; 
float raquetaDer_x = 140, raquetaDer_y = 50;

//Tamaño de las raquetas
const float raqueta_width = 15;
const float raqueta_height = 80;

//Puntuacion de los jugadores
int puntJugadorIzq = 0;
int puntJugadorDer = 0;

GLfloat T1[16] = {1.,0.,0.,0.,\
                  0.,1.,0.,0.,\
                  0.,0.,1.,0.,\
                  0.,0.,0.,1.};
GLfloat S[16] = {1.,0.,0.,0.,\
                 0.,1.,0.,0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};


#define PI 3.1415926535898 
GLint circle_points = 100; 
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius){
  GLint i;
  GLdouble angle;
  glBegin(GL_POLYGON); 
  for (i = 0; i < circle_points; i++) {    
    angle = 2*PI*i/circle_points; 
    glVertex2f(centerx+radius*cos(angle), centery+radius*sin(angle)); 
  } 
  glEnd();
}

GLfloat RadiusOfBall = 15.;


//Funcion que dibuja las raquetas
void draw_paddle(float x, float y) {
    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + raqueta_width, y);
        glVertex2f(x + raqueta_width, y + raqueta_height);
        glVertex2f(x, y + raqueta_height);
    glEnd();
}

// Draw the ball, centered at the origin
void draw_ball() {
  glColor3f(0.6,0.3,0.);
  MyCircle2f(0.,0.,RadiusOfBall);
  
}

//Funcion que pone la pelota en el centro de la pantalla
void resetBall() {
    xpos = glutGet(GLUT_WINDOW_WIDTH) / 2.0;
    ypos = glutGet(GLUT_WINDOW_HEIGHT) / 2.0;
}

//Funcion que despliega texto en la pantalla de juego
void drawText(float x, float y, char *text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void Display(void)
{
  // swap the buffers
  glutSwapBuffers(); 

  //clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT);
  // 160 is max X value in our world
	// Define X position of the ball to be at center of window

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

	xpos += xdir * 3.0;
  ypos += ydir * 2.0 - (1.0 - sy) * RadiusOfBall;
 	
	// Shape has hit the ground! Stop moving and start squashing down and then back up 
	if (ypos <= RadiusOfBall && ydir == -1  ){ 
		sy = sy*squash ; 
		
		if (sy < 0.8)
			// reached maximum suqash, now unsquash back up 
			squash = 1.1;
		else if (sy > 1.) {
			// reset squash parameters and bounce ball back upwards
			sy = 1.;
			squash = 0.9;
			ydir = 1;
		}
		sx = 1./sy;
	} 

  //Colision con la parte superior
  if (ypos + RadiusOfBall >= height){
    ydir = -1;  
    sy = 0.8;   
    squash = 1.1;
  }

  //Colisiones en la izquierda y derecha
  if (xpos - RadiusOfBall <= 0 || xpos + RadiusOfBall >= width){
    xdir *= -1; 
    sx = 0.8;   
    squash = 1.1;
  }

  //Escala de la pelota despues del squash
  if (sx < 1.0){
      sx *= squash;
      if (sx > 1.0) {
          sx = 1.0;
          squash = 0.9;
      }
  }

  // Colisión raqueta izquierda
  if (xpos - RadiusOfBall <= raquetaIzq_x + raqueta_width &&  ypos + RadiusOfBall >= raquetaIzq_y &&  ypos - RadiusOfBall <= raquetaIzq_y + raqueta_height) {
      xdir = 1;  
  }

  // Colisión con la raqueta derecha
  if (xpos + RadiusOfBall >= raquetaDer_x &&  ypos + RadiusOfBall >= raquetaDer_y &&  ypos - RadiusOfBall <= raquetaDer_y + raqueta_height) {
      xdir = -1;
  }


  // Si la pelota toca el borde de la izquierda, el jugador derecho gana un punto
  if (xpos - RadiusOfBall <= 0){
      puntJugadorDer++;

      printf("Puntos jugador 2: %d\n" , puntJugadorDer);
      resetBall();
      
  }

  // Si la pelota toca el borde de la derecha, el jugador izquierdo gana un punto
  if (xpos + RadiusOfBall >= glutGet(GLUT_WINDOW_WIDTH)){
      puntJugadorIzq++;

      printf("Puntos jugador 1: %d\n" , puntJugadorIzq);
      resetBall();
      
  }

  //Translate the bouncing ball to its new position
  T[12]= xpos;
  T[13] = ypos;
  glLoadMatrixf(T);

  T1[13] = -RadiusOfBall;
  // Translate ball back to center
  glMultMatrixf(T1);
  S[0] = sx;
  S[5] = sy;
  // Scale the ball about its bottom
  glMultMatrixf(S);
  T1[13] = RadiusOfBall;
  // Translate ball up so bottom is at the origin
  glMultMatrixf(T1);
  
  draw_ball();

  // Llamada a la funcion para dibujar las raquetas
  glLoadIdentity(); 
  draw_paddle(raquetaIzq_x, raquetaIzq_y);
  
  glLoadIdentity(); 
  draw_paddle(raquetaDer_x, raquetaDer_y);

  char puntuacion[50];
  sprintf(puntuacion, "Jugador 1: %d  Jugador 2: %d", puntJugadorIzq, puntJugadorDer);
  glColor3f(0, 0, 0);
  drawText(glutGet(GLUT_WINDOW_WIDTH) / 2 - 100, glutGet(GLUT_WINDOW_HEIGHT) - 30, puntuacion);

  glutPostRedisplay(); 
}


void keyboard(unsigned char key, int x, int y){
    int vel = 25; 

    switch (key) {
      //Controles de la raqueta izq
        case 'w': 
            if (raquetaIzq_y + raqueta_height < glutGet(GLUT_WINDOW_HEIGHT))
                raquetaIzq_y += vel;
            break;
        case 's': 
            if (raquetaIzq_y > 0)
                raquetaIzq_y -= vel;
            break;

        //Controles raqueta der
        case 'o': 
            if (raquetaDer_y + raqueta_height < glutGet(GLUT_WINDOW_HEIGHT))
                raquetaDer_y += vel;
            break;
        case 'l': 
            if (raquetaDer_y > 0)
                raquetaDer_y -= vel;
            break;
    }
}


void reshape (int w, int h){
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, w, 0.0, h); 
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void init(void){

  glClearColor(1.0, 1.0, 0.0, 1.0);

  //Cambio para la posicion de la pelota 
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  xpos = width / 2.0;  
  ypos = height / 2.0;  

  xdir = 1; ydir = 1;
  sx = 1.; sy = 1.; squash = 0.9;
  rot = 0;

  // Posiciones de las raquetas
  raquetaIzq_x = 20;
  raquetaIzq_y = height / 2.0 - raqueta_height / 2.0;

  raquetaDer_x = width - 20 - raqueta_width;
  raquetaDer_y = height / 2.0 - raqueta_height / 2.0;
  

}


int main(int argc, char* argv[])
{

  glutInit( & argc, argv );
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (1280, 720);   
  glutCreateWindow("Bouncing Ball");
  init();
  glutDisplayFunc(Display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  return 1;
}

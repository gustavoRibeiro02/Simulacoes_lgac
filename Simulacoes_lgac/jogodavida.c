#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <GL/glut.h>
#include <time.h>

#define TAM 200

float width, height;
int ca[TAM][TAM] = {0};


void drawScene(void)
{
	int i, j;
	// Clear the rendering window

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(i=0;i<TAM;i++)
		for(j=0;j<TAM;j++)
		{
			if(ca[i][j]==1)
				glColor3f( 0., 0., 0. );
			else
				if(ca[i][j]==0)
					glColor3f( 1.0, 1.0, 1.0 );

			glVertex2f( (j), (TAM-i) );//*5
		}
	glEnd();
	glutSwapBuffers();

}


// Initialize OpenGL's rendering modes
void initRendering()
{

	glEnable ( GL_DEPTH_TEST );

	// Uncomment out the first block of code below, and then the second block,
	//		to see how they affect line and point drawing.

	// The following commands should cause points and line to be drawn larger
	//	than a single pixel width.
	glPointSize(4);
	glLineWidth(2);

/*
	// The following commands should induce OpenGL to create round points and
	//	antialias points and lines.  (This is implementation dependent unfortunately).
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/

}

// Called when the window is resized
//		w, h - width and height of the window in pixels.

void resizeWindow(int w, int h)
{
   width = w;
   height = h;
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0.0, TAM, 0.0, TAM, -TAM ,TAM);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void updateCA()
{
    int i, j, k, l, m, cont = 0;
    int cb[TAM][TAM] = {0};

     for(i=1; i<TAM-1; i++){
            for(j=1; j<TAM-1; j++){

                if(ca[i-1][j]==1){
                    cont++;
                }
                if(ca[i+1][j]==1){
                    cont++;
                }
                if(ca[i][j-1]==1){
                    cont++;
                }
                if(ca[i][j+1]==1){
                    cont++;
                }
                /*
                                if(cont % 2 == 0){
                    cb[i][j] = 0;
                }else{
                    cb[i][j] = 1;
                }
                */
                
                if(cont==3 || cont==1){
                    cb[i][j]=1;
                }
                if(cont==2 || cont==0 || cont==4){
                    cb[i][j]=0;
                }
                
                cont = 0;
                    
            }
        }

                /*Copia a matriz auxiliar pra matriz principal*/
        for(k = 0; k < TAM; k++){
            for(l = 0; l < TAM; l++){
                ca[k][l] = cb[k][l];
            }
        }

}

// Função callback chamada pela GLUT a cada intervalo de tempo
// (a window não está sendo redimensionada ou movida)
void Timer(int value)
{

    updateCA();

	glutTimerFunc(33,Timer, 1);

}

void key(unsigned char key, int x, int y)
{
	glutTimerFunc(33, Timer, 1);
}


int main(int argc, char** argv){
    setlocale(LC_ALL, "Portuguese");

    ca[2][2] = 1;
    ca[2][3] = 1;
    ca[3][4] = 1;
    ca[4][3] = 1;

    glutInit(&argc,argv);

	// The image is not animated so single buffering is OK.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Window position (from top corner), and size (width and hieght)
	glutInitWindowPosition( 20, 60 );
	glutInitWindowSize( 360, 360 );
	glutCreateWindow( "SimpleDraw - Press space bar to toggle images" );

	// Initialize OpenGL as we like it..
	initRendering();

	// call this whenever window needs redrawing
	glutDisplayFunc( drawScene );

	// Set up the callback function for resizing windows
	glutReshapeFunc( resizeWindow );

	// Call this for background processing
	// A funcao display eh pra exibir a cada passo de tempo
    glutIdleFunc( drawScene );

    glutKeyboardFunc(key);


	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

}

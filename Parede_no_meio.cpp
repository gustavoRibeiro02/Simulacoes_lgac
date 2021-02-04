#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <GL/glut.h>
#include <time.h>
#define TAM 200

float width, height;
int ca[TAM][TAM] = {0},V0[TAM][TAM] = {0}, V1[TAM][TAM] = {0}, V2[TAM][TAM] = {0}, V3[TAM][TAM] = {0};
int auxV0[TAM][TAM]={0}, auxV1[TAM][TAM]={0}, auxV2[TAM][TAM]={0}, auxV3[TAM][TAM]={0}, contorno[TAM][TAM]={0};


void drawScene(void)
{
	int i, j;
	// Clear the rendering window

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(i=0;i<TAM;i++)
		for(j=0;j<TAM;j++)
		{
            if(contorno[i][j]==1)
			     glColor3f( 0., 0., 0. );
            else if(ca[i][j]==1)
                glColor3f(0.75, 0.75, 0.75);
            else if(ca[i][j]==2)
                glColor3f(0.5, 0.5, 0.5);
            else if(ca[i][j]==3)
                glColor3f(0.25, 0.25, 0.25);
            else if(ca[i][j]==4)
				glColor3f( 0., 0., 0. );
            else if(ca[i][j]==0)
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
    int i, j;
      // COLISÃO
    for(i=0; i<TAM; i++){
        for(j=0; j<TAM; j++){
            if(V0[i][j]==1 && V2[i][j]==1 && V3[i][j]==0 && V1[i][j]==0 && contorno[i][j]==0){
                auxV1[i][j]=1;
                auxV3[i][j]=1;
                auxV2[i][j]=0;
                auxV0[i][j]=0;
            }else if(V3[i][j]==1 && V1[i][j]==1 && V0[i][j]==0 && V2[i][j]==0 && contorno[i][j]==0){
                auxV2[i][j]=1;
                auxV0[i][j]=1;
                auxV1[i][j]=0;
                auxV3[i][j]=0;
            }else{
                auxV0[i][j]=V0[i][j];
                auxV1[i][j]=V1[i][j];
                auxV2[i][j]=V2[i][j];
                auxV3[i][j]=V3[i][j];
            }
        }
    }


    // CONTORNO
    for(i=0; i<TAM; i++)
    	for(j=0; j<TAM; j++){
    		if(contorno[i][j])
    		{
    			if(auxV0[i][j])
    			{
    				auxV0[i][j] = 0;
    				V0[i][j] = 0;
    				auxV2[i][j] = 1;
    			}
    			else if(auxV1[i][j])
    			{
    				auxV1[i][j] = 0;
    				V1[i][j] = 0;
    				auxV3[i][j] = 1;

    			}
    			else if(auxV2[i][j])
    			{
    				auxV2[i][j] = 0;
    				V2[i][j] = 0;
    				auxV0[i][j] = 1;
    			}
    			else if(auxV3[i][j])
    			{
    				auxV3[i][j] = 0;
    				V3[i][j] = 0;
    				auxV1[i][j] = 1;
    			}
    		}


    }

    //PROPAGÇÃO
    for(i=1; i<TAM-1; i++){
        for(j=1; j<TAM-1; j++){
        	if(contorno[i][j]==0){
        		V0[i][j]=auxV0[i+1][j];
        		V0[i-1][j]=auxV0[i][j];
        	}
        	if(contorno[i][j]==0){
        		V1[i][j]=auxV1[i][j-1];
        		V1[i][j+1]=auxV1[i][j];
        	}
        	if(contorno[i][j]==0){
        		V2[i][j]=auxV2[i-1][j];
        		V2[i+1][j]=auxV2[i][j];
        	}
        	if(contorno[i][j]==0){
        		V3[i][j]=auxV3[i][j+1];
        		V3[i][j-1]=auxV3[i][j];
        	}

        }
    }

    //calculo da densidade
    for(i=0; i<TAM; i++){
        for(j=0; j<TAM; j++){
            ca[i][j]=(V0[i][j]+V1[i][j]+V2[i][j]+V3[i][j]);
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
    int i,j,k;
    for(i=0;i<TAM;i++){
        contorno[i][TAM-120]=(i<=75||i>=125)?1:0;
        contorno[0][i]=1;
        contorno[TAM-1][i]=1;
        contorno[i][0]=1;
        contorno[i][TAM-1]=1;
    }
    for(i=0;i<TAM;i++){
        for(j=0;j<TAM-120;j++){

            V0[i][j]=rand()%2;
            V1[i][j]=rand()%2;
            V2[i][j]=rand()%2;
            V3[i][j]=rand()%2;

        }
    }
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

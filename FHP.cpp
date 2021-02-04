#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <fstream>
#include <iostream>
#define ny 500
#define nx 1000
#define ny_p 50
#define nx_p 100
using namespace std;

float width, height;
int ca[ny][nx] = {0},V0[ny][nx] = {0}, V1[ny][nx] = {0}, V2[ny][nx] = {0}, V3[ny][nx] = {0}, V4[ny][nx]= {0}, V5[ny][nx]= {0};
int auxV0[ny][nx]= {0}, auxV1[ny][nx]= {0}, auxV2[ny][nx]= {0}, auxV3[ny][nx]= {0}, contorno[ny][nx]= {0},auxV4[ny][nx]= {0}, auxV5[ny][nx]= {0};
float ca_p[ny_p][nx_p]= {0.0};
double u[ny_p][nx_p],ux[ny][nx],uy[ny][nx],ux_p[ny_p][nx_p],uy_p[ny_p][nx_p];

void save_vtk(char *nome)
{
    ofstream arqvtk;
    arqvtk.open(nome, ios::out);
    arqvtk << "# vtk DataFile Version 3.0\n";
    arqvtk << "vtk output\n";
    arqvtk << "ASCII\n";
    arqvtk << "DATASET RECTILINEAR_GRID\n";
    arqvtk << "DIMENSIONS " << nx_p << " " << ny_p << " 1\n";

    arqvtk << "X_COORDINATES " << nx_p << " double\n";
    for(int j=0; j<nx_p; j++)
        arqvtk << j << " ";
    arqvtk << "\n";

    arqvtk << "Y_COORDINATES " << ny_p << " double\n";
    for(int i=0; i<ny_p; i++)
        arqvtk << i << " ";
    arqvtk << "\n";

    arqvtk << "Z_COORDINATES 1 double\n";
    arqvtk << "0\n";

    arqvtk << "POINT_DATA " << nx_p*ny_p << "\n";
    arqvtk << "FIELD FieldData 1\n";
    arqvtk << "VelocityMagnitude 1 " << nx_p*ny_p << " double\n";

    for(int i=0; i<ny_p; i++)
        for(int j=0; j<nx_p; j++)

        {
            arqvtk << u[i][j] << " ";
        }

    arqvtk << "\n";

    arqvtk << "FIELD FieldData 1\n";
    arqvtk << "Density 1 " << nx_p*ny_p << " double\n";


    for(int i=0; i<ny_p; i++)
        for(int j=0; j<nx_p; j++)
        {
            arqvtk << ca_p[i][j] << " ";
        }

    arqvtk << "\n";

    arqvtk << "VECTORS vectors double\n";

    for(int i=0; i<ny_p; i++)
        for(int j=0; j<nx_p; j++)

        {
            arqvtk << ux_p[i][j] << " "<< uy_p[i][j] << " "<< 0.0 << "    ";
        }


    arqvtk << "\n";
    arqvtk.close();
}

void updateCA()
{
    int i, j;
    // COLISÃO
    for(i=0; i<ny; i++)
    {
        for(j=0; j<nx; j++)
        {

            if(V0[i][j]==1 && V2[i][j]==1 && V3[i][j]==0 && V1[i][j]==0 && V4[i][j]==0 && V5[i][j]==0 && contorno[i][j]==0)
            {
                if(rand()%2==1)
                {
                    auxV1[i][j]=1;
                    auxV3[i][j]=1;
                    auxV2[i][j]=0;
                    auxV0[i][j]=0;
                    auxV4[i][j]=0;
                    auxV5[i][j]=0;
                }
                else
                {
                    auxV1[i][j]=0;
                    auxV3[i][j]=0;
                    auxV2[i][j]=0;
                    auxV0[i][j]=0;
                    auxV4[i][j]=1;
                    auxV5[i][j]=1;
                }
            }
            else if(V3[i][j]==1 && V1[i][j]==1 && V0[i][j]==0 && V2[i][j]==0 && V4[i][j]==0 && V5[i][j]==0 && contorno[i][j]==0)
            {
                if(rand()%2==1)
                {
                    auxV1[i][j]=0;
                    auxV3[i][j]=0;
                    auxV2[i][j]=1;
                    auxV0[i][j]=1;
                    auxV4[i][j]=0;
                    auxV5[i][j]=0;
                }
                else
                {
                    auxV1[i][j]=0;
                    auxV3[i][j]=0;
                    auxV2[i][j]=0;
                    auxV0[i][j]=0;
                    auxV4[i][j]=1;
                    auxV5[i][j]=1;
                }
            }
            else if(V3[i][j]==0 && V1[i][j]==0 && V0[i][j]==0 && V2[i][j]==0 && V4[i][j]==1 && V5[i][j]==1 && contorno[i][j]==0)
            {
                if(rand()%2==1)
                {
                    auxV1[i][j]=1;
                    auxV3[i][j]=1;
                    auxV2[i][j]=0;
                    auxV0[i][j]=0;
                    auxV4[i][j]=0;
                    auxV5[i][j]=0;
                }
                else
                {
                    auxV1[i][j]=0;
                    auxV3[i][j]=0;
                    auxV2[i][j]=1;
                    auxV0[i][j]=1;
                    auxV4[i][j]=0;
                    auxV5[i][j]=0;
                }
            }
            else if(V3[i][j]==0 && V1[i][j]==1 && V0[i][j]==0 && V2[i][j]==1 && V4[i][j]==1 && V5[i][j]==0 && contorno[i][j]==0)
            {
                auxV1[i][j]=0;
                auxV3[i][j]=1;
                auxV2[i][j]=0;
                auxV0[i][j]=1;
                auxV4[i][j]=0;
                auxV5[i][j]=1;
            }
            else if(V3[i][j]==1 && V1[i][j]==0 && V0[i][j]==1 && V2[i][j]==0 && V4[i][j]==0 && V5[i][j]==1 && contorno[i][j]==0)
            {
                auxV1[i][j]=1;
                auxV3[i][j]=0;
                auxV2[i][j]=1;
                auxV0[i][j]=0;
                auxV4[i][j]=1;
                auxV5[i][j]=0;
            }
            else
            {
                auxV0[i][j]=V0[i][j];
                auxV1[i][j]=V1[i][j];
                auxV2[i][j]=V2[i][j];
                auxV3[i][j]=V3[i][j];
                auxV4[i][j]=V4[i][j];
                auxV5[i][j]=V5[i][j];
            }
        }
    }
    // CONTORNO
    int left=0, right=nx-1, top=0, bottom=ny-1;
    //printf("Antes\n%d %d\n", auxV4[top][right], auxV4[bottom][left] );
    float r;
    for(i=top+1; i<bottom-1; i++)
    {
        float vel_profile =  1.0 / ((ny/2-1./2.)*(ny/2-1./2.)) * ((i-0)-1)*(ny - (i-0));

        r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        auxV4[i][left] = r <= vel_profile;
        //printf(" %f %f %d\n",vel_profile, r, r <= vel_profile);

        r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        auxV3[i][left] = r <= vel_profile;

        r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        auxV2[i][left] = r <= vel_profile;


        auxV0[i][left] = 0;
        auxV1[i][left] = 0;
        auxV5[i][left] = 0;

        auxV4[i][right] = 0;
        auxV3[i][right] = 0;
        auxV2[i][right] = 0;


    }
    for(j=0; j<nx; j++)
    {

        //top
        if (auxV0[top][j])
        {
            auxV0[top][j] = 0;
            V0[top][j] = 0;
            auxV2[top][j] = 1;
        }

        if (auxV4[top][j])
        {
            auxV4[top][j] = 0;
            V4[top][j] = 0;
            auxV5[top][j] = 1;
        }

        //bottom
        if (auxV5[bottom][j])
        {
            auxV5[bottom][j] = 0;
            V5[bottom][j] = 0;
            auxV4[bottom][j] = 1;
        }
        if (auxV2[bottom][j])
        {
            auxV2[bottom][j] = 0;
            V2[bottom][j] = 0;
            auxV0[bottom][j] = 1;
        }
    }

    //printf("Depois\n%d %d\n", auxV4[top][right], auxV4[bottom][left] );

    //PROPAGÇÃO
    for(i=1; i<ny-1; i++)
    {
        for(j=1; j<nx-1; j++)
        {
            //if(contorno[i][j]==0){
            V0[i][j]=auxV0[i+1][j+1];
            V0[i-1][j-1]=auxV0[i][j];
            //}
            //if(contorno[i][j]==0){
            V1[i][j]=auxV1[i][j+1];
            V1[i][j-1]=auxV1[i][j];
            //}
            //if(contorno[i][j]==0){
            V2[i][j]=auxV2[i-1][j-1];
            V2[i+1][j+1]=auxV2[i][j];
            //}
            //if(contorno[i][j]==0){
            V3[i][j]=auxV3[i][j-1];
            V3[i][j+1]=auxV3[i][j];
            //}
            //if(contorno[i][j]==0){
            V4[i][j]=auxV4[i+1][j-1];
            V4[i-1][j+1]=auxV4[i][j];
            //}
            //if(contorno[i][j]==0){
            V5[i][j]=auxV5[i-1][j+1];
            V5[i+1][j-1]=auxV5[i][j];
            //}

        }
    }
    int k,l;
    float soma,somaux,somauy;

    //calculo da densidade
    for(i=0; i<ny; i++)
    {
        for(j=0; j<nx; j++)
        {
            ca[i][j]=(V0[i][j]+V1[i][j]+V2[i][j]+V3[i][j]+V4[i][j]+V5[i][j]);
            ux[i][j] = ( cos(M_PI/3)*V4[i][j] + cos(2*M_PI/3)*V0[i][j] + cos(M_PI)*V1[i][j] + cos(4*M_PI/3)*V5[i][j] + cos(5*M_PI/3)*V2[i][j] + cos(2*M_PI)*V3[i][j] )/ca[i][j];
            if(isnan(ux[i][j]))
                ux[i][j] = 0.0;
            uy[i][j] = ( sin(M_PI/3)*V4[i][j] + sin(2*M_PI/3)*V0[i][j] + sin(M_PI)*V1[i][j] + sin(4*M_PI/3)*V5[i][j] + sin(5*M_PI/3)*V2[i][j] + sin(2*M_PI)*V3[i][j] )/ca[i][j];
            if(isnan(uy[i][j]))
                uy[i][j] = 0.0;
        }
    }
    int x=10,y=10;
    for(i=0; i<ny_p; i++)
    {
        for(j=0; j<nx_p; j++)
        {
            soma=0;
            somaux=0;
            somauy=0;
            for(k=0; k<y; k++)
            {
                for(l=0; l<x; l++)
                {
                    soma+=ca[y*i+k][x*j+l]/6.0;
                    somaux += ux[y*i+k][x*j+l];
                    somauy += uy[y*i+k][x*j+l];
                }
            }
            ca_p[i][j]=soma/100.0;
            ux_p[i][j]=somaux/100.0;
            uy_p[i][j]=somauy/100.0;
            u[i][j]=sqrt(ux_p[i][j]*ux_p[i][j]+uy_p[i][j]*uy_p[i][j]);
            if(isnan(u[i][j])){
                u[i][j]=0;
            }
        }
    }
}
int main()
{
    int t=0;
    char nome[50];
    while(t<1000)
    {
    	cout << "t " << t << std::endl;

	    updateCA();
	    if(t%10==0)
	    {
	    	sprintf(nome, "resultado_%d.vtk", t/10);
	    	save_vtk(nome);
	    }
	    t++;
    }
    return 0;
}


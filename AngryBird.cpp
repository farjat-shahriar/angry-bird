#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
double angle;
struct point
{
	double x,y,z;
};

struct point pos,l,u,r;
double X=0,Z=0,f=0;
int Y=-1,c=0;
float bh=-13,bv=14,brh=0,brv=0,var,k=0;
float brickh[]={16,12.5,9,6,7.5,14,7.5,7.5,14,14,10.8,12.2,10.5,8.8};
float brickv[]={9,9,9,9,11,11,13,15,13,15,16,18,20,18};
struct point arra[100];
void drawAxes()
{

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);{
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1000,0,0);
        glVertex3f(-1000,0,0);

        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(0,-1000,0);
        glVertex3f(0, 1000,0);

        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0,0, 1000);
        glVertex3f(0,0,-1000);
    }glEnd();
}

void pyramid()
{
    glBegin(GL_TRIANGLES);
    glColor3f(1,1,0);
    glVertex3d(0,5,0);
    glVertex3d(5,-3,0);
    glVertex3d(-5,-3,0);

    glColor3f(1,0.5,0);
    glVertex3d(0,5,0);
    glVertex3d(5,-3,0);
    glVertex3d(0,0,5);

    glColor3f(0.5,0.5,0);
    glVertex3d(0,5,0);
    glVertex3d(0,0,5);
    glVertex3d(-5,-3,0);

    glColor3f(0.5,1,0);
    glVertex3d(0,0,5);
    glVertex3d(5,-3,0);
    glVertex3d(-5,-3,0);

    glEnd();
}
void drawGrid()
{
	int i;
    glColor3f(0.6, 0.6, 0.6);	//grey
    glBegin(GL_LINES);{
        for(i=-8;i<=8;i++){

            if(i==0)
                continue;	//SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i*10, -90, 0);
            glVertex3f(i*10,  90, 0);

            //lines parallel to X-axis
            glVertex3f(-90, i*10, 0);
            glVertex3f( 90, i*10, 0);
        }
    }glEnd();

}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void draw_cylinder(double radius,double height,int segments)
{
    int i;
     struct point points[2][100];
     for(i=0;i<=segments;i++)
    {
        points[0][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[0][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[0][i].z=0;

        points[1][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[1][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[1][i].z=height;
    }

    for (i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f(i%2,i%2,(i+1)%2);

				glVertex3f(points[0][i].x,points[0][i].y,points[0][i].z);
				glVertex3f(points[0][i+1].x,points[0][i+1].y,points[0][i+1].z);

				glVertex3f(points[1][i+1].x,points[1][i+1].y,points[1][i+1].z);
				glVertex3f(points[1][i].x,points[1][i].y,points[1][i].z);

			}glEnd();


    }
}
void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x= radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y= radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,1);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x+1,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius, int slices, int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,1.0,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawEyes(double radius, int slices, int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1.0f,1.0f,1.0f);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
		}
	}
}

void ground()
{
    glBegin(GL_QUADS);

            glColor3f(0.54f, 0.27f, 0.07f);
            glVertex3f(20.0f, -10.0f, 0.0f);					    // Top front Of The Quad (right)
            glVertex3f(20.0f, 10.0f, 0.0f);
            glVertex3f(-20.0f, 10.0f, 0.0f);				// Top back Of The Quad (right)
            glVertex3f(-20.0f, -10.0f, 0.0f);					// Bottom front Of The Quad (right)

            glVertex3f(20.0f, -10.0f, 5.0f);					    // Top front Of The Quad (right)
            glVertex3f(20.0f, 10.0f, 5.0f);
            glVertex3f(-20.0f, 10.0f, 5.0f);				// Top back Of The Quad (right)
            glVertex3f(-20.0f, -10.0f, 5.0f);




            glVertex3f(20.0f, -10.0f, 0.0f);
            glVertex3f(20.0f, -10.0f, 5.0f);
            glVertex3f(-20.0f, -10.0f, 5.0f);
            glVertex3f(-20.0f, -10.0f, 0.0f);

            glVertex3f(20.0f, 10.0f, 0.0f);
            glVertex3f(20.0f, 10.0f, 5.0f);
            glVertex3f(-20.0f, 10.0f, 5.0f);
            glVertex3f(-20.0f, 10.0f, 0.0f);



            glVertex3f(20.0f, 10.0f, 0.0f);
            glVertex3f(20.0f, 10.0f, 5.0f);
            glVertex3f(20.0f, -10.0f, 5.0f);
            glVertex3f(20.0f, -10.0f, 0.0f);

            glVertex3f(-20.0f, 10.0f, 0.0f);
            glVertex3f(-20.0f, 10.0f, 5.0f);
            glVertex3f(-20.0f, -10.0f, 5.0f);
            glVertex3f(-20.0f, -10.0f, 0.0f);

        glEnd();
}

void grass()
{
    glBegin(GL_QUADS);

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(20.0f, -10.0f, 5.0f);					    // Top front Of The Quad (right)
            glVertex3f(20.0f, 10.0f, 5.0f);
            glVertex3f(-20.0f, 10.0f, 5.0f);				// Top back Of The Quad (right)
            glVertex3f(-20.0f, -10.0f, 5.0f);					// Bottom front Of The Quad (right)

            glVertex3f(20.0f, -10.0f, 7.0f);					    // Top front Of The Quad (right)
            glVertex3f(20.0f, 10.0f, 7.0f);
            glVertex3f(-20.0f, 10.0f, 7.0f);				// Top back Of The Quad (right)
            glVertex3f(-20.0f, -10.0f, 7.0f);



            glVertex3f(20.0f, -10.0f, 5.0f);
            glVertex3f(20.0f, -10.0f, 7.0f);
            glVertex3f(-20.0f, -10.0f, 7.0f);
            glVertex3f(-20.0f, -10.0f, 5.0f);

            glVertex3f(20.0f, 10.0f, 5.0f);
            glVertex3f(20.0f, 10.0f, 7.0f);
            glVertex3f(-20.0f, 10.0f, 7.0f);
            glVertex3f(-20.0f, 10.0f, 5.0f);



            glVertex3f(20.0f, 10.0f, 5.0f);
            glVertex3f(20.0f, 10.0f, 7.0f);
            glVertex3f(20.0f, -10.0f, 7.0f);
            glVertex3f(20.0f, -10.0f, 5.0f);

            glVertex3f(-20.0f, 10.0f, 5.0f);
            glVertex3f(-20.0f, 10.0f, 7.0f);
            glVertex3f(-20.0f, -10.0f, 7.0f);
            glVertex3f(-20.0f, -10.0f, 5.0f);

        glEnd();
}
void grass1()
{
    glBegin(GL_QUADS);

            glColor3f(0.0f, 0.39f, 0.0f);
            glVertex3f(20.0f, -10.0f, 7.0f);					    // Top front Of The Quad (right)
            glVertex3f(20.0f, 10.0f, 7.0f);
            glVertex3f(-20.0f, 10.0f, 7.0f);				// Top back Of The Quad (right)
            glVertex3f(-20.0f, -10.0f, 7.0f);					// Bottom front Of The Quad (right)

            glVertex3f(20.0f, -10.0f, 8.0f);					    // Top front Of The Quad (right)
            glVertex3f(20.0f, 10.0f, 8.0f);
            glVertex3f(-20.0f, 10.0f, 8.0f);				// Top back Of The Quad (right)
            glVertex3f(-20.0f, -10.0f, 8.0f);




            glVertex3f(20.0f, -10.0f, 7.0f);
            glVertex3f(20.0f, -10.0f, 8.0f);
            glVertex3f(-20.0f, -10.0f, 8.0f);
            glVertex3f(-20.0f, -10.0f, 7.0f);

            glVertex3f(20.0f, 10.0f, 7.0f);
            glVertex3f(20.0f, 10.0f, 8.0f);
            glVertex3f(-20.0f, 10.0f, 8.0f);
            glVertex3f(-20.0f, 10.0f, 7.0f);



            glVertex3f(20.0f, 10.0f, 7.0f);
            glVertex3f(20.0f, 10.0f, 8.0f);
            glVertex3f(20.0f, -10.0f, 8.0f);
            glVertex3f(20.0f, -10.0f, 7.0f);

            glVertex3f(-20.0f, 10.0f, 7.0f);
            glVertex3f(-20.0f, 10.0f, 8.0f);
            glVertex3f(-20.0f, -10.0f, 8.0f);
            glVertex3f(-20.0f, -10.0f, 7.0f);

        glEnd();
}

void rec_brick(double r, double g, double b)
{
  // Draw A Quad
        glBegin(GL_QUADS);

            glColor3f(r,g,b);
            glVertex3f(2.0f, -1.0f, -1.0f);					    // Top front Of The Quad (right)
            glVertex3f(2.0f, 1.0f, -1.0f);
            glVertex3f(-2.0f, 1.0f, -1.0f);				// Top back Of The Quad (right)
            glVertex3f(-2.0f, -1.0f, -1.0f);					// Bottom front Of The Quad (right)

            glColor3f(r,g,b);
            glVertex3f(2.0f, -1.0f, 1.0f);					    // Top front Of The Quad (right)
            glVertex3f(2.0f, 1.0f, 1.0f);
            glVertex3f(-2.0f, 1.0f, 1.0f);				// Top back Of The Quad (right)
            glVertex3f(-2.0f, -1.0f, 1.0f);




            glColor3f(r,g,b);
            glVertex3f(2.0f, -1.0f, -1.0f);
            glVertex3f(2.0f, -1.0f, 1.0f);
            glVertex3f(-2.0f, -1.0f, 1.0f);
            glVertex3f(-2.0f, -1.0f, -1.0f);

            glColor3f(r,g,b);
            glVertex3f(2.0f, 1.0f, -1.0f);
            glVertex3f(2.0f, 1.0f, 1.0f);
            glVertex3f(-2.0f, 1.0f, 1.0f);
            glVertex3f(-2.0f, 1.0f, -1.0f);



            glColor3f(r,g,b);
            glVertex3f(2.0f, 1.0f, -1.0f);
            glVertex3f(2.0f, 1.0f, 1.0f);
            glVertex3f(2.0f, -1.0f, 1.0f);
            glVertex3f(2.0f, -1.0f, -1.0f);

            glColor3f(r,g,b);
            glVertex3f(-2.0f, 1.0f, -1.0f);
            glVertex3f(-2.0f, 1.0f, 1.0f);
            glVertex3f(-2.0f, -1.0f, 1.0f);
            glVertex3f(-2.0f, -1.0f, -1.0f);

        glEnd();
}

void rec2_brick(double r, double g, double b)
{
  // Draw A Quad
        glBegin(GL_QUADS);

            glColor3f(r,g,b);
            glVertex3f(4.0f, -1.0f, -0.5f);					    // Top front Of The Quad (right)
            glVertex3f(4.0f, 1.0f, -0.5f);
            glVertex3f(-4.0f, 1.0f, -0.5f);				// Top back Of The Quad (right)
            glVertex3f(-4.0f, -1.0f, -0.5f);					// Bottom front Of The Quad (right)

            glColor3f(r,g,b);
            glVertex3f(4.0f, -1.0f, 0.5f);					    // Top front Of The Quad (right)
            glVertex3f(4.0f, 1.0f, 0.5f);
            glVertex3f(-4.0f, 1.0f, 0.5f);				// Top back Of The Quad (right)
            glVertex3f(-4.0f, -1.0f, 0.5f);




            glColor3f(r,g,b);
            glVertex3f(4.0f, -1.0f, -0.5f);
            glVertex3f(4.0f, -1.0f, 0.5f);
            glVertex3f(-4.0f, -1.0f, 0.5f);
            glVertex3f(-4.0f, -1.0f, -0.5f);

            glColor3f(r,g,b);
            glVertex3f(4.0f, 1.0f, -0.5f);
            glVertex3f(4.0f, 1.0f, 0.5f);
            glVertex3f(-4.0f, 1.0f, 0.5f);
            glVertex3f(-4.0f, 1.0f, -0.5f);



            glColor3f(r,g,b);
            glVertex3f(4.0f, 1.0f, -0.5f);
            glVertex3f(4.0f, 1.0f, 0.5f);
            glVertex3f(4.0f, -1.0f, 0.5f);
            glVertex3f(4.0f, -1.0f, -0.5f);

            glColor3f(r,g,b);
            glVertex3f(-4.0f, 1.0f, -0.5f);
            glVertex3f(-4.0f, 1.0f, 0.5f);
            glVertex3f(-4.0f, -1.0f, 0.5f);
            glVertex3f(-4.0f, -1.0f, -0.5f);

        glEnd();
}
void ground_rec_brick()
{
  // Draw A Quad
        glBegin(GL_QUADS);

            glColor3f(0.365f,0.239f,0.18f);
            glVertex3f(5.0f, -2.0f, -2.0f);					    // Top front Of The Quad (right)
            glVertex3f(5.0f, 2.0f, -2.0f);
            glVertex3f(-5.0f, 2.0f, -2.0f);				// Top back Of The Quad (right)
            glVertex3f(-5.0f, -2.0f, -2.0f);					// Bottom front Of The Quad (right)

            glColor3f(0.365f,0.239f,0.18f);
            glVertex3f(5.0f, -2.0f, 2.0f);					    // Top front Of The Quad (right)
            glVertex3f(5.0f, 2.0f, 2.0f);
            glVertex3f(-5.0f, 2.0f, 2.0f);				// Top back Of The Quad (right)
            glVertex3f(-5.0f, -2.0f, 2.0f);




            glColor3f(0.365f,0.239f,0.18f);
            glVertex3f(5.0f, -2.0f, -2.0f);
            glVertex3f(5.0f, -2.0f, 2.0f);
            glVertex3f(-5.0f, -2.0f, 2.0f);
            glVertex3f(-5.0f, -2.0f, -2.0f);

            glColor3f(0.365f,0.239f,0.18f);
            glVertex3f(5.0f, 2.0f, -2.0f);
            glVertex3f(5.0f, 2.0f, 2.0f);
            glVertex3f(-5.0f, 2.0f, 2.0f);
            glVertex3f(-5.0f, 2.0f, -2.0f);



            glColor3f(0.365f,0.239f,0.18f);
            glVertex3f(5.0f, 2.0f, -2.0f);
            glVertex3f(5.0f, 2.0f, 2.0f);
            glVertex3f(5.0f, -2.0f, 2.0f);
            glVertex3f(5.0f, -2.0f, -2.0f);

            glColor3f(0.365f,0.239f,0.18f);
            glVertex3f(-5.0f, 2.0f, -2.0f);
            glVertex3f(-5.0f, 2.0f, 2.0f);
            glVertex3f(-5.0f, -2.0f, 2.0f);
            glVertex3f(-5.0f, -2.0f, -2.0f);

        glEnd();
}


void sq_brick(double r, double g, double b)
{
        glBegin(GL_QUADS);

            glColor3f(r,g,b);
            glVertex3f(1.0f, -1.0f, -1.0f);					    // Top front Of The Quad (right)
            glVertex3f(1.0f, 1.0f, -1.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);				// Top back Of The Quad (right)
            glVertex3f(-1.0f, -1.0f, -1.0f);					// Bottom front Of The Quad (right)

            glColor3f(r,g,b);
            glVertex3f(1.0f, -1.0f, 1.0f);					    // Top front Of The Quad (right)
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);				// Top back Of The Quad (right)
            glVertex3f(-1.0f, -1.0f, 1.0f);




            glColor3f(r,g,b);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);

            glColor3f(r,g,b);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);



            glColor3f(r,g,b);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);

            glColor3f(r,g,b);
            glVertex3f(-1.0f, 1.0f, -1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);

        glEnd();
}

void background(){
    glBegin(GL_QUADS);
    glColor3f(0.45f,0.60f,0.98f);
    glVertex3f(100.0f,0.0f,100.0f);
    glVertex3f(100.0f,0.0f,-100.0f);
    glVertex3f(-100.0f,0.0f,-100.0f);
    glVertex3f(-100.0f,0.0f,100.0f);
    glEnd();
}


void my_scene()
{
    glPushMatrix();
    glTranslated(0,25,0);
    background();
    glPopMatrix();
    ground();
    grass();
    grass1();


    glPushMatrix();
    glTranslated( -12, 0, 10);
    ground_rec_brick();
    glPopMatrix();


    glPushMatrix();
    glTranslated( brickh[0], 0, brickv[0]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();


    glPushMatrix();
    glTranslated( brickh[1], 0, brickv[1]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();


    glPushMatrix();
    glTranslated( brickh[2], 0, brickv[2]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();

    glPushMatrix();
    glTranslated( brickh[3], 0, brickv[3]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();

    glPushMatrix();
    glTranslated( brickh[4], 0, brickv[4]);
    rec_brick(0.365f,0.239f,0.18f);
    glPopMatrix();


    glPushMatrix();
    glTranslated( brickh[5], 0, brickv[5]);
    rec_brick(0.365f,0.239f,0.18f);
    glPopMatrix();


    glPushMatrix();
    glTranslated( brickh[6], 0, brickv[6]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();

    glPushMatrix();
    glTranslated( brickh[7], 0, brickv[7]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();


    glPushMatrix();
    glTranslated( brickh[8], 0, brickv[8]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();

    glPushMatrix();
    glTranslated( brickh[9], 0, brickv[9]);
    sq_brick(0.388f, 0.969f, 0.941);
    glPopMatrix();

    glPushMatrix();
    glTranslated( brickh[10], 0, brickv[10]);
    rec2_brick(0.365f,0.239f,0.18f);
    glPopMatrix();



    glPushMatrix();
    glTranslated( brickh[11], 0, brickv[11]);
    sq_brick(0.478f, 0.502f, 0.525f); // ash
    glPopMatrix();
    glPushMatrix();
    glTranslated( brickh[12], 0, brickv[12]);
    sq_brick(0.478f, 0.502f, 0.525f); // ash
    glPopMatrix();
    glPushMatrix();
    glTranslated( brickh[13], 0, brickv[13]);
    sq_brick(0.478f, 0.502f, 0.525f); // ash
    glPopMatrix();


    if(c<=27){
        glPushMatrix();
        glTranslated( bh, 0, bv);
        drawSphere(1.5, 15, 15);
        glPopMatrix();


        glPushMatrix();
        glTranslated( bh+0.8, -1.5, bv+0.5);
        drawEyes(0.3, 15, 15);
        glPopMatrix();

        glPushMatrix();
        glTranslated( bh+1, -1, bv-1);
        glRotatef(0,0,0,1);
        drawCone(0.4, 1, 15);
        glPopMatrix();


    }


}



void keyboardListener(unsigned char key, int xx,int yy){
    double x,y,z;
    double rate = 0.01;
	switch(key){

		case '1':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+1*r.x*sin(rate);
			l.y = l.y*cos(rate)+r.y*sin(rate);
			l.z = l.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-x*sin(rate);
			r.y = r.y*cos(rate)-y*sin(rate);
			r.z = r.z*cos(rate)-z*sin(rate);}
			break;
        case '2':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+r.x*sin(-rate);
			l.y = l.y*cos(-rate)+r.y*sin(-rate);
			l.z = l.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-x*sin(-rate);
			r.y = r.y*cos(-rate)-y*sin(-rate);
			r.z = r.z*cos(-rate)-z*sin(-rate);
			}
			break;
        case '3':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '4':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+1*u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case '6':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(rate)+u.x*sin(rate);
			r.y = r.y*cos(rate)+u.y*sin(rate);
			r.z = r.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '5':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(-rate)+u.x*sin(-rate);
			r.y = r.y*cos(-rate)+u.y*sin(-rate);
			r.z = r.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case 'p':
            if(c<=17){
                printf("%d-",c);
                c+=1;
                bh += 0.8;
                if(c>=15){
                    bv+=0;
                }
                else{
                    bv += 0.5;
                }
            }
            else if(c>=17 && c<= 27){
                printf("%d-",c);
                c+=1;
                bh += 0.8;
                if(c<=10){
                    bv+=0;
                }
                else{
                    bv -= 0.5;
                }
            }
            if(c>=27)
            {
                c+=1;
                for(int i=0;i<14;i++)
                {
                    var = rand() % 100;
                    var = var /100;
                    if(brickv[i]> 9 && brickh[i]<22){
                        brickh[i] += var;
                        brickv[i] -= var;
                    }
                    else if(brickv[i]<=9 && brickh[i]<22){
                        brickh[i] += var;
                    }
                    else if(brickh[i]>=22){
                        brickh[i] += var;
                        brickv[i] -= var;
                    }

                }
            }

		default:
			break;
	}

}


void specialKeyListener(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_LEFT :
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_RIGHT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;
		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y)
{	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP

			}
			break;


		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP

				//(600/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
			}
			break;
			//........

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display()
{

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    //drawSphere(50,30,30);
    //glRotated(30,1,0,0);
    //quad();
    //drawSS();

    //draw_cylinder(20,50,20);

    //pyramid();
    //scene();
    my_scene();


    //drawCircle(30,24);

    //drawCone(20,50,50);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

    angle+=0.1;
	glutPostRedisplay();
}

void init(){
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	5000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv)
{
    srand (time(NULL));
    pos.x=0;
    pos.y=-49.083097;
    pos.z=11.301849;
    l.x=-0.000016;u.x=-0.000796;r.x=-1.000000;
    l.y=0.994043;u.y=0.108987;r.y=-0.000103;
    l.z=-0.108987;u.z=0.994043;r.z=-0.000790;


	glutInit(&argc,argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}


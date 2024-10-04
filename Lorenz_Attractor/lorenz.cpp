//Angela Harumi Booth
//CSCI 4229
//Please view READ ME for additional information

#include <GL/glut.h>
#include <stdio.h>

/* Global Variables */
/* Coordinates */
double x = 1.0;
double y = 1.0;
double z = 1.0;
/* Lorenz Parameters */
/*These values were taken from https://www.prinmath.com/csci5229/misc/lorenz.c */
double s = 10.0;
double b = 2.6666;
double r = 28.0;
/* Time step */
double dt = 0.001;

/* Camera rotation angles */
float angleX = 0.0f;
float angleY = 0.0f;

/* Function to draw the 3D axes */
/*From ex7.c provided by Professor Schreuder*/
void drawAxes()
{
    const double len = 30.0; // Length of axes
    glBegin(GL_LINES);
    // X axis
    glColor3f(1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(len, 0.0, 0.0);
    // Y axis
    glColor3f(0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, len, 0.0);
    // Z axis
    glColor3f(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, len);
    glEnd();

}
/* Display function to render the attractor */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply camera rotations
    glTranslatef(0.0f, 0.0f, -100.0f); // Move back to view the plot
    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotate around x-axis
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotate around y-axis

    //Draw axis
    drawAxes();

    // Set line color to a gradient (optional)
    glColor3f(1.0f, 0.0f, 0.0f); // Start color red

    // Reset initial conditions
    x = 1.0;
    y = 1.0;
    z = 1.0;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 50000; i++)
    {
        //Lorenz attractor
        //Source: https://www.prinmath.com/csci5229/misc/lorenz.c
        double dx = s * (y - x);
        double dy = x * (r - z) - y;
        double dz = x * y - b * z;
        x += dt * dx;
        y += dt * dy;
        z += dt * dz;

        // Optionally change color dynamically
        glColor3f((float)i / 50000, 0.0f, 1.0f - (float)i / 50000);

        glVertex3f(x, y, z);
    }
    glEnd();

    glutSwapBuffers();
}

/* Reshape function to handle window resizing */
void reshape(int width, int height)
{
    // Prevent division by zero
    if (height == 0) height = 1;

    // Set the viewport to cover the new window
    // We set the width and height to be the same as the new window size 
    // to ensure the scene is redrawn with adjustments to its new size.
    glViewport(0, 0, width, height); 

    // Adjust the projection matrix to maintain the aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

/* Switch to allow for changes to Lorenz Parameters and adjusting camera angle */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w': angleX -= 5.0f; break; // Rotate up
    case 's': angleX += 5.0f; break; // Rotate down
    case 'a': angleY -= 5.0f; break; // Rotate left
    case 'd': angleY += 5.0f; break; // Rotate right
    case '1': s += 1.0; break;       // Increase 's'
    case '2': s -= 1.0; break;       // Decrease 's'
    case '3': r += 1.0; break;       // Increase 'r'
    case '4': r -= 1.0; break;       // Decrease 'r'
    case '5': b += 0.1; break;       // Increase 'b'
    case '6': b -= 0.1; break;       // Decrease 'b'
    }
    glutPostRedisplay(); // Redraw with updated parameters
}

/* Initialization function */
void init()
{
    glMatrixMode(GL_PROJECTION);            //Set martix mode to Projection matrix
    glLoadIdentity();                       //Load indentity matrix into projection matric
    gluPerspective(60.0, 1.0, 1.0, 1000.0);  //FOV = 60 Aspect ratio = 1 Clipping plane distance = 1 (anything closer will not be drawn) Far clipping plane distance = 1000
    glMatrixMode(GL_MODELVIEW);             //Allows us to rotate/move camera and adjusts how scene is viewed
    glEnable(GL_DEPTH_TEST);                // Enable depth testing for proper 3D rendering. Ensures anything drawn closer to the camera is drawn in front of pixels that further.
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Assignment 1: Angela Harumi Booth");
    glutInitWindowSize(800, 800);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Register reshape callback
    glutKeyboardFunc(keyboard); // Register keyboard callback
    init();
    glutMainLoop();
    return 0;
}

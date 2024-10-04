#include <GL/glut.h>
#include <cmath>

// Variables to control camera position
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;
float cameraDistance = 10.0f;

// Function to draw a circle (for petals and flower center)
void drawCircle(float radius, int segments) {
    float angleStep = 2.0f * M_PI / segments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);  // Center of the circle
    for (int i = 0; i <= segments; i++) {
        float angle = i * angleStep;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        glVertex3f(x, 0.0f, y);  // Vertex on the circle's edge
    }
    glEnd();
}

// Function to draw a flower
void drawFlower() {
    int numPetals = 6;
    float petalRadius = 0.1f;  // Smaller petals
    float petalDistance = 0.15f;  // Distance from the center

    // Draw the flower's center (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow center
    drawCircle(0.05f, 30);  // Smaller center of the flower

    // Draw petals around the center (red)
    glColor3f(1.0f, 0.0f, 0.0f);  // Red petals
    for (int i = 0; i < numPetals; i++) {
        float angle = i * 2.0f * M_PI / numPetals;
        float x = cos(angle) * petalDistance;
        float y = sin(angle) * petalDistance;

        glPushMatrix();
        glTranslatef(x, 0.0f, y);  // Position each petal
        drawCircle(petalRadius, 30);  // Draw petal
        glPopMatrix();
    }
}

// Function to draw a tree trunk (a manually created cylinder)
void drawTrunk(float baseRadius, float height, int segments) {
    float angleStep = 2.0f * M_PI / segments;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = i * angleStep;
        float x = cos(angle) * baseRadius;
        float z = sin(angle) * baseRadius;

        glVertex3f(x, 0.0f, z);  // Bottom circle
        glVertex3f(x, height, z);  // Top circle
    }
    glEnd();

    // Draw the base of the trunk
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float angle = i * angleStep;
        float x = cos(angle) * baseRadius;
        float z = sin(angle) * baseRadius;
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

// Function to draw the canopy (a manually created cone)
void drawCanopy(float baseRadius, float height, int segments) {
    float angleStep = 2.0f * M_PI / segments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, height, 0.0f);  // Cone tip
    for (int i = 0; i <= segments; i++) {
        float angle = i * angleStep;
        float x = cos(angle) * baseRadius;
        float z = sin(angle) * baseRadius;
        glVertex3f(x, 0.0f, z);  // Base of the cone
    }
    glEnd();
}

// Function to draw a tree (trunk + canopy)
void drawTree() {
    // Draw the trunk
    glColor3f(0.55f, 0.27f, 0.07f);  // Brown color for the trunk
    drawTrunk(0.3f, 2.0f, 20);

    // Move canopy above the trunk
    glTranslatef(0.0f, 2.0f, 0.0f);

    // Draw the canopy
    glColor3f(0.0f, 0.8f, 0.0f);  // Green color for the canopy
    drawCanopy(1.0f, 1.5f, 20);
}

// Function to manually generate a donut (torus)
//Source for torus implementation: https://www.songho.ca/opengl/gl_torus.html
void drawManualTorus(float innerRadius, float outerRadius, int sides, int rings) {
    float ringStep = 2.0f * M_PI / rings;  // Step size for the angle around the larger circle
    float sideStep = 2.0f * M_PI / sides;  // Step size for the angle around the smaller cross-sectional circle

    // Loop through each ring (outer circle)
    for (int i = 0; i <= rings; ++i) {
        float theta = i * ringStep;          // Angle for the current ring
        float nextTheta = (i + 1) * ringStep; // Angle for the next ring

        glBegin(GL_QUAD_STRIP);  // Start drawing a series of quads
        // Loop through each side (small circle)
        for (int j = 0; j <= sides; ++j) {
            float phi = j * sideStep;        // Angle for the current side
            float cosPhi = cos(phi);         // Cache cos(phi) for efficiency
            float sinPhi = sin(phi);         // Cache sin(phi) for efficiency

            // Compute the first vertex on the current ring
            float x0 = (outerRadius + innerRadius * cosPhi) * cos(theta);
            float y0 = innerRadius * sinPhi;
            float z0 = (outerRadius + innerRadius * cosPhi) * sin(theta);

            // Compute the second vertex on the next ring
            float x1 = (outerRadius + innerRadius * cosPhi) * cos(nextTheta);
            float y1 = innerRadius * sinPhi;
            float z1 = (outerRadius + innerRadius * cosPhi) * sin(nextTheta);

            // First vertex
            glVertex3f(x0, y0, z0);

            // Second vertex
            glVertex3f(x1, y1, z1);
        }
        glEnd();  // End drawing the quad strip
    }
}

// Function to draw a hanging donut (tire swing) with a line connecting it to the tree's top
void drawDonutWithLineToTree() {
    // Draw the donut (tire swing)
    glPushMatrix();
    glTranslatef(-2.25f, 1.0f, 0.0f);  // Move closer (x = -2.5) and lower (y = 1.0)
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to hang vertically
    glColor3f(0.2f, 0.2f, 0.2f);  // Set color to darker grey
    drawManualTorus(0.1f, 0.4f, 30, 30);  // Manually draw the donut
    glPopMatrix();

    // Draw the line (rope) connecting the donut to the top of the tree
    glColor3f(1.0f, 1.0f, 0.0f);  // Set line color to yellow
    glBegin(GL_LINES);
    glVertex3f(-2.25f, 1.5f, 0.0f);  // Top of the donut
    glVertex3f(-3.0f, 3.5f, 0.0f);  // Top of the tree
    glEnd();
}


// Function to draw the grass (a large green quad)
void drawGrass() {
    glColor3f(0.0f, 0.6f, 0.0f);  // Darker green color for the grass
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);  // Bottom-left corner
    glVertex3f( 10.0f, 0.0f, -10.0f);  // Bottom-right corner
    glVertex3f( 10.0f, 0.0f,  10.0f);  // Top-right corner
    glVertex3f(-10.0f, 0.0f,  10.0f);  // Top-left corner
    glEnd();
}

// Function to draw multiple flowers
void drawFlowers() {
    // Draw flower 1
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 2.0f);  // Flower 1 location
    drawFlower();
    glPopMatrix();

    // Draw flower 2
    glPushMatrix();
    glTranslatef(-1.5f, 0.01f, 1.0f);  // Flower 2 location
    drawFlower();
    glPopMatrix();

    // Draw flower 3
    glPushMatrix();
    glTranslatef(1.5f, 0.01f, 1.5f);  // Flower 3 location
    drawFlower();
    glPopMatrix();

    // Draw flower 4
    glPushMatrix();
    glTranslatef(2.0f, 0.01f, 0.0f);  // Flower 4 location
    drawFlower();
    glPopMatrix();
}

// Function to draw the entire scene with multiple tree instances, flowers, and a donut (torus)
void drawScene() {
    // Draw the grass (ground)
    drawGrass();

    // First tree instance
    glPushMatrix();
    glTranslatef(-3.0f, 0.0f, 0.0f);  // Move left
    glScalef(1.0f, 1.0f, 1.0f);  // Default scale
    drawTree();  // Draw tree
    glPopMatrix();

    // Second tree instance
    glPushMatrix();
    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);  // Rotate around Y-axis (apply rotation first)
    glTranslatef(3.0f, 0.0f, 0.0f);  // Then translate to move it to the right
    glScalef(0.7f, 0.7f, 0.7f);  // Scale down the second tree
    drawTree();  // Draw tree
    glPopMatrix();

    // Draw a hanging donut (tire swing)
    drawDonutWithLineToTree();
    // Draw flowers on the grass
    drawFlowers();
}

// Display function for rendering the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera positioning
    gluLookAt(cameraDistance * cos(cameraAngleY) * sin(cameraAngleX), 
              cameraDistance * sin(cameraAngleY),
              cameraDistance * cos(cameraAngleY) * cos(cameraAngleX),
              0.0f, 0.0f, 0.0f,   // Look at the origin
              0.0f, 1.0f, 0.0f);  // Up vector

    drawScene();

    glutSwapBuffers();
}

// Keyboard function to control the camera
void keyboard(int key, int x, int y) {
    //prevent user from seeing below grass
    const float upperLimit = M_PI / 2 - 0.1f;   // Limit to 90 degrees up
    const float lowerLimit = 0.1f;        // Limit to just slightly above the horizon

    switch (key) {
    case GLUT_KEY_LEFT:
        cameraAngleX -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        cameraAngleX += 0.1f;
        break;
    case GLUT_KEY_UP:
        cameraAngleY += 0.1f;
        if (cameraAngleY > upperLimit)  // Limit upward movement
            cameraAngleY = upperLimit;
        break;
    case GLUT_KEY_DOWN:
        cameraAngleY -= 0.1f;
        if (cameraAngleY < lowerLimit)  // Limit downward movement to avoid seeing under the grass
            cameraAngleY = lowerLimit;
        break;
    }
    glutPostRedisplay();
}

// Initialization function
void init() {
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for proper occlusion
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);  // Perspective projection
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("HW3 Angela Booth");

    init();
    
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);  // Use arrow keys for camera control

    glutMainLoop();
    return 0;
}

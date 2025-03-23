/*********************************
 * Ergasia 3 – 14/1/2025
 * Kostopoulou Georgia - 03449
 * Tsitoni Eleni – 03447
 *********************************/

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "views.hpp"

float cameraPosX = 0.0f,cameraPosY = 2.0f, cameraPosZ = 30.0f;
float cameraLookX = 0.0f, cameraLookY = 0.0f, cameraLookZ = -1.0f;
float cameraSpeed = 1.0f;

void setupLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);

    GLfloat globalAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f}; 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat lightDiffuse[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightAmbient[]  = {0.4f, 0.4f, 0.4f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);

}


void updateLighting() {
    GLfloat lightPosition[] = {cameraPosX, cameraPosY + 5.0f, cameraPosZ - 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.0f, 1.0f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 
              cameraPosX + cameraLookX, cameraPosY + cameraLookY, cameraPosZ + cameraLookZ, 
              0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    drawGrass();    
    drawSky();
    drawHouse();
    glTranslatef(5.0f, ROOM_HEIGHT, 0.0f);
    drawRoof(7.2f, 12.0f, 2.9f, roofTextID);
    glutSwapBuffers();
}

void cleanup() {
    if (glutGetWindow()) { 
        GLuint textures[] = {floorTexID, skyTexID, wallTexID, grassTexID, gravelTexID, bathroomTexID, carpetTextID, roofTextID};
        glDeleteTextures(8, textures);
    } 

}
void handleKeyboard(unsigned char key, int x, int y) {
    (void)x; 
    (void)y;
    const float rotationAngle = 5.0f * (M_PI / 180.0f); 
    switch (key) {
        case 'F': case 'f': 
            cameraPosX += cameraLookX * cameraSpeed;
            cameraPosY += cameraLookY * cameraSpeed;
            cameraPosZ += cameraLookZ * cameraSpeed;
            break;
        case 'B': case 'b': 
            cameraPosX -= cameraLookX * cameraSpeed;
            cameraPosY -= cameraLookY * cameraSpeed;
            cameraPosZ -= cameraLookZ * cameraSpeed;
            break;
        case 'R': case 'r': { 
            float newLookX = cameraLookX * cos(rotationAngle) - cameraLookZ * sin(rotationAngle);
            float newLookZ = cameraLookX * sin(rotationAngle) + cameraLookZ * cos(rotationAngle);
            cameraLookX = newLookX;
            cameraLookZ = newLookZ;
            break;
        }
        case 'L': case 'l': { 
            float newLookX = cameraLookX * cos(-rotationAngle) - cameraLookZ * sin(-rotationAngle);
            float newLookZ = cameraLookX * sin(-rotationAngle) + cameraLookZ * cos(-rotationAngle);
            cameraLookX = newLookX;
            cameraLookZ = newLookZ;
            break;
        }
        case 'U': case 'u': 
            cameraPosY += cameraSpeed;
            break;
        case 'D': case 'd': 
            cameraPosY -= cameraSpeed;
            break;
        case 'Q': case 'q': 
            quitApplication();
            exit(0);
            break;
        default:
            break;
    }

    if (cameraPosX < -50.0f) cameraPosX = -50.0f;
    if (cameraPosX > 50.0f) cameraPosX = 50.0f;
    if (cameraPosY < 1.0f) cameraPosY = 1.0f;
    if (cameraPosY > 40.0f) cameraPosY = 40.0f;
    if (cameraPosZ < -50.0f) cameraPosZ = -50.0f;
    if (cameraPosZ > 50.0f) cameraPosZ = 50.0f;

    updateLighting();
    glutPostRedisplay();
}

void setupMaterials() {
    GLfloat matAmbient[]   = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat matDiffuse[]   = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat matSpecular[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT,   matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    atexit(cleanup);

    // First Window
    glutCreateWindow("3D House Exploration");
    glutFullScreen();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);

    updateLighting();
    setupLighting();
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    setupMaterials();
    
    loadTextures();
    initGrassPoints();
    initSkyPoints();

    // Second Window
   glutCreateWindow("Projections");
    glutFullScreen();
    glutDisplayFunc(viewsDisplay);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    loadTextures();
    glutKeyboardFunc(handleKeyboard);

    glutMainLoop();
    glutSwapBuffers();
    return 0;
} 
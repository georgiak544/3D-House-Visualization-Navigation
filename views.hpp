

#ifndef HOUSE_H
#define HOUSE_H

#include <GL/glut.h>

static const float ROOM_HEIGHT = 3.0f;
static const float EXT_WALL_THK = 0.25f;
static const float INT_WALL_THK = 0.1f;
static const float FLOOR_THK = 0.1f;
static const float CEIL_THK = 0.1f;

extern GLuint floorTexID;
extern GLuint skyTexID;
extern GLuint wallTexID;
extern GLuint grassTexID;
extern GLuint gravelTexID;
extern GLuint bathroomTexID;
extern GLuint roofTextID;
extern GLuint carpetTextID;

unsigned char* loadBitmap(const char* filename, int* width, int* height);
GLuint loadTexture(const char* filename);
void loadTextures();

void initGrassPoints();
void initSkyPoints();

void drawGrass();
void drawSky();

void drawExteriorWalls(float length, float width, float height);

void drawBox(float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ, float colorR, float colorG, float colorB);
void drawBoxWithTexture(float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ, GLuint textureID);

void drawShop(float L, float W, float H);
void drawGreatRoom(float L, float W, float H);
void drawDining(float L, float W, float H);
void drawBathroom(float L, float W, float H);
void drawKitchen(float L, float W, float H);
void drawMasterBedroom(float L, float W, float H);
void drawAisle(float L, float W, float H);
void drawBedroom2(float L, float W, float H);
void drawDoor(float x, float y, float z, float width, float height, float thickness, float red, float green, float blue);

void drawBed(float x, float y, float z, float width, float depth, float height);
void drawSofa(float x, float y, float z, float width, float depth, float height);
void drawCoffeeTable(float x, float y, float z, float width, float depth, float height);
void drawChair(float x, float y, float z, float width, float depth, float height);
void drawDiningTable(float x, float y, float z, float width, float depth, float height);
void drawNightstand(float x, float y, float z, float width, float depth, float height);
void drawPlantPot(float x, float y, float z, float potHeight, float potRadius, float foliageRadius);

void setRoofMaterial();
void drawRoof(float length, float width, float height, GLuint textureID);
void drawBeams(float x, float y, float z, float width, float height, float depth);
void drawRoof(float x, float y, float z, float width, float height, float depth);

void drawHouse();
void drawTree(float x, float y, float z, float trunkHeight, float trunkRadius, float foliageRadius);

void quitApplication();
void viewsDisplay();

#endif 

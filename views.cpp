
#include <GL/glut.h>
#include <cmath>
#include <iostream>

static const float ROOM_HEIGHT  = 3.0f;
static const float EXT_WALL_THK = 0.25f;
static const float INT_WALL_THK = 0.1f; 
static const float FLOOR_THK    = 0.1f;
static const float CEIL_THK     = 0.1f;

GLuint floorTexID;
GLuint skyTexID;
GLuint wallTexID;
GLuint grassTexID;
GLuint gravelTexID;
GLuint bathroomTexID;
GLuint carpetTextID;
GLuint roofTextID;

const int gridSize = 20; 
GLfloat grassControlPoints[gridSize][gridSize][3];

const int skyGridSize = 20; 
GLfloat skyControlPoints[skyGridSize][skyGridSize][3];
GLfloat textureCoords[skyGridSize][skyGridSize][2];

unsigned char* loadBitmap(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return nullptr;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file); 

    *width = *(int*)&header[18];  
    *height = *(int*)&header[22]; 

    int imageSize = 3 * (*width) * (*height); 
    unsigned char* data = new unsigned char[imageSize];
    fread(data, sizeof(unsigned char), imageSize, file);
    fclose(file);

    for (int i = 0; i < imageSize; i += 3) {
        std::swap(data[i], data[i + 2]); 
    }

    return data;
}

GLuint loadTexture(const char* filename) {
    int width, height;
    unsigned char* image = loadBitmap(filename, &width, &height);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] image; 

    return textureID;
}

void loadTextures() {
    floorTexID = loadTexture("wood.bmp");
    wallTexID = loadTexture("extwall.bmp");
    grassTexID = loadTexture("grass.bmp");
    gravelTexID = loadTexture("stone.bmp");
    skyTexID = loadTexture("sky.bmp");
    bathroomTexID = loadTexture("bathroom.bmp");
    carpetTextID = loadTexture("carpet.bmp");
    roofTextID = loadTexture("roof.bmp");
}

// Using for making objects
void drawBox(
    float scaleX, float scaleY, float scaleZ,
    float translateX, float translateY, float translateZ,
    float colorR, float colorG, float colorB
) {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(colorR, colorG, colorB);

    glPushMatrix();
        glTranslatef(translateX, translateY, translateZ);
        glScalef(scaleX, scaleY, scaleZ);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void drawBoxWithTexture(
    float scaleX, float scaleY, float scaleZ,
    float translateX, float translateY, float translateZ,
    GLuint textureID
) {
    glEnable(GL_TEXTURE_2D);         
    glBindTexture(GL_TEXTURE_2D, textureID); 

    glPushMatrix();
        glTranslatef(translateX, translateY, translateZ);
        glScalef(scaleX, scaleY, scaleZ);

        float repeatX = scaleX; 
        float repeatY = scaleY; 
        float repeatZ = scaleZ; 

        // Front
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glTexCoord2f(repeatX, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
            glTexCoord2f(repeatX, repeatY); glVertex3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(0.0f, repeatY); glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();

        // Back
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(repeatX, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glTexCoord2f(repeatX, repeatY); glVertex3f(0.5f, 0.5f, -0.5f);
            glTexCoord2f(0.0f, repeatY); glVertex3f(-0.5f, 0.5f, -0.5f);
        glEnd();

        // Left
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(repeatZ, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glTexCoord2f(repeatZ, repeatY); glVertex3f(-0.5f, 0.5f, 0.5f);
            glTexCoord2f(0.0f, repeatY); glVertex3f(-0.5f, 0.5f, -0.5f);
        glEnd();

        // Right
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glTexCoord2f(repeatZ, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
            glTexCoord2f(repeatZ, repeatY); glVertex3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(0.0f, repeatY); glVertex3f(0.5f, 0.5f, -0.5f);
        glEnd();

        // Top
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
            glTexCoord2f(repeatX, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
            glTexCoord2f(repeatX, repeatZ); glVertex3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(0.0f, repeatZ); glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();

        // Bottom
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(repeatX, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glTexCoord2f(repeatX, repeatZ); glVertex3f(0.5f, -0.5f, 0.5f);
            glTexCoord2f(0.0f, repeatZ); glVertex3f(-0.5f, -0.5f, 0.5f);
        glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D); 
}


// Initialize checkpoints for the sky
void initSkyPoints() {
    float radius = 80.0f;      
    int divisions = skyGridSize - 1; 

    for (int i = 0; i < skyGridSize; ++i) {
        for (int j = 0; j < skyGridSize; ++j) {
          
            float theta = (float)i / divisions * M_PI / 2; 
            float phi = (float)j / divisions * 2 * M_PI;   

            float x = radius * sin(theta) * cos(phi);
            float z = radius * sin(theta) * sin(phi);
            float y = radius * cos(theta) - radius / 2 + 30; 

            skyControlPoints[i][j][0] = x;
            skyControlPoints[i][j][1] = y;
            skyControlPoints[i][j][2] = z;

             textureCoords[i][j][0] = (float)j / divisions; 
             textureCoords[i][j][1] = 1.0f - (float)i / divisions; 
        }
    }
}
void drawSky() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyTexID); 

    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < skyGridSize - 1; ++i) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < skyGridSize; ++j) {
            glTexCoord2f(textureCoords[i][j][0], textureCoords[i][j][1]);
            glVertex3f(skyControlPoints[i][j][0], skyControlPoints[i][j][1], skyControlPoints[i][j][2]);

            glTexCoord2f(textureCoords[i + 1][j][0], textureCoords[i + 1][j][1]);
            glVertex3f(skyControlPoints[i + 1][j][0], skyControlPoints[i + 1][j][1], skyControlPoints[i + 1][j][2]);
        }
    glEnd();    
    }
    glDisable(GL_TEXTURE_2D);
}

// Initialize checkpoints for the grass
void initGrassPoints() {
    float gridSpacing = 10.0f;

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            float x = (i - gridSize / 2.0f) * gridSpacing;
            float z = (j - gridSize / 2.0f) * gridSpacing;

            grassControlPoints[i][j][0] = x;
            grassControlPoints[i][j][2] = z;
            // House area
            if (x >= -10 && x <= 10 && z >= -10 && z <= 10) {
                grassControlPoints[i][j][1] = -1.0f; 
            }
            else if (fabs(x + 20.0f) < 50.0f && fabs(z + 40.0f) < 50.0f) {
                float distance = sqrt(pow(x + 20.0f, 2) + pow(z + 40.0f, 2));
                grassControlPoints[i][j][1] = 10.0f - (distance / 50.0f) * 10.0f; 

                if (grassControlPoints[i][j][1] < 0.0f) {
                    grassControlPoints[i][j][1] = 0.0f; 
                }
            } 
            else {
                grassControlPoints[i][j][1] = 0.0f; 
            }
        }
    }
}

void drawGrass() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexID);

    glColor3f(0.0f, 0.5f, 0.0f); 

    for (int i = 0; i < gridSize - 1; ++i) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < gridSize; ++j) {
            glTexCoord2f((float)i / gridSize * 5, (float)j / gridSize * 5);
            glVertex3f(grassControlPoints[i][j][0],
                       grassControlPoints[i][j][1],
                       grassControlPoints[i][j][2]);

            glTexCoord2f((float)(i + 1) / gridSize * 5, (float)j / gridSize * 5);
            glVertex3f(grassControlPoints[i + 1][j][0],
                       grassControlPoints[i + 1][j][1],
                       grassControlPoints[i + 1][j][2]);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

void drawExteriorWalls(float length, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Front wall------------------------------------------------------------------------------------------------------------------------------------
    drawBoxWithTexture(5.5f , height, EXT_WALL_THK, -0.75f, height / 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);
    drawBoxWithTexture(1.5f, height - 2.0f, EXT_WALL_THK, 2.75f, (height + 2.0f)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);
    drawBoxWithTexture(1.5f, height - 2.0f, EXT_WALL_THK,  2.75f , (height - 2.0f)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);

    drawBoxWithTexture(4.1f , height, EXT_WALL_THK, 5.55f, height / 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);
    drawBoxWithTexture(1.4f, height , EXT_WALL_THK, 9.35f, (height)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);
    drawBoxWithTexture(1.1f, height - 2.0f, EXT_WALL_THK, 8.15f , (height + 2.0f)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);

    drawBoxWithTexture(2.1f, height - 2.0f, EXT_WALL_THK, 11.1f , (height + 2.0f)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);
    drawBoxWithTexture(2.1f, height - 2.0f, EXT_WALL_THK, 11.1f , (height - 2.0f)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);
    drawBoxWithTexture(1.0f, height, EXT_WALL_THK, 12.6f, (height)/ 2.0f, width / 2.0f - INT_WALL_THK, wallTexID);

    // Right wall----------------------------------------------------------------------------------------------------------------------------
    drawBoxWithTexture(EXT_WALL_THK - 0.1f, height, 2.2f, (length + 8.7 )/ 2.0f + EXT_WALL_THK, height / 2.0f, 5.0f, wallTexID); 
    drawBoxWithTexture(EXT_WALL_THK- 0.1f, height - 2.0f,  2.4f, (length + 8.7)/ 2.0f + EXT_WALL_THK, (height + 2.0f)/ 2.0f, 3.1f, wallTexID); 
    drawBoxWithTexture(EXT_WALL_THK- 0.1f, height - 2.0f,  2.4f, (length + 8.7)/ 2.0f + EXT_WALL_THK, (height - 2.0f)/ 2.0f, 3.1f, wallTexID);
    drawBoxWithTexture(EXT_WALL_THK - 0.1f, height,  7.9f, (length + 8.7)/ 2.0f + EXT_WALL_THK , (height)/ 2.0f, -2.0f, wallTexID);

    // Left wall-------------------------------------------------------------------------------------------------------------------------
    drawBoxWithTexture(EXT_WALL_THK - 0.1f, height, width, (-length + 10) / 2.0f + EXT_WALL_THK - 0.2, height / 2.0f, 0.0f, wallTexID);

    // Back wall-------------------------------------------------------------------------------------------------------------------------
    drawBoxWithTexture(1.3f , height, EXT_WALL_THK, 12.7f, height / 2.0f, -width / 2.0f + EXT_WALL_THK - 0.2f, wallTexID); 
    drawBoxWithTexture(2.1f, height - 2.0f, EXT_WALL_THK, 11.0f, (height + 2.0f)/ 2.0f, -width / 2.0f + EXT_WALL_THK - 0.2f, wallTexID); 
    drawBoxWithTexture(2.1f, height - 2.0f, EXT_WALL_THK, 11.0f, (height - 2.0f)/ 2.0f, -width / 2.0f + EXT_WALL_THK - 0.2f , wallTexID); 
    drawBoxWithTexture(1.5f, height, EXT_WALL_THK, 9.3f, height / 2.0f, -width / 2.0f + EXT_WALL_THK - 0.2f, wallTexID); 
    drawBoxWithTexture(1.1f, height - 2.0f, EXT_WALL_THK, 8.2f, (height + 2.0f)/ 2.0f, -width / 2.0f + EXT_WALL_THK - 0.2f, wallTexID); 
    drawBoxWithTexture(11.1f, height, EXT_WALL_THK, 2.1f, (height)/ 2.0f, -width / 2.0f + EXT_WALL_THK - 0.2, wallTexID); 
}

void drawShop(float L, float W, float H) {
    // Floor
    drawBoxWithTexture(L, 0.1f, W, 0.0f, -0.05f, 0.0f, floorTexID);

    // Internal Walls 
    drawBox(L - 1.5f, H, INT_WALL_THK, -0.7f, H / 2.0f, W / 2.0f - INT_WALL_THK - 0.1, 0.96f, 0.87f, 0.70f); // front
    drawBox(1.5f, H - 2.0f, INT_WALL_THK, L - 4.4f, (H + 2.0f)/ 2.0f, W / 2.0f - INT_WALL_THK - 0.1, 0.96f, 0.87f, 0.70f); // front
    drawBox(1.5f, H - 2.0f, INT_WALL_THK, L - 4.4f, (H - 2.0f)/ 2.0f, W / 2.0f - INT_WALL_THK - 0.1, 0.96f, 0.87f, 0.70f); // front

    drawBox(L - 0.1, H, INT_WALL_THK, 0.0f, H / 2.0f, -W / 2.0f + INT_WALL_THK + 0.1, 0.96f, 0.87f, 0.70f); // back

    drawBox(INT_WALL_THK, H, W, -L / 2.0f + INT_WALL_THK, H / 2.0f, 0.0f, 0.96f, 0.87f, 0.70f); // left

    drawBox(INT_WALL_THK, H, W - 7.0f, L / 2.0f - INT_WALL_THK, H / 2.0f, -3.3f, 0.96f, 0.87f, 0.70f); // right
    drawBox(INT_WALL_THK, H, 5.9f, L / 2.0f - INT_WALL_THK, H / 2.0f, 3.1f, 0.96f, 0.87f, 0.70f); // right
    drawBox(INT_WALL_THK, H - 2.0f, 1.1f, L / 2.0f - INT_WALL_THK, (H + 2.0f) / 2.0f, -0.3f, 0.96f, 0.87f, 0.70f); // right
}

void drawGreatRoom(float L, float W, float H){

    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, floorTexID);

    // Internal walls 
    drawBox(L -1.7f, H, INT_WALL_THK, -0.7f, H / 2.0f, W / 2.0f - 4.9f - INT_WALL_THK, 0.05f, 0.25f, 0.10f); // back
    drawBox(0.6f, H, INT_WALL_THK, L -3.2f, H / 2.0f, W / 2.0f -  INT_WALL_THK- 4.9f, 0.05f, 0.25f, 0.10f); // back
    drawBox(1.1f, H - 2.0f, INT_WALL_THK, L -3.9f, (H + 2.0f)/ 2.0f  , W / 2.0f - INT_WALL_THK- 4.9f, 0.05f, 0.25f, 0.10f); // back
    drawBox(INT_WALL_THK, H, W, -L / 2.0f + INT_WALL_THK, H / 2.0f, 0.0f, 0.05f, 0.25f, 0.10f); // left
    drawBox(INT_WALL_THK, H, W, L / 2.0f - INT_WALL_THK, H / 2.0f, 0.0f, 0.8f, 0.8f, 0.7f); // right

}

void drawDining(float L, float W, float H){
    
    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, floorTexID);

    // Internal walls
    drawBox(INT_WALL_THK, H, W - 1.1, -L / 2.0f + INT_WALL_THK, H / 2.0f, 0.5f, 0.8f, 0.8f, 0.7f); // left
    drawBox(INT_WALL_THK, H - 2, 1.1, -L / 2.0f + INT_WALL_THK, (H + 2.0f) / 2.0f, -1.5f, 0.8f, 0.8f, 0.7f); // left

    drawBox(INT_WALL_THK, H, W - 2.5f, L / 2.0f - INT_WALL_THK, H / 2.0f, -1.3f, 0.8f, 0.8f, 0.7f); // right
    drawBox(INT_WALL_THK, H, 1.4f, L / 2.0f - INT_WALL_THK, H / 2.0f, 1.3f, 0.8f, 0.8f, 0.7f); // right
    drawBox(INT_WALL_THK, H - 2.0f, 1.1f, L / 2.0f - INT_WALL_THK, (H + 2.0f) / 2.0f, 0.1f, 0.8f, 0.8f, 0.7f); // right
}

void drawBathroom(float L, float W, float H){

    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, bathroomTexID);

    // Internal walls with texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bathroomTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    drawBoxWithTexture(L, H, INT_WALL_THK, 0, H / 2.0f, W / 2.0f - INT_WALL_THK, bathroomTexID); // FRONT
    drawBoxWithTexture(L, H, INT_WALL_THK, 0, H / 2.0f, -W / 2.0f + INT_WALL_THK, bathroomTexID); // BACK
    drawBoxWithTexture(INT_WALL_THK, H, W , L/2 - INT_WALL_THK, H / 2.0f, 0.0f, bathroomTexID); // RIGHT

    drawBoxWithTexture(INT_WALL_THK, H, (W - 1.1f)/2 , -L/2 + INT_WALL_THK, H / 2.0f, 1.1f, bathroomTexID); // LEFT
    drawBoxWithTexture(INT_WALL_THK, H, (W - 1.1f)/2 , -L/2 + INT_WALL_THK, H / 2.0f, -1.1f, bathroomTexID); // LEFT
    drawBoxWithTexture(INT_WALL_THK, H - 2, 1.1f , -L/2 + INT_WALL_THK, (H + 2) / 2.0f, 0.0f, bathroomTexID); // LEFT
}

void drawKitchen(float L, float W, float H){

    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, floorTexID);

    // Interal walls 
    drawBox(L - 1.7f, H, INT_WALL_THK, -0.7f, H / 2.0f, W / 2.0f - INT_WALL_THK - 0.1f, 0.8f, 0.8f, 0.7f); // front
    drawBox(0.6f, H, INT_WALL_THK, L -3.2f, H / 2.0f, W / 2.0f - INT_WALL_THK - 0.1f, 0.8f, 0.8f, 0.7f); // front
    drawBox(1.1f, H - 2.0f, INT_WALL_THK, -0.7f + L -3.2f, (H + 2.0f)/ 2.0f, W / 2.0f - INT_WALL_THK - 0.1f, 0.8f, 0.8f, 0.7f); // front
   
    drawBox(INT_WALL_THK, H, W, -L / 2.0f + INT_WALL_THK, H / 2.0f, 0.0f, 0.8f, 0.8f, 0.7f); // left
    drawBox(INT_WALL_THK, H, W, L / 2.0f - INT_WALL_THK, H / 2.0f, 0.0f, 0.8f, 0.8f, 0.7f); // right
}

void drawMasterBedroom(float L, float W, float H){

    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, carpetTextID);

    // Interal walls 
    drawBox(1.0f, H - 2.0f, INT_WALL_THK, -1.5f, (H + 2)/ 2.0f, W / 2.0f - INT_WALL_THK, 0.85f, 0.93f, 1.0f); // front
    drawBox(L - 1.0f, H, INT_WALL_THK, 0.5f, H / 2.0f, W / 2.0f - INT_WALL_THK, 0.85f, 0.93f, 1.0f); // front

    drawBox(L - 3.0f, H, INT_WALL_THK, -1.5f, H / 2.0f, -W / 2.0f + INT_WALL_THK, 0.85f, 0.93f, 1.0f); // back
    drawBox(1.0f, H, INT_WALL_THK, 1.5f, H / 2.0f, -W / 2.0f + INT_WALL_THK, 0.85f, 0.93f, 1.0f); // back
    drawBox(2.0f, H - 2.0f, INT_WALL_THK, 0.0f, (H + 2.0f)/ 2.0f, -W / 2.0f + INT_WALL_THK, 0.85f, 0.93f, 1.0f); // back
    drawBox(2.0f, H - 2.0f, INT_WALL_THK, 0.0f, (H - 2.0f)/ 2.0f, -W / 2.0f + INT_WALL_THK, 0.85f, 0.93f, 1.0f); // back

    drawBox(INT_WALL_THK, H, W, -L / 2.0f + INT_WALL_THK, H / 2.0f, 0.0f, 0.85f, 0.93f, 1.0f); // left
    drawBox(INT_WALL_THK, H, W, L / 2.0f - INT_WALL_THK, H / 2.0f, 0.0f, 0.85f, 0.93f, 1.0f); // right
}

void drawAisle(float L, float W, float H){

    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, floorTexID);

    // Interal walls 
    drawBox(L, H - 2.0f, INT_WALL_THK, 0.0f, (H + 2.0f)/ 2.0f, W / 2.0f - INT_WALL_THK, 0.8f, 0.8f, 0.7f); // front

    drawBox(L, H - 2.0f, INT_WALL_THK, 0.0f, (H + 2.0f) / 2.0f, -W / 2.0f + INT_WALL_THK, 0.8f, 0.8f, 0.7f); // back

    drawBox(INT_WALL_THK, H, W - 1.1f, -L / 2.0f + INT_WALL_THK, H / 2.0f, -0.5f, 0.8f, 0.8f, 0.7f); // left
    drawBox(INT_WALL_THK, H - 2.0f, W , -L / 2.0f + INT_WALL_THK, (H + 2.0f)/ 2.0f, 0.0f, 0.8f, 0.8f, 0.7f); // left

    drawBox(INT_WALL_THK, H, (W - 1.1f)/2 , L / 2.0f - INT_WALL_THK, H / 2.0f, 1.1, 0.8f, 0.8f, 0.7f); // right
    drawBox(INT_WALL_THK, H,(W - 1.1f)/2, L / 2.0f - INT_WALL_THK, H / 2.0f, -1.0f, 0.8f, 0.8f, 0.7f); // right
    drawBox(INT_WALL_THK, H - 2.0f, 1.1f, L / 2.0f - INT_WALL_THK, (H + 2.0f)/ 2.0f, 0.1f, 0.8f, 0.8f, 0.7f); // right
}

void drawBedroom2(float L, float W, float H){

    // Floor with texture
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBoxWithTexture(L, FLOOR_THK, W, 0.0f, -FLOOR_THK / 2.0f, 0.0f, carpetTextID);

    // Interal walls
    drawBox(L - 3.0f, H, INT_WALL_THK, -1.5f, H / 2.0f, -W / 2.0f + INT_WALL_THK + 3.8, 1.0f, 0.8f, 0.9f); // front
    drawBox(1.0f, H, INT_WALL_THK, 1.5f, H / 2.0f, -W / 2.0f + INT_WALL_THK+ 3.8, 1.0f, 0.8f, 0.9f); // front
    drawBox(2.0f, H - 2.0f, INT_WALL_THK, 0.0f, (H + 2.0f)/ 2.0f, -W / 2.0f + INT_WALL_THK+ 3.8, 1.0f, 0.8f, 0.9f); // front
    drawBox(2.0f, H - 2.0f, INT_WALL_THK, 0.0f, (H - 2.0f)/ 2.0f, -W / 2.0f + INT_WALL_THK+ 3.8, 1.0f, 0.8f, 0.9f); // front

    drawBox(L - 0.8f, H, INT_WALL_THK, 0.5f, H / 2.0f, -W / 2.0f + INT_WALL_THK, 1.0f, 0.8f, 0.9f); // back
    drawBox(1.0f, H - 2.0f, INT_WALL_THK, -1.5f, (H + 2.0f) / 2.0f, -W / 2.0f + INT_WALL_THK, 1.0f, 0.8f, 0.9f); // back

    drawBox(INT_WALL_THK, H, W, -L / 2.0f + INT_WALL_THK, H / 2.0f, 0.0f, 1.0f, 0.8f, 0.9f); // left

    drawBox(INT_WALL_THK, H, W - 2.0f, L / 2.0f - INT_WALL_THK, H / 2.0f, 0.9f, 1.0f, 0.8f, 0.9f); // right
    drawBox(INT_WALL_THK, H - 2.0f, W - 2.0f, L / 2.0f - INT_WALL_THK, (H + 2.0f)/ 2.0f, -0.9f, 1.0f, 0.8f, 0.9f); // right
    drawBox(INT_WALL_THK, H - 2.0f, W - 2.0f, L / 2.0f - INT_WALL_THK, (H - 2.0f)/ 2.0f, -0.9f, 1.0f, 0.8f, 0.9f); // right
}

void drawDoor(float x, float y, float z, float width, float height, float thickness, 
              float red, float green, float blue) {
    glPushMatrix();

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    drawBox(width, height, thickness, x, y + height / 2.0f, z, red, green, blue);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void setRoofMaterial() {
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };  
    GLfloat mat_diffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    GLfloat mat_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f }; 
    GLfloat mat_shininess[] = { 30.0f };               

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

}

void drawRoof(float length, float width, float height, GLuint textureID) {
    float halfLength = (length + 10) / 2.0f;
    float halfWidth = width / 2.0f;

    setRoofMaterial();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
     glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f); 

    // Front Rectangle
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLength, 0.0f, halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLength, 0.0f, halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLength, height, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLength, height, 0.0f);
    glEnd();

    // Back Rectangle
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLength, 0.0f, -halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLength, 0.0f, -halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLength, height, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLength, height, 0.0f);
    glEnd();
    
    // Left Triangle
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(-halfLength, height, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLength, 0.0f, halfWidth); 
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfLength, 0.0f, -halfWidth);
    glEnd();

    // Right Triangle
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(halfLength, height, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfLength, 0.0f, halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLength, 0.0f, -halfWidth); 
    glEnd();

    glDisable(GL_TEXTURE_2D);

    drawBox(length + 9.5, 0.1f, width - 0.2f, 0.0, -0.1, 0.0, 0.8f, 0.8f, 0.8f); // ceiling
}

void drawBed(float x, float y, float z, float width, float depth, float height) {
    glPushMatrix();
    glTranslatef(x, y, z); 

    drawBox(width, height / 2.0f, depth, 0.0f, height / 4.0f, 0.0f, 0.8f, 0.8f, 0.9f); // Mattress
    drawBox(width, height / 2.0f, 0.2f, 0.0f, height / 2.0f, -depth / 2.0f + 0.1f, 0.6f, 0.4f, 0.3f); // Headboard

    glPopMatrix();
}

void drawSofa(float x, float y, float z, float width, float depth, float height) {
    glPushMatrix();
    glTranslatef(x, y, z); 
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    drawBox(width - 0.4, height / 2.0f, depth , 0.0f, height / 4.0f, 0.0f, 0.6f, 0.6f, 0.6f); // Base

    drawBox(width, (height + 0.7)/ 2.0f, 0.2f, 0.0f, height / 2.0f, -depth / 2.0f + 0.1f, 0.4f, 0.4f, 0.4f); // Backrest
    drawBox(0.2f, (height + 0.5) / 2.0f, depth, -width / 2.0f + 0.1f, height / 4.0f, 0.0f, 0.4f, 0.4f, 0.4f); // Left armrest
    drawBox(0.2f,(height + 0.5) / 2.0f, depth, width / 2.0f - 0.1f, height / 4.0f, 0.0f, 0.4f, 0.4f, 0.4f); // Right armrest

    glPopMatrix();
}

void drawCoffeeTable(float x, float y, float z, float width, float depth, float height) {
    float legWidth = 0.1f;

    glPushMatrix();
    glTranslatef(x, y, z); 
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    drawBox(width, height / 5.0f, depth, 0.0f, height - height / 10.0f, 0.0f, 0.35f, 0.25f, 0.15f); // Table surface

    drawBox(legWidth, height, legWidth,
            -width / 2.0f + legWidth / 2.0f, height / 2.0f - height / 10.0f, -depth / 2.0f + legWidth / 2.0f, 0.35f, 0.25f, 0.15f); // Front-left leg
    drawBox(legWidth, height, legWidth,
            width / 2.0f - legWidth / 2.0f, height / 2.0f - height / 10.0f, -depth / 2.0f + legWidth / 2.0f, 0.35f, 0.25f, 0.15f); // Front-right leg
    drawBox(legWidth, height, legWidth,
            -width / 2.0f + legWidth / 2.0f, height / 2.0f - height / 10.0f, depth / 2.0f - legWidth / 2.0f, 0.35f, 0.25f, 0.15f); // Back-left leg
    drawBox(legWidth, height, legWidth,
            width / 2.0f - legWidth / 2.0f, height / 2.0f - height / 10.0f, depth / 2.0f - legWidth / 2.0f, 0.35f, 0.25f, 0.15f); // Back-right leg

    glPopMatrix();
}

void drawChair(float x, float y, float z, float width, float depth, float height) {
    float legWidth = 0.1f;

    glPushMatrix();
    glTranslatef(x, y, z); 

    drawBox(width, height / 5.0f, depth, 0.0f, height / 10.0f, 0.0f, 0.6f, 0.3f, 0.0f);// Seat

    drawBox(width, height / 2.0f, 0.1f, 0.0f, height / 4.0f + height / 10.0f, -depth / 2.0f + 0.05f, 0.6f, 0.3f, 0.0f);// Backrest

    drawBox(legWidth, height / 2.0f, legWidth,
            -width / 2.0f + legWidth / 2.0f, -height / 4.0f, -depth / 2.0f + legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Front-left leg
    drawBox(legWidth, height / 2.0f, legWidth,
            width / 2.0f - legWidth / 2.0f, -height / 4.0f, -depth / 2.0f + legWidth / 2.0f, 0.35f, 0.25f, 0.15f); // Front-right leg
    drawBox(legWidth, height / 2.0f, legWidth,
            -width / 2.0f + legWidth / 2.0f, -height / 4.0f, depth / 2.0f - legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Back-left leg
    drawBox(legWidth, height / 2.0f, legWidth,
            width / 2.0f - legWidth / 2.0f, -height / 4.0f, depth / 2.0f - legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Back-right leg

    glPopMatrix();
}

void drawDiningTable(float x, float y, float z, float width, float depth, float height) {
    float legWidth = 0.1f;

    glPushMatrix();
    glTranslatef(x, y, z);

    glColor3f(0.35f, 0.25f, 0.15f);
    drawBox(width, height / 5.0f, depth, 0.0f, height - height / 10.0f, 0.0f, 0.35f, 0.25f, 0.15f);//surface
    drawBox(legWidth, height, legWidth,
            -width / 2.0f + legWidth / 2.0f, height / 2.0f - height / 10.0f, -depth / 2.0f + legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Front-left leg
    drawBox(legWidth, height, legWidth,
            width / 2.0f - legWidth / 2.0f, height / 2.0f - height / 10.0f, -depth / 2.0f + legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Front-right leg
    drawBox(legWidth, height, legWidth,
            -width / 2.0f + legWidth / 2.0f, height / 2.0f - height / 10.0f, depth / 2.0f - legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Back-left leg
    drawBox(legWidth, height, legWidth,
            width / 2.0f - legWidth / 2.0f, height / 2.0f - height / 10.0f, depth / 2.0f - legWidth / 2.0f, 0.35f, 0.25f, 0.15f);// Back-right leg

    glPopMatrix();
}

void drawOutdoorSofa(float x, float y, float z, float width, float depth, float height) {
    glPushMatrix();
    glTranslatef(x, y, z); 
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    glColor3f(0.4f, 0.3f, 0.2f); 
    drawBox(width, height / 5.0f, depth, 0.0f, height / 10.0f, 0.0f, 0.4f, 0.3f, 0.2f); // Base

    glColor3f(0.5f, 0.4f, 0.3f); 
    drawBox(width, height / 2.0f, 0.2f, 0.0f, height / 4.0f + height / 10.0f, -depth / 2.0f + 0.1f, 0.5f, 0.4f, 0.3f);// Backrest
    drawBox(0.2f, height / 2.0f, depth, -width / 2.0f + 0.1f, height / 4.0f, 0.0f, 0.5f, 0.4f, 0.3f);// Left armrest 
    drawBox(0.2f, height / 2.0f, depth, width / 2.0f - 0.1f, height / 4.0f, 0.0f, 0.5f, 0.4f, 0.3f);// Right armrest

    glColor3f(0.7f, 0.7f, 0.7f);
    drawBox(width - 0.2f, height / 6.0f, depth - 0.2f, 0.0f, height / 10.0f + 0.05f, 0.0f, 0.7f, 0.7f, 0.7f);// Cushions

    glPopMatrix();
}

void drawOutdoorTable(float x, float y, float z, float width, float depth, float height) {
    glPushMatrix();
    glTranslatef(x, y, z); 
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // Tabletop
    glColor3f(0.5f, 0.4f, 0.3f); // Medium brown
    drawBox(width, height / 8.0f, depth, 0.0f, height - height / 16.0f, 0.0f, 0.5f, 0.4f, 0.3f);

    // Legs
    float legWidth = 0.1f;

    drawBox(legWidth, height, legWidth, -width / 2.0f + legWidth / 2.0f, (height )/ 2.0f - height / 16.0f, -depth / 2.0f + legWidth / 2.0f, 0.5f, 0.4f, 0.3f);
    drawBox(legWidth, height, legWidth, width / 2.0f - legWidth / 2.0f, (height  )/ 2.0f - height / 16.0f, -depth / 2.0f + legWidth / 2.0f, 0.5f, 0.4f, 0.3f);
    drawBox(legWidth, height, legWidth, -width / 2.0f + legWidth / 2.0f, (height )/ 2.0f - height / 16.0f, depth / 2.0f - legWidth / 2.0f, 0.5f, 0.4f, 0.3f);
    drawBox(legWidth, height, legWidth, width / 2.0f - legWidth / 2.0f, height / 2.0f - height / 16.0f, depth / 2.0f - legWidth / 2.0f, 0.5f, 0.4f, 0.3f);

    glPopMatrix();
}


void drawNightstand(float x, float y, float z, float width, float depth, float height) {
    glPushMatrix();
    glTranslatef(x, y, z); 

    glColor3f(0.6f, 0.4f, 0.3f);  
    drawBox(width, height, depth, 0.0f, height / 2.0f, 0.0f, 0.6f, 0.4f, 0.3f);
    drawBox(width * 0.8f, height * 0.3f, 0.02f, 0.0f, height / 2.0f - height * 0.35f, depth / 2.0f - 0.01f, 0.6f, 0.4f, 0.3f);

    glColor3f(0.0f, 0.0f, 0.0f); 
    drawBox(0.2f, 0.05f, 0.05f, 0.0f, height / 2.0f - height * 0.35f, depth / 2.0f, 0.0f, 0.0f, 0.0f);

    glPopMatrix();
}

void drawPlantPot(float x, float y, float z, float potHeight, float potRadius, float foliageRadius) {
    glPushMatrix();

    // Pot
    glTranslatef(x, y, z);
    glColor3f(0.5f, 0.25f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
        glRotatef(-90, 1.0f, 0.0f, 0.0f); 
        gluCylinder(quad, potRadius, potRadius, potHeight, 20, 20);
    glPopMatrix();

    // Plant
    glTranslatef(0.0f, potHeight, 0.0f);
    glColor3f(0.0f, 0.4f, 0.1f); 
    glutSolidSphere(foliageRadius, 10, 10);

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawRoof(float x, float y, float z, float width, float height, float depth) {

     glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glPushMatrix();
    glTranslatef(x - width / 4.0f + 0.1, y + height / 2.0f, z);
    glRotatef(-160.0f, 0.0f, 0.0f, 1.0f);              
    glColor3f(0.5f, 0.5f, 0.5f);                         
    drawBox(width / 2.0f, 0.1f, depth, 0, 0, 0, 0.5f, 0.5f, 0.5f);         
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + width / 4.0f - 0.1, y + height / 2.0f, z); 
    glRotatef(160.0f, 0.0f, 0.0f, 1.0f);              
    glColor3f(0.5f, 0.5f, 0.5f);                        
    drawBox(width / 2.0f, 0.1f, depth, 0, 0, 0, 0.5f, 0.5f, 0.5f);         
    glPopMatrix();

}

void drawBeams(float x, float y, float z, float width, float height, float depth) {
     glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glColor3f(0.0f, 0.0f, 0.0f); 
    drawBox(0.1f, height, 0.1f, x - width / 2 + 0.15, y + height / 2, z - depth / 2, 0.0f, 0.0f, 0.0f); // Front-left
    drawBox(0.1f, height, 0.1f, x + width / 2 - 0.15, y + height / 2, z - depth / 2, 0.0f, 0.0f, 0.0f); // Front-right
    drawBox(0.1f, height, 0.1f, x - width / 2 + 0.15, y + height / 2, z + depth / 2, 0.0f, 0.0f, 0.0f); // Back-left
    drawBox(0.1f, height, 0.1f, x + width / 2 - 0.15, y + height / 2, z + depth / 2, 0.0f, 0.0f, 0.0f); // Back-right

}

void drawTree(float x, float y, float z, float trunkHeight, float trunkRadius, float foliageRadius) {
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
    glTranslatef(x, y, z);
     glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // trunk
    glColor3f(0.55f, 0.27f, 0.07f); 
    glPushMatrix();
    glRotatef(-90, 1.0f, 0.0f, 0.0f); 
    gluCylinder(quad, trunkRadius, trunkRadius, trunkHeight, 20, 20);
    glPopMatrix();

    // foliage
    glColor3f(0.0f, 0.4f, 0.1f); 
    for (int i = 0; i < 3; ++i) { 
        float currentRadius = foliageRadius * (1.0f - i * 0.2f);
        glPushMatrix();
            glTranslatef(0.0f, trunkHeight - i * foliageRadius * 0.5f, 0.0f);
            glutSolidSphere(currentRadius, 20, 20);
        glPopMatrix();
    }
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawHouse(){

    drawRoof(8.1f, 2.0f, -7.6f, 4.4f, 1.5f, 3.0f);
    drawBeams(8.1f, 0.0f, -7.6f, 4.4f, 2.5f, 3.0f);
    drawRoof(8.1f, 2.0f, 7.6f, 4.4f, 1.5f, 3.0f);
    drawBeams(8.1f, 0.0f, 7.6f, 4.4f, 2.5f, 3.0f);
    drawExteriorWalls(17.1f, 12.1f, ROOM_HEIGHT);

    drawSofa(4.5f, 0.0f, -3.5f, 4.0f, 2.0f, 1.0f);
    drawCoffeeTable(6.5f, 0.0f, -3.5f, 2.0f, 1.0f, 0.5f);
    drawDiningTable(5.2f, 0.0f, 2.0f, 3.0f, 2.0f, 1.0f);
    drawBed(11.6f, 0.0f, -4.2f, 2.4f, 3.0f, 1.0f);
    drawBed(12.0f, 0.0f, 3.4f, 1.8f, 2.5f, 1.0f);
    drawChair(4.5f, 0.4f, 1.0f, 1.0f, 1.0f, 1.0f);
    drawChair(6.0f, 0.4f, 1.0f, 1.0f, 1.0f, 1.0f);
    drawNightstand(9.8f, 0.0f, -5.5f, 1.0f, 0.6f, 0.8f);
    drawNightstand(10.6f, 0.0f, 2.4f, 1.0f, 0.6f, 0.8f);
    drawPlantPot(9.5f, 0.0f, 7.0f, 0.5f, 0.25f, 0.5f);
    drawPlantPot(7.0f, 0.0f, 7.0f, 0.5f, 0.25f, 0.5f);
    drawPlantPot(10.0f, 0.0f, -7.0f, 0.5f, 0.25f, 0.5f);
    drawTree(-25.0f, -1.0f, 20.0f, 5.0f, 0.5f, 2.0f);
    drawTree(20.0f, -1.0f, -10.0f, 6.0f, 0.6f, 2.5f);
    drawTree(20.0f, -1.0f, 20.0f, 5.5f, 0.4f, 2.2f);
    drawOutdoorSofa(7.0f, 0.0f, -8.0f, 3.0f, 1.5f, 1.0f);
    drawOutdoorTable(8.5f, 0.0f, -8.0f, 1.0f, 1.0f, 0.6f);

    glRotatef(180, 0.0f, 1.0f, 0.0f);
    drawChair(-6.0f, 0.4f, -3.0f, 1.0f, 1.0f, 1.0f);
    drawChair(-4.5f, 0.4f, -3.0f, 1.0f, 1.0f, 1.0f);
    glRotatef(-180, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
        glTranslatef(9.6f, 0.0f, 0.2f);
        drawAisle(1.0f, 3.5f, ROOM_HEIGHT);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.0f, 0.0f, 9.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawBoxWithTexture(17.0f, FLOOR_THK, 6.0f, 0.0f, -FLOOR_THK / 2.0f, 0.0f, gravelTexID);
        glTranslatef(0.0f, 0.0f, -18.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawBoxWithTexture(17.0f, FLOOR_THK, 6.0f, 0.0f, -FLOOR_THK / 2.0f, 0.0f, gravelTexID);
    glPopMatrix();

    drawShop(7.0f, 11.9f, ROOM_HEIGHT);

    glPushMatrix();
        glTranslatef(6.3f, 0.0f, -3.4f);
        drawGreatRoom(5.8f, 5.2f, ROOM_HEIGHT);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6.3f, 0.0f, -3.4f); 
        drawDoor(-7.2f, 0.0f, 9.4f, 3.6f, 2.5f, 0.3f, 0.4f, 0.4f, 0.4f); 
        drawDoor(-7.2f, 0.0f, -2.6f, 3.6f, 2.5f, 0.3f, 0.4f, 0.4f, 0.4f); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6.3f, 0.0f, 1.23f);
        drawDining(5.8f, 4.2f, ROOM_HEIGHT);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6.3f, 0.0f, 4.6f);
        drawKitchen(5.8f, 2.8f, ROOM_HEIGHT);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(11.1f, 0.0f, -3.7f);
        drawMasterBedroom(4.0f, 4.4f, ROOM_HEIGHT);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(11.55f, 0.0f, 0.23f);
        drawBathroom(3.1f, 3.4f, ROOM_HEIGHT);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(11.1f, 0.0f, 3.9f);
        drawBedroom2(4.0f, 4.0f, ROOM_HEIGHT);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0.0f, ROOM_HEIGHT, 0.0f);    
    glPopMatrix();
}

void quitApplication() {
    exit(0);
}

void viewsDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    // Viewport 1: Floor plan without roof (orthographic view)
    glViewport(0, height / 2, width / 2, height / 2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 10); 
    glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
    gluLookAt(5.0, 5.0, 0.0,  
              5.0, 0.0, 0.0,  
              0.0, 0.0, 1.0); 

    initGrassPoints();
    drawHouse();
    drawGrass();
    drawSky(); 

    // Viewport 2: Floor plan with roof (orthographic view)
    glViewport(width / 2, height / 2, width / 2, height / 2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 10); 
    glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
    gluLookAt(5.0, 5.0, 0.0,  
              5.0, 0.0, 0.0,  
              0.0, 0.0, 1.0); 

    initGrassPoints();
    drawHouse();
    drawGrass();
    drawSky(); 
    glTranslatef(5.0f, ROOM_HEIGHT, 0.0f);
    drawRoof(7.2f, 12.2f, 2.9f, roofTextID);

    // Viewport 3: Front of the house (perspective view)
    glViewport(0, 0, width / 2, height / 2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / height, 1.0f, 200.0f); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 20.0, 
              5.0, 0.0, 0.0,  
              0.0, 1.0, 0.0); 

    initSkyPoints();
    initGrassPoints();
    drawHouse();
    drawGrass();
    drawSky(); 
    glTranslatef(5.0f, ROOM_HEIGHT, 0.0f);
    drawRoof(7.2f, 12.2f, 2.9f, roofTextID);

    // Viewport 4: Room interior view (perspective view)
    glViewport(width / 2, 0, width / 2, height / 2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / height, 1.0f, 200.0f); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(8.0, 2.0, 6.0,
              5.0, 1.0, 0.0,   
              0.0, 1.0, 0.0);  

    initSkyPoints();
    initGrassPoints();
    drawHouse();
    drawGrass();
    drawSky(); 
    glTranslatef(5.0f, ROOM_HEIGHT, 0.0f);
    drawRoof(7.2f, 12.2f, 2.9f, roofTextID);
    glutSwapBuffers();
}






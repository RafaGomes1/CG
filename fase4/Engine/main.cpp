//
// Created by goncalo on 24-02-2024.
//

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "headers/main.h"
#include "../Utils/MatrixOperations.h"
#include "headers/Transformation.h"
#include "headers/Model.h"
#include "headers/Group.h"
#include "headers/Light.h"
#include <cmath>
#include <memory>
#include <vector>
#include <IL/il.h>

using namespace std;
using namespace tinyxml2;

struct Camara{
    float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
    float alpha = M_PI_4;
    float beta = M_PI_4;
    float radius = 5.0f;
    float lookX = 0.0f, lookY = 0.0f, lookZ = 0.0f;
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
    float fov = 45.0f, nearPlane = 1.0f, farPlane = 1000.0f;
    float prevMouseX, prevMouseY, atualMouseX, atualMouseY;
    int tracking = 0;
    float velocity = 0.5f;
    bool mode = true; // true -> Spherical Mode; false -> Free mode
}cam;

Camara initialCam;

struct Window{
    int width = 500, height = 500;
}win;


struct OpenGLSettings{
    int mode = GL_FILL, face = GL_FRONT_AND_BACK;
    bool showAxis = false;
    bool toggleAnimation = true;
    int time_elapsed = 0;
    int last_time_elapsed = 0;
    int total_time_elapsed = 0;
    int pausedTime = 0;
    int startTime = 0;
    bool toogleCurvesDrawing = false;
}glSetts;



vector<Light> lights;
unique_ptr<Group> group;
int gl_lights[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

/*
 *
 * XML Parsing
 *
 */

unique_ptr<Group> parseXML(const char* filename){
    XMLDocument doc;
    unique_ptr<Group> group2;
    if (doc.LoadFile(filename) == XML_SUCCESS) {
        // std::cout << "[DEPOIS DO FILE SUCESS!]" << std::endl;

        XMLElement* worldElement = doc.FirstChildElement("world");
        if(worldElement){
            // std::cout << "[ENTREI NO PARSE]" << std::endl;

            // dados da window
            XMLElement* windowElement = worldElement->FirstChildElement("window");
            if (windowElement) {
                windowElement->QueryIntAttribute("width", &win.width);
                windowElement->QueryIntAttribute("height", &win.height);
            }
            // dados da camara
            XMLElement* cameraElement = worldElement->FirstChildElement("camera");
            if (cameraElement) {
                XMLElement* positionElement = cameraElement->FirstChildElement("position");
                if (positionElement) {

                    positionElement->QueryFloatAttribute("x", &initialCam.posX);
                    positionElement->QueryFloatAttribute("y", &initialCam.posY);
                    positionElement->QueryFloatAttribute("z", &initialCam.posZ);

                    cam.radius = sqrt(initialCam.posX * initialCam.posX + initialCam.posY * initialCam.posY + initialCam.posZ * initialCam.posZ);
                    cam.alpha = atan2(initialCam.posX, initialCam.posZ); // Ajuste para usar atan2
                    cam.beta = asin(initialCam.posY / cam.radius); // Mantém o cálculo de beta

                    initialCam.radius = cam.radius;
                    initialCam.alpha = cam.alpha;
                    initialCam.beta = cam.beta;

                    cam.posX = initialCam.posX;
                    cam.posY = initialCam.posY;
                    cam.posZ = initialCam.posZ;
                }
                XMLElement* lookAtElement = cameraElement->FirstChildElement("lookAt");
                if (lookAtElement) {
                    lookAtElement->QueryFloatAttribute("x", &cam.lookX);
                    lookAtElement->QueryFloatAttribute("y", &cam.lookY);
                    lookAtElement->QueryFloatAttribute("z", &cam.lookZ);

                    initialCam.lookX = cam.lookX;
                    initialCam.lookY = cam.lookY;
                    initialCam.lookZ = cam.lookZ;
                }
                XMLElement* upElement = cameraElement->FirstChildElement("up");
                if (upElement) {
                    upElement->QueryFloatAttribute("x", &cam.upX);
                    upElement->QueryFloatAttribute("y", &cam.upY);
                    upElement->QueryFloatAttribute("z", &cam.upZ);

                    initialCam.upX = cam.upX;
                    initialCam.upY = cam.upY;
                    initialCam.upZ = cam.upZ;
                }
                XMLElement* projectionElement = cameraElement->FirstChildElement("projection");
                if (projectionElement) {
                    projectionElement->QueryFloatAttribute("fov", &cam.fov);
                    projectionElement->QueryFloatAttribute("near", &cam.nearPlane);
                    projectionElement->QueryFloatAttribute("far", &cam.farPlane);
                    initialCam.fov = cam.fov;
                    initialCam.nearPlane = cam.nearPlane;
                    initialCam.farPlane = cam.farPlane;
                }
            }

            XMLElement* lightsXML = worldElement->FirstChildElement("lights");
            float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f, posX = 0.0f, posY = 0.0f, posZ = 0.0f, cutoff = 0.0f;
            if (lightsXML){
                // std::cout << "[ENTREI NAS LUZES]" << std::endl;
                
                for(XMLElement* light = lightsXML->FirstChildElement("light"); light != NULL; light = light->NextSiblingElement("light")){
                    // std::cout << "[ENTREI NUMA LUZ]" << std::endl;

                    const char* type = light->Attribute("type");

                    // std::cout << type << std::endl;


                    Light l;
                    l.setType(type);

                    // std::cout << "[CRIEI E SETEI O TIPO]" << std::endl;
                    // std::cout << l << std::endl;



                    if (strcmp(type, "point") == 0)
                    {
                        // std::cout << "[SO ENTREI NO POINT]" << std::endl;

                        posX = atof(light->Attribute("posx"));
                        posY = atof(light->Attribute("posy"));
                        posZ = atof(light->Attribute("posz"));

                        Point p(posX,posY,posZ);

                        l.setPosition(p);
                    }
                    if (strcmp(type, "directional") == 0)
                    {
                        // std::cout << "[ENTREI NO CASO DIRECTIONAL]" << std::endl;

                        posX = atof(light->Attribute("dirx"));
                        posY = atof(light->Attribute("diry"));
                        posZ = atof(light->Attribute("dirz"));

                        // std::cout << "[LI TODAS AS DIRECOES]" << std::endl;

                        Point d(posX,posY,posZ);

                        l.setDirection(d);
                    }
                    if (strcmp(type, "spot") == 0)
                    {
                        // std::cout << "[SO ENTREI NO SPOTLIGHT]" << std::endl;

                        posX = atof(light->Attribute("posx"));
                        posY = atof(light->Attribute("posy"));
                        posZ = atof(light->Attribute("posz"));

                        Point p(posX,posY,posZ);

                        posX = atof(light->Attribute("dirx"));
                        posY = atof(light->Attribute("diry"));
                        posZ = atof(light->Attribute("dirz"));

                        Point d(posX,posY,posZ);

                        cutoff = atoi(light->Attribute("cutoff"));

                        l.setPosition(p);
                        l.setDirection(d);
                        l.setCutoff(cutoff);
                    }

                    // std::cout << "[INFO] Light parsed: " << l << std::endl;

                    lights.push_back(l);

                }
            }
            // std::cout << "[SAI DAS LUZES]" << std::endl;

            // for (int i = 0; i < lights.size(); i++){
            //         std::cout << "[CAM] Light parsed: " << lights[i] << std::endl;

            // }


            vector<string> modelos;
            // dados modelos
            XMLElement* groupElement = worldElement->FirstChildElement("group");

            if (groupElement) {
                group2 = std::make_unique<Group>(groupElement);
            }
        }
        // std::cout << "[ACABEI]" << std::endl;

        return group2;
    }
    else
    {
        return nullptr;
    }
}

/*
 * Camera Functions
 */

// Spherical mode

void calcSphericalCam()
{
    cam.posX = cam.radius * cos(cam.beta) * sin(cam.alpha);
    cam.posY = cam.radius * sin(cam.beta);
    cam.posZ = cam.radius * cos(cam.beta) * cos(cam.alpha);
}

// Free mode

void moveCameraForward()
{
    vector<float> direction = MatrixOperations::normalizeMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ});

    cam.posX += cam.velocity * direction[0];
    cam.posY += cam.velocity * direction[1];
    cam.posZ += cam.velocity * direction[2];

    cam.lookX = cam.posX + direction[0];
    cam.lookY = cam.posY + direction[1];
    cam.lookZ = cam.posZ + direction[2];
}

void moveCameraBackward()
{
    vector<float> direction = MatrixOperations::normalizeMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ});

    cam.posX += (-cam.velocity) * direction[0];
    cam.posY += (-cam.velocity) * direction[1];
    cam.posZ += (-cam.velocity) * direction[2];

    cam.lookX = cam.posX + direction[0];
    cam.lookY = cam.posY + direction[1];
    cam.lookZ = cam.posZ + direction[2];
}

void moveCameraRight()
{
    vector<float> res = MatrixOperations::normalizeMatrix(MatrixOperations::crossMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ},{cam.upX, cam.upY, cam.upZ}));
    vector<float> direction = MatrixOperations::normalizeMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ});

    cam.posX += cam.velocity * res[0];
    cam.posY += cam.velocity * res[1];
    cam.posZ += cam.velocity * res[2];

    cam.lookX = cam.posX + direction[0];
    cam.lookY = cam.posY + direction[1];
    cam.lookZ = cam.posZ + direction[2];
}


void moveCameraLeft()
{
    vector<float> res = MatrixOperations::normalizeMatrix(MatrixOperations::crossMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ},{cam.upX, cam.upY, cam.upZ}));
    vector<float> direction = MatrixOperations::normalizeMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ});

    cam.posX += (-cam.velocity) * res[0];
    cam.posY += (-cam.velocity) * res[1];
    cam.posZ += (-cam.velocity) * res[2];

    cam.lookX = cam.posX + direction[0];
    cam.lookY = cam.posY + direction[1];
    cam.lookZ = cam.posZ + direction[2];
}

void calcLookAt()
{
    cam.lookX = cam.posX + cam.radius * cos(cam.beta) * sin(cam.alpha);
    cam.lookY = cam.posY + cam.radius * sin(cam.beta);
    cam.lookZ = cam.posZ + cam.radius * cos(cam.beta) * cos(cam.alpha);
}


// General Functions

void printCamaraInfo() {
    std::cout << "\n----------------[Camera Info]----------------" << std::endl;
    std::cout << "posX: " << cam.posX << "; " << "posY: " << cam.posY << "; " << "posZ: " << cam.posZ << std::endl;
    std::cout << "alpha: " << cam.alpha << "; " << "beta: " << cam.beta << "; " << "radius: " << cam.radius << std::endl;
    std::cout << "lookX: " << cam.lookX << "; " << "lookY: " << cam.lookY <<  ";" << "lookZ: " << cam.lookZ << std::endl;
    std::cout << "upX: " << cam.upX << "; " << "upY: " << cam.upY << "; " << "upZ: " << cam.upZ << std::endl;
    std::cout << "fov: " << cam.fov << "; " << "nearPlane: " << cam.nearPlane << "; " << "farPlane: " << cam.farPlane << std::endl;
    std::cout << "mode: " << (cam.mode ? "Spherical Mode" : "Free Mode") << std::endl;
    std::cout << "velocity: " << cam.velocity << std::endl;
}

void setInitialCam()
{
    cam.alpha = initialCam.alpha;
    cam.beta = initialCam.beta;
    cam.radius = initialCam.radius;
    cam.lookX = initialCam.lookX;
    cam.lookY = initialCam.lookY;
    cam.lookZ = initialCam.lookZ;
    cam.upX = initialCam.upX;
    cam.upY = initialCam.upY;
    cam.upZ = initialCam.upZ;
    cam.fov = initialCam.fov;
    cam.nearPlane = initialCam.nearPlane;
    cam.farPlane = initialCam.farPlane;
    cam.posX = initialCam.posX;
    cam.posY = initialCam.posY;
    cam.posZ = initialCam.posZ;
}

void resetCam()
{
    if (cam.mode)
    {
        setInitialCam();
        calcSphericalCam();
    }
    else
    {
        setInitialCam();
        calcSphericalCam();
        vector<float> direction = MatrixOperations::normalizeMatrix({cam.lookX - cam.posX, cam.lookY - cam.posY, cam.lookZ - cam.posZ});
        cam.alpha = M_PI + cam.alpha;
        cam.beta = -cam.beta;
        calcLookAt();
    }
}

void printCameraMode()
{
    std::cout << "[INFO] Camera mode: " << (cam.mode ? "Spherical-Camera" : "First-Person-Camera") << std::endl;
}

// Camera Mouse Motion

void processMouseMotion(int xx, int yy) {
    if (cam.tracking) {
        float deltaX = xx - cam.atualMouseX;
        float deltaY = yy - cam.atualMouseY;

        const float smoothingFactor = 0.05f;

        cam.atualMouseX += deltaX * smoothingFactor;
        cam.atualMouseY += deltaY * smoothingFactor;

        float interpolatedDeltaX = cam.atualMouseX - cam.prevMouseX;
        float interpolatedDeltaY = cam.atualMouseY - cam.prevMouseY;

        cam.alpha += interpolatedDeltaX * 0.01f;
        cam.beta += interpolatedDeltaY * 0.01f;


        if (cam.beta < - M_PI_2) cam.beta += M_PI * 2;
        else if (cam.beta > (3 * M_PI_2)) cam.beta -= M_PI * 2;

        cam.prevMouseX = cam.atualMouseX;
        cam.prevMouseY = cam.atualMouseY;
        glutPostRedisplay();
    }
}

void lightsExecution(const std::vector<Light>& lightsE){
    for(int i = 0; i < lightsE.size(); i++){

        const Light& l = lightsE[i];

        vector<float> lP = l.getPosition();

        vector<float> lD = l.getDirection();

        size_t cutoff = l.getCutoff();
        const GLfloat c = l.getCutoff();

        std::string t = l.getType();

        GLfloat dark[4] = {0.2, 0.2, 0.2, 1.0};
        GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};

        if (t == "point") {
            
            glLightfv(gl_lights[i], GL_AMBIENT, dark);
            glLightfv(gl_lights[i], GL_DIFFUSE, white);
            glLightfv(gl_lights[i], GL_SPECULAR, white);

            glLightfv(gl_lights[i], GL_POSITION, lP.data());
        }
        if (t == "directional"){

            glLightfv(gl_lights[i], GL_DIFFUSE, white);
            glLightfv(gl_lights[i], GL_SPECULAR, white);

            glLightfv(gl_lights[i], GL_POSITION, lD.data());
            
        }
        if (t == "spot") {

            glLightfv(gl_lights[i], GL_AMBIENT, dark);
            glLightfv(gl_lights[i], GL_DIFFUSE, white);
            glLightfv(gl_lights[i], GL_SPECULAR, white);

            glLightfv(gl_lights[i], GL_POSITION, lP.data());
            glLightfv(gl_lights[i], GL_SPOT_DIRECTION, lD.data());
            glLightfv(gl_lights[i], GL_SPOT_CUTOFF, &c);
            glLightf(gl_lights[i], GL_SPOT_EXPONENT, 0.0);
        }
    }
}

/*
 *
 * OpenGL functions
 *
 */
void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(w == 0)
        w = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(cam.fov ,ratio, cam.nearPlane ,cam.farPlane);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void drawAxis()
{
    // put axis drawing in here
    glBegin(GL_LINES);
    // X-axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-5000.0f, 0.0f, 0.0f);
    glVertex3f( 5000.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -5000.0f, 0.0f);
    glVertex3f(0.0f, 5000.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -5000.0f);
    glVertex3f(0.0f, 0.0f, 5000.0f);
    glEnd();

}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set the camera
    glLoadIdentity();
    if (cam.mode)
    {
        calcSphericalCam();
        gluLookAt(cam.posX, cam.posY, cam.posZ, cam.lookX, cam.lookY, cam.lookZ, cam.upX, cam.beta > M_PI_2 ? -cam.upY : cam.upY, cam.upZ);
    }
    else
    {
        calcLookAt();
        gluLookAt(cam.posX, cam.posY, cam.posZ, cam.lookX, cam.lookY, cam.lookZ, cam.upX, cam.upY, cam.upZ);
    }

    if (glSetts.showAxis)
    {
        glDisable(GL_LIGHTING);
        drawAxis();


    }
    glPolygonMode(glSetts.face, glSetts.mode);
    // put the geometric transformations here
    int current_time = glutGet(GLUT_ELAPSED_TIME);

    if (glSetts.toggleAnimation)
    {
        if (glSetts.pausedTime > 0)
        {
            glSetts.startTime += (current_time - glSetts.pausedTime);
            glSetts.pausedTime = 0;
        }
        glSetts.time_elapsed = current_time - glSetts.startTime;

    }

    // figure drawing here
    group->draw(glSetts.time_elapsed, glSetts.toogleCurvesDrawing);
    if (lights.size() > 0){
        glEnable(GL_LIGHTING);
    }

    lightsExecution(lights);


    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events

void keyboardCallback(unsigned char key, int x, int y){
    switch(key) {
        case 'a': // esquerda
            if (cam.mode)
            {
                cam.alpha -= M_PI / 16;
            }
            else
            {
                moveCameraLeft();
            }
            break;
        case 'd': // direita
            if (cam.mode)
            {
                cam.alpha += M_PI / 16;
            }
            else
            {
                moveCameraRight();
            }
            break;
        case 'w': // cima
            if (cam.mode)
            {
                cam.beta += M_PI / 16;
                if (cam.beta > (3 * M_PI_2)) cam.beta -= M_PI * 2;
            }
            else
            {
                moveCameraForward();
            }
            break;
        case 's': // baixo
            if (cam.mode)
            {
                cam.beta -= M_PI/16;
                if (cam.beta < - M_PI_2) cam.beta += M_PI * 2;
            }
            else
            {
                moveCameraBackward();
            }
            break;
        case 'q':
            if (!cam.mode)
            {
                cam.posY += cam.velocity;
                cam.lookY += cam.velocity;
            }
            else
            {
                cam.radius -= cam.velocity;
            }
            break;
        case 'e':
            if (!cam.mode)
            {
                cam.posY -= cam.velocity;
                cam.lookY -= cam.velocity;
            }
            else
            {
                cam.radius += cam.velocity;
            }
            break;
        case '+':
            if (cam.velocity < 10.0f)
            {
                cam.velocity += 0.1f;
            }

            break;
        case '-':
            if (cam.velocity > 0.2f)
            {
                cam.velocity -= 0.1f;
            }
            break;
        case 'f':
            glSetts.mode = GL_FILL;
            break;
        case 'l':
            glSetts.mode = GL_LINE;
            break;
        case 'p':
            glSetts.mode = GL_POINT;
            break;
        case 'b':
            glSetts.face = GL_BACK;
            break;
        case 'n':
            glSetts.face = GL_FRONT;
            break;
        case 'm':
            glSetts.face = GL_FRONT_AND_BACK;
            break;
        case 'r':
            resetCam();
            break;
        case 'c': // Toggle camera mode
            if(cam.mode) // Spherical Mode
            {
                resetCam();
                cam.alpha = M_PI + cam.alpha;
                cam.beta = -cam.beta;
                calcLookAt();
                cam.mode = !cam.mode;
            }
            else // Free mode
            {
                resetCam();
                cam.beta = asin(cam.posY/cam.radius);
                cam.alpha = atan(cam.posX/cam.posZ);
                cam.mode = !cam.mode;
            }
            printCameraMode();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            cam.atualMouseX = xx;
            cam.atualMouseY = yy;
            cam.prevMouseX = xx;
            cam.prevMouseY = yy;
            cam.tracking = 1;
        } else {
            cam.tracking = 0;
        }
    }
}

void printCommands()
{
    std::cout << "\n----------------[Commands]----------------" << std::endl;
    std::cout << "\n##### Camera and Movement Commands #####" << std::endl;
    std::cout << "'c' -> Toggle between 'First-Person-Camera'(FPC) and 'Spherical-Camera'(SC)" << std::endl;
    std::cout << "'awsd' -> Move the camera around." << std::endl;
    std::cout << "'q' -> In FPC increments the height. In SC moves the camera closer. " << std::endl;
    std::cout << "'e' -> In FPC decrements the height. In SC moves the camera further. " << std::endl;
    std::cout << "'+' -> Increase camera velocity. " << std::endl;
    std::cout << "'-' -> Decrease camera velocity.. " << std::endl;

    std::cout << "'r' -> Resets the camera position. " << std::endl;
    std::cout << "'Mouse Left Press + Drag' -> Move the camera around" << std::endl;

    std::cout << "\n##### Rendering Modes #####" << std::endl;
    std::cout << "'f' -> Sets the render mode to FILL. " << std::endl;
    std::cout << "'l' -> Sets the render mode to LINES. " << std::endl;
    std::cout << "'p' -> Sets the render mode to POINTS. " << std::endl;

    std::cout << "\n##### Face culling #####" << std::endl;
    std::cout << "'b' -> Sets the face culling to BACK. " << std::endl;
    std::cout << "'n' -> Sets the face culling to FRONT. " << std::endl;
    std::cout << "'m' -> Sets the face culling to FRONT and BACK. " << std::endl;

    std::cout << "\n##### Toggles #####" << std::endl;
    std::cout << "'f1' -> Toggle the axis drawing. " << std::endl;
    std::cout << "'f2' -> Toggle animation. " << std::endl;
    std::cout << "'f3' -> Toggle drawing of Catmull-Rom curves. " << std::endl;
    std::cout << "'f11' -> Show camera information (DEBUG). " << std::endl;
    std::cout << "'f12' -> Show commands information." << std::endl;

}

void processSpecialKeys(int key, int x, int y) {
    switch (key){
        case GLUT_KEY_UP:{
            if (!cam.mode)
            {
                cam.posY += 1.0f;
                cam.lookY += 1.0f;
            }
            else
            {
                cam.radius += 1.0f;
            }
            break;
        }
        case GLUT_KEY_DOWN:{
            if (!cam.mode)
            {
                cam.posY -= 1.0f;
                cam.lookY -= 1.0f;
            }
            else
            {
                cam.radius -= 1.0f;
            }
            break;
        }
        case GLUT_KEY_F1:
            glSetts.showAxis = !glSetts.showAxis ;
            std::cout << "[INFO] Drawing Axis: " << (glSetts.showAxis ? "ACTIVE" : "INACTIVE") << std::endl;

            break;
        case GLUT_KEY_F2: // Stop animation
            if (glSetts.toggleAnimation)
            {
                glSetts.pausedTime = glutGet(GLUT_ELAPSED_TIME);
            }
            glSetts.toggleAnimation = !glSetts.toggleAnimation;
            std::cout << "[INFO] Animation status: " << (glSetts.toggleAnimation ? "ACTIVE" : "INACTIVE") << std::endl;
            break;
        case GLUT_KEY_F3: // Toggle Catmull-Rom curves drawings.
            glSetts.toogleCurvesDrawing = !glSetts.toogleCurvesDrawing;
            std::cout << "[INFO] Drawing of Catmull-Rom Cuves: " << (glSetts.toogleCurvesDrawing ? "ACTIVE" : "INACTIVE") << std::endl;
            break;
        case GLUT_KEY_F11:
            printCamaraInfo();
            break;
        case GLUT_KEY_F12:
            printCommands();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    std::cout << "[CG ENGINE]" << std::endl;
    if(argc != 2){
        std::cerr << "[ERROR] Invalid number of arguments! Please insert a xml file name located in the ../XMLFiles/ folder." << std::endl;
        exit(EXIT_FAILURE);
    }

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutCreateWindow("ENGINE@CG");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "[ERROR] Failed initiating GLEW: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "[OK] GLEW initiated successfully!" << std::endl;
    }

// Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

// put here the registration of the keyboard callbacks
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(processSpecialKeys);

    glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
    

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_2D);
	float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glEnable(GL_RESCALE_NORMAL);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    string xml = "../XMLFiles/" + (string)argv[1];
    std::cout << xml << std::endl;

    group = parseXML(xml.c_str());

    // Iluminação

    std::cout << "[SIZE]: " << lights.size() << std::endl;

    if(lights.size() > 0){
        glEnable(GL_LIGHTING);
        glEnable(GL_RESCALE_NORMAL);
        
        if(lights.size() > 8){
            std::cerr << "[ERROR] Number of lights exceeded " << glewGetErrorString(err) << std::endl;
            exit(EXIT_FAILURE);
        }

        GLfloat white[4] = {1.0,1.0,1.0,1.0};

        for (int i = 0; i < lights.size(); i++){
            glEnable(gl_lights[i]);
            glLightfv(gl_lights[i], GL_DIFFUSE, white);
            glLightfv(gl_lights[i], GL_SPECULAR, white);
        }

        float amb[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

        std::cout << "[OK] Luzes Iniciadas com Sucesso!" << std::endl;
    }

    if (!group)
    {
        std::cerr << "[ERROR] Failed reading xml file!" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "[OK] Finishing Parsing of xml File!" << std::endl;
    }

    glutInitWindowPosition(200,200);
    glutInitWindowSize(win.width,win.height);

    std::cout << "[INFO] Initiating Glut loop." << std::endl;
    printCommands();
// enter GLUT's main cycle
    glutMainLoop();

    if(group) {
        group->clearModelVBOs();
    }

    return 1;
}
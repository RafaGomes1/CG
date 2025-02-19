//
// Created by goncalo on 24-02-2024.
//

#include "headers/main.h"
#include "../Generator/headers/point.h"
#include "headers/tinyxml2.h"
#include <cmath>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
using namespace tinyxml2;

struct Camara{
    float alpha = M_PI_4;
    float beta = M_PI_4;
    float radius = 5.0f;
    float lookX = 0.0f, lookY = 0.0f, lookZ = 0.0f;
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
    float fov = 45.0f, nearPlane = 1.0f, farPlane = 1000.0f;
}cam;

struct Window{
    int width = 500, height = 500;
}win;

struct OpenGLSettings{
    int mode = GL_LINE, face = GL_FRONT_AND_BACK;
}glSetts;

vector<Point> points;



// Função para dar parse dos ficheiros 3D
void read_3D_File(string filename){
    ifstream inputFile(filename);

    string line;
    while (getline(inputFile, line)){
        istringstream iss(line);
        float x, y, z;
        char delimiter;

        iss >> x >> delimiter >> y >> delimiter >> z;
        points.emplace_back(x, y, z);
    }
}


void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

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


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(cam.radius*cos(cam.beta)*sin(cam.alpha),
              cam.radius*sin(cam.beta),
              cam.radius*cos(cam.beta)*cos(cam.alpha),
              cam.lookX,cam.lookY,cam.lookZ,
              cam.upX, cam.upY, cam.upZ);

// put axis drawing in here
    glBegin(GL_LINES);
    // X-axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

    // put the geometric transformations here

    // figure drawing here
    int j = 0;
    for (auto i = points.begin(); i != points.end(); i += 3) {
        Point p1 = *i;
        Point p2 = *(i + 1);
        Point p3 = *(i + 2);

        if ((j % 2) == 0)
        {
            glColor3f(1,1,1);
        }
        else
        {
            glColor3f(0.5,1,0.3);
        }
        glPolygonMode(glSetts.face, glSetts.mode);
        glBegin(GL_TRIANGLES);
        glVertex3f(p1.getX(),p1.getY(),p1.getZ());
        glVertex3f(p2.getX(),p2.getY(),p2.getZ());
        glVertex3f(p3.getX(),p3.getY(),p3.getZ());
        glEnd();

        j += 3;

    }
    // End of frame
    glutSwapBuffers();
}

// Não sei se assumo que os .3D ja estao criados ou nao
// O que fazer quando há mais do que um .3D?
void parseXML(const char* filename){
    XMLDocument doc;
    if (doc.LoadFile(filename) == XML_SUCCESS) {
        XMLElement* worldElement = doc.FirstChildElement("world");
        if(worldElement){
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
                    float posX, posY, posZ;
                    positionElement->QueryFloatAttribute("x", &posX);
                    positionElement->QueryFloatAttribute("y", &posY);
                    positionElement->QueryFloatAttribute("z", &posZ);

                    cam.radius = sqrt(posX * posX + posY * posY + posZ * posZ);
                    cam.alpha = acos(posZ/sqrt(posX * posX + posZ * posZ));
                    cam.beta = asin(posY/cam.radius);
                }
                XMLElement* lookAtElement = cameraElement->FirstChildElement("lookAt");
                if (lookAtElement) {
                    lookAtElement->QueryFloatAttribute("x", &cam.lookX);
                    lookAtElement->QueryFloatAttribute("y", &cam.lookY);
                    lookAtElement->QueryFloatAttribute("z", &cam.lookZ);
                }
                XMLElement* upElement = cameraElement->FirstChildElement("up");
                if (upElement) {
                    upElement->QueryFloatAttribute("x", &cam.upX);
                    upElement->QueryFloatAttribute("y", &cam.upY);
                    upElement->QueryFloatAttribute("z", &cam.upZ);
                }
                XMLElement* projectionElement = cameraElement->FirstChildElement("projection");
                if (projectionElement) {
                    projectionElement->QueryFloatAttribute("fov", &cam.fov);
                    projectionElement->QueryFloatAttribute("near", &cam.nearPlane);
                    projectionElement->QueryFloatAttribute("far", &cam.farPlane);
                }

            }
            vector<string> modelos;
            // dados modelos
            XMLElement* groupElement = worldElement->FirstChildElement("group");
            if (groupElement) {
                XMLElement* modelsElement = groupElement->FirstChildElement("models");
                if (modelsElement) {
                    for (XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
                        string file = (modelElement->Attribute("file"));
                        modelos.push_back("../3DFiles/" + file);
                    }
                }
                for (const auto& model : modelos) {
                    read_3D_File(model);
                }
            }
        }
    }
}

// write function to process keyboard events

void keyboardCallback(unsigned char key, int x, int y){
    switch(key) {
        case 'a': // esquerda
            cam.alpha -= 0.1f;
            break;
        case 'd': // direita
            cam.alpha += 0.1f;
            break;
        case 'w': // cima
            if(cam.beta <= 1.5f)
                cam.beta += 0.1f;
            break;
        case 's': // baixo
            if(cam.beta >= -1.5f)
                cam.beta -= 0.1f;
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
        default:
            break;


    }
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
    switch (key){
        case GLUT_KEY_UP:{
            cam.radius -= 0.1f;
            break;
        }
        case GLUT_KEY_DOWN:{
            cam.radius += 0.1f;
            break;
        }
        default:
            break;
    }
    glutPostRedisplay();
}




int main(int argc, char **argv) {

    if(argc != 2){
        return -1;
    }
    string xml = "../XMLFiles/" + (string)argv[1];
    parseXML(xml.c_str());
// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(win.width,win.height);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);


// put here the registration of the keyboard callbacks
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(processSpecialKeys);


//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}

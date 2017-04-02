#include <stdio.h>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define WINDOW_ANCHO 500
#define WINDOW_ALTO 500

// Espacio para las variables globales de la ventana
float gl_fovy = 60.0f;
float gl_fovy_min = 10.0f, gl_fovy_max = 90.0f, gl_fovy_paso = 1.0f;
float gl_cerca = 1.0f, gl_lejos = 100.0f;
float gl_center = 7.0f; // centro donde pondremos los objetos


// Espacio para otras variables globales
double theta = M_PI_2, phi = 0.0;
double radio = 7.0;

float r_carbono = 1.0f;
float r_hidrogeno = 0.5f;
float r_enlace = 0.2f;
float l_enlace = 3.0f;

// Espacio para la declaración de funciones
void InitGlew();

void InitGL();

void Display();

void ReshapeSize(int ancho, int alto);

void PosicionRaton(int x, int y);

void RuedaRaton(int rueda, int sentido, int x, int y);

//main
int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_ANCHO, WINDOW_ALTO);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("OpenGL Practica ");
    InitGlew(); // despues de crear la primera ventana
    InitGL();
    glutDisplayFunc(Display); // registra la funcion de rendering
    glutReshapeFunc(ReshapeSize);
    glutMotionFunc(PosicionRaton);
    glutMouseWheelFunc(RuedaRaton);

    glutMainLoop(); // bucle principal

    return 0;
}

void InitGlew() {

    // para poder utilizar trasnparentemente todas las extensiones de OpenGL

    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_init));
    } else
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void setViewport() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, ancho, alto); // utiliza la totalidad de la ventana
}

void setProjection() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(gl_fovy, (float) ancho / (float) alto, gl_cerca, gl_lejos);
}

void setModelView() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setLight0() {
    float ambient[4] = {0.2, 0.2, 0.2, 1.0};
    float diffuse[4] = {0.8, 0.8, 0.8, 1.0};
    float specular[4] = {2, 2, 2, 1.0};

    float position[4] = {1, 1, 1, 1.0};
    float direction[4] = {-1, -1, -1, 1.0};
    float cut_off = 90.0f;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cut_off);

    glEnable(GL_LIGHT0);
}

void InitGL() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // para cambiar el criterio de lo que se ve.
    // LESS es el de defecto: se ve lo que tiene menor profundidad
    glDepthFunc(GL_LESS);
    // TO DO

    setViewport();
    setProjection();

    glEnable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
//    glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);

    setLight0();
}

// cambio de dimensiones de la ventana, manteniendo la dimension de los objetos
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void ReshapeSize(int ancho, int alto) {
#pragma clang diagnostic pop

    setViewport();
    setProjection();
    glutPostRedisplay();
}

void PosicionRaton(int x, int y) {

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    theta = M_PI * (double) y / (double) alto;
    phi = -M_PI + 2 * M_PI * (double) x / (double) ancho;
    glutPostRedisplay();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void RuedaRaton(int rueda, int sentido, int x, int y) {
#pragma clang diagnostic pop

    gl_fovy -= gl_fovy_paso * (float) sentido;
    if (gl_fovy < gl_fovy_min) gl_fovy = gl_fovy_min;
    if (gl_fovy > gl_fovy_max) gl_fovy = gl_fovy_max;
    setProjection();
    glutPostRedisplay();
}


void setMaterial(float r, float g, float b) {
    const GLfloat ambient[4] = {0.3, 0.3, 0.3, 1.0};
    const GLfloat diffuse[4] = {r, g, b, 1.0};
    const GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};
    const GLfloat shininess = 100.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void carbono(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setMaterial((float) 200 / 255, (float) 200 / 255, (float) 200 / 255);
    glutSolidSphere(r_carbono, 20, 20);
    glPopMatrix();
}

void hidrogeno(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setMaterial((float) 143 / 255, (float) 143 / 255, (float) 255 / 255);
    glutSolidSphere(r_hidrogeno, 20, 20);
    glPopMatrix();
}

void enlace(float x1, float y1, float z1, float x2, float y2, float z2) {
    glPushMatrix();

    glTranslatef(x1, y1, z1);
    x2 -= x1;
    y2 -= y1;
    z2 -= z1;
    float d = (float) sqrt(x2 * x2 + y2 * y2 + z2 * z2);
    float angle = (float) (acos(z2 / d) * 180 / M_PI);
    glRotatef(-angle, y2, -x2, 0.0f);
    setMaterial(200 / 255, 200 / 255, 200 / 255);
    glutSolidCylinder(r_enlace, d, 20, 20);

    glPopMatrix();
}

// función de gestion de ventana
void Display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra todo lo existente en el framebuffer
    setModelView();

    // poicionamos el centro de coordenadas
    glTranslatef(0.0f, 0.0f, -gl_center);

    // el eje cenital es el Y
    float x = (float) (radio * sin(theta) * sin(phi));
    float y = (float) (radio * cos(theta));
    float z = (float) (radio * sin(theta) * cos(phi));
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // mira al (0,0,0)

    // TO DO

    //coordenadas de los átomos de hidrógeno
    double angle = 109.5;
    double pangle = angle * M_PI / 180;
    float cos_enlace = l_enlace * (float) cos(pangle);
    float sin_enlace = l_enlace * (float) sin(pangle);

    float x1 = 0.0f;
    float y1 = 1.0f * l_enlace;
    float z1 = 0.0f;

    float x2 = 0.0f;
    float y2 = cos_enlace;
    float z2 = sin_enlace;

    float x3 = (float) (sin_enlace * sin(2 * M_PI / 3));
    float y3 = cos_enlace;
    float z3 = (float) (sin_enlace * cos(2 * M_PI / 3));

    float x4 = (float) (-sin_enlace * sin(2 * M_PI / 3));
    float y4 = cos_enlace;
    float z4 = (float) (sin_enlace * cos(2 * M_PI / 3));

    //carbono en el centro
    carbono(0, 0, 0);
    hidrogeno(x2, y2, z2);
    hidrogeno(x3, y3, z3);
    hidrogeno(x4, y4, z4);

    enlace(0, 0, 0, x1, y1, z1);
    enlace(0, 0, 0, x2, y2, z2);
    enlace(0, 0, 0, x3, y3, z3);
    enlace(0, 0, 0, x4, y4, z4);

    glPushMatrix();
    {
        glTranslatef(x1, y1, z1);
        glRotatef(180, 0, 0, 1);
        glRotatef(180, 0, 1, 0);

        carbono(0, 0, 0);
        hidrogeno(x2, y2, z2);
        hidrogeno(x3, y3, z3);
        hidrogeno(x4, y4, z4);

        enlace(0, 0, 0, x2, y2, z2);
        enlace(0, 0, 0, x3, y3, z3);
        enlace(0, 0, 0, x4, y4, z4);
    }
    glPopMatrix();

    glFlush(); // actualiza el framebuffer
    glutSwapBuffers(); // en caso de animacion
}

#include <GLUT/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define GL_SILENCE_DEPRECATION

GLdouble PI = 3.14159265;
GLfloat yMove = 50, xMove = 10;
GLfloat ySteps = 2, xSteps = 1;
int yFlag = 1, xFlag = 1, animateFlag = 1;

//TRIGGER ANIMATION
void Animate() {
    if(animateFlag == 1) {
        //Animate about y-axis
        if(yMove > -50 && yFlag == 1) {
            yMove -= ySteps;
            if(yMove <= -50) {
                yFlag = 0;
            }
        }
        if(yMove < 50 && yFlag == 0) {
            yMove += ySteps;
            if(yMove >= 50) {
                yFlag = 1;
            }
        }

        //Animate about x-axis
        if(xMove > -10 && xFlag == 1) {
            xMove -= xSteps;
            if(xMove <= -10) {
                xFlag = 0;
            }
        }
        if(xMove < 10 && xFlag == 0) {
            xMove += xSteps;
            if(xMove >= 10) {
                xFlag = 1;
            }
        }

        glutPostRedisplay(); //Redraw the screen
    }
}




//ASHOKA CHAKRA
void DrawAshokaChakra(GLdouble xCenter, GLdouble yCenter, GLdouble radius) {
    int numSpokes = 24;
    int i;
    GLdouble angle, x, y;

    //Draw the circle
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    GLdouble radianConversion = PI / 180;
    for (i = 0; i < 360; i++) {
        angle = i * radianConversion;
        x = xCenter + radius * cos(angle);
        y = yCenter + radius * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();

    //Draw the spokes
    glBegin(GL_LINES);
    GLdouble angleIncrement = 2 * PI / numSpokes;
    for (i = 0; i < numSpokes; i++) {
        angle = i * angleIncrement;
        x = xCenter + radius * cos(angle);
        y = yCenter + radius * sin(angle);
        glVertex2d(xCenter, yCenter);
        glVertex2d(x, y);
    }
    glEnd();
}

//MAIN DRAW FUNCTION
void Draw() {
    GLfloat x[4], y1[4], y2[4], y3[4], y4[4];
    glClear(GL_COLOR_BUFFER_BIT);

    /* Initialize the control points */
    x[0] = 100; x[1] = 200; x[2] = 200;  x[3] = 300 - xMove;
    y1[0] = 450; y1[1] = 450 + yMove; y1[2] = 450 - yMove; y1[3] = 450; //Upper boundary of orange
    y2[0] = 400; y2[1] = 400 + yMove; y2[2] = 400 - yMove; y2[3] = 400; //Upper boundary of white
    y3[0] = 350; y3[1] = 350 + yMove; y3[2] = 350 - yMove; y3[3] = 350; //Upper boundary of green
    y4[0] = 300; y4[1] = 300 + yMove; y4[2] = 300 - yMove; y4[3] = 300; //Lower boundary of green

    /* Bézier curve algorithm to determine the intermediate points */
    GLdouble xt[200], y1t[200], y2t[200], y3t[200], y4t[200], t;
    int i,c; //c is the number of intermediate points
    for (i = 0, t = 0, c = 0; t <= 1; i++, t += 0.01) {
       xt[i] = pow(1 - t, 3) * x[0] + 3 * t * pow(1 - t, 2) * x[1] + 3 * pow(t, 2) * (1 - t) * x[2] + pow(t, 3) * x[3];
       y1t[i] = pow(1 - t, 3) * y1[0] + 3 * t * pow(1 - t, 2) * y1[1] + 3 * pow(t, 2) * (1 - t) * y1[2] + pow(t, 3) * y1[3];
       y2t[i] = pow(1 - t, 3) * y2[0] + 3 * t * pow(1 - t, 2) * y2[1] + 3 * pow(t, 2) * (1 - t) * y2[2] + pow(t, 3) * y2[3];
       y3t[i] = pow(1 - t, 3) * y3[0] + 3 * t * pow(1 - t, 2) * y3[1] + 3 * pow(t, 2) * (1 - t) * y3[2] + pow(t, 3) * y3[3];
       y4t[i] = pow(1 - t, 3) * y4[0] + 3 * t * pow(1 - t, 2) * y4[1] + 3 * pow(t, 2) * (1 - t) * y4[2] + pow(t, 3) * y4[3];
       c++;
    }


    /* Draw the flag */

    //Draw the orange part
    glColor3f(1, 0.25f, 0);
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i < c; i++) {
            glVertex2d(xt[i], y1t[i]);
            glVertex2d(xt[i], y2t[i]);
        }
    glEnd();

    //Draw the white part
    glColor3f(1, 1, 1);
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i < c; i++) {
            glVertex2d(xt[i], y2t[i]);
            glVertex2d(xt[i], y3t[i]);
        }
    glEnd();

    //Draw the green part
    glColor3f(0.1f, 0.5f, 0.1f);
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i < c; i++) {
            glVertex2d(xt[i], y3t[i]);
            glVertex2d(xt[i], y4t[i]);
        }
    glEnd();

    /* Draw the Ashoka Chakra */
    GLdouble chakraXCenter = xt[c / 2];
    GLdouble chakraYCenter = (y2t[c / 2] + y3t[c / 2]) / 2;
    GLdouble chakraRadius = (y2t[c / 2] - y3t[c / 2]) / 2;
    DrawAshokaChakra(chakraXCenter, chakraYCenter, chakraRadius);

    /* Draw the pole */
    glColor3f( 0.6f, 0.6f, 0.3f);
    glRecti(90, 460, 100, 50);

    glFlush();
}

void Menu(int n) {
    switch (n) {
        case 1:
            animateFlag = 1;
            break;
        case 2:
            animateFlag = 0;
            break;
        default:
            exit(0);
    }

    glutPostRedisplay(); //Redraw the screen
}

void MyInit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);

    //Create the menu
    glutCreateMenu(Menu);
    glutAddMenuEntry("Start",1);
    glutAddMenuEntry("Stop",2);
    glutAddMenuEntry("Exit",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argC, char *argV[]) {
    glutInit(&argC, argV);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Animate Flag");
    MyInit();
    glutDisplayFunc(Draw);
    glutIdleFunc(Animate);
    glutMainLoop();
 }

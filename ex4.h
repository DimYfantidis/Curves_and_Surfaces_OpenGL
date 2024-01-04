#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma once

#include <GL/glut.h>
#include "globals.h"
int dim = 7 ;
int v_angle = 0;
int c_angle = 0 ;
double c_R = 10 ;
int selectedControlPoint = -1;


GLfloat MIT[4][4] ;
GLfloat NCT[4][4][3] ;

GLfloat MI[4][4] = {
        {4.0, 2.0, 14.0/9.0, 4.0/3.0},
        {2, 14.0/9.0, 4.0/3.0, 98.0/81.0},
        {4.0/3, 11.0/9.0, 31.0/27.0, 89.0/81.0},
        {1.0, 1.0, 1.0, 1}
};



GLfloat controlPoints[4][4][3] = {
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 2.0}, {0.0, 0.0, 3.0}},
        {{0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 2.0}, {0.0, 1.0, 3.0}},
        {{0.0, 2.0, 0.0}, {0.0, 2.0, 1.0}, {0.0, 2.0, 2.0}, {0.0, 2.0, 3.0}},
        {{0.0, 3.0, 0.0}, {0.0, 3.0, 1.0}, {0.0, 3.0, 2.0}, {0.0, 3.0, 3.0}}
};

//GLfloat newControlPoints[4][4][3] = {{{0.0,0.0,70.74897119341563},{0.0,0.0,46.781893004115226},{0.0,0.0,36.161865569272976},{0.0,0.0,29.77777777777778}
//                                     },{{0.0,60.55692729766804,70.74897119341563},{0.0,38.50754458161865,46.781893004115226},{0.0,29.214906264288977,36.161865569272976},{0.0,23.790123456790123,29.77777777777778}
//                                     },{{0.0,121.11385459533608,70.74897119341563},{0.0,77.0150891632373,46.781893004115226},{0.0,58.429812528577955,36.161865569272976},{0.0,47.58024691358025,29.77777777777778}
//                                     },{{0.0,181.67078189300412,70.74897119341563},{0.0,115.52263374485597,46.781893004115226},{0.0,87.64471879286694,36.161865569272976},{0.0,71.37037037037037,29.77777777777778}
//                                     }};

GLfloat newControlPoints[4][4][3] = {
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 2.0}, {0.0, 0.0, 3.0}},
        {{0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 2.0}, {0.0, 1.0, 3.0}},
        {{0.0, 2.0, 0.0}, {0.0, 2.0, 1.0}, {0.0, 2.0, 2.0}, {0.0, 2.0, 3.0}},
        {{0.0, 3.0, 0.0}, {0.0, 3.0, 1.0}, {0.0, 3.0, 2.0}, {0.0, 3.0, 3.0}}
};
void printM(GLfloat A[4][4][3]){
    for(int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 4 ; j++){
            int p = i * 4 + j;
            printf("P - %d = %f, %f, %f \n",p,A[i][j][0],A[i][j][1],A[i][j][2]);
        }
    }
}

void transpose(GLfloat A[4][4][3], GLfloat B[4][4][3])
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
            B[i][j][0] = A[j][i][0];
            B[i][j][1] = A[j][i][1];
            B[i][j][2] = A[j][i][2];
        }
}

void multiplyMatrices(GLfloat matrixA[4][4], GLfloat matrixB[4][3], GLfloat result[4][3]) {


    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

void changePoints(GLfloat mm[4][4], GLfloat cp[][4][3], GLfloat ncp[][4][3]){
    multiplyMatrices(mm,cp[0],ncp[0]);
    multiplyMatrices(mm,cp[1],ncp[1]);
    multiplyMatrices(mm,cp[2],ncp[2]);
    multiplyMatrices(mm,cp[3],ncp[3]);
    transpose(ncp,NCT);
    multiplyMatrices(mm,NCT[0],cp[0]);
    multiplyMatrices(mm,NCT[1],cp[1]);
    multiplyMatrices(mm,NCT[2],cp[2]);
    multiplyMatrices(mm,NCT[3],cp[3]);
    transpose(cp,ncp);



}

void keyboard(int key,int x,int y){

    if ( key == GLUT_KEY_RIGHT){
        c_angle += 2;
    }
    if ( key == GLUT_KEY_LEFT){
        c_angle -= 2;
    }
    if ( key == GLUT_KEY_UP){
        v_angle += 2;
    }
    if ( key == GLUT_KEY_DOWN){
        v_angle -= 2;
    }
    if ( key == GLUT_KEY_PAGE_UP){
        dim --;
    }
    if (key == GLUT_KEY_PAGE_DOWN){
        dim ++;
    }
    if (key == 26 && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {

    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double hrad = c_angle*3.14159/180;
    double vrad = v_angle*3.14159/180;
    glOrtho(-dim,dim,-dim,dim,-dim-40,dim+40);
    gluLookAt(c_R*sin(hrad),0,c_R*cos(hrad),0,0,0,0,1,0);


    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();


}

// Display function of the 4th exercise
void displayFourth(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &newControlPoints[0][0][0]);


    for (int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= 4; j++) {
            glEvalCoord2f((GLfloat)j / 4.0, (GLfloat)i / 4.0);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= 4; j++) {
            glEvalCoord2f((GLfloat)i / 4.0, (GLfloat)j / 4.0);
        }
        glEnd();
    }
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 4; i++) {
        glEvalCoord2f((GLfloat)i / 4.0,1);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 4; i++) {
        glEvalCoord2f(1,(GLfloat)i / 4.0);
    }
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
    {
        for (int i = 0; i < 4; i++) {
            for(int j = 0 ; j < 4 ; j++){
                glVertex3fv(newControlPoints[i][j]);
            }
        }
    }
    glEnd();

	glutSwapBuffers();
}
void motion(int x, int y) {
    if (selectedControlPoint != -1) {
        GLint viewport[4];
        GLdouble modelview[16], projection[16];
        GLfloat winX, winY, winZ;
        GLdouble posX, posY, posZ;


        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        winX = (float)x;
        winY = (float)viewport[3] - (float)y;
        glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

        // Update the position of the selected control point
        int i = selectedControlPoint / 4;
        int j = selectedControlPoint % 4;
        newControlPoints[i][j][0] = posX;
        newControlPoints[i][j][1] = posY;
        newControlPoints[i][j][2] = posZ;
        //gluProject(posX,posY,posZ,modelview,projection,viewport,&newWinX,&newWinY, &newWinZ);
        //lastCPS = selectedControlPoint;

        glutPostRedisplay();
    }
}



void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        GLint viewport[4];
        GLdouble modelview[16], projection[16];
        GLfloat winX, winY, winZ;
        GLdouble posX, posY, posZ;

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        winX = (float)x;
        winY = (int)((float)viewport[3] - (float)y);
        glReadPixels(x, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

        // Check if a control point is selected
        printf("%f,%f,%f\n",posX,posY,posZ);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (fabs(posX - newControlPoints[i][j][0]) < 0.2 &&
                    fabs(posY - newControlPoints[i][j][1]) < 0.2 &&
                    fabs(posZ - newControlPoints[i][j][2]) < 0.2) {
                    selectedControlPoint = i * 4 + j;
                    printf("--%d\n",selectedControlPoint);
                    break;
                }
            }
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        selectedControlPoint = -1;
    }
}



// Any initialization for the 4th exercise should be placed here.
void initFourth(void)
{
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_DEPTH_TEST);
	current_frame = 4;
    changePoints(MI, controlPoints, newControlPoints);
    printM(newControlPoints);
	glutDisplayFunc(displayFourth);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(keyboard);
}

#pragma clang diagnostic pop
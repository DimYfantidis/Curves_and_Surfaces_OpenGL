#pragma once

#include <GL/glut.h>
#include "globals.h"


// The seven control points, (p0, p1, p2, p3) for the first 
// cubic spline and (p3, p4, p5, p6) for the second one.
static point2f controlPointsEx1[7] = {
	{-0.7f, -0.6f},
	{-0.6f, 0.5f},
	{-0.2f, -0.5f},
	{0.0f, 0.2f},
	{0.3f, -0.8f},
	{0.6f, -0.2f},
	{0.8f, 0.3f}
};
static int numPointsAddedByUserEx1 = 0;


void getCoefficients(point2f C[4], const point2f p[4])
{
	// Interpolating Geometry Matrix
	static const GLfloat M_I[4][4] = {
		{001.0f, 000.0f, 000.0f, 000.0f},
		{-05.5f, 009.0f, -04.5f, 001.0f},
		{009.0f, -22.5f, 018.0f, -04.5f},
		{-04.5f, 013.5f, -13.5f, 004.5f}
	};

	// Coefficients computed according to the formula:
	//		C = matrix_mul(M_I, P)
	// x coordinates of c1, c2, c3, c4
	C[0][0] = M_I[0][0] * p[0][0] + M_I[0][1] * p[1][0] + M_I[0][2] * p[2][0] + M_I[0][3] * p[3][0];
	C[1][0] = M_I[1][0] * p[0][0] + M_I[1][1] * p[1][0] + M_I[1][2] * p[2][0] + M_I[1][3] * p[3][0];
	C[2][0] = M_I[2][0] * p[0][0] + M_I[2][1] * p[1][0] + M_I[2][2] * p[2][0] + M_I[2][3] * p[3][0];
	C[3][0] = M_I[3][0] * p[0][0] + M_I[3][1] * p[1][0] + M_I[3][2] * p[2][0] + M_I[3][3] * p[3][0];
	// y coordinates of c1, c2, c3, c4
	C[0][1] = M_I[0][0] * p[0][1] + M_I[0][1] * p[1][1] + M_I[0][2] * p[2][1] + M_I[0][3] * p[3][1];
	C[1][1] = M_I[1][0] * p[0][1] + M_I[1][1] * p[1][1] + M_I[1][2] * p[2][1] + M_I[1][3] * p[3][1];
	C[2][1] = M_I[2][0] * p[0][1] + M_I[2][1] * p[1][1] + M_I[2][2] * p[2][1] + M_I[2][3] * p[3][1];
	C[3][1] = M_I[3][0] * p[0][1] + M_I[3][1] * p[1][1] + M_I[3][2] * p[2][1] + M_I[3][3] * p[3][1];
}

// Display function for the first exercise of the project.
void displayFirst(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (numPointsAddedByUserEx1 > 7)
	{
		printf("Error: Reached Unreachable state (numPointsAddedByUserEx1 > 7)\n");
		exit(EXIT_FAILURE);
	}

	if (numPointsAddedByUserEx1 == 7)
	{
		// The coefficients of the current cubic spline.
		point2f coeff[4];
		// The target point to be projected on the screen.
		point2f drawPoint;

		glBegin(GL_LINE_STRIP);
		{
			// Draw first spline in RED.
			glColor3f(1.0f, 0.0f, 0.0f);

			// Calculate c0, c1, c2, c3 for the first cubic spline (using p0, p1, p2, p3).
			getCoefficients(coeff, controlPointsEx1);

			// Calculate p(u) = c0 + c1 * u + c2 * u^2 + c3 * u^3 for the first cubic spline.
			for (float u = 0.0f; u <= 1.0f; u += 0.01f)
			{
				drawPoint[0] = coeff[0][0] + coeff[1][0] * u + coeff[2][0] * u * u + coeff[3][0] * u * u * u;	// p_x(u)
				drawPoint[1] = coeff[0][1] + coeff[1][1] * u + coeff[2][1] * u * u + coeff[3][1] * u * u * u;	// p_y(u)
				glVertex2fv(drawPoint);
			}
			// Draw second spline in GREEN.
			glColor3f(0.0f, 1.0f, 0.0f);

			// Calculate c0, c1, c2, c3 for the second cubic spline (using p3, p4, p5, p6).
			getCoefficients(coeff, controlPointsEx1 + 3);

			// Calculate p(u) = c0 + c1 * u + c2 * u^2 + c3 * u^3 for the second cubic spline.
			for (float u = 0.0f; u <= 1.0f; u += 0.01f)
			{
				drawPoint[0] = coeff[0][0] + coeff[1][0] * u + coeff[2][0] * u * u + coeff[3][0] * u * u * u;	// p_x(u)
				drawPoint[1] = coeff[0][1] + coeff[1][1] * u + coeff[2][1] * u * u + coeff[3][1] * u * u * u;	// p_y(u)
				glVertex2fv(drawPoint);
			}
		}
		glEnd();
	}
	else
	{
		char buffer[70];
		snprintf(buffer, 200, "Enter 7 Points by clicking anywhere on the screen (%d left)", 7 - numPointsAddedByUserEx1);
		renderString2f(-1.0f, 0.95f, GLUT_BITMAP_9_BY_15, buffer, 1.0f, 1.0f, 1.0f);
	}
	
	// Draw control points in WHITE.
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_POINTS);
	{
		for (int i = 0; i < numPointsAddedByUserEx1; ++i)
			glVertex2fv(controlPointsEx1[i]);
	}
	glEnd();

	if (showPointTags)
	{
		float x, y;
		char p_tag[5];

		for (int i = 0; i < numPointsAddedByUserEx1; ++i)
		{
			x = controlPointsEx1[i][0];
			y = controlPointsEx1[i][1];
			snprintf(p_tag, 5, "p%d", i);
			renderString2f(x - 0.05f, y + 0.04f, GLUT_BITMAP_9_BY_15, p_tag, 1.0f, 1.0f, 1.0f);
		}
	}
	glutSwapBuffers();
}

// The index of the control point to be translated.
static int vertexBeingMovedIndexEx1;

void MotionFirst(int x, int y)
{
	point2f gotoCoords;
	// The screen coordinates where the selected point will be moved to.
	gotoCoords[0] = (float)(x - WINDOW_WIDTH_HALF) / WINDOW_WIDTH_HALF;
	gotoCoords[1] = (float)((WINDOW_HEIGHT - y) - WINDOW_HEIGHT_HALF) / WINDOW_HEIGHT_HALF;

	// Guards the control points from sliding out of the screen.
	if (absf(gotoCoords[0]) > 1.0f) {
		gotoCoords[0] = sgnf(gotoCoords[0]);
	}
	if (absf(gotoCoords[1]) > 1.0f) {
		gotoCoords[1] = sgnf(gotoCoords[1]);
	}
	controlPointsEx1[vertexBeingMovedIndexEx1][0] = gotoCoords[0];
	controlPointsEx1[vertexBeingMovedIndexEx1][1] = gotoCoords[1];
	glutPostRedisplay();
}

void mouseFirst(int button, int state, int x, int y)
{
	point2f screen_pos;
	float distances[7];

	// Convert x, y pixel window coordinates to gluOrtho2D(-1, 1, -1, 1) projection coordinates.
	screen_pos[0] = (float)(x - WINDOW_WIDTH_HALF) / WINDOW_WIDTH_HALF;
	screen_pos[1] = (float)((WINDOW_HEIGHT - y) - WINDOW_HEIGHT_HALF) / WINDOW_HEIGHT_HALF;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			// Calculate the distances between each control point and the clicked screen coordinates.
			for (int i = 0; i < 7; ++i)
				distances[i] = pointDistance2f(screen_pos, controlPointsEx1[i]);
			// Get the index of the closest vertex to the clicked point.
			if (getMinFloatWithIndex(distances, 7, &vertexBeingMovedIndexEx1) < 0.03)
				// If the distance is smaller than a threshold, start moving the control point.
				glutMotionFunc(MotionFirst);
		}
		else
		{
			glutMotionFunc(NULL);
		}
	}
}

void mouseInitFirst(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (numPointsAddedByUserEx1 < 7) {
			controlPointsEx1[numPointsAddedByUserEx1][0] = (float)(x - WINDOW_WIDTH_HALF) / WINDOW_WIDTH_HALF;
			controlPointsEx1[numPointsAddedByUserEx1][1] = (float)((WINDOW_HEIGHT - y) - WINDOW_HEIGHT_HALF) / WINDOW_HEIGHT_HALF;
			numPointsAddedByUserEx1++;
		}
		if (numPointsAddedByUserEx1 == 7)
			glutMouseFunc(mouseFirst);
	}
}

void initFirst(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	glLineWidth(2.0);
	glPointSize(5.0);

	current_frame = 1;

	glutMouseFunc(mouseInitFirst);
	glutDisplayFunc(displayFirst);
}
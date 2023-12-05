#pragma once

#include <GL/glut.h>
#include "globals.h"


point2f controlPointsEx3[7] = {
	{-0.8f, 0.0f},	// p0
	{-0.6f, 0.8f},	// p1
	{-0.3f, 0.8f},	// p2 = -p4 for C^1 continuity
	{0.0f, 0.0f},	// p3 = Junction point 
	{0.3f, -0.8f},	// p4 = -p2 for C^1 continuity
	{0.7f, -0.9f},	// p5
	{0.8f, 0.3f}	// p6
};


void displayThird(void)
{
	const point2f* p = controlPointsEx3;
	point2f drawPoint;
	point2f* convexHull[7];
	int hullSize;

	glClear(GL_COLOR_BUFFER_BIT);

	// Tirquoise blue: #3F888F
	glColor3ub(0x3f, 0x88, 0x8f);

	glBegin(GL_LINE_STRIP);
	{
		float u_comp;

		// First Bezier Curve
		for (float u = 0.0f; u <= 1.0f; u += 0.01f)
		{
			u_comp = 1 - u;

			drawPoint[0] = u_comp * u_comp * u_comp * p[0][0] +
				3 * u * u_comp * u_comp * p[1][0] +
				3 * u * u * u_comp * p[2][0] +
				u * u * u * p[3][0];

			drawPoint[1] = u_comp * u_comp * u_comp * p[0][1] +
				3 * u * u_comp * u_comp * p[1][1] +
				3 * u * u * u_comp * p[2][1] +
				u * u * u * p[3][1];

			glVertex2fv(drawPoint);
		}
		// Second Bezier Curve
		for (float u = 0.0f; u <= 1.0f; u += 0.01f)
		{
			u_comp = 1 - u;

			drawPoint[0] = u_comp * u_comp * u_comp * p[3][0] +
				3 * u * u_comp * u_comp * p[4][0] +
				3 * u * u * u_comp * p[5][0] +
				u * u * u * p[6][0];

			drawPoint[1] = u_comp * u_comp * u_comp * p[3][1] +
				3 * u * u_comp * u_comp * p[4][1] +
				3 * u * u * u_comp * p[5][1] +
				u * u * u * p[6][1];

			glVertex2fv(drawPoint);
		}
	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	{
		glVertex2fv(p[0]);
		glVertex2fv(p[1]);
		glVertex2fv(p[2]);
		glVertex2fv(p[3]);
		glVertex2fv(p[4]);
		glVertex2fv(p[5]);
		glVertex2fv(p[6]);
	}
	glEnd();

	if (showConvexHulls)
	{
		getConvexHull(convexHull, &hullSize, controlPointsEx3, 7);
		glColor3ub(0x00, 0x00, 0x58);
		glBegin(GL_LINE_LOOP);
		{
			for (int i = 0; i < hullSize; ++i)
				glVertex2fv(*convexHull[i]);
		}
		glEnd();
	}

	if (showPointTags)
	{
		float x, y;
		char p_tag[5];

		for (int i = 0; i < 7; ++i)
		{
			x = controlPointsEx3[i][0];
			y = controlPointsEx3[i][1];
			snprintf(p_tag, 5, "p%d", i);
			renderString2f(x - 0.05f, y + 0.04f, GLUT_BITMAP_9_BY_15, p_tag, 1.0f, 1.0f, 1.0f);
		}
	}

	glutSwapBuffers();
}


// The index of the control point to be translated
static int vertexBeingMovedIndexEx3;

void MotionThird(int x, int y)
{
	GLfloat dx;
	GLfloat dy;
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
	dx = gotoCoords[0] - controlPointsEx3[vertexBeingMovedIndexEx3][0];
	dy = gotoCoords[1] - controlPointsEx3[vertexBeingMovedIndexEx3][1];

	controlPointsEx3[vertexBeingMovedIndexEx3][0] = gotoCoords[0];
	controlPointsEx3[vertexBeingMovedIndexEx3][1] = gotoCoords[1];

	if (vertexBeingMovedIndexEx3 == 2 || vertexBeingMovedIndexEx3 == 4)
	{
		controlPointsEx3[3][0] = (controlPointsEx3[2][0] + controlPointsEx3[4][0]) / 2;
		controlPointsEx3[3][1] = (controlPointsEx3[2][1] + controlPointsEx3[4][1]) / 2;
	}
	else if (vertexBeingMovedIndexEx3 == 3) 
	{
		controlPointsEx3[2][0] += dx;
		controlPointsEx3[2][1] += dy;

		controlPointsEx3[4][0] += dx;
		controlPointsEx3[4][1] += dy;
	}

	glutPostRedisplay();
}

void mouseThird(int button, int state, int x, int y)
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
				distances[i] = pointDistance2f(screen_pos, controlPointsEx3[i]);
			// Get the index of the closest vertex to the clicked point.
			if (getMinFloatWithIndex(distances, 7, &vertexBeingMovedIndexEx3) < 0.03)
				// If the distance is smaller than a threshold, start moving the control point.
				glutMotionFunc(MotionThird);
		}
		else
		{
			glutMotionFunc(NULL);
		}
	}
}

void initThird(void)
{
	current_frame = 3;
	glutMouseFunc(mouseThird);
	glutDisplayFunc(displayThird);
}

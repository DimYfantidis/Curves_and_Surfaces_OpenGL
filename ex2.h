#pragma once

#include <GL/glut.h>
#include "globals.h"


// The spline's 7 control points.
// Last one coincides with the first, thus 6 points.
static point2f controlPointsEx2[6] = {
	{-0.32f, -0.67f},	// Last and first point
	{-0.75f, -0.15f},
	{-0.37f, 00.59f},
	{00.29f, 00.42f},
	{00.58f, 00.08f},
	{00.53f, -0.60f},
};
static int numPointsAddedByUserEx2 = 0;


// Bernstein basis polynomial
float bernstein_6(int i, float u) 
{
	static const float binomialCoefficients[7] = { 1.0, 6.0, 15.0, 20.0, 15.0, 6.0, 1.0 };
	return (float)(binomialCoefficients[i] * pow(u, i) * pow(1-u, 6-i));
}

void displaySecond(void)
{
	float b[7];
	point2f* p = controlPointsEx2;
	point2f drawPoint;
	point2f* convexHull[6];
	int hullSize;


	glClear(GL_COLOR_BUFFER_BIT);

	if (numPointsAddedByUserEx2 > 6)
	{
		printf("Error: Reached Unreachable state (numPointsAddedByUserEx2 > 6)\n");
		exit(EXIT_FAILURE);
	}

	if (numPointsAddedByUserEx2 == 6)
	{
		// Draw control points in MAGENTA
		glColor3f(1.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		{
			for (float u = 0.0f; u <= 1.0f; u += 0.01f)
			{
				// Initialize sum
				drawPoint[0] = 0.0f;
				drawPoint[1] = 0.0f;

				// Plot B-spline of order 6
				for (int i = 0; i < 7; ++i)
				{
					b[i] = bernstein_6(i, u);
					// last point is equal to first using mod 6
					drawPoint[0] += b[i] * p[i % 6][0];
					drawPoint[1] += b[i] * p[i % 6][1];
				}
				glVertex2fv(drawPoint);
			}
		}
		glEnd();
	}
	else
	{
		char buffer[70];
		snprintf(buffer, 200, "Enter 6 Points by clicking anywhere on the screen (%d left)", 6 - numPointsAddedByUserEx2);
		renderString2f(-1.0f, 0.95f, GLUT_BITMAP_9_BY_15, buffer, 1.0f, 1.0f, 1.0f);
	}

	// Draw control points in WHITE
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_POINTS);
	{
		for (int i = 0; i < numPointsAddedByUserEx2; ++i)
			glVertex2fv(controlPointsEx2[i]);
	}
	glEnd();

	if (showConvexHulls)
	{
		// Draw control points in some sort of dark purple (?)
		glColor3ub(0x4d, 0x1a, 0x7f);
		getConvexHull(convexHull, &hullSize, controlPointsEx2, numPointsAddedByUserEx2);

		glBegin(GL_LINE_LOOP);
		{
			// Plot each point of the convex hull
			for (int i = 0; i < hullSize; ++i)
				glVertex2fv(*convexHull[i]);
		}
		glEnd();
	}

	if (showPointTags)
	{
		float x, y;
		char p_tag[5];

		for (int i = 0; i < numPointsAddedByUserEx2; ++i)
		{
			x = controlPointsEx2[i][0];
			y = controlPointsEx2[i][1];
			snprintf(p_tag, 5, "p%d", i);
			renderString2f(x - 0.05f, y + 0.04f, GLUT_BITMAP_9_BY_15, p_tag, 1.0f, 1.0f, 1.0f);
		}
	}

	glutSwapBuffers();
}


// The index of the control point to be translated
static int vertexBeingMovedIndexEx2;

void MotionSecond(int x, int y)
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
	controlPointsEx2[vertexBeingMovedIndexEx2][0] = gotoCoords[0];
	controlPointsEx2[vertexBeingMovedIndexEx2][1] = gotoCoords[1];
	glutPostRedisplay();
}

void mouseSecond(int button, int state, int x, int y)
{
	point2f screen_pos;
	float distances[6];

	// Convert x, y pixel window coordinates to gluOrtho2D(-1, 1, -1, 1) projection coordinates.
	screen_pos[0] = (float)(x - WINDOW_WIDTH_HALF) / WINDOW_WIDTH_HALF;
	screen_pos[1] = (float)((WINDOW_HEIGHT - y) - WINDOW_HEIGHT_HALF) / WINDOW_HEIGHT_HALF;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			// Calculate the distances between each control point and the clicked screen coordinates.
			for (int i = 0; i < 6; ++i)
				distances[i] = pointDistance2f(screen_pos, controlPointsEx2[i]);
			// Get the index of the closest vertex to the clicked point.
			if (getMinFloatWithIndex(distances, 6, &vertexBeingMovedIndexEx2) < 0.03)
				// If the distance is smaller than a threshold, start moving the control point.
				glutMotionFunc(MotionSecond);
		}
		else
		{
			glutMotionFunc(NULL);
		}
	}
}

void mouseInitSecond(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (numPointsAddedByUserEx2 < 6) {
			controlPointsEx2[numPointsAddedByUserEx2][0] = (float)(x - WINDOW_WIDTH_HALF) / WINDOW_WIDTH_HALF;
			controlPointsEx2[numPointsAddedByUserEx2][1] = (float)((WINDOW_HEIGHT - y) - WINDOW_HEIGHT_HALF) / WINDOW_HEIGHT_HALF;
			numPointsAddedByUserEx2++;
		}
		if (numPointsAddedByUserEx2 == 6)
			glutMouseFunc(mouseSecond);
	}
}

void initSecond(void)
{
	current_frame = 2;
	glutMouseFunc(mouseInitSecond);
	glutDisplayFunc(displaySecond);
}

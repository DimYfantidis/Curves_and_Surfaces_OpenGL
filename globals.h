#pragma once

#include <math.h>
#include <stdbool.h>
#include <GL/glut.h>


// Point typedefs
typedef GLfloat point2f[2];
typedef GLfloat point3f[3];
typedef GLdouble point2d[2];
typedef GLdouble point3d[3];
typedef GLint point2i[2];
typedef GLint point3i[3];

// window dimensions
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// half of each window dimension
int WINDOW_WIDTH_HALF;
int WINDOW_HEIGHT_HALF;

// Id of the main menu (used for menus).
int window_menu_id;
// Id of the extra menu (used for menus).
int extra_menu_id;
// Set to the number of the exercise (1, 2, 3 or 4)
int current_frame;

// Hides/Shows convex hulls (toggled through extra menu)
bool showConvexHulls = false;
// Hides/Shows the points' nametags (toggled through extra menu)
bool showPointTags = false;


// Get the distance of two 2D points of float data-type coordinates
float pointDistance2f(point2f p1, point2f p2)
{
	float diff_x = p2[0] - p1[0];
	float diff_y = p2[1] - p1[1];
	return (float)sqrt(diff_x * diff_x + diff_y * diff_y);
}

// Get the distance of two 2D points of double data-type coordinates
double pointDistance2d(point2d p1, point2d p2)
{
	double diff_x = p2[0] - p1[0];
	double diff_y = p2[1] - p1[1];
	return sqrt(diff_x * diff_x + diff_y * diff_y);
}

// Get the distance of two 3D points of float data-type coordinates
float pointDistance3f(point3f p1, point3f p2)
{
	float diff_x = p2[0] - p1[0];
	float diff_y = p2[1] - p1[1];
	float diff_z = p2[2] - p1[2];
	return (float)sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

// Get the distance of two 3D points of double data-type coordinates
double pointDistance3d(point3d p1, point3d p2)
{
	double diff_x = p2[0] - p1[0];
	double diff_y = p2[1] - p1[1];
	double diff_z = p2[2] - p1[2];
	return sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

/* Returns the minimum element of an array along with its index
 * Input: 
 *	- arr: The array
 *  - n: Length of array
 *  - index: index of minimum element is returned by reference
 * 
 * Returns: the minimum element */
float getMinFloatWithIndex(float arr[], int n, int* index)
{
	float min = arr[0];
	*index = 0;

	for (int i = 1; i < n; ++i) 
	{
		if (arr[i] < min)
		{
			min = arr[i];
			*index = i;
		}
	}
	return min;
}

// Absolute value function for float data type
float absf(float x) 
{
	return (x >= 0.0f ? x : -x);
}

// Sign value function for float data type
float sgnf(float x)
{
	return (x >= 0.0f ? 1.0f : -1.0f);
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(const point2f p, const point2f q, const point2f r)
{
	float val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1]);
	if (val == 0.0f) {
		// collinear 
		return 0;
	}
	// clock or counterclock wise 
	return (val > 0.0f) ? 1 : 2;
}

/* Code that I stole from here: https://www.geeksforgeeks.org/convex-hull-using-jarvis-algorithm-or-wrapping/.
 * Computes the convex hull of a set of points. 
 * Input:
 *	- hull: Array of references to the input points that make up the convex hull.
 *			Should have as much memory capacity as the input points' set (= n).
 *	- hull_length: The number of points that make up the convex hull
 *	- points: The input points' set
 *	- n: Number of input points
 */
void getConvexHull(const point2f** hull, int* hull_length, const point2f* points, int n)
{
	// There must be at least 3 points 
	if (n < 3) {
		*hull_length = 0;
		return;
	}

	// Find the leftmost point 
	int l = 0;
	for (int i = 1; i < n; i++)
		if (points[i][0] < points[l][0])
			l = i;

	int k = 0;
	// Start from leftmost point, keep moving counterclockwise 
	// until reach the start point again.  This loop runs O(h) 
	// times where h is number of points in result or output. 
	int p = l, q;
	do
	{
		// Add current point to result 
		hull[k++] = &points[p];

		// Search for a point 'q' such that orientation(p, q, 
		// x) is counterclockwise for all points 'x'. The idea 
		// is to keep track of last visited most counterclock- 
		// wise point in q. If any point 'i' is more counterclock- 
		// wise than q, then update q. 
		q = (p + 1) % n;
		for (int i = 0; i < n; i++)
		{
			// If i is more counterclockwise than current q, then 
			// update q 
			if (orientation(points[p], points[i], points[q]) == 2)
				q = i;
		}
		// Now q is the most counterclockwise with respect to p 
		// Set p as q for next iteration, so that q is added to 
		// result 'hull' 
		p = q;
	} 
	while (p != l);  // While we don't come to first point 

	// Return hull's size by reference
	*hull_length = k;
}


/* Renders a string on the screen
 * Input: 
 *	- x, y: Screen coordinates as floats
 *	- font: The font type of the string, preferably: GLUT_BITMAP_9_BY_15
 *	- string: The string to be rendered
 *	- r, g, b: The color of the string
 */
void renderString2f(float x, float y, void* font, const char* string, float r, float g, float b)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	{
		glColor3f(r, g, b);
		glRasterPos2f(x, y);
		for (int i = 0; string[i] != '\0'; ++i)
			glutBitmapCharacter(font, string[i]);
	}
	glPopAttrib();
}


/* Renders a string in the world
 * Input: 
 *	- x, y, z: World coordinates as floats
 *	- font: The font type of the string, preferably: GLUT_BITMAP_9_BY_15
 *	- string: The string to be rendered
 *	- r, g, b: The color of the string
 */
void renderString3f(float x, float y, float z, void* font, const char* string, float r, float g, float b)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	{
		glColor3f(r, g, b);
		glRasterPos3f(x, y, z);
		for (int i = 0; string[i] != '\0'; ++i)
			glutBitmapCharacter(font, string[i]);
	}
	glPopAttrib();
}
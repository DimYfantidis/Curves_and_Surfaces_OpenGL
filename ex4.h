#pragma once

#include <GL/glut.h>
#include "globals.h"


// Display function of the 4th exercise
void displayFourth(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
}

// Any initialization for the 4th exercise should be placed here.
void initFourth(void)
{
	current_frame = 4;
	glutDisplayFunc(displayFourth);
}

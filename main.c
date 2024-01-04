#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "globals.h"
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"


const char* exNames[] = {
	"Dual Cubic Splines (C^0 cont.)",
	"Closed Bezier order #6",
	"Dual Cubic Bezier (C^1 cont.)", 
	"Bicubic Surface"
};

void main_menu(int op_id)
{
	static char title[BUFSIZ];

	if (op_id >= 1 && op_id <= 4) 
	{
		strcpy(title, exNames[op_id - 1]);
		strcat(title, "  (Dimitris Yfantidis - 3938)");

		printf("Projecting: %s\n", exNames[op_id - 1]);
		glutSetWindowTitle(title);
	}
	switch (op_id)
	{
	case 1:
        glDisable(GL_DEPTH_TEST);
		initFirst();
		break;
	case 2:
        glDisable(GL_DEPTH_TEST);
		initSecond();
		break;
	case 3:
        glDisable(GL_DEPTH_TEST);
		initThird();
		break;
	case 4:
		initFourth();
		break;
	default:
		printf("Error: Reached unreachable state\n");
		exit(EXIT_FAILURE);
	}
	glutPostRedisplay();
}

// Extra menu
void extra_menu(int op_id)
{
	int* numVerticesByUser[4] = {
		&numPointsAddedByUserEx1,
		&numPointsAddedByUserEx2,
		NULL,
		NULL
	};
	void (*initFunctions[4])(void) = {
		initFirst,
		initSecond,
		initThird,
		initFourth
	};

	switch (op_id)
	{
	// DO NOT EDIT
	case 5:
		showConvexHulls = !showConvexHulls;
		if (showConvexHulls)
			printf("[OPTION]: Convex Hulls Enabled\n");
		else
			printf("[OPTION]: Convex Hulls Hidden\n");
		break;
	// DO NOT EDIT
	case 6:
		showPointTags = !showPointTags;
		if (showConvexHulls)
			printf("[OPTION]: Point nametags Enabled\n");
		else
			printf("[OPTION]: Point nametags Hidden\n");
		break;
	// DO NOT EDIT
	case 7:
		if (current_frame < 1 || current_frame > 5)
		{
			printf("Error: Reached Unreachable state (current_frame < 0 || current_frame > 4)");
			exit(EXIT_FAILURE);
		}
		if (numVerticesByUser[current_frame - 1] != NULL) {
			*numVerticesByUser[current_frame - 1] = 0;
		}
		initFunctions[current_frame - 1]();

		printf("[OPTION]: Current window has been reset\n");
		break;
	// Add extra case if necessary...



	// DO NOT EDIT
	default:
		printf("Error: Reached unreachable state\n");
		exit(EXIT_FAILURE);
	}
	glutPostRedisplay();
}

// Main menu
void create_window_menu(void)
{
	extra_menu_id = glutCreateMenu(extra_menu);
	{
		glutAddMenuEntry("Toggle Convex Hull", 5);
		glutAddMenuEntry("Toggle points' tags", 6);
		glutAddMenuEntry("Reset current menu", 7);
		// Add extra case if necessary...
	}

	window_menu_id = glutCreateMenu(main_menu);
	{
		glutAddMenuEntry(exNames[0], 1);
		glutAddMenuEntry(exNames[1], 2);
		glutAddMenuEntry(exNames[2], 3);
		glutAddMenuEntry(exNames[3], 4);
		glutAddSubMenu("More Options", extra_menu_id);
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Reshape function for resizing the window dynamically according to the user.
void reshape(int width, int height) 
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	WINDOW_WIDTH_HALF = WINDOW_WIDTH / 2;
	WINDOW_HEIGHT_HALF = WINDOW_HEIGHT / 2;

	glViewport(0, 0, width, height);
}


int main(int argc, char* argv[])
{
	// -------------- DO NOT EDIT (START) --------------
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("");

	WINDOW_WIDTH_HALF = WINDOW_WIDTH / 2;
	WINDOW_HEIGHT_HALF = WINDOW_HEIGHT / 2;
	glutReshapeFunc(reshape);

	create_window_menu();
	main_menu(1);
	// -------------- DO NOT EDIT (END) --------------

	glutMainLoop();

	return EXIT_SUCCESS;
}
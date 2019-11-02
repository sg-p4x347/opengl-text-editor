/*-----------------------------------------------------------------------------------------
 COURSE:				CSC525
 PROGRAMMERS:			Gage Coates (Coates347)
 MODIFIED BY:			Gage Coates (Coates347)
 DATE MODIFIED:			

 FOLDER:				
 DESCRIPTION:			
 NOTE:					N/A
 FILES:					
 IDE/COMPILER:			MicroSoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
	1.		Double click on project2.sln	to OPEN the project
	2.		Press Ctrl+F7					to COMPILE
	3.		Press Ctrl+F5					to EXECUTE
-----------------------------------------------------------------------------------------*/
#include "pch.h"
#include <GL/glut.h>				// include GLUT library
#include "TextEditorWindow.h"


int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	
	// create a generic help window
	Window::Create(std::make_shared<Window>("Help", Vector2(400, 0), Vector2(400, 200), -200, 200, -200, 200, [=]() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);
		// flush out the buffer contents
		glFlush();
	}));

	// create a specialized text-editor window
	Window::Create(std::make_shared<TextEditorWindow>());

	glutMainLoop();
	
	return 0;
}
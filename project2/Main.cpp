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
	auto helpWindow = Window::Create(std::make_shared<Window>("Help", Vector2(400, 0), Vector2(400, 200), -200, 200, -200, 200));
	helpWindow->SetRender([=]() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);
		Vector2 world = helpWindow->ScreenToWorld(Vector2(10,10));
		glPushMatrix();
		glScalef(1.f/50.f,1.f/50.f,1.f/50.f);
		glRasterPos2d(world.x, world.y);
		glColor3d(0, 0, 0);
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'W');
		//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'A');
		// flush out the buffer contents
		glFlush();
		glPopMatrix();
	});

	// create a specialized text-editor window
	auto textEditorWindow = Window::Create(std::make_shared<TextEditorWindow>());
	textEditorWindow->SetRender([]() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);
		// flush out the buffer contents
		glFlush();

	});
	glutMainLoop();
	
	return 0;
}
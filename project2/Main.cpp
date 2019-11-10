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
#include "Bitmap.h"
#include "Window.h"
#include "Font.h"
#include "FontUtil.h"
#include "Color.h"

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	
	
	
	// create a generic help window
	//auto helpWindow = Window::Create(std::make_shared<Window>("Help", Vector2(400, 0), Vector2(400, 200), 0, 1, 0, 1));
	//helpWindow->SetRender([=]() {
	//	// draw the background
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	Vector2 position(100, 100);

	//	FontUtil::Render(*helpWindow, position, "Hello World", "times", 12, Color());
	//	// flush out the buffer contents
	//	glFlush();

	//});

	// create a specialized text-editor window
	auto textEditorWindow = Window::Create(std::make_shared<ote::TextEditorWindow>());
	glutTimerFunc(1000, ote::TextEditorWindow::ToggleCarat, textEditorWindow->GetID());
	glutMainLoop();
	
	return 0;
}
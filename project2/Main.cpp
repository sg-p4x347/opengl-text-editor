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
#include "HelpWindow.h"

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	
	
	
	// create a help window
	Window::Create(std::make_shared<HelpWindow>());


	// create a specialized text-editor window
	Window::Create(std::make_shared<ote::TextEditorWindow>());
	glutMainLoop();
	
	return 0;
}
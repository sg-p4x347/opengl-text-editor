/*-----------------------------------------------------------------------------------------
 COURSE:				CSC525-001: Project 2
 PROGRAMMERS:			Gage Coates		(Coates347)
						Paul Durham		(Durham321)
						Anthony Harris	(bob999)
 Last MODIFIED:			11/10/2019 22:00
 DESCRIPTION:			This is a basic text editing program that supports multiple fonts, 
						colors, and sizes of text. 
						Features include:
							- Context menu 
								- (right click the text editor window)
							- Text coloring 
								- (options available in the context menu)
							- Multiple font sizes 
								- (options available in the context menu)
							- All standard fonts supported by Windows 
								- (options avialable in the context menu)
							- Arbitrary text selection 
								- (click and drag to select sequential characters)
							- Carat positioning 
								- (click to move the carat between characters)
							- Save text to a file 
								- (availible in the context menu)
							- Quit 
								- (avialable in the context menu)
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
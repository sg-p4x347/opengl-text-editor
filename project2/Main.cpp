/*-----------------------------------------------------------------------------------------
 COURSE:				CSC525-001: Project 2
 PROGRAMMERS:			Gage Coates		(Coates347)
							Contributions:	Features 3, 4, 6, 7
							Percentage:		33%
						Paul Durham		(Durham321)
							Contributions:	Feature 1, Core Class Development
							Percentage:		33%
						Anthony Harris	(bob999)
							Contributions:	Features 2, 5, 8, 9
							Percentage:		33%
 Last MODIFIED:			11/10/2019 22:00
 DESCRIPTION:			This is a basic text editing program that supports multiple fonts, 
						colors, and sizes of text. The program uses OpenGL. 
						Features include:
							1) Tabular document display - BONUS
								- (note the tab at the top of the document)
							2) Context menu (also referred to as Main menu)
								- (right click the text editor window)
							3) Text coloring 
								- (options available in the context menu)
							4) Multiple font sizes - BONUS
								- (options available in the context menu)
							5) All standard fonts supported by Windows - BONUS 
								- (options avialable in the context menu)
							6) Arbitrary text selection - BONUS
								- (click and drag to select sequential characters)
							7) Carat positioning 
								- (click to move the carat between characters)
							8) Save text to a file 
								- (availible in the context menu)
							9) Quit 
								- (avialable in the context menu)
 Requirements:			1) Minimum 30 characters per row and 30 rows
							- Fulfilled by using Font Size == 8
						2) User specified color of to-be-entered text (minimum 3 colors)
							- Fulfilled by the color submenu of the context menu
						3) User specified font of to-be-entered text (minimum 3 fonts)
							- Fulfilled by the font submenu of the context menu
						4) Arbitrary positioning of carat
							- Fulfilled by implementation of TextEditorWindow class
						5) Automatic text break into new line when max row space exceeded
							- Fulfilled by implementation of TextEditorWindow class
						6) Enter key moves display position to beginning of the next row
							- Fulfilled by implementation of KeyboardFunc in 
							  TextEditorWindow class
						7) Backspace key erases previously typed character(s)
							- Fulfilled by implementation of KeyboardFunc in
							  TextEditorWindow class
						8) User can save all current text in flat text format in a file
						  defined as C:\Temp\typed.txt
							- Fulfilled by implementation of Save function in Document
							  class
						9) User can terminate the program from menu option
							- Fulfilled by implementation of MainMenuFunc in
							  TextEditorWindow class
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
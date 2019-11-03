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
#include <ft2build.h>
#include "Window.h"
#include FT_FREETYPE_H
FT_Library  freetype;

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	// Initialize freetype
	FT_Error error = FT_Init_FreeType(&freetype);
	if (error)
	{
		return 1;
	}
	
	
	// create a generic help window
	auto helpWindow = Window::Create(std::make_shared<Window>("Help", Vector2(400, 0), Vector2(400, 200), 0, 1, 0, 1));
	helpWindow->SetRender([=]() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);

		FT_Face face;

		auto error = FT_New_Face(freetype,
			"C:\\Windows\\Fonts\\wingding.ttf",
			0,
			&face);
		if (error == FT_Err_Unknown_File_Format)
		{
			cout << "the font file could be openedand read, but it appears that its font format is unsupported";
		}
		else if (error)
		{
			cout << "another error code means that the font file could not be opened or read, or that it is broken";
		}
		FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */
		int           pen_x, pen_y, n;
		pen_x = 300;
		pen_y = 200;
		string text = "This is the FORTE font!";
		for (n = 0; n < text.length(); n++)
		{
			FT_UInt  glyph_index;
			

			/* retrieve glyph index from character code */
			glyph_index = FT_Get_Char_Index(face,128083);
			
			error = FT_Set_Char_Size(
				face,    /* handle to face object           */
				0,       /* char_width in 1/64th of points  */
				16 * 64,   /* char_height in 1/64th of points */
				glutGet(GLUT_WINDOW_WIDTH),     /* horizontal device resolution    */
				glutGet(GLUT_WINDOW_HEIGHT));   /* vertical device resolution      */
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error)
				continue;  /* ignore errors */

			  /* convert to an anti-aliased bitmap */
			error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (error)
				continue;

			/* now, draw to our target surface */
			glColor3f(0.f, 0.f, 0.f);
			
			//glRasterPos2d(pen_x + slot->bitmap_left, pen_y - slot->bitmap_top);
			/*Bitmap test = Bitmap::FromFile("test.bmp");
			glDrawPixels(test.GetWidth(), test.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)test.GetPixels());*/
			/* increment pen position */
			Bitmap bmp(slot->bitmap.width, slot->bitmap.rows);
			for (int x = 0; x < bmp.GetWidth(); x++) {
				for (int y = 0; y < bmp.GetHeight(); y++) {
					uint8_t alpha = slot->bitmap.buffer[y * slot->bitmap.width + x];
					bmp.Set(x, (bmp.GetHeight() - 1) - y, Pixel(0,0,0, alpha));
				}
			}
			Vector2 world = helpWindow->ScreenToWorld(Vector2(pen_x + slot->bitmap_left, (pen_y + slot->bitmap.rows )- slot->bitmap_top));
			glRasterPos2d(world.x, world.y);
			glDrawPixels(bmp.GetWidth(), bmp.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bmp.GetPixels());
			
			pen_x += slot->advance.x >> 6;
			pen_y += slot->advance.y >> 6; /* not useful for now */
		}
		// flush out the buffer contents
		glFlush();

	});

	// create a specialized text-editor window
	auto textEditorWindow = Window::Create(std::make_shared<TextEditorWindow>());
	
	glutMainLoop();
	
	return 0;
}
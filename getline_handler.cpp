// getline.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "getline_handler.h"
#include <iostream>
#include "Windows.h"
#include "conio.h"



int main()
{
	getline_handler glh(32, 8);
	char* something;
	char* input;

	std::cout << "Type something" << std::endl;
	something = glh.getline();

	glh.set_prompt("Type something else: ");
	glh.set_color(getline_handler::color::yellow, true, getline_handler::color::red, false);
	glh.set_color_highlight(getline_handler::color::red, true, getline_handler::color::black, true);
	glh.set_color_prompt(getline_handler::color::yellow, true, getline_handler::color::red, false);
	something = glh.getline();
	

	std::cout << "Press a button to continue . . .";
	_getch();
    return 0;
}



getline_handler::getline_handler(size_t str_max_len, usi history_amount)
	{
	this->history_amount = history_amount;
	this->str_max_len = str_max_len;
	set_prompt(" > ");

	//allocate memory
	this->lines = new char*[history_amount];
	this->line_length = new size_t[history_amount];
	for (usi i = 0; i < history_amount; i++)
		{
		line_length[i] = 0;
		lines[i] = new char[str_max_len+2];
		for (size_t j = 0; j < str_max_len+2; j++)
			{
			lines[i][j] = '\0';
			}
		}

	//set defaults
	ins = false;
	set_color(color::white, false, color::black, false);
	set_color_prompt(color::aqua, false, color::black, false);
	set_color_highlight(color::black, false, color::black, true);
	}


getline_handler::~getline_handler()
	{
	for (size_t i = 0; i < history_amount; i++)
		{
		delete lines[i];
		}
	delete lines;
	delete line_length;
	}

void getline_handler::set_prompt(char* new_prompt)
	{
	prompt = new_prompt;
	prompt_len = strlen(prompt);
	}

/*void getline_handler::set_background_color(color c, bool light)
	{
	color_main = (color)(
		//background
		((c | (light ? (color::light) : (color::dark))) << 4) |
		//foreground
		(color_main & 0b00001111)
		);
	}

void getline_handler::set_foreground_color(color c, bool lght)
	{
	color_main = (color)(
		//background 
		(color_main & 0b11110000) |
		//foreground
		(c | (lght ? (color::light) : (color::dark)))
		);
	}*/

void getline_handler::set_color(color foreground, bool fg_light, color background, bool bg_light)
	{
	color_main = (color)(
		//background
		((background | (bg_light ? (color::light) : (color::dark))) << 4) |
		//foreground
		(foreground | (fg_light ? (color::light) : (color::dark)))
		);
	}

void getline_handler::set_color_prompt(color foreground, bool fg_light, color background, bool bg_light)
	{
	color_prompt = (color)(
		//background
		((background | (bg_light ? (color::light) : (color::dark))) << 4) |
		//foreground
		(foreground | (fg_light ? (color::light) : (color::dark)))
		);
	}

void getline_handler::set_color_highlight(color foreground, bool fg_light, color background, bool bg_light)
	{
	color_highlight = (color)(
		//background
		((background | (bg_light ? (color::light) : (color::dark))) << 4) |
		//foreground
		(foreground | (fg_light ? (color::light) : (color::dark)))
		);
	}

void getline_handler::set_color(color c)
	{
	color_main = c;
	}
void getline_handler::set_color_highlight(color c)
	{
	color_highlight = c;
	}


COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
	{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
		{
		return cbsi.dwCursorPosition;
		}
	else
		{
		// The function failed. Call GetLastError() for details.
		COORD invalid = {0, 0};
		return invalid;
		}
	}

bool getline_handler::clear_highlight()
	{
	if (cursor_x1 != cursor_x2)
		{//text is selected

		highlight_min_x = min(cursor_x1, cursor_x2);
		highlight_max_x = max(cursor_x1, cursor_x2);
		size_t highlight_delta = highlight_max_x - highlight_min_x;

		for (size_t i = highlight_max_x; i < line_length[current_line]; i++)
			{
			line[i - highlight_delta] = line[i];
			}
		line_length[current_line] -= highlight_delta;
		line[line_length[current_line]] = '\0';
		cursor_x1 = highlight_min_x;
		cursor_x2 = cursor_x1;
		return(true);
		}
	return(false);
	}

char * getline_handler::getline()
	{
	COORD old_pos;
	COORD new_pos;

	// save previous state beg
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hstdout, &csbi);
	// Save previous state end

	//Reset input values
	cursor_x1 = 0;
	cursor_x2 = 0;
	current_line = 0;
	line_length[current_line] = 0;
	line = lines[current_line];
	for (size_t i = 0; i < str_max_len; i++)
		{
		line[i] = '\0';
		}

	//draw prompt
	SetConsoleTextAttribute(hstdout, color_prompt);
	std::cout << std::endl << prompt;
	SetConsoleTextAttribute(hstdout, color_main);

	old_pos = GetConsoleCursorPosition(hstdout);
	new_pos = old_pos;
	//Clear line
	new_pos.X = prompt_len;
	SetConsoleCursorPosition(hstdout, new_pos);

	for (size_t i = 0; i < str_max_len; i++)
		{
		std::cout << ' ';
		}

	SetConsoleCursorPosition(hstdout, new_pos);

	//main cycle
	char c;
	//while (true){c = _getch();std::cout << (int)c << " ";}
	while (true)
		{
		//get input
		c = _getch();
		switch (c)
			{
				case 13: //enter
					goto END;
				case '\b': //backspace
					if (!clear_highlight())
						{
						if (cursor_x1 > 0)
							{
							cursor_x1--;
							cursor_x2 = cursor_x1;
							for (size_t i = cursor_x1; i < line_length[current_line]; i++)//not "line_length[current_line]-1" because this way we copy the '\0' as well
								{//move all characters left one position
								line[i] = line[i + 1];
								}
							line_length[current_line]--;
							}
						else
							{
							std::cout << '\a';
							}
						}
					break;
				case -32:
					c = _getch();
					switch (c)
						{
							case 83: //canc
								if (!clear_highlight())
									{
									if (cursor_x1 < line_length[current_line])
										{
										for (size_t i = cursor_x1; i < line_length[current_line]; i++)//not "line_length[current_line]-1" because this way we copy the '\0' as well
											{//move all characters left one position
											line[i] = line[i + 1];
											}
										line_length[current_line]--;
										}
									else
										{
										std::cout << '\a';
										}
									}
								break;
							case 72: //up
							case 80: //down
								break;

							case 75: //left
								if (cursor_x1 > 0)
									{
									cursor_x1--;
									}
								else
									{
									std::cout << '\a';
									}
								if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
									{//without shift
									cursor_x2 = cursor_x1;
									}
								break;

							case 77: //right
								if (cursor_x1 < line_length[current_line])
									{
									cursor_x1++;
									}
								else
									{
									std::cout << '\a';
									}
								if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
									{//without shift
									cursor_x2 = cursor_x1;
									}
								break;
						}
					break;
				//Other
				case 'q':case 'w':case 'e':case 'r':case 't':case 'y':case 'u':case 'i'://chars
				case 'o':case 'p':case 'a':case 's':case 'd':case 'f':case 'g':case 'h':
				case 'j':case 'k':case 'l':case 'z':case 'x':case 'c':case 'v':case 'b':
				case 'n':case 'm':case 'Q':case 'W':case 'E':case 'R':case 'T':case 'Y':
				case 'U':case 'I':case 'O':case 'P':case 'A':case 'S':case 'D':case 'F':
				case 'G':case 'H':case 'J':case 'K':case 'L':case 'Z':case 'X':case 'C':
				case 'V':case 'B':case 'N':case 'M':
				case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7'://numbers
				case '8':case '9':
				case '(':case ')':case '[':case ']':case '{':case '}':case '<':case '>'://parenthesis
				case '.':case ',':case ':':case ';':case '!':case '?':case '\'':case '"'://punctuation
				case '/':case '*':case '-':case '+':case '%':case '^':case '='://operations
				case '\\':case '|':case '&':case '$':case '_':case '~':	//other

					clear_highlight();
					if (!ins)
						{//type
						if (line_length[current_line] < str_max_len)//can still add a char at [str_max_len-2] because we don't need the \0. There's always a \0 at [str_max_len], since allocated string is long [str_max_len+1]
							{
							if (cursor_x1 < line_length[current_line])
								{
								for (size_t i = line_length[current_line]; i >= cursor_x1; i--)
									{//move all characters right one position
									line[i + 1] = line[i];
									}
								}
							line[cursor_x1] = c;
							cursor_x1++;
							cursor_x2 = cursor_x1;
							line_length[current_line]++;
							}
						else
							{
							std::cout << "\a";
							}
						}

			}

		//Draw
		old_pos = GetConsoleCursorPosition(hstdout);
		new_pos = old_pos;
		//Clear line
		new_pos.X = prompt_len;
		SetConsoleCursorPosition(hstdout, new_pos);

		for (size_t i = 0; i < str_max_len; i++)
			{
			std::cout << ' ';
			}

		SetConsoleCursorPosition(hstdout, new_pos);
		if (cursor_x1 != cursor_x2)
			{
			//calc highlight
			highlight_min_x = min(cursor_x1, cursor_x2);
			highlight_max_x = max(cursor_x1, cursor_x2);

			//draw highlight
			SetConsoleTextAttribute(hstdout, color_main);
			for (size_t i = 0; i < highlight_min_x; i++)
				{
				std::cout << line[i];
				}

			SetConsoleTextAttribute(hstdout, color_highlight);
			for (size_t i = highlight_min_x; i < highlight_max_x; i++)
				{
				std::cout << line[i];
				}

			SetConsoleTextAttribute(hstdout, color_main);
			for (size_t i = highlight_max_x; i < line_length[current_line]; i++)
				{
				std::cout << line[i];
				}
			}
		else
			{
			SetConsoleTextAttribute(hstdout, color_main);
			std::cout << line;
			}
		std::cout << std::flush;

#ifdef _DEBUG
		//debug coords
		new_pos.X = prompt_len;
		new_pos.Y = old_pos.Y+1;
		SetConsoleCursorPosition(hstdout, new_pos);
		std::cout << "Cursor at [" << cursor_x1 << ", " << cursor_x2 << "], line length = " << line_length[current_line] << "/" << str_max_len << " at line " << current_line;

		//debug string
		new_pos.X = prompt_len;
		new_pos.Y = old_pos.Y + 2;
		SetConsoleCursorPosition(hstdout, new_pos);
		for (size_t i = 0; i < str_max_len+1; i++)
			{
			if (line[i] != '\0')
				{
				std::cout << line[i];
				}
			else
				{
				std::cout << "\\";
				}
			}
#endif


		new_pos.Y = old_pos.Y;
		new_pos.X = prompt_len + cursor_x1;
		SetConsoleCursorPosition(hstdout, new_pos);
		//Draw end
		}
END:


	//main cycle end

	char* ret = new char[line_length[current_line]+1];
	for (size_t i = 0; i < line_length[current_line]+1; i++)
		{
		ret[i] = line[i];

		}

	// recover previous state beg
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
	// recover previous state end
	old_pos = GetConsoleCursorPosition(hstdout);
	old_pos.X = 0;
#ifdef _DEBUG
	old_pos.Y+=3;
#else
	old_pos.Y++;
#endif
	SetConsoleCursorPosition(hstdout, old_pos);

	return ret;
	}
#pragma once

#include "Windows.h"
#include "conio.h"
#define usi unsigned short int

class getline_handler
	{
	public:
		enum color : unsigned char //unsigned short
			{
			black = 0b00000000,
			blue = 0b00000001,
			green = 0b00000010,
			red = 0b00000100,
			white = 0b00001111,
			aqua = blue | green,
			cyan = blue | green,
			purple = blue | red,
			yellow = green | red,
			light = 0b00001000,
			dark = 0b00000000
			};

	private:
		//memory
		char** lines;
		size_t* line_length;
		size_t str_max_len;
		usi history_amount;
		size_t prompt_len;
		
		//style
		char* prompt;
		color color_main;
		color color_highlight;
		color color_prompt;
		
		//draw
		void erase_output_line();

		//editing
		void erase_line(usi line_index);

		//usage
		size_t cursor_x1;
		size_t cursor_x2;
		size_t highlight_max_x;
		size_t highlight_min_x;
		usi current_line;
		usi active_lines;
		char* line;
		bool ins;
		bool clear_highlight();
		void clear_line(COORD& new_pos, HANDLE& hstdout);
		void new_line();

	public:

		//style
		bool sound;
		void set_prompt(char* new_prompt);
		void set_color(color foreground, bool fg_light, color background, bool bg_light);
		void set_color_highlight(color foreground, bool fg_light, color background, bool bg_light);
		void set_color_prompt(color foreground, bool fg_light, color background, bool bg_light);
		void set_color(color c);
		void set_color_highlight(color c);
		void set_color_prompt(color c);

		//functions
		getline_handler();
		getline_handler(size_t str_max_len, usi history_amount);
		getline_handler(size_t str_max_len, usi history_amount, bool sound);
		~getline_handler();

		char* getline();

	};


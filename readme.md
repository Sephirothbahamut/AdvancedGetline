# Advanced Getline
Advanced getline is a class made to have an input which behavest like a textbox.

## Authors:
Sephirothbahamut

## Targeted OSs
   - [x] **Windows** (current version)
   - [ ] **Linux** (planned)
   - [ ] **MAC** (not planned, Linux version could possibly work on mac)
   
## Features
Once the class is instantiated, you will be able to call its main function "getline" which will return a newly allocated string.
Other functions allow you to customize the look, changing colors or the prompt string.
   - [x] Type text (yeah, this too)
   - [x] [Enter] to close the input (of course)
   - [x] Move the cursor with [Left] and [Right] arrows.
   - [x] Move the cursor with [Home/Begin] and [End] keys.
   - [x] Highlight the text with [Shift]+[Left] and [Shift]+[Right].
   - [x] Type replacing highlighted text
   - [x] [Del] and [Backspace] delete highlighted text.
   - [x] [Del] and [Backspace] without highlighted text behave as you expect them to.
   - [x] [Up] and [Down] arrows allow navigation in previous input lines (ready to be completed)
   - [ ] [Ctrl]+[X], [Ctrl]+[V] to cut highlighted text (copying to actual clipboard) and paste.
   - [ ] Mouse cursor interaction (far far future, in a galaxy far away... possibly... maybe... unlikely)
   - [x] Sound system alert when the cursor is trying to go out of bounds.
   
## About **Linux** version
The current high **Windows** dependency should not scare. This is an evolution of an "advanced getline" i started on linux, so i already have much of the code ready. But expect a lot of OS related "#ifdef/#endif"s with the **Windows**-**Linux** version.

## Methods
Note: the .cpp file already contains a main method with a working usage example;
```
getline_handler(size_t str_max_len, usi history_amount, bool sound)
```
The constructor, it takes the maximum length for input strings (str_max_len, the amount of lines it must keep in memory to make them accessible on future calls via [Up] and [Down] arrows (set it to 1 if in doubt), and a boolean. If sound is true the user will hear a system alert sound when the cursor is going out of bounds.

```
getline_handler(size_t str_max_len, usi history_amount)
```
Like the previous one, sound is true by default.
```
getline_handler()
```
Simpliest constructor, it initializes all the settings to a certain default:
max string length: 128, history amount: 10, sound: true


```
char* getline()
```
The core of everything, call this function to have a fancy textbox in your console.
NOTE: the function will return a dynamically allocated array of characters. Its length will be the exact string length plus the terminating character. Remember to delete it when it's not needed!!!

### The following methods are used only for graphics
```
void set_prompt(char* )
```
Sets the argument as prompt. By default the prompt is " > ".


```
void set_color(color foreground, bool fg_light, color background, bool bg_light)
void set_color_highlight(color foreground, bool fg_light, color background, bool bg_light)
void set_color_prompt(color foreground, bool fg_light, color background, bool bg_light)
```
These 3 will set the colors of your textbox. The "light" boolean will make a color either light or dark. The colors must belong to class's own enum "color".
The available colors are:
   - White
   - Black
   - Red
   - Green
   - Blue
   - Yellow
   - Cyan/Aqua
   - Purple
   
The following are the default colors:
```
set_color(color::white, false, color::black, false);	silver on black text
set_color_prompt(color::aqua, false, color::black, false);  aqua on black prompt
set_color_highlight(color::black, false, color::black, true);  black on dark gray highlight
```
## Known bugs

## Solved bugs
When selecting from the rightmost position and deleting highlighted content, a bug prevents the line from clearing. The input will still be correct, despite the prompt showing more characters than needed. Launching the process in debug mode will still show the actuall string down below.

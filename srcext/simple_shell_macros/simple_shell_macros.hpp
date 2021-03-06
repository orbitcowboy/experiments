/*! \file simple_shell_macros.hpp
*   \brief Here are a lot of useful macros to modify the behaviour
*          of the Linux/Unix shell (tested with ksh/bash).
*          The major features are:
*          - colourize console output
*          - change font attributes e.g. bold, underlined
*          - move the cursor position to specific coordinates
*          - reset to default
*          - scroll down/up
*
*   @todo: Adapt to C (currently only C++ is supported)
*   @todo: Add Windows console support
*
*   References:
*       - HTTP://ascii-table.com/ansi-escape-sequences-vt-100.php
*
*/

#ifndef SIMPLE_SHELL_MACROS_HPP
#define SIMPLE_SHELL_MACROS_HPP

#ifdef WIN32
#include <iostream>
#include <windows.h>
using namespace std;
#endif


// --------
// colours:
// --------
#define RED_COLOUR              "\033[22;31m"
#define GREEN_COLOUR            "\033[22;32m"
#define BROWN_COLOUR            "\033[22;33m"
#define BLUE_COLOUR             "\033[22;34m"
#define MAGENTA_COLOUR          "\033[22;35m"
#define CYAN_COLOUR             "\033[22;36m"
#define GRAY_COLOUR             "\033[22;37m"
#define DARK_GREY_COLOUR        "\033[01;30m"
#define LIGHT_RED_COLOUR        "\033[01;31m"
#define LIGHT_GREEN_COLOUR      "\033[01;32m"
#define YELLOW_COLOUR           "\033[01;33m"
#define LIGHT_BLUE_COLOUR       "\033[01;34m"
#define LIGHT_MAGENTA_COLOUR    "\033[01;35m"
#define LIGHT_CYAN_COLOUR       "\033[01;36m"
#define WHITE_COLOUR            "\033[01;37m"
#define BLACK_COLOUR            "\033[22;30m"
#define DEFAULT_COLOUR          "\033[22;39m"

// -------------------------------------------------------------------------------------
/// Colourize a specific string
///
/// \param msg --> the string to be colourized
// -------------------------------------------------------------------------------------
#define COULOURIZE_RED(msg)              RED_COLOUR             << msg << DEFAULT_COLOUR
#define COULOURIZE_GREEN(msg)            GREEN_COLOUR           << msg << DEFAULT_COLOUR
#define COULOURIZE_BROWN(msg)            BROWN_COLOUR           << msg << DEFAULT_COLOUR
#define COULOURIZE_BLUE(msg)             BLUE_COLOUR            << msg << DEFAULT_COLOUR
#define COULOURIZE_MAGENTA(msg)          MAGENTA_COLOUR         << msg << DEFAULT_COLOUR
#define COULOURIZE_CYAN(msg)             CYAN_COLOUR            << msg << DEFAULT_COLOUR
#define COULOURIZE_GRAY(msg)             GRAY_COLOUR            << msg << DEFAULT_COLOUR
#define COULOURIZE_DARK_GRAY(msg)        DARK_GREY_COLOUR       << msg << DEFAULT_COLOUR
#define COULOURIZE_LIGHT_RED(msg)        LIGHT_RED_COLOUR       << msg << DEFAULT_COLOUR
#define COULOURIZE_LIGHT_GREEN(msg)      LIGHT_GREEN_COLOUR     << msg << DEFAULT_COLOUR
#define COULOURIZE_YELLOW(msg)           YELLOW_COLOUR          << msg << DEFAULT_COLOUR
#define COULOURIZE_LIGHT_BLUE(msg)       LIGHT_BLUE_COLOUR      << msg << DEFAULT_COLOUR
#define COULOURIZE_LIGHT_MAGENTA(msg)    LIGHT_MAGENTA_COLOUR   << msg << DEFAULT_COLOUR
#define COULOURIZE_LIGHT_CYAN(msg)       LIGHT_CYAN_COLOUR      << msg << DEFAULT_COLOUR
#define COULOURIZE_WHITE(msg)            WHITE_COLOUR           << msg << DEFAULT_COLOUR
#define COULOURIZE_BLACK(msg)            BLACK_COLOUR           << msg << DEFAULT_COLOUR

/*
// -----------------------------------------------------------------------------------------
/// Send a message with a specific colour (c++-version)
///
/// \param msg --> the message
// -----------------------------------------------------------------------------------------
#ifdef WIN32
#define COUT_RED(msg)\
{\
    HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);\
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);\
    std::cout << msg;\
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );\
}
#else // POSIX */
#define COUT_RED(msg)           std::cout << COULOURIZE_RED(msg)
#define COUT_GREEN(msg)         std::cout << COULOURIZE_GREEN(msg)
#define COUT_BROWN(msg)         std::cout << COULOURIZE_BROWN(msg)
#define COUT_BLUE(msg)          std::cout << COULOURIZE_BLUE(msg)
#define COUT_MAGENTA(msg)       std::cout << COULOURIZE_MAGENTA(msg)
#define COUT_CYAN(msg)          std::cout << COULOURIZE_CYAN(msg)
#define COUT_GRAY(msg)          std::cout << COULOURIZE_GRAY(msg)
#define COUT_DARK_GRAY(msg)     std::cout << COULOURIZE_DARK_GRAY(msg)
#define COUT_LIGHT_RED(msg)     std::cout << COULOURIZE_LIGHT_RED(msg)
#define COUT_LIGHT_GREEN(msg)   std::cout << COULOURIZE_LIGHT_GREEN(msg)
#define COUT_YELLOW(msg)        std::cout << COULOURIZE_YELLOW(msg)
#define COUT_LIGHT_BLUE(msg)    std::cout << COULOURIZE_LIGHT_BLUE(msg)
#define COUT_LIGHT_MAGENTA(msg) std::cout << COULOURIZE_LIGHT_MAGENTA(msg)
#define COUT_LIGHT_CYAN(msg)    std::cout << COULOURIZE_LIGHT_CYAN(msg)
#define COUT_WHITE(msg)         std::cout << COULOURIZE_WHITE(msg)
#define COUT_BLACK(msg)         std::cout << COULOURIZE_BLACK(msg)
//#endif
// -------------------
/// font attributes  :
//--------------------
#define FONT_BOLD              "\33[1m"
#define FONT_ITALIC            "\33[3m"
#define FONT_UNDERLINED        "\33[4m"
#define FONT_REVERSE           "\33[7m"
#define FONT_STRIKED           "\33[9m"
#define FONT_DEFAULT           "\33[0m"

// -------------------------------------------------------------------
/// Send a message with a specific attribute e.g. bold etc.
///
/// \param msg --> the message
// -------------------------------------------------------------------
#define BOLD( msg )        FONT_BOLD       << msg << FONT_DEFAULT
#define ITALIC(msg)        FONT_ITALIC     << msg << FONT_DEFAULT
#define UNDERLINED(msg)    FONT_UNDERLINED << msg << FONT_DEFAULT
#define REVERSE(msg)       FONT_REVERSE    << msg << FONT_DEFAULT
#define STRIKED( msg )     FONT_STRIKED    << msg << FONT_DEFAULT


// -------------------------------------------------------------------
/// Cout a message with a specific attribute e.g. bold etc.
///
/// \param msg --> the message
// -------------------------------------------------------------------
#define COUT_BOLD( msg )     std::cout << FONT_BOLD         << msg << FONT_DEFAULT
#define COUT_ITALIC(msg)     std::cout << FONT_ITALIC       << msg << FONT_DEFAULT
#define COUT_UNDERLINED(msg) std::cout << FONT_UNDERLINED   << msg << FONT_DEFAULT
#define COUT_REVERSE(msg)    std::cout << FONT_REVERSE      << msg << FONT_DEFAULT
#define COUT_STRIKED( msg )  std::cout << FONT_STRIKED      << msg << FONT_DEFAULT

// -----------------------------------------------
/// clear the screen
// -----------------------------------------------
#ifdef WIN32
#define CLEAR_SCREEN  std::system("cls");
#else // POSIX
#define CLEAR_SCREEN  std::cout << "\33[2J"
#endif
// -----------------------------------------------
/// go to a specific position
///
/// \param x --> the x position
/// \param y --> the y position
// -----------------------------------------------
#define GOTO       ( x, y  ) std::cout << "\33[" << y << ";" << x << "H"

// -----------------------------------------------
/// move to a specific position
///
/// \param x --> the x position
/// \param y --> the y position
// -----------------------------------------------
#define NOVE_TO    ( y, x  ) std::cout << "\33[" << y << ";" << x << "f"

// -----------------------------------------------
/// insert a line
/// \param count --> the number of lines
// -----------------------------------------------
#define INSERT_LINE( count ) std::cout << "\33[" << count << "L"

// -----------------------------------------------
/// clear the current line
// -----------------------------------------------
#define CLEAR_LINE    std::cout << "\33[2K"

// -----------------------------------------------
/// delete the current line
/// \param count --> the number of lines
// -----------------------------------------------
#define DELETE_LINE( count ) std::cout << "\33[" << count << "M"

// -----------------------------------------------
/// insert a character
/// \param count --> the number of characters
// -----------------------------------------------
#define INSERT_CHAR( count ) std::cout << "\33[" << count << "@"
// -----------------------------------------------
/// delete a character
/// \param count --> the number of characters
// -----------------------------------------------
#define DELETE_CHAR( count ) std::cout << "\33[" << count << "P"

// -----------------------------------------------
/// scroll up
/// \param count --> the number of lines
// -----------------------------------------------
#define SCROLL_UP(   count ) std::cout << "\33[" << count << "S"

// -----------------------------------------------
/// scroll down
/// \param count --> the number of lines
// -----------------------------------------------
#define SCROLL_DOWN( count ) std::cout << "\33[" << count << "T"

// ----------------------------------------------------------------
/// reset the whole screen, this means, revert all to default
// ----------------------------------------------------------------
#define RESET_SCREEN  std::cout << "\33c"

// -------------------------------------------
// settings, affecting the cursor position :
// -------------------------------------------
// -----------------------------------------------
/// cursor up
/// \param count --> the number of lines
// -----------------------------------------------
#define CURSOR_UP(    count ) std::cout << "\33[" << count << "A"

// -----------------------------------------------
/// cursor down
/// \param count --> the number of lines
// -----------------------------------------------
#define CURSOR_DOWN(  count ) std::cout << "\33[" << count << "B"

// -----------------------------------------------
/// cursor right
/// \param count --> the number of columns
// -----------------------------------------------
#define CURSOR_RIGHT( count ) std::cout << "\33[" << count << "C"

// -----------------------------------------------
/// cursor left
/// \param count --> the number of columns
// -----------------------------------------------
#define CURSOR_LEFT(  count ) std::cout << "\33[" << count << "D"


#endif // SIMPLE_SHELL_MACROS_HPP

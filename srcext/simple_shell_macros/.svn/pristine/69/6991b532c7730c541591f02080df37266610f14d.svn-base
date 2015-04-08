#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "simple_shell_macros.hpp"

using namespace std;

int main(void)
{
    /*
    #include <iostream>
    #include <windows.h>

    using namespace std;

    int main()
    {

        HANDLE hConsole;
        hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

    // Use the three primary colors for mixing any other color.
    // Use FOREGROUND_INTENSITY for brighter colors.
        SetConsoleTextAttribute
        (hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Bright red text\n";

        SetConsoleTextAttribute
        (hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "Bright blue text\n";

        SetConsoleTextAttribute
        (hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
        cout << "Back to normal white text\n";*/

#if WIN32
    COUT_RED       ("normal red 	    ") ;
#else

    CLEAR_SCREEN;

    COUT_BLACK(BOLD("bold black	        "))     << std::endl;
    COUT_BLACK(BOLD(UNDERLINED(("bold underlined black"))))     << std::endl;
    COUT_BLACK     ("normal black 	    ")       << std::endl;
    COUT_RED(ITALIC("italic red 	    "))    << std::endl;
    COUT_RED       ("normal red 	    ")         << std::endl;
    COUT_GREEN     ("normal green	    ")        << std::endl;
    COUT_GREEN(REVERSE("reverse green	"))     << std::endl;
    COUT_BROWN    ("brown	    		")      << std::endl;
    COUT_BROWN(UNDERLINED("underlined brown"))  << std::endl;
    COUT_BLUE(STRIKED("normal blue     "))      << std::endl;
    COUT_BLUE("striked blue     ")              << std::endl;
    COUT_MAGENTA        ("magenta  ")       << std::endl;
    COUT_CYAN           ("cyan     ")       << std::endl;
    COUT_GRAY           ("gray     ")       << std::endl;
    COUT_DARK_GRAY      ("dark gray")       << std::endl;
    COUT_LIGHT_RED      ("light red")       << std::endl;
    COUT_LIGHT_GREEN    ("light green")     << std::endl;
    COUT_YELLOW         ("yellow	 ")    << std::endl;
    COUT_LIGHT_BLUE     ("light blue ")     << std::endl;
    COUT_LIGHT_MAGENTA  ("light magenta")   << std::endl;
    COUT_LIGHT_CYAN     ("light cyan   ")   << std::endl;
    COUT_WHITE          ("white 	   ")  << std::endl;

    std::cout << FONT_BOLD << FONT_ITALIC << RED_COLOUR << "Fetter, kursiver, roter text" << DEFAULT_COLOUR << FONT_DEFAULT << std::endl;

    CLEAR_LINE;

    CURSOR_UP(30);

    //SCROLL_DOWN(10);

    //SCROLL_UP(11);

    //GOTO(10,-20);
    //RESET_SCREEN;

#endif
    return(0);
}

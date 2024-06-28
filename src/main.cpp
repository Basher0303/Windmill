#include <iostream>
#include <cmath>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "colors.h"
#include "keys.h"
#include "game.h"


int SelectItemMenu = 0;

void PrintMenu()
{
    std::cout << "\x1B[2J\x1B[H";
    std::cout << "\n\tВыберете уровень бота:\n";
    std::cout << (SelectItemMenu == 0 ? "->  " : "   ") << "Легкий\n";
    std::cout << (SelectItemMenu == 1 ? "->  " : "   ") << "Средний\n";
    std::cout << (SelectItemMenu == 2 ? "->  " : "   ") << "Сложный\n";
}

int Controller()
{
    PrintMenu();
    while (true)
    {
        struct termios oldt,
        newt;
        int ch;
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
        if(ch == KEY_W)
        {
            if(SelectItemMenu > 0)
            {
                SelectItemMenu -= 1;
                PrintMenu();
            }
        }
        else if(ch == KEY_S)
        {
            if(SelectItemMenu < 2)
            {
                SelectItemMenu += 1;
                PrintMenu();
            }
        }

        else if(ch == KEY_SPACE)
        {
            return SelectItemMenu;
        }      
    }
    return -1;
}

int main()
{
    int botLevel = Controller();

    Game game(botLevel);
    game.Start();

    return 1;
}

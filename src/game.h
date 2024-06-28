#pragma once
#include "colors.h"
#include "keys.h"
#include "enums.h"
#include "field.h"
#include "bot.h"
#include "human.h"

class Game
{
private:
    Field field;
    Human* playerBlue;
    Bot* playerRed;
    StageGame stageGame;


    //Проверка на сделанную мельинцу
    //Аргументы: 1)Позиция шашки 2)Тип(цвет) игрока
    bool CheckForMadeMill(int pos, TypePlayer typePlayer)
    {
        if(typePlayer == TypePlayer::bluePlayer)
        {
            if(field.CountLinesCapturedBlue(pos) > 0)
                return true;
        }
        else
        {
            if(field.CountLinesCapturedRed(pos) > 0)
                return true;
        }

        return false;
    }

    //Вывод подсказки
    void PrintPrompt(TypePlayer typePlayerWalks,  Action action)
    {
        std::cout << "\033[3B\033[25D";
        if(typePlayerWalks == TypePlayer::bluePlayer)
            std::cout << CLR_BLUE << "Ход синих: ";
        else
            std::cout << CLR_RED << "Ход красных: ";

        std::cout << CLR_WHITE;
        if(action == Action::addition)
            std::cout << "Поставьте фишку.";
        else if(action == Action::deletion)
            std::cout << "Уберите вражескую фишку.";
        else if(action == Action::selection)
            std::cout << "Выберите фишку для перемещения.";
        else if(action == Action::moving)
            std::cout << "Выберите куда хотите переместить фишку.";

        std::cout << "\n";
        //std::cout << "Вызовы оценочной функции: " << playerRed->countFunc << " раз.\n";
    }

    //Вывод количества шашек
    void PrintNumsCheckers()
    {
        std::cout << "\033[14A\033[25C" << CLR_BLUE << "У синего игрока:\t"<< CLR_WHITE << field.CountBlueCheckers() << " шашек\n";
        std::cout << "\033[25C" << CLR_RED << "У красного игрока:\t"<< CLR_WHITE << field.CountRedCheckers() << " шашек\n";
        std::cout << "\033[25C";
    }


    //Вывод управления игры
    void PrintControl()
    {
        std::cout << "\033[5B\033[25C" << "\tУправление:\n";
        std::cout << "\033[25C" << "Перемещение курсора: - W, A, S, D (Английская раскладка клавиатуры)\n";
        std::cout << "\033[25C" << "Выделить шашку: - Space\n";
        std::cout << "\033[25C" << "Снять выделение с шашки: - Esc\n";
        std::cout << "\033[25C" << "Переместить шашку: - Space\n";
        std::cout << "\033[25C" << "Убрать шашку: - Space\n";
    }

    //Вывод дисплея игры
    void PrintDisplay(TypePlayer typePlayerWalks,  Action action)
    {
        field.Print();  
        PrintNumsCheckers();
        PrintControl();
        PrintPrompt(typePlayerWalks, action);
    }

public:
    Game(int botLevel)
    {
        playerBlue = new Human(&field, TypePlayer::bluePlayer);
        playerRed = new Bot(&field, TypePlayer::redPlayer, botLevel);
    }
    ~Game()
    {

    };


    //Начало игры
    void Start()
    {

        //Стадия добавления шашек
        for (int i = 0; i < 9; i++)
        {
            //Пока игрок не добавит шашку
            do
            {
                PrintDisplay(playerBlue->GetTypePlayer(), Action::addition);
            } while (playerBlue->AddChecker() == 0);
            
            if(CheckForMadeMill(field.Cursor.GetPosition(), playerBlue->GetTypePlayer()) == true)
            {
                //Пока игрок не удалит шашку
                do
                {
                    PrintDisplay(playerBlue->GetTypePlayer(), Action::deletion);
                } while (playerBlue->DestroyChecker() == 0);
                
            } 

            int addPos = playerRed->AddChecker();
            if(CheckForMadeMill(addPos, playerRed->GetTypePlayer()) == true)
            {
                addPos = playerRed->DestroyChecker();
            } 
            field.Cursor.SetPosition(addPos);
            PrintDisplay(playerRed->GetTypePlayer(), Action::addition);
        }

        //Стадия перемещения шашек
        stageGame = StageGame::stageMovements;
        while(true)
        {
            bool selected = false;
            bool moved = false;

            PrintDisplay(playerBlue->GetTypePlayer(), Action::selection);
            //Пока игрок не передвинет шашку
            while (moved == false)
            {
                //Пока игрок не выделит шашку
                if(selected == false)
                {
                    selected = (playerBlue->SelectChecker() == 1 ? true : false);
                    PrintDisplay(playerBlue->GetTypePlayer(), Action::selection);
                }
                else
                {
                    PrintDisplay(playerBlue->GetTypePlayer(), Action::moving);

                    int res = playerBlue->MoveChecker();
                    if(res == 1)
                    {
                        moved = true;
                    }
                    else if(res == -1)
                    {
                        selected = false;
                        PrintDisplay(playerBlue->GetTypePlayer(), Action::selection);
                    }
                    else
                        PrintDisplay(playerBlue->GetTypePlayer(), Action::moving);
                }
            }
            
            if(CheckForMadeMill(field.Cursor.GetPosition(), playerBlue->GetTypePlayer()) == true)
            {
                //Пока игрок не удалит шашку
                do
                {
                    PrintDisplay(playerBlue->GetTypePlayer(), Action::deletion);
                } while (playerBlue->DestroyChecker() == false);
                
            } 


            int movePos = playerRed->MoveChecker();
            if(CheckForMadeMill(movePos, playerRed->GetTypePlayer()) == true)
            {
                movePos = playerRed->DestroyChecker();
            } 
            field.Cursor.SetPosition(movePos);
            PrintDisplay(playerRed->GetTypePlayer(), Action::moving);


            //Конец игры
            if(field.CountBlueCheckers() < 3 || field.CountMovesBlue() == 0)
            {
                std::cout << CLR_RED << "\n━━━━━━━━━━━━━┫Конец игры┣━━━━━━━━━━━━━\n";
                std::cout << "Победил красный игрок.\n\n\n" << CLR_WHITE;
                exit(1);        
            }
            else if(field.CountRedCheckers() < 3 || field.CountMovesRed() == 0)
            {
                std::cout << CLR_BLUE << "\n━━━━━━━━━━━━━┫Конец игры┣━━━━━━━━━━━━━\n";
                std::cout << "Победил синий игрок.\n\n\n" << CLR_WHITE;
                exit(1);          
            }

        }     
    }
};



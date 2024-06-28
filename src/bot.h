#pragma once

class Bot
{
private:
    //Тип(цвет) игрока
    TypePlayer typePlayer;
    //Тип(цвет) шашек
    Field::PointStatus pointStatus;
    Field* field;

    //Уровени сложности
    const int levelsDifficulty[3][2] = 
    {
        {1, 1},
        {2, 2},
        {4, 3},
    };

    int maxRecursiveLevel = 0;
    int maxRecursiveLevelAdding = 0;

    //Оценочная функция
    int EvaluationFunction()
    {
        int res = 0;
        for (size_t i = 0; i < 16; i++)
        {
            res += pow(field->Lines[i].CountRedСheckers(), 3);
            res -= pow(field->Lines[i].CountBlueСheckers(), 3);
        }
        countFunc += 1;
        return res;
    }

    //Оценочная функция добавления шашки
    int EvaluationFunctionAdd()
    {
        int res = 0;
        for (size_t i = 0; i < 16; i++)
        {
            res += field->CountMovesRed();
            res -= field->CountMovesBlue();
            res += pow(field->Lines[i].CountRedСheckers(), 3);
            res -= pow(field->Lines[i].CountBlueСheckers(), 3);
        }
        
        return res;
    }

    //МинниМакс перевдижения шашки.
    //Аргументы: 1)Поле 2)Тип игрока 3)Уровень рекурсии 4)Позиция лучшей шашки для перевдижения 
    //           5)Новая поцизия лучшей шашки для передвижения
    int MinMax(TypePlayer typePlayer, int recursiveLevel, int& movePos1, int& movePos2, int alpha = -10000, int beta = 10000)
    {
        //На последнем уровне дерева
        if (recursiveLevel >= maxRecursiveLevel * 2)
        {
            return EvaluationFunction();
        }

        bool firstTest = true;
        int test = -10000;

        //Лучшая шашка для хода
        int bestCheckers;
        //Лучшая позиция хода
        int bestMovePos;

        int minMax = (typePlayer == TypePlayer::redPlayer ? -10000 : 10000);

        //Перебираем все шашки данного игрока
        for (int i = 0; i < 24; i++)
        {
            if((*field).Points[i].GetStatus() == (typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue))
            {
                //Выбираем все возмоные ходы данной шашки
                int freeMovePos = -1;
                for (int j = 0; j < 4; j++)
                {
                    if(j == 0)
                        freeMovePos = (*field).Points[i].Contacts.indexPointTop;
                    else if(j == 1)
                        freeMovePos = (*field).Points[i].Contacts.indexPointBottom;
                    else if(j == 2)
                        freeMovePos = (*field).Points[i].Contacts.indexPointLeft;
                    else if(j == 3)
                        freeMovePos = (*field).Points[i].Contacts.indexPointRight;

                    if(freeMovePos != -1 && (*field).IsPointFree(freeMovePos))
                    {
                        //Ходим в свободную точку
                        (*field).MoveСhecker(i, freeMovePos, typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue);

                        //Оцениваем поле после хода
                        //Если после хода собралась мельница
                        if(typePlayer == TypePlayer::bluePlayer ? field->CountLinesCapturedBlue(freeMovePos) : field->CountLinesCapturedRed(freeMovePos) > 0)
                        {
                            int deletePos = -1;
                            test = MinMaxDestroy(typePlayer == TypePlayer::redPlayer ? TypePlayer::redPlayer : TypePlayer::bluePlayer, deletePos);
                        }
                        else
                        {
                            test = MinMax(typePlayer == TypePlayer::redPlayer ? TypePlayer::bluePlayer : TypePlayer::redPlayer, recursiveLevel + 1, movePos1, movePos2, alpha, beta);
                        }

                        firstTest = false;

                        //Восстанавливаем исходное состояние поля
                        (*field).MoveСhecker(freeMovePos, i, typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue);

                        //Если ход лучше тех, что были до него
                        if( (test > minMax && typePlayer == TypePlayer::redPlayer) || (test < minMax && typePlayer == TypePlayer::bluePlayer) || firstTest == true)
                        {
                            minMax = test;
                            bestCheckers = i;
                            bestMovePos = freeMovePos;
                        }

                        if(typePlayer == TypePlayer::redPlayer)
                            alpha = std::max(alpha, test);
                        else
                            beta = std::min(beta, test);

                        if(beta < alpha)
                            break;                        
                    }
                }
            }
        }

        //Если некуда ходить
        if(firstTest == true)
        {
            return EvaluationFunction();
        }

        //Записать лучший вариант хода
        if(recursiveLevel == 0 && firstTest == false)
        {
            movePos1 = bestCheckers;
            movePos2 = bestMovePos;
        }

        return minMax;
    }

    //МинниМакс для добавления шашки
    //Аргументы: 1)Поле 2)Тип игрока 3)Уровень рекурсии 4)Лучшая позиция добавляемой шашки
    int MinMaxAdd(TypePlayer typePlayer, int recursiveLevel, int& addPos, int alpha = -10000, int beta = 1000)
    {
        //На последнем уровне дерева
        if (recursiveLevel >= maxRecursiveLevelAdding * 2)
        {
            return EvaluationFunctionAdd();
        }

        int test = -10000;
        bool firstTest = true;

        int bestAddPos = 0;         //Лучшая позиция хода

        int minMax = (typePlayer == TypePlayer::redPlayer ? -10000 : 10000);

        //Перебираем все свободные места
        for (int i = 0; i < 24; i++)
        {
            if((*field).Points[i].GetStatus() == Field::PointStatus::free)
            {
                //Ставим шашку в свободное место
                (*field).AddСhecker(i, typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue);

                //Оцениваем поле после добавления шашки
                //Если после хода собралась мельница
                test = MinMaxAdd(typePlayer == TypePlayer::redPlayer ? TypePlayer::bluePlayer : TypePlayer::redPlayer, recursiveLevel + 1, addPos, alpha, beta);

                //Восстанавливаем исходное состояние поля
                (*field).DeleteChecker(i, typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue, true);

                //Если ход лучше тех, что были до него
                if( (test > minMax && typePlayer == TypePlayer::redPlayer) || (test < minMax && typePlayer == TypePlayer::bluePlayer) || firstTest == true)
                {
                    minMax = test;
                    bestAddPos = i;
                    firstTest = false;
                }

                if(typePlayer == TypePlayer::redPlayer)
                    alpha = std::max(alpha, test);
                else
                    beta = std::min(beta, test);

                if(beta < alpha)
                    break;
            }
        }

        //Записать лучший вариант хода
        if(recursiveLevel == 0)
        {
            addPos = bestAddPos;
        }

        return minMax;
    }

    //МинниМакс для уничтожения шашки
    //Аргументы: 1)Поле 2)Тип игрока 3)Позиция лучшей удалаляемой шашки
    int MinMaxDestroy(TypePlayer typePlayer, int& deletePos)
    {
        int test = 0;

        //Лучшая шашка для удаления
        int bestCheckers;

        int minMax = (typePlayer == TypePlayer::redPlayer ? -10000 : 10000);

        //Перебираем все шашки врага, которые можно удалить
        for (int i = 0; i < 24; i++)
        {
            //Цвет удаляемой шашки
            Field::PointStatus removedType = (typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfBlue : Field::PointStatus::captOfRed);

            if((*field).Points[i].GetStatus() == removedType && (*field).DeleteChecker(i, removedType, false) == true)
            {
                int movePos1 = -1, movePos2 = -1;
                //Оцениваем поле после удаления шашки
                test = EvaluationFunction();

                //Восстанавливаем исходное состояние поля
                (*field).AddСhecker(i, removedType);

                //Если ход лучше тех, что были до него
                if( (test > minMax && typePlayer == TypePlayer::redPlayer) || (test < minMax && typePlayer == TypePlayer::bluePlayer))
                {
                    minMax = test;
                    bestCheckers = i;
                }                
                
            }
        }
        deletePos = bestCheckers;

        return minMax;
    }


public:
    Bot(Field *field, TypePlayer typePlayer, int levelDifficulty)
    {
        this->field = field;
        this->typePlayer = typePlayer;
        this->pointStatus = (typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue);

        maxRecursiveLevel = levelsDifficulty[levelDifficulty][0];
        maxRecursiveLevelAdding = levelsDifficulty[levelDifficulty][1];
    };

    ~Bot()
    {

    };

    TypePlayer GetTypePlayer()
    {
        return typePlayer;
    }

    int countFunc = 0;

    //Добавить шашку
    //Возвращает позицию добавленной шашки 
    int AddChecker()
    {
        int addPos = 0;
        MinMaxAdd(typePlayer, 0, addPos);
        field->AddСhecker(addPos, pointStatus);

        return addPos;
    }

    //Переместить шашку
    //Возвращает позицию передвинутой шашки
    int MoveChecker()
    {
        int movePos1 = -1, movePos2 = -1;
        countFunc = 0;
        MinMax(typePlayer, 0, movePos1, movePos2);
        field->MoveСhecker(movePos1, movePos2, pointStatus);

        return movePos2;
    }

    //Уничтожить шашку
    //Возвращает позицию удаленной шашки
    int DestroyChecker()
    {
        int delPos = 0;
        MinMaxDestroy(typePlayer, delPos);
        field->DeleteChecker(delPos, (typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfBlue : Field::PointStatus::captOfRed), false);

        return delPos;
    }
};
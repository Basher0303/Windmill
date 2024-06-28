#pragma once
#include <iostream>
#include <string.h>
#include "colors.h"


class Field
{
public:
    enum PointStatus
    {
        free,
        captOfRed,
        captOfBlue,
    }; 

private:
    const char SYMBOL_POINT[4] = "○";
    const char SYMBOL_POINT_CAPT[4] = "◉"; //◉✖ø
    const char SYMBOL_POINT_SELECT[4] = "●";
    const char SYMBOL_CURSOR[4] = "✖";

    class Cursor
    {
    private:
        int position = 0;

    public:
        void SetPosition(int pos)
        {
            if(pos > -1 && pos < 25)
                position = pos;
        }
        int GetPosition()
        {
            return position;
        }
    };

    class Point
    {
    private: 
        struct Contacts
        {
            int indexPointTop = -1;
            int indexPointBottom = -1;
            int indexPointLeft = -1;
            int indexPointRight = -1;
        };
        
        PointStatus status = PointStatus::free;

    public:
        Contacts Contacts;
        bool IsSelected = false;
        int Position = -1;

        PointStatus GetStatus()
        {
            return status;
        }
        bool SetStatus(PointStatus stat)
        {
            if(stat != PointStatus::free && status != PointStatus::free)
                return false;
                
            status = stat;
            return true;
        }
    };

    class Line
    {
    private:
        bool isNew = false;
    public:
        Point* PointsInLine[3];
        
        //Количество красных шашек в линии
        int CountRedСheckers()
        {
            int res = 0;
            for (size_t i = 0; i < 3; i++)
            {
                if((*PointsInLine[i]).GetStatus() == PointStatus::captOfRed)
                    res += 1;
            }
            
            return res;
        }

        //Количество красных шашек в линии
        int CountBlueСheckers()
        {
            int res = 0;
            for (size_t i = 0; i < 3; i++)
            {
                if((*PointsInLine[i]).GetStatus() == PointStatus::captOfBlue)
                    res += 1;
            }
            
            return res;
        }        
    };

    void InitPoints()
    {
        for (int i = 0; i < 24; i++)
        {
            Points[i].Position = i;
        }
        
        Points[0].Contacts.indexPointBottom = 7;
        Points[0].Contacts.indexPointRight = 1;

        Points[1].Contacts.indexPointBottom = 9;
        Points[1].Contacts.indexPointLeft = 0;
        Points[1].Contacts.indexPointRight = 2;

        Points[2].Contacts.indexPointBottom = 3;
        Points[2].Contacts.indexPointLeft = 1;

        Points[3].Contacts.indexPointTop = 2;
        Points[3].Contacts.indexPointBottom = 4;
        Points[3].Contacts.indexPointLeft = 11;
        
        Points[4].Contacts.indexPointTop = 3;
        Points[4].Contacts.indexPointLeft = 5;

        Points[5].Contacts.indexPointTop = 13;
        Points[5].Contacts.indexPointLeft = 6;
        Points[5].Contacts.indexPointRight = 4;

        Points[6].Contacts.indexPointTop = 7;
        Points[6].Contacts.indexPointRight = 5;

        Points[7].Contacts.indexPointTop = 0;
        Points[7].Contacts.indexPointBottom = 6;
        Points[7].Contacts.indexPointRight = 15;

        Points[8].Contacts.indexPointBottom = 15;
        Points[8].Contacts.indexPointRight = 9;

        Points[9].Contacts.indexPointTop = 1;
        Points[9].Contacts.indexPointBottom = 17;
        Points[9].Contacts.indexPointLeft = 8;
        Points[9].Contacts.indexPointRight = 10;

        Points[10].Contacts.indexPointBottom = 11;
        Points[10].Contacts.indexPointLeft = 9;

        Points[11].Contacts.indexPointTop = 10;
        Points[11].Contacts.indexPointBottom = 12;
        Points[11].Contacts.indexPointLeft = 19;
        Points[11].Contacts.indexPointRight = 3;

        Points[12].Contacts.indexPointTop = 11;
        Points[12].Contacts.indexPointLeft = 13;

        Points[13].Contacts.indexPointTop = 21;
        Points[13].Contacts.indexPointBottom = 5;
        Points[13].Contacts.indexPointLeft = 14;
        Points[13].Contacts.indexPointRight = 12;
        
        Points[14].Contacts.indexPointTop = 15;
        Points[14].Contacts.indexPointRight = 13;

        Points[15].Contacts.indexPointTop = 8;
        Points[15].Contacts.indexPointBottom = 14;
        Points[15].Contacts.indexPointLeft = 7;
        Points[15].Contacts.indexPointRight = 23; 

        Points[16].Contacts.indexPointBottom = 23;
        Points[16].Contacts.indexPointRight = 17;   

        Points[17].Contacts.indexPointTop = 9;
        Points[17].Contacts.indexPointLeft = 16;
        Points[17].Contacts.indexPointRight = 18;   

        Points[18].Contacts.indexPointBottom = 19;
        Points[18].Contacts.indexPointLeft = 17;  

        Points[19].Contacts.indexPointTop = 18;
        Points[19].Contacts.indexPointBottom = 20;
        Points[19].Contacts.indexPointRight = 11;   

        Points[20].Contacts.indexPointTop = 19;
        Points[20].Contacts.indexPointLeft = 21;  

        Points[21].Contacts.indexPointBottom = 13;
        Points[21].Contacts.indexPointLeft = 22;
        Points[21].Contacts.indexPointRight = 20;   

        Points[22].Contacts.indexPointTop = 23;
        Points[22].Contacts.indexPointRight = 21;   

        Points[23].Contacts.indexPointTop = 16;
        Points[23].Contacts.indexPointBottom = 22;
        Points[23].Contacts.indexPointLeft = 15;                                                                                                                 
    }

    void InitLines()
    {
        //Инициализация сторон квадратов
        for (int k = 0; k < 3; k++)
        {
            for (int i = k * 4; i < k * 4 + 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if(i == k * 4 + 4 - 1 && j == 2)
                        Lines[i].PointsInLine[j] = &Points[k * 4 * 2];
                    else
                        Lines[i].PointsInLine[j] = &Points[j+i*2];
                }
            }
        }  
        for (int i = 0, k = 1; i < 4; i++, k += 2)
        {
            for (int j = 0; j < 3; j++)
            {
                Lines[12+i].PointsInLine[j] = &Points[k+j*8];
            }  
        }                                                 
    }
    
    
public:
    int countPoints = 24;
    int posPointSelected = -1;
    Point Points[24];
    Line Lines[16];
    Cursor Cursor;

    Field()
    {
        InitLines();
        InitPoints();
    }

    //Свободна ли точка?
    bool IsPointFree(int pos)
    {
        return Points[pos].GetStatus() == PointStatus::free;
    }

    //Добавить шашку
    bool AddСhecker(int pos, PointStatus status)
    {
        bool res = Points[pos].SetStatus(status);
        if(res == true)
        {
            return res; 
        }

        return res;
    }

    //Удалить шашку
    //Аргументы: 1)Позиция 2)Статус шашки 3)Уничтожать шашки в мельнице?
    bool DeleteChecker(int pos, PointStatus status, bool destroyMill)
    {
        bool res = false;
        if(Points[pos].GetStatus() == PointStatus::free || Points[pos].GetStatus() != status)
        {
            res = false;
        }
        else
        {
            if(destroyMill == false && (status == PointStatus::captOfBlue ? CountBlueCheckers() : CountRedCheckers()) > 3 && IsPointInCapturedLine(pos) == true)
            {
                res = false;
            }
            else
            {
                Points[pos].SetStatus(PointStatus::free);
                res = true;
            }
        }
        return res;
    }

    //Выделить шашку
    bool SelectСhecker(int pos, PointStatus status)
    {
        bool res = false;
        if(Points[pos].GetStatus() != status)
        {
            res = false;
        }
        else
        {
            Points[pos].IsSelected = true;
            res = true;
        }
        return res;        
    }

    //Снять выделение с шашки
    bool DeselectСhecker(int pos, PointStatus status)
    {
        Points[pos].IsSelected = false;
        return true;           
    }

    //Переместить шашку
    bool MoveСhecker(int pos1, int pos2, PointStatus status)
    {
        bool res = false;
        if(Points[pos2].GetStatus() == PointStatus::free)
        {
            if(Points[pos1].Contacts.indexPointTop== pos2)
                res = true;
            else if(Points[pos1].Contacts.indexPointBottom == pos2)
                res = true;
            else if(Points[pos1].Contacts.indexPointLeft == pos2)
                res = true;
            else if(Points[pos1].Contacts.indexPointRight == pos2)
                res = true;
            
            if(res == true)
            {
                PointStatus buff = Points[pos1].GetStatus();
                Points[pos1].SetStatus(Points[pos2].GetStatus());
                Points[pos2].SetStatus(buff);
                Points[pos1].IsSelected = false;
            }
        }
        else
            res = false;

        return res;
    }

    //Количество линий захваченных синим игроком
    int CountLinesCapturedBlue(int pos)
    {
        int res = 0;
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 3; j++)
            {   
                if(Lines[i].PointsInLine[j]->Position == pos && Lines[i].CountBlueСheckers() == 3)
                    res += 1; 
            }
        }
        return res;
    }

    //Количество линий захваченных красным игроком
    int CountLinesCapturedRed(int pos)
    {
        int res = 0;
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 3; j++)
            {   
                if(Lines[i].PointsInLine[j]->Position == pos && Lines[i].CountRedСheckers() == 3)
                    res += 1; 
            }
        }
        return res;      
    }

    

    //Количество красных шашек
    int CountRedCheckers()
    {
        int res = 0;
        for (int i = 0; i < 24; i++)
        {
            if(Points[i].GetStatus() == PointStatus::captOfRed)
                res += 1;

        }
        return res;
    }

    //Количество синих шашек
    int CountBlueCheckers()
    {
        int res = 0;
        for (int i = 0; i < 24; i++)
        {
            if(Points[i].GetStatus() == PointStatus::captOfBlue)
                res += 1;

        }
        return res;
    }

    //Количество ходов у красного игрока
    int CountMovesRed()
    {
        int res = 0;
        for (int i = 0; i < 24; i++)
        {
           if(Points[i].GetStatus() == PointStatus::captOfRed)
           {
                if(Points[i].Contacts.indexPointTop != -1 && Points[Points[i].Contacts.indexPointTop].GetStatus() == PointStatus::free)
                    res += 1;
                if(Points[i].Contacts.indexPointBottom != -1 && Points[Points[i].Contacts.indexPointBottom].GetStatus() == PointStatus::free)
                    res += 1;
                if(Points[i].Contacts.indexPointLeft != -1 && Points[Points[i].Contacts.indexPointLeft].GetStatus() == PointStatus::free)
                    res += 1;
                if(Points[i].Contacts.indexPointRight != -1 && Points[Points[i].Contacts.indexPointRight].GetStatus() == PointStatus::free)
                    res += 1;
           } 
        }
        return res;
    }

    //Количество ходов у синего игрока
    int CountMovesBlue()
    {
        int res = 0;
        for (int i = 0; i < 24; i++)
        {
           if(Points[i].GetStatus() == PointStatus::captOfBlue)
           {
                if(Points[i].Contacts.indexPointTop != -1 && Points[Points[i].Contacts.indexPointTop].GetStatus() == PointStatus::free)
                    res += 1;
                if(Points[i].Contacts.indexPointBottom != -1 && Points[Points[i].Contacts.indexPointBottom].GetStatus() == PointStatus::free)
                    res += 1;
                if(Points[i].Contacts.indexPointLeft != -1 && Points[Points[i].Contacts.indexPointLeft].GetStatus() == PointStatus::free)
                    res += 1;
                if(Points[i].Contacts.indexPointRight != -1 && Points[Points[i].Contacts.indexPointRight].GetStatus() == PointStatus::free)
                    res += 1;
           } 
        }
        return res;
    }

    //Находится точка в захваченной линии?
    bool IsPointInCapturedLine(int pos)
    {
        for (int i = 0; i < 16; i++)
        {
            //Найдена ли точка в линии?
            bool pointLocated = false;

            for (int j = 0; j < 3; j++)
            {
                if(Lines[i].PointsInLine[j]->Position == pos)
                {
                    pointLocated = true;
                    break;
                }
            }
            
            if(pointLocated == true)
            {
                //Захвачена ли линия?
                bool isLineCaptured = true;
                PointStatus status = Lines[i].PointsInLine[0]->GetStatus();

                for (int j = 0; j < 3; j++)
                {
                    if(Lines[i].PointsInLine[j]->GetStatus() != status || status == PointStatus::free)
                    {
                        isLineCaptured = false;
                        break;
                    }
                }

                if(isLineCaptured == true)
                    return true;
            }
            
        }
        return false;
    }

    //Вывод поля в консоль
    void Print()
    {
        std::cout << "\x1B[2J\x1B[H";
        std::cout << CLR_RED;
        std::string clrPoints[24];
        for (int i = 0; i < 24; i++)
        {
            if(Points[i].GetStatus() == PointStatus::free) 
            {
                clrPoints[i] = CLR_WHITE;
                if(Cursor.GetPosition() == i)
                    clrPoints[i] += SYMBOL_CURSOR;
                else if(Points[i].IsSelected == true)
                    clrPoints[i] += SYMBOL_POINT_SELECT;
                else
                    clrPoints[i] += SYMBOL_POINT;
            }
            else if(Points[i].GetStatus() == PointStatus::captOfRed)
            {
                clrPoints[i] = CLR_RED;
                if(Cursor.GetPosition() == i)
                    clrPoints[i] += SYMBOL_CURSOR;
                else if(Points[i].IsSelected == true)
                    clrPoints[i] += SYMBOL_POINT_SELECT;
                else
                    clrPoints[i] += SYMBOL_POINT_CAPT;
            }
            else if(Points[i].GetStatus() == PointStatus::captOfBlue)
            {
                clrPoints[i] = CLR_BLUE; 
                if(Cursor.GetPosition() == i)
                    clrPoints[i] += SYMBOL_CURSOR;
                else if(Points[i].IsSelected == true)
                    clrPoints[i] += SYMBOL_POINT_SELECT;
                else
                    clrPoints[i] += SYMBOL_POINT_CAPT;       
            }
            clrPoints[i] += CLR_WHITE;
        }
        
        std::cout << clrPoints[0] << "━━━━━━━━" << clrPoints[1] << "━━━━━━━━" << clrPoints[2] << "\n";
        std::cout << "┃        ┃        ┃\n";
        std::cout << "┃  " << clrPoints[8] << "━━━━━" << clrPoints[9] << "━━━━━" << clrPoints[10] << "  ┃\n";
        std::cout << "┃  ┃     ┃     ┃  ┃\n";
        std::cout << "┃  ┃  " << clrPoints[16] << "━━" << clrPoints[17] << "━━" << clrPoints[18] << "  ┃  ┃\n";
        std::cout << "┃  ┃  ┃     ┃  ┃  ┃\n";
        std::cout << clrPoints[7] << "━━" << clrPoints[15] << "━━" << clrPoints[23] << "     " << clrPoints[19] << "━━" << clrPoints[11] << "━━" << clrPoints[3] << "\n";
        std::cout << "┃  ┃  ┃     ┃  ┃  ┃\n";   
        std::cout << "┃  ┃  " << clrPoints[22] << "━━" << clrPoints[21] << "━━" << clrPoints[20] << "  ┃  ┃\n";
        std::cout << "┃  ┃     ┃     ┃  ┃\n";
        std::cout << "┃  " << clrPoints[14] << "━━━━━" << clrPoints[13] << "━━━━━" << clrPoints[12] << "  ┃\n";
        std::cout << "┃        ┃        ┃\n";
        std::cout << clrPoints[6] << "━━━━━━━━" << clrPoints[5] << "━━━━━━━━" << clrPoints[4] << "\n";            
    }
};


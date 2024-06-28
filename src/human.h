#pragma once

class Human
{
private:
    //Тип(цвет) игрока
    TypePlayer typePlayer;
    //Тип(цвет) шашек
    Field::PointStatus pointStatus;
    Field* field;

    int TakeAction(Action action)
    {
        bool success = false;

        struct termios oldt,
        newt;
        int ch;
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

        if(ch == KEY_W || ch == KEY_UP)
        {
            if(field->Points[field->Cursor.GetPosition()].Contacts.indexPointTop != -1)
                field->Cursor.SetPosition(field->Points[field->Cursor.GetPosition()].Contacts.indexPointTop);
                
        }
        else if(ch == KEY_S || ch == KEY_DOWN)
        {
            if(field->Points[field->Cursor.GetPosition()].Contacts.indexPointBottom != -1)
                field->Cursor.SetPosition(field->Points[field->Cursor.GetPosition()].Contacts.indexPointBottom);
        }
        else if(ch == KEY_A || ch == KEY_LEFT)
        {
            if(field->Points[field->Cursor.GetPosition()].Contacts.indexPointLeft != -1)
                field->Cursor.SetPosition(field->Points[field->Cursor.GetPosition()].Contacts.indexPointLeft);
        }
        else if(ch == KEY_D || ch == KEY_RIGHT)
        {
            if(field->Points[field->Cursor.GetPosition()].Contacts.indexPointRight != -1)
                field->Cursor.SetPosition(field->Points[field->Cursor.GetPosition()].Contacts.indexPointRight);
        }
        
        else if(ch == KEY_SPACE)
        {
            if(action == Action::addition)
            {
                success = field->AddСhecker(field->Cursor.GetPosition(), typePlayer == TypePlayer::bluePlayer ? Field::PointStatus::captOfBlue : Field::PointStatus::captOfRed);
                if(success == true)
                    return 1;
            }
            else if(action == Action::deletion)
            {
                success = field->DeleteChecker(field->Cursor.GetPosition(), (typePlayer == TypePlayer::bluePlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue), false);
                if(success == true)
                    return 1;
            }
            else if(action == Action::selection)
            {
                success = field->SelectСhecker(field->Cursor.GetPosition(), (typePlayer == TypePlayer::bluePlayer ? Field::PointStatus::captOfBlue : Field::PointStatus::captOfRed));
                if(success == true)
                {
                    field->posPointSelected = field->Cursor.GetPosition();
                    return 1;
                }
            }
            else if(action == Action::moving)
            {
                success = field->MoveСhecker(field->posPointSelected, field->Cursor.GetPosition(), (typePlayer == TypePlayer::bluePlayer ? Field::PointStatus::captOfBlue : Field::PointStatus::captOfRed));
                if(success == true)
                {
                    field->posPointSelected = -1;   
                    return 1;
                }
            }
        }   

        else if(ch == KEY_ESCAPE)
        {
            if(action == Action::moving)
            {
                field->DeselectСhecker(field->posPointSelected, (typePlayer == TypePlayer::bluePlayer ? Field::PointStatus::captOfBlue : Field::PointStatus::captOfRed));
                field->posPointSelected = -1;
                return -1;
            }
        } 

        return 0;
    }

public:
    Human(Field *field, TypePlayer typePlayer)
    {
        this->field = field;
        this->typePlayer = typePlayer;
        this->pointStatus = (typePlayer == TypePlayer::redPlayer ? Field::PointStatus::captOfRed : Field::PointStatus::captOfBlue);
    }
    ~Human()
    {

    };

    TypePlayer GetTypePlayer()
    {
        return typePlayer;
    }

    int AddChecker()
    {
        return TakeAction(Action::addition);
    }  

    int SelectChecker()
    {
        return TakeAction(Action::selection);
    }

    int MoveChecker()
    {
        return TakeAction(Action::moving);
    }

    int DestroyChecker()
    {
        return TakeAction(Action::deletion);
    }
};


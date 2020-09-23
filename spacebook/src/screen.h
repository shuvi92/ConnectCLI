#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>

class Screen 
{
    public:
        Screen();
        void change_mode();
        bool get_edit();
        bool get_notification();

    private:
        bool _edit;
        bool _notification;
};

#endif
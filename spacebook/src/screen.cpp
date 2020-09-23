#include "screen.h"

/* Constructor */
Screen::Screen()
{
    _edit = false;
    _notification = true;
}

/* Switch between edit and notification mode */
void Screen::change_mode()
{
    if (_edit == false && _notification == true)
    {
        _edit = true;
        _notification = false;
        std::string menu_banner = R"(
                          ===== USER MENU SELECTION MODE =====
                           +++++ PRESS / TO SWITCH MODE +++++    
------------------------------------------------------------------------------------------)";
        std::cout << menu_banner << std::endl;
        
    }
    else
    {
        _edit = false;
        _notification = true;
        std::string noti_banner = R"(
------------------------------------------------------------------------------------------
                          ===== NOTIFICATION STREAM MODE =====
                           +++++ PRESS / TO SWITCH MODE +++++    
------------------------------------------------------------------------------------------)";
        std::cout << noti_banner << std::endl;
    }
}

/* Return status of edit screen */
bool Screen::get_edit()
{
    return _edit;
}

/* Return status of notification screen */
bool Screen::get_notification()
{
    return _notification;
}
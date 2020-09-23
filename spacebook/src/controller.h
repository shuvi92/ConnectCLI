#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "communicator.h"

class Controller 
{
    public:
        Controller (    User& user, 
                        std::map<int, Post>& posts,
                        std::map<std::string, TSN::user_information>& list, 
                        Ops& ops,
                        Screen& screen,
                        Communicator& comm ) : 
            _user(user), 
            _posts(posts),
            _list(list), 
            _ops(ops), 
            _view( View(_user, _posts)), 
            _screen(screen), 
            _communicator(comm) {}

        void cli_command(int choice);
        void cli_interface();
        void edit_info();
        
    private:
        User& _user;
        std::map<int, Post>& _posts;
        std::map<std::string, TSN::user_information>& _list;
        Ops& _ops;
        View _view;
        Screen& _screen;
        Communicator& _communicator;

        struct termios original, changes;

};


#endif
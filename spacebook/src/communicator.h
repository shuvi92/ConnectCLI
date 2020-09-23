#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <termios.h>
#include <map>
#include <fstream>
#include "view.h"
#include "screen.h"



class Communicator
{
    public:
        Communicator (  User& user,
                        std::map<int, Post>& posts, 
                        std::map<std::string, 
                        TSN::user_information>& list, 
                        Ops& ops,
                        Screen& screen ) : 
            _user(user), 
            _posts(posts),
            _list(list), 
            _ops(ops), 
            _view( View(_user, _posts )), 
            _screen(screen) {}

        void clear_data();
        void get_interest_post();
        void get_pm();
        void get_request();
        void get_user_info();
        void post_user_info();
        void receive_response();
        void search_response(TSN::request post_request);
        void send_pm(std::map<std::string, TSN::user_information>::iterator it);
        void send_response(TSN::request V);
        void send_request();
        

    private:
        User& _user;
        std::map<int, Post>& _posts;
        std::map<std::string, TSN::user_information>& _list;
        Ops& _ops;
        View _view;
        Screen& _screen;
        std::map<std::string, std::vector<Post>> post_list;
};

#endif


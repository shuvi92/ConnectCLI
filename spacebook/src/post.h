#ifndef POST_H
#define POST_H

#include <iostream>
#include <map>
#include <vector>

class Post
{
    public:
        Post();
        Post(int sn, time_t raw_time, std::string post) : _sn(sn), _time(raw_time), _post(post){}

        bool get_child_sn(std::string c_uuid, int c_sn);
        bool is_child();
        bool is_parent();
        int get_child_size();
        int get_psn();    
        int get_sn();
        std::map<std::string, std::vector<int>> get_child_list();
        std::string get_post();
        std::string get_puuid();
        time_t get_time();
        void set_as_child(std::string p_uuid, int p_sn);        
        void set_as_parent(std::string c_uuid, int c_sn);
        void set_post(std::string post);
        void set_sn(int sn);


    private:
        int _sn;
        time_t _time;
        std::string _post;
        std::string _puuid;
        int _psn;
        std::map<std::string, std::vector<int>> _child_list;
        bool _parent = false;
        bool _child = false;
};

#endif
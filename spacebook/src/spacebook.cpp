#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <map>
#include <algorithm>
#include <mutex>
#include <boost/algorithm/string.hpp>

#include "util.cpp"
#include "controller.h"


int main ( int argc, char* argv[] )
{
    /* Declaring variables */
    std::map<std::string, TSN::user_information> user_list;
    User main_user;
    std::map<int, Post> main_post;
    Ops ops_obj;
    Screen mode;
    Communicator ops_com(main_user, main_post, user_list, ops_obj, mode);
    Controller controller(main_user, main_post, user_list, ops_obj, mode, ops_com);

    /* Exporting to .tsn file */
    std::string user_profile = "./data/user_profile.tsn";
    std::string user_posts = "./data/post.tsn";
    std::ifstream profile_stream(user_profile);
    std::ifstream post_stream(user_posts);

    /* Exporting or Importing User Profile */
    /* Create file if doesn't exist and enter new user profile */
    if( !(profile_stream.is_open()) )
    {
        std::cout << "User file does not exist. Creating new profile...\n" << std::endl;

        boost::uuids::uuid main_uuid = boost::uuids::random_generator()();
        main_user.set_uuid(main_uuid);
        controller.edit_info();

        profile_stream.close();
    }
    /* Load existing profile into main_user if user profile file exist  */
    else
    {
        std::string input_info;

        std::getline(profile_stream, input_info);
        boost::uuids::uuid temp_uuid = boost::lexical_cast<boost::uuids::uuid>(input_info);
        main_user.set_uuid(temp_uuid);

        std::getline(profile_stream, input_info);
        main_user.set_fname(input_info);

        std::getline(profile_stream, input_info);
        main_user.set_lname(input_info);

        std::getline(profile_stream, input_info);
        main_user.set_age(std::stoi(input_info));

        while(std::getline(profile_stream, input_info))
        {
            main_user.add_interest(input_info);
        }
        profile_stream.close();
    }
    /* Exporting or Importing Post  */
    /* If Post file doesn't exist, create one and load content  */
    if( !(post_stream.is_open()) )
    {
        std::cout << "Post file does not exist. Creating new post file...\n" << std::endl;
        std::ofstream ost(user_posts, std::fstream::out | std::fstream::app);

        for (std::map<int, Post>::iterator i = main_post.begin(); i != main_post.end(); i++)
        {
            ost << std::to_string(i->first) + ";" + std::to_string(i->second.get_time()) 
                + ";" + i->second.get_post() << std::endl;
        }
        ost.close();
    }
    /* If post file exist, load into user posts instead  */
    else
    {
        std::string input_post;
        while(std::getline(post_stream, input_post))
        {
            std::vector<std::string> temp_str;
            boost::algorithm::split(temp_str, input_post, boost::is_any_of(";"));
            Post temp_post(stoi(temp_str[0]), (time_t)stol(temp_str[1]), temp_str[2]);
            main_post[main_post.size() + 1] = temp_post;
        }
    }

    /* Allow 6 functions to run simultanously  */
    std::thread post_user_info_thread(&Communicator::post_user_info, &ops_com);
    std::thread receive_user_info_thread(&Communicator::get_user_info, &ops_com);
    std::thread main_interface_thread(&Controller::cli_interface, &controller);
    std::thread receive_request_thread(&Communicator::get_request, &ops_com);
    std::thread receive_response_thread(&Communicator::receive_response, &ops_com);
    std::thread receive_pm_thread(&Communicator::get_pm, &ops_com);

    post_user_info_thread.join();
    receive_user_info_thread.join();
    main_interface_thread.join();
    receive_request_thread.join();
    receive_response_thread.join();
    receive_pm_thread.join();
}
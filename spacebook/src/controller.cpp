#include "controller.h"
#include "util.cpp"

/* Main User Controller */
void Controller::cli_interface()
{
    /* Declare Variables */
    tcgetattr(STDIN_FILENO, &original);
    changes = original;
    changes.c_lflag &= ~ (ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &changes);
    
    std::string S;
    int cmd = -1;

    /* Show banner */
    _view.print_banner();
    std::string menu_banner = R"(
------------------------------------------------------------------------------------------
                          ===== USER MENU SELECTION MODE =====
                           +++++ PRESS / TO SWITCH MODE +++++    
------------------------------------------------------------------------------------------)";
        std::cout << menu_banner << std::endl;

    /* Looping through the menu */
    while (cmd != 0)
    {
        int i = getchar();
        unsigned char ch = i;
        std::string T(1, ch);

        /* Taking string inputs */
        if (_screen.get_edit() == true)
        {
            if ( isprint(ch) )
            {
                std::cout << ch;
                S = S + T;
            }
            /* Exit Screen input by / */
            else if(ch == '/')
            {
                _screen.change_mode();
                tcsetattr(STDIN_FILENO, TCSANOW, &changes);
            }          
        }
        /* Switches between mode */
        if (ch == '/')
        {
            // Changes back to notification mode
            if (_screen.get_edit() == true)
            {
                S = ""; // resetting S
                _screen.change_mode();
                tcsetattr(STDIN_FILENO, TCSANOW, &changes);               
            }
            // Changes to edit mode
            else
            {
                _screen.change_mode();
                _view.show_menu();
            }
        }
        /* Execute menu based on keywords and reset keyword after execution */
        else if (S.compare("0") == 0 || S.compare("exit") == 0 || S.compare("quit") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(0);
        }
        else if (S.compare("1") == 0 || S.compare("list") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(1);
        }
        else if (S.compare("2") == 0 || S.compare("show") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(2);
        }
        else if (S.compare("3") == 0 || S.compare("request") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(3);
        }
        else if (S.compare("4") == 0 || S.compare("post") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(4);
        }
        else if (S.compare("5") == 0 || S.compare("edit") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(5);
        }
        else if (S.compare("6") == 0 || S.compare("pm") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(6);
        }
        else if (S.compare("7") == 0 || S.compare("interest") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(7);
        }
        else if (S.compare("8") == 0 || S.compare("resync") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(8);
        }
        else if (S.compare("9") == 0 || S.compare("about") == 0 || S.compare("help") == 0)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
            S = ""; 
            cli_command(9);
        }
    }
}

/* Command Execution function */
void Controller::cli_command(int cmd)
{
    /* Allow output to be display */
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    std::cout << std::endl;
    switch(cmd)
    {
        /* List all users */
        case 1: 
        {
            std::cout << "There are " << _list.size() << " users online." << std::endl;
            /* Iterate through the _list map and print out values */
            _view.print_user_headers();
            for (std::map<std::string, TSN::user_information>::iterator i = _list.begin(); i != _list.end(); ++i)
            {
                _view.list_users(i);
            }
            break;
        }
        /* Show specific user */
        case 2:
        {
            std::string uuid_request;
            bool str_found = false;

            std::cout << "Enter user's UUID (or part of it) to request:  ";
            std::cin >> uuid_request; cin.ignore();
            /* Iterate through local map to find requested user */
            for (std::map<std::string, TSN::user_information>::iterator it = _list.begin(); it != _list.end(); it++)
            {
                size_t found = it->first.find(uuid_request);
                if (found != std::string::npos)
                {
                    _view.show_user_info(it);
                    str_found = true;
                    break;
                }
            }
            if (str_found == false)
            {
                std::cout << "User not found" << std::endl;
            }
            break;
        }
        /* Request Post from any number of UUID */
        case 3:
        {
            std::map<std::string, TSN::user_information>::iterator i;

            // Displaying current UUID stored in _list map
            std::cout << "Available UUIDs for request: " << std::endl;
            _view.print_user_headers();
            for (i = _list.begin(); i != _list.end(); ++i)
            {
                _view.list_users(i);
            }
            /* Function call to send request. See communicator's function */
            _view.print_separator();
            _communicator.send_request();
            break;
        }
        /* Add new post */
        case 4: // 
        {
            /* Variables and getting post inputs */
            int post_option, p_sn;
            std::string new_post, p_uuid;
            std::string user_posts = "./data/post.tsn";
            std::fstream post_stream(user_posts);

            std::cout << "Enter post content\n>>  ";
            getline(std::cin, new_post);

            /* Selection between standalone post or comment another post */
            _view.show_post_menu();
            std::cin >> post_option;
 
            Post temp_post(_posts.size() + 1, F_Utility::get_raw_time(), new_post);

            /* Adding a post as part of another post */
            if (post_option == 2)
            {
                std::cout << "Enter the post's parent UUID (or part of it): ";
                std::cin >> p_uuid; cin.ignore();
                for (std::map<std::string, TSN::user_information>::iterator it = _list.begin(); it != _list.end(); it++)
                {
                    /* Search for sub string as part of UUID */
                    size_t found = it->first.find(p_uuid);
                    if (found != std::string::npos)
                    {
                        std::cout << "Selected " << it->second.first_name << " " << it->second.last_name << std::endl;
                        p_uuid = it->first;
                        break;
                    }
                }
                std::cout << "Enter the post's serial number: ";
                std::cin >> p_sn; cin.ignore();
                temp_post.set_as_child(p_uuid, p_sn);                
            }
            /* Display Confirmation */
            _posts[_posts.size() + 1] = temp_post;
            std::cout << "Post number " << std::to_string(_posts.size()) << " added on " 
                << F_Utility::get_date_time(temp_post.get_time()) << std::endl;

            /* Append post data to local file */
            if( !(post_stream.is_open()) )
            {
                std::cout << "Post file does not exist. Creating new post file...\n" << std::endl;
            }
            std::ofstream ost(user_posts, std::fstream::out | std::fstream::app);
            ost << std::to_string(temp_post.get_sn()) + ";" + std::to_string(temp_post.get_time()) 
                   + ";" + temp_post.get_post() << std::endl;
            ost.close();
            post_stream.close();

            break;
        }
        /* Edit User. This will wipe first, last, age, and all interests */
        case 5:
        {
            _user.clear_interest();
            /* Reenter all users info except posts */
            edit_info();

            std::cout << "User edit successful " << std::endl;
            
            break;
        }
        /* Send Private Messages */
        case 6:
        {
            std::string recipient_uuid; 
            bool str_found = false;
            /* Display all available users */
            _view.print_user_headers();
            for (std::map<std::string, TSN::user_information>::iterator i = _list.begin(); i != _list.end(); ++i)
            {
                _view.list_users(i);
            }

            /* Displaying current UUID stored in _list map and getting UUID and message */
            std::cout << "Enter user's UUID (or part of it) to send PM:  ";
            std::cin >> recipient_uuid; cin.ignore();

            for (std::map<std::string, TSN::user_information>::iterator it = _list.begin(); it != _list.end(); it++)
            {
                /* Check substring for UUID */
                size_t found = it->first.find(recipient_uuid);
                if (found != std::string::npos)
                {
                    str_found = true;
                    _communicator.send_pm(it);
                    break;
                }
            }
            if (str_found == false)
            {
                std::cout << "User not found" << std::endl;
            }        
            break;
        }
        /* Search post by interests */
        case 7:
        {
            /* Declare variables */
            int uuid_count;
            std::string uuid_selection;
            TSN::request input_request;
            uuid_count = 0;

            if (_list.size() > 0)
            {
                /* Prepping Owner's UUID to be send out */
                F_Utility::prep_uuid(input_request.uuid, _user.get_uuid());
                /* Send request for all posts from all users */
                for (std::map<std::string, TSN::user_information>::iterator i = _list.begin(); i != _list.end(); ++i)
                {
                    input_request.user_requests.length(_list.size());
                    F_Utility::prep_uuid(input_request.user_requests[uuid_count].fulfiller_uuid, i->first );  
                    input_request.user_requests[uuid_count].requested_posts.length(i->second.number_of_highest_post);
                    
                    for (unsigned int j = 0; j < i->second.number_of_highest_post; j++)
                    {
                        input_request.user_requests[uuid_count].requested_posts[j] = j + 1;          
                    }
                    uuid_count++;
                }
                _ops.get_dds_request().publish(input_request);
                sleep(5);
                _communicator.get_interest_post();
            }
            else
            {
                std::cout << "No users available\n";
            }
            break;      
        }
        /* Clear all local data */
        case 8: 
        {
            _list.clear();
            _communicator.clear_data();
            std::cout << "All data cleared\n";
            break;
        }
        /* Display about and help section */
        case 9:
        {
            _view.show_about();
            break;
        }
        /* Exit program */
        case 0:
        {
            exit(0);
        }
    }
    /* Display returning to menu animation */
    _view.print_return_progress();
    /* Lock screen input */
    if (_screen.get_edit() == true)
    {
        _view.show_menu();
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &changes);
}

/* Edit info and store to local profile */
void Controller::edit_info()
{
    std::string fname, lname, interest, file_name = "./data/user_profile.tsn";
    int _age;

    std::cout << "Enter First Name: ";
    std::cin >> fname; std::cin.ignore();
    _user.set_fname(fname);

    std::cout << "Enter Last Name: ";
    std::cin >> lname; std::cin.ignore();
    _user.set_lname(lname);

    std::cout << "Age: ";
    std::cin >> _age; std::cin.ignore();
    _user.set_age(_age);

    std::cout << "Enter -1 to exit enterring interests: " << std::endl;
    do
    {
    std::cout << ">>  ";
    getline(std::cin, interest);
        if (interest.compare("-1") != 0)
        {
            _user.add_interest(interest);
        }
    } while (interest.compare("-1") != 0);

    std::ofstream ostream(file_name, std::ofstream::out | std::ofstream::trunc);
    ostream << _user.get_uuid() << std::endl;
    ostream << _user.get_fname() << std::endl;
    ostream << _user.get_lname() << std::endl;
    ostream << _user.get_age() << std::endl;
    for (int i = 0; i < _user.get_interest_count(); i++)
    {
        ostream << _user.get_interest(i) << std::endl;
    }
    ostream.close();
}
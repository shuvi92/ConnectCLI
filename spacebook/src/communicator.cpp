#include <boost/lexical_cast.hpp>
#include "communicator.h"
#include "util.cpp"

/* Clear local map that stores remote post */
void Communicator::clear_data()
{
    post_list.clear();
}

/* Function to reach out to all remote user to get all request */
void Communicator::get_interest_post()
{
    std::cout << "Displaying all posts based on current interests:\n" << std::endl;
	std::string str;
    std::map<std::string, vector<Post>>::iterator it;
    /* Check all available remote users */
    for (it = post_list.begin(); it != post_list.end(); it++)
    {
        /* Check all posts from each users */
        for (unsigned int i = 0; i < it->second.size(); i++)
        {
            /* Check all owner's interest */
            for (int j = 0; j < _user.get_interest_count(); j++)
            {
                /* Compare each owner's interest with all avaiable posts */
                if (F_Utility::check_keyword(it->second[i].get_post(), _user.get_interest(j) ) == true)
                {
                    std::cout << "Post # " << std::to_string(it->second[i].get_sn()) << " from " << it->first << std::endl;
                    std::cout <<  _list[it->first].first_name  << " " << _list[it->first].last_name << " wrote on "
                            << F_Utility::get_date_time(it->second[i].get_time())
                            << "    " << it->second[i].get_post() <<  "\n" << std::endl;   
                }
            }
        }
    }
}

/* Function to get private messages using thread */
void Communicator::get_pm()
{
    while (1)
    {
        std::vector<TSN::private_message> V = _ops.get_dds_pm().recv();
        for (unsigned int i = 0; i < V.size(); i++)
        {
            std::map<std::string, TSN::user_information>::iterator it;
            it = _list.find(V[i].sender_uuid);

            /* Display private message */
            if (_user.get_uuid().compare(V[i].receiver_uuid) == 0 && it != _list.end())
            {
                std::cout << "Private Message Received from " << V[i].sender_uuid << std::endl;
                std::cout << it->second.first_name << " " << it->second.last_name << " wrote on "
                          << F_Utility::get_date_time(V[i].date_of_creation)
                          << "    " << V[i].message_body << "\n" <<std::endl;
                _view.print_separator();
            }
        }
        sleep(1);
    }
}

/* Function that get TSN::request every 1 second using thread. Using Thread */
void Communicator::get_request()
{
    while(1)
    {
        /* Receive and iterate through request arrays */
        std::vector<TSN::request> V = _ops.get_dds_request().recv ();
        for (unsigned int i=0; i<V.size(); i++)
        {
            send_response(V[i]);
        }
        sleep(1);
    }
}

/* Function that get user_information constantly. Using Thread */
void Communicator::get_user_info()
{
    std::map<std::string, int> online_timer;
    std::map<std::string, TSN::user_information>::iterator it;
    while(1)
    {
        std::vector<TSN::user_information> V = _ops.get_dds_user_info().recv ();
        for (unsigned int i = 0; i < V.size(); i++)
        {
            if (_user.get_uuid().compare(V[i].uuid) != 0)
            {
                it = _list.find(V[i].uuid);
                /* If user does not exist then notify that the user is online */
                if ( it == _list.end() )
                {
                    if (_screen.get_notification() == true)
                    {
                        std::cout << V[i].first_name << " " << V[i].last_name << " is online" << std::endl;
                        _view.print_separator();
                    }
                    online_timer[V[i].uuid] = 35;
                }
                /* If user already exist then reset the timer */
                else if ( it != _list.end())
                {
                    online_timer[it->first] = 35;
                }
                /* Only add user to memory if the first character of their first and last name is alpha */
                if (isalpha(V[i].first_name[0]) && isalpha(V[i].last_name[0]))
                {
                    _list[V[i].uuid] = V[i];
                }
            }
        }
        /* Check every user to make sure their timer is still up */
        for (std::map<std::string, int>::iterator i = online_timer.begin(); i != online_timer.end(); i++)
        {
            if (i->second > 0)
            {
                i->second--;
            }
            /* Remove user from both maps if their timer has expired
               This means they went offline as well */
            if (i->second == 0 && _screen.get_notification() == true)
            {
                std::cout << _list[i->first].first_name << " " << _list[i->first].last_name << " is offline" << std::endl;
                _list.erase(i->first);
                online_timer.erase(i);
            }
            assert(i->second >= 0);
        }
        sleep(1);
    }
}

/* Post User Information every 30 seconds. Using Thread */
void Communicator::post_user_info()
{
    while(1)
    {
        TSN::user_information p_user;
        /* Store all user information into p_user */
        F_Utility::prep_uuid(p_user.uuid, _user.get_uuid());
        p_user.first_name = DDS::string_dup(_user.get_fname().c_str());
        p_user.last_name = DDS::string_dup(_user.get_lname().c_str());
        p_user.date_of_birth = _user.get_age();
        p_user.interests.length(_user.get_interest_count());
        for (int i = 0; i < _user.get_interest_count(); i++)
        {
            p_user.interests[i] = DDS::string_dup(_user.get_interest(i).c_str());
        }
        p_user.number_of_highest_post = _posts.size();
        /* Send out user information */
        _ops.get_dds_user_info().publish(p_user);
        
        sleep(30);
    }

}

/* Function to get responses. This does not use thread */
void Communicator::receive_response()
{
    while(1)
    {
        std::vector<TSN::response> V = _ops.get_dds_response().recv ();
        for (unsigned int i = 0; i < V.size(); i++)
        {
            if (_user.get_uuid().compare(V[i].uuid) != 0)
            {
                /* Temp post creation */
                Post temp_post((int)(V[i].post_id), V[i].date_of_creation, boost::lexical_cast<std::string>(V[i].post_body));

                /* Store all child posts into temp_post */
                for (unsigned int j = 0; j < V[i].child_posts.length(); j++)
                {
                    temp_post.set_as_parent(V[i].child_posts[j].owner_uuid, V[i].child_posts[j].post_id);
                }

                /* Store parent's info if available */
                if (isdigit(V[i].parent_post.post_id))
                {
                    temp_post.set_as_child(V[i].parent_post.owner_uuid, V[i].parent_post.post_id);
                }
                /* Check if post is a child post of owner's */
                if (_user.get_uuid() == V[i].parent_post.owner_uuid)
                {
                    std::map<int, Post>::iterator j = _posts.find(V[i].parent_post.post_id);
                    if (j != _posts.end())
                    {
                        if ( j->second.get_child_sn(V[i].uuid, V[i].post_id) == false)
                        {
                            j->second.set_as_parent(V[i].uuid, V[i].post_id);
                        }
                    }
                } 
                /*  Check if the post's child post list has the owner's post as child */
                for (unsigned int j = 0; j < V[i].child_posts.length(); j++)
                {
                    if (_user.get_uuid().compare(V[i].child_posts[j].owner_uuid) == 0)
                    {
                        std::map<int, Post>::iterator k = _posts.find(V[i].child_posts[j].post_id);
                        if (k != _posts.end())
                        {
                            k->second.set_as_child(V[i].uuid, V[i].child_posts[j].post_id);
                        }
                    }
                }
                
                /* Making sure there's no duplicate post */
                bool e_found = false;
                std::map<std::string, std::vector<Post>>::iterator it = post_list.find(V[i].uuid);
                if (it != post_list.end())
                {
                    for (std::vector<Post>::iterator kt = it->second.begin(); kt != it->second.end(); kt++)
                    {
                        if (kt->get_post() == temp_post.get_post())
                        {
                            e_found = true;
                        }
                    }
                }
                if (e_found == false)
                {
                    post_list[V[i].uuid].push_back(temp_post);
                }
            }
        }
        sleep(1);   
    }
}

/* Searches through local map of remote posts to find the one we want */
void Communicator::search_response(TSN::request post_request)
{
    
    std::map<std::string, vector<Post>>::iterator it;
    std::map<std::string, vector<Post>>::iterator jt;
    /* Going through the number of requested users */
    for (unsigned int i = 0; i < post_request.user_requests.length(); i++)
    {
        /* Find the main remote user list to see if the user exist */
        it = post_list.find(post_request.user_requests[i].fulfiller_uuid);
        if (it != post_list.end())
        {
            /* Going through all the requested post from a specific user */
            for (unsigned int k = 0; k < post_request.user_requests[i].requested_posts.length(); k++)
            {
                /* Going through the local user vector of post */
                for (unsigned int n = 0; n < it->second.size(); n++)
                {
                    /* Check for matching local remote user list and remote remote list */
                    if ((unsigned int)(it->second[n].get_sn()) == post_request.user_requests[i].requested_posts[k])
                    {
                        std::cout << "Post # " << std::to_string(it->second[n].get_sn()) << " from " << it->first << std::endl;
                        std::cout <<  _list[it->first].first_name  << " " << _list[it->first].last_name << " wrote on "
                                  << F_Utility::get_date_time(it->second[n].get_time())
                                  << "    " << it->second[n].get_post() <<  "\n" << std::endl;

                        /* Child list easier to use as a temp map variable */
                        std::map<std::string, std::vector<int>> temp_c_post = it->second[n].get_child_list();
                        /* Go through all child post */
                        for (std::map<std::string, std::vector<int>>::iterator kt = temp_c_post.begin(); kt != temp_c_post.end(); kt++)
                        {
                            /* Match UUID */
                            jt = post_list.find(kt->first);
                            if (jt != post_list.end())
                            {   
                                /* Iterate through all post from specific child */
                                for (unsigned int l = 0; l < kt->second.size(); l++)
                                {
                                    /* Go through all post from specific user */
                                    for (unsigned m = 0; m < jt->second.size(); m++)
                                    {
                                        /* Display comments if matches */
                                        if ( kt->second[l] == jt->second[m].get_sn())
                                        {
                                            std::cout << "|\tPost # " << std::to_string(jt->second[m].get_sn()) << " from " << jt->first << std::endl;
                                            std::cout <<  "|\t" << _list[jt->first].first_name  << " " << _list[jt->first].last_name << " wrote on "
                                                    << F_Utility::get_date_time(jt->second[m].get_time())
                                                    << "|\t    " << jt->second[m].get_post() <<  "\n" << std::endl;                                        
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/* Function to send out private messages */
void Communicator::send_pm(std::map<std::string, TSN::user_information>::iterator it)
{
    std::string message_content;
    TSN::private_message pm_packet;    

    std::cout << "Message to send: "; 
    getline(std::cin, message_content);

    /* Prepping both sender and receiver' UUIDs to be send */
    F_Utility::prep_uuid(pm_packet.sender_uuid, _user.get_uuid());
    F_Utility::prep_uuid(pm_packet.receiver_uuid, it->first);

    /* Prepping message body into private_message to be send */
    pm_packet.message_body = DDS::string_dup(message_content.c_str());
    pm_packet.date_of_creation = F_Utility::get_raw_time();

    /* Publish private message and confirmation */
    _ops.get_dds_pm().publish(pm_packet);
    std::cout << "Private Message sent!" << std::endl;
}

/* Function to send out TSN::response */
void Communicator::send_response(TSN::request V)
{
    TSN::response user_response;
    /* Iterate through list of requested users */
    for (unsigned int j = 0; j < V.user_requests.length(); j++)
    {
        /* Check to see if owner is part of listed requested users */
        if (_user.get_uuid().compare(V.user_requests[j].fulfiller_uuid) == 0)
        {
            /* Prep owner's UUID to be send back as part of TSN::Response */
            F_Utility::prep_uuid(user_response.uuid, V.user_requests[j].fulfiller_uuid);

            for (unsigned int k = 0; k < V.user_requests[j].requested_posts.length(); k++)
            {
                /* Iterate through list of post numbers */
                user_response.post_id = V.user_requests[j].requested_posts[k];
                std::map<int, Post>::iterator it = _posts.find(user_response.post_id);
                user_response.post_body = DDS::string_dup(it->second.get_post().c_str());
                user_response.date_of_creation = it->second.get_time();
                
                /* Attach any child UUIDs and post number if post is parent */
                if (it->second.is_parent() == true)
                {
                    int child_count = 0, total_size = 0;
                    std::map<std::string, std::vector<int>> child_list = it->second.get_child_list();
                    for (std::map<std::string, std::vector<int>>::iterator l = child_list.begin(); l != child_list.end(); l++)
                    {
                        total_size += l->second.size();
                    }
                    user_response.child_posts.length(total_size);
                    for (std::map<std::string, std::vector<int>>::iterator l = child_list.begin(); l != child_list.end(); l++)
                    {
                        for (unsigned int m = 0; m < l->second.size(); m++)
                        {
                            F_Utility::prep_uuid(user_response.child_posts[child_count].owner_uuid, l->first);
                            user_response.child_posts[child_count++].post_id = l->second[m];
                        }
                    }
                }
                /* Attach any parent UUID and post numbers if post is child */
                if (it->second.is_child() == true)
                {
                    F_Utility::prep_uuid(user_response.parent_post.owner_uuid, it->second.get_puuid());
                    user_response.parent_post.post_id = it->second.get_psn();
                }
                _ops.get_dds_response().publish(user_response);
                sleep(1);
            }
        }
    }
}

/* Function to send out TSN::request */
void Communicator::send_request()
{
    int uuid_count = 1, sn_selection, uuid_size, uuid_remainder, post_size, post_remainder;
    std::string uuid_selection;
    TSN::request input_request;
    std::map<std::string, TSN::user_information>::iterator i;

    F_Utility::prep_uuid(input_request.uuid, _user.get_uuid());
    /* Get number of UUID to request from
     This is mainly to fix a bug with IDL sequence needing exact size before adding elements */
    std::cout << "How many users are you requesting from ? ";
    std::cin >> uuid_size; cin.ignore();
    /* Condition to make sure entered UUID is valid */
    if ((unsigned int)uuid_size > _list.size() || uuid_size < 1)
    {
        std::cout << "Invalid input. Number of UUID request must be between 1 and " << _list.size() << std::endl;
        return;
    }
    /* Coutdown for UUID Request */
    uuid_remainder = uuid_size;
    input_request.user_requests.length(uuid_size);
    /* Reset UUID for re-use */
    uuid_count = 0; 

    do {
        /* Select individual UUID and find it in map */
        std::cout << uuid_remainder << " selection(s) remain, Enter user's UUID (or part of it) to request: ";
        std::cin >> uuid_selection; cin.ignore();

        /* Search through the main user list for matching UUID with entered string */
        for (i = _list.begin(); i != _list.end(); i++)
        {
            size_t found = i->first.find(uuid_selection);
            if (found != std::string::npos)
            {
                int sn_count = 0;
                /* Prep UUID to be send out once found */
                F_Utility::prep_uuid(input_request.user_requests[uuid_count].fulfiller_uuid, i->first);

                std::cout << "How many posts to request:  ";
                std::cin >> post_size; cin.ignore();

                post_remainder = post_size;
                input_request.user_requests[uuid_count].requested_posts.length(post_size);

                /* Sorta Error handling for user input */
                if (post_size < 1)
                {
                    std::cout << "Invalid input. Number of posts request must be between 1 and " 
                                << i->second.number_of_highest_post << std::endl;
                    return;
                }
                else
                {
                    post_remainder = post_size;
                    input_request.user_requests[uuid_count].requested_posts.length(post_size);
                    do 
                    {
                        std::cout << post_remainder << " selection(s) remain, enter Post #:  ";
                        std::cin >> sn_selection; cin.ignore();
                        if (sn_selection >= 1)
                        {
                            input_request.user_requests[uuid_count].requested_posts[sn_count++] = sn_selection;
                        }
                        post_remainder--;
                    } while( post_remainder != 0 );
                    uuid_count++;
                }
            }
        }
        uuid_remainder--;
    } while( uuid_remainder != 0 );

    /* Only post if # of UUID request is greater than 0 */
    if (uuid_size > 0)
    {
        _ops.get_dds_request().publish(input_request);
        std::cout << "Request sent! Waiting 5 seconds before displaying responses...\n" << std::endl;
        sleep(5);
        search_response(input_request);
    }
    else
    {
        std::cout << "No UUID selected" << std::endl;
    }    
}
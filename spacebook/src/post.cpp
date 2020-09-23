#include <assert.h>
#include "post.h"

/* Constructor with no input variables */
Post::Post()
{
}

/* Find out if a child post exist */
bool Post::get_child_sn(std::string c_uuid, int c_sn)
{
    std::map<std::string, std::vector<int>>::iterator i = _child_list.find(c_uuid);
    if (i != _child_list.end())
    {
        for (int j = 0; j < i->second.size(); j++)
        {
            if (i->second[j] == c_sn)
            {
                return true;
            }
        }
    }
    return false;
}

/* If post is a child */
bool Post::is_child()
{
    return _child;
}

/* If post is a parent */
bool Post::is_parent()
{
    return _parent;
}

/* Get size of child list */
int Post::get_child_size()
{
    int total_post = 0;
    for (std::map<std::string, std::vector<int>>::iterator i = _child_list.begin(); i != _child_list.end(); i++)
    {
        for (int j = 0; j < i->second.size(); j++)
        {
            total_post++;
        }
    }
    return total_post;
}

/* Get post parent serial number */
int Post::get_psn()
{
    assert ( _psn > 0);
    return _psn;
}

/* get post serial number */
int Post::get_sn()
{
    assert ( _sn > 0);
    return _sn;
}

/* Get the whole map of child posts info */
std::map<std::string, std::vector<int>> Post::get_child_list()
{
    return _child_list;
}

/* Return post body */
std::string Post::get_post()
{
    return _post;
}

/* Return post serial number */
std::string Post::get_puuid()
{
    return _puuid;
}

/* Return post time in raw format */
time_t Post::get_time()
{
    time_t _temp = _time;
    return _temp;
}

/* Set post as child post */
void Post::set_as_child(std::string puuid, int psn)
{
    _child = true;

    _puuid = puuid;
    _psn = psn;
}

/* Set post as parent post */
void Post::set_as_parent(std::string c_uuid, int c_sn)
{
    _parent = true;  
    _child_list[c_uuid].push_back(c_sn);
}

/* Change post body. Only use for importing post info from file */
void Post::set_post(std::string post)
{
    _post = post;
}

/* Change post serial. Only use for importing post info from file */
void Post::set_sn(int sn)
{
    assert (sn > 0);
    _sn = sn;
}
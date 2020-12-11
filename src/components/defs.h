#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>
#include <cstring>
#include <map>
#include <unordered_set>
using namespace std;

// consider using a namespace here

const unsigned int MAX_BLACKLIST = 30000;
const unsigned int CLIENT_NUM = 3;
const unsigned int PROXY_NUM = 5;
const unsigned int FILTER_SIZE = (MAX_BLACKLIST / PROXY_NUM);
const unsigned int MAX_CONNECTIONS = 20;
const unsigned int SERVER_PORT = 10000;
const unsigned int PROXY_PORT[PROXY_NUM] = {10001, 10002, 10003, 10004, 10005};


unordered_set<string> PREFIXES = {"INI", "GET", "PUT", "NON", "DEN"};
const unsigned int PREFIX_NUM = PREFIXES.size();

map<string, int> create_map()
{
	map<string, int> m;
	m["proxy1"] = PROXY_PORT[0];
	m["proxy2"] = PROXY_PORT[1];
	m["proxy3"] = PROXY_PORT[2];
	m["proxy4"] = PROXY_PORT[3];
	m["proxy5"] = PROXY_PORT[4];
	return m;
}

map <string, int> PROXIES = create_map();

// Data paths
const char* OBJECT_PATH = "../../data/objects.min";
const char* BLACKLIST_PATH = "../../data/blacklist.min";
const char* PROXY_PATH[PROXY_NUM] = 
{
	"../../data/proxy1_blacklist.min",
   	"../../data/proxy2_blacklist.min",
   	"../../data/proxy3_blacklist.min",
   	"../../data/proxy4_blacklist.min",
   	"../../data/proxy5_blacklist.min"
};

const char* CLIENT_PATH[CLIENT_NUM] =
{
	"../../data/client0_requests.min",
	"../../data/client1_requests.min",
	"../../data/client2_requests.min"
};


#endif

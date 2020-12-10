#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>
#include <cstring>
#include <map>
#include <unordered_set>
using namespace std;

// consider using a namespace here

// Prefixes
unordered_set<string> PREFIXES = {"INI", "GET", "PUT", "NON", "DEN"};
const unsigned int PREFIX_NUM = PREFIXES.size();


// Total proxies
const unsigned int PROXY_NUM = 5;

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

// Proxy ports
const unsigned int SERVER_PORT = 10000;
const unsigned int PROXY_PORT[PROXY_NUM] = {10001, 10002, 10003, 10004, 10005};

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


#endif

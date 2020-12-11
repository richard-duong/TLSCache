#include <iostream>
#include <string>
using namespace std;

/* exit_on_fail
 * =========================================================
 * Objective:
 * A way to standardize debugging by exiting and printing
 * on failure of execution
 *
 * Input:
 * string name				: name of client/proxy/server
 * string issue				: which connection method to check
 * bool validity			: value to compare for pass/fail
 *
 * Output:
 * None
 *
 * Result:
 * If validity == true		: Print failure and exit
 * If validity == false		: Continue quietly
 */

void exit_on_fail(string name, string issue, bool validity){
	if(validity){
		cout << name << " - " << issue << " has detected a failure" << endl;
		exit(1);
	}
}



/* verbose_exit_on_fail
 * =========================================================
 * Objective:
 * A more verbose version of exit_on_fail(char*, char*, int)
 * that prints and exits on failure, but also prints and
 * continues on success
 *
 * Input:
 * string name				: name of client/proxy/server
 * string issue				: which connection method to check
 * bool validity			: value to compare for pass/fail
 *
 * Output:
 * None
 *
 * Result:
 * If validity == true		: Print failure and exit
 * If validity == false 	: Print success and continue
 */

void verbose_exit_on_fail(string name, string issue, bool validity){
	if(validity){
		cout << name << " - " << issue << " has detected a failure" << endl;
		exit(1);
	}
	else{
		cout << name << " - " << issue << " has ran successfully" << endl;
	}
}

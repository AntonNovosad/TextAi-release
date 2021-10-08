#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include <boost/variant.hpp>
#include <mysql.h>
#include <mysql_connection.h>

#include "getdb.h"

using namespace std;
using namespace cgicc;

int main()
{
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << getDb() << "\n";
    
    return 0;
}

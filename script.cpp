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

double antiPlagiarism(string text, string fragment);

int main()
{
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";
    
    name = form("name");
    if (!name.empty()) {
    	cout << antiPlagiarism(getDb(), name) << "\n";
    } else {
    	cout << "Text is not provided!\n";
    }	

    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}

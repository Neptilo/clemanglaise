#include "Parser.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <exception>
#include <iostream>

using namespace std;
Parser::Parser(string file_in, string file_out) {
    m_filein = file_in;
    m_fileout = file_out;
	ifstream file(m_filein.c_str());
	if (file) {
		//file_in already exists
	} else {
		//We create file_in
		ofstream file(m_filein.c_str());
		if (file) {
			//Initialize
			file << "Hello : Bonjour, Salut";
		}
	}
}

Parser::~Parser()
{
    //dtor
}

/**
*/
string Parser::getFilein(){
    return m_filein;
}
string Parser::getFileout(){
    return m_fileout;
}
/**
 *@return the numberth line of the file m_filein
 */
string Parser::getline(const unsigned int & number) const {
    string currentline("");
    unsigned int i(0);
    ifstream flow(m_filein.c_str(), ios::in);
    if (flow) {
        while( i<number && std::getline(flow, currentline)) {
            i++;
        }
    } else {
       throw string("failure while opening file "+ m_filein);
    }
    return currentline;
}

/**
 * @return the file number of lines
 */
int Parser::nblines() const {
    string s;
    unsigned int count(0);
    ifstream flow(m_filein.c_str(), ios::in);
    if (flow) {
        while(std::getline(flow,s)) ++count;
    } else {
       throw string("failure while opening file "+ m_filein);
    }
    return count;
}



/**
 * @return a random line in a file
 */
string Parser::getRandomLine() const {
    unsigned int line;
    string randomLine("");
    unsigned int lines;
    string const nomFichier(m_filein);
    try {
            lines = nblines();
            ifstream monFlux(nomFichier.c_str(), ios::in);
            if(monFlux) {
                line = (lines > 0)? rand() % lines + 1 : 0;
                monFlux.close();
                randomLine = getline(line);
            } else {
                throw string("failure while opening file "+ m_filein);
            }
        } catch (string const & e) {
            throw;
        }
        return randomLine;
}

inline string Parser::trim_right_copy(const string& s, const string& delimiters ) const {
  return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

inline string Parser::trim_left_copy(const string& s, const string& delimiters) const {
  return s.substr( s.find_first_not_of( delimiters ) );
}

inline string Parser::trim_copy(const string& s,const string& delimiters) const {
  return trim_left_copy( trim_right_copy( s, delimiters ), delimiters );
}

vector<string> &Parser::split(const string &s, char delim, vector<string> &elems) const {
    std::stringstream ss(s);
    string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(trim_copy(item));
    }
    return elems;
}


vector<string> Parser::split(const std::string &s, char delim) const {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void Parser::writeInFile(const string& text) {
    ofstream flow(m_fileout.c_str());
    if (flow) {
        flow << text;
    } else {
        throw string("Error while opening " + m_fileout);
    }
}

string Parser::get_working_path() {
	char temp[MAXPATHLEN];
	return ( getcwd(temp, MAXPATHLEN) ? std::string(temp) : std::string("") );
}
/*
int main() {
    //Initialize random numbers
    srand(time(0));
    string espace(" ");
    string endline("\n");
    Parser* parser = new Parser("/home/mbit/2A/stage/documentation/vocabulary1.rst");
    vector<string> text(parser->split(parser->getRandomLine(), ':'));
    parser->writeInFile(endline + text.at(0) + endline + text.at(1) + endline + espace + endline + endline + endline + endline);
    return 0;
}
*/

#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
const int MAXPATHLEN(256);
const std::string espace(" ");
const std::string endline("\n");

class Parser
{
    public:
        Parser(std::string file_in="file_in", std::string file_out="file_out");
        virtual ~Parser();
        /**
     *@return the numberth line of the file filename
     */
    std::string getline(const unsigned int & number) const;
    /**
     * @return the file number of lines
     */
    int nblines() const;
    /**
     * @return a random line in a file
     */
    std::string getRandomLine() const;
    inline std::string trim_right_copy(const std::string& s, const std::string& delimiters = " \f\n\r\t\v*" ) const;
    inline std::string trim_left_copy(const std::string& s, const std::string& delimiters = " \f\n\r\t\v*" ) const;
    inline std::string trim_copy(const std::string& s,const std::string& delimiters = " \f\n\r\t\v*" ) const;
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) const;
    std::vector<std::string> split(const std::string &s, char delim) const;
    std::string getFilein();
    std::string getFileout();
    void writeInFile(const std::string& text);
	void parse();
	static std::string get_working_path();


    private:
        std::string m_filein;
        std::string m_fileout;
};

#endif // PARSER_H

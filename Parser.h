#ifndef PARSER_H
#define PARSER_H
#include <QString>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
const QString prec="/";
#else
const QString prec="";
#endif
const QString endline("\n");
const QString space(" ");
class Parser
{
    public:
        Parser(QString file_in="file_in", QString file_out="file_out");
		virtual ~Parser();
		QString getline(const unsigned int & number) const;
		unsigned int nblines() const;
		QString getFilein() const;
		QString getFileout() const;
		QString getRandomLine() const;
		void parse();
		void writeInFile(const QString& text);
		static QString get_working_path();
		static QString get_working_path(const QString& file);

    private:
        QString m_filein;
        QString m_fileout;
};

#endif // PARSER_H

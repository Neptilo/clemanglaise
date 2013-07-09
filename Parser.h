#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QString>

#if defined(Q_OS_WIN) && !defined(__CYGWIN__)
const QString prec="/";
#else
const QString prec="";
#endif
const QString endline("\n");
const QString space(" ");

class Parser: public QObject {
	Q_OBJECT
    public:
		Parser(QObject* parent =0, QString file_in="file_in", QString file_out="file_out" );
		virtual ~Parser();
		QString getline(const unsigned int & number) const;
		unsigned int nblines() const;
		QString getFilein() const;
		QString getFileout() const;
		QString getRandomLine() const;
		void parse();
		void writeInFile(const QString& text);
		void appendInFile(const QString& text);
		static QString get_working_path();
		static QString get_working_path(const QString& file);

	signals:
		void appendDone();

    private:
        QString m_filein;
        QString m_fileout;


};
#endif // PARSER_H

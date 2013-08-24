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
		Parser(const QString& srcDst, QObject* parent =0, QString file_in="file_in", QString file_out="file_out");
		virtual ~Parser();
		QString getline(const unsigned int & number) const;
		unsigned int nblines() const;
		QString getFilein() const;
		QString getFileout() const;
		QString getRandomLine() const;
		QString split_line(QString line) const;
		QString search(const QString& word) const;
		void parse();
		void writeInFile(const QString& text, const QString& file);
		void appendInFile(const QString& text);
		void deleteLineId(const int & id);
		static QString get_working_path();
		static QString get_working_path(const QString& file);

	signals:
		void appendDone();

    private:
        QString m_filein;
        QString m_fileout;
		QString m_srcDst;


};
#endif // PARSER_H

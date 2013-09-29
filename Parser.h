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
		/**
		 *@return the numberth line of filename
		 */
		static QString getline(const unsigned int & number, const QString & filename);
		unsigned int nblines(const QString &files) const;
		QString getFilein() const;
		QString getFileout() const;
		QString getSrcDst() const;
		static QString getThemeFile();
		static QString getTheme(const int & id);
		QString getRandomLine() const;
		QString split_line(QString line) const;
		QString search(const QString& word, const QString &files) const;
		void parse();
		void writeInFile(const QString& text, const QString& files);
		void appendInFile(const QString& text, const QString& files);
		void deleteLineId(const int & id, const QString& files);
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

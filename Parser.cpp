#include <QDir>
#include <QFile>
#include <QTextStream>
#include "Parser.h"
Parser::Parser(QObject* parent, QString file_in, QString file_out):QObject(parent) {
    m_filein = file_in;
    m_fileout = file_out;

	QFile file(m_filein);
	if (!file.exists()) {
		//Opening file in write only mode
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		// Creation of QTextStream from QFile's object
		QTextStream flux(&file);
		// choose corresponding codec: UTF-8
		flux.setCodec("UTF-8");
		// write in file
		flux << "Hello : Bonjour, Salut";
	}
}

Parser::~Parser()
{
    //dtor
}

QString Parser::getFilein() const {
    return m_filein;
}

QString Parser::getFileout() const{
    return m_fileout;
}

/**
 *@return the numberth line of the file m_filein
 */
QString Parser::getline(const unsigned int & number) const {
    QString currentline("");
    unsigned int i(0);
	QFile file(m_filein);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream flux(&file);
	while( i<number && !flux.atEnd()) {
		i++;
		currentline = flux.readLine();
	}
    return currentline;
}

unsigned int Parser::nblines() const {
    QString s;
    unsigned int count(0);
	QFile file(m_filein);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream flux(&file);

	while(!flux.atEnd()) {
		flux.readLine();
		++count;
	}
    return count;
}

QString Parser::getRandomLine() const {
    unsigned int line;
    unsigned int lines(nblines());
    QString const nomFichier(m_filein);
	QFile file(m_filein);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream flux(&file);
	line = (lines > 0)? rand() % lines + 1 : 0;
	file.close();
	return getline(line);
}

void Parser::appendInFile(const QString& text) {
	QFile file(m_filein);
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return;
	// Creation of QTextStream from QFile object
	QTextStream flux(&file);
	// choose codec UTF-8
	flux.setCodec("UTF-8");
	// write lines into file
	flux << text;
	emit appendDone();
}
void Parser::writeInFile(const QString& text) {
	QFile file(m_fileout);
	//Open file in write only
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	// Creation of QTextStream from QFile object
	QTextStream flux(&file);
	// choose codec UTF-8
	flux.setCodec("UTF-8");
	// write lines into file
	flux << text;
}

void Parser::parse() {
    QStringList text = getRandomLine().split(QRegExp("\\s*:\\s*"));
	writeInFile(endline + text[0] + endline + text[1] + endline + space + endline + endline + endline + endline); 
}

QString Parser::get_working_path() {
	return QDir::currentPath(); 
}

QString Parser::get_working_path(const QString & file) {
	return "file://" + prec + Parser::get_working_path() + "/" + file; 
}

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
		flux << "Hello : Bonjour, Salut"<<endl;
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
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
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
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return -1;
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
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
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
	QString real_text = split_line(getRandomLine());
	writeInFile(real_text);
}

QString Parser::split_line(QString line) const {
    QStringList text = line.split(QRegExp("\\s*:\\s*"));
	QString defaultText("");
	int l(text.size());
	int MAX(6);
	QStringList temp;
	for(int i=0; i<l; i++){
		temp << text[i];
	}
	for(int j = l; j<MAX; j++){
		temp<< defaultText;
	}
	/*
	 * temp[0] = name
	 * temp[1] = meaning
	 * temp[2] = nature
	 * temp[3] = comment
	 * temp[4] = example
	 * temp[5] = pronunciation
	 * temp[6] = score //non géré
	 */  
    	
	QString real_text(endline);
	for(int k=0; k<MAX; k++) {
		real_text += temp[k] + endline;
	}
	return real_text;
}

QString Parser::get_working_path() {
	return QDir::currentPath(); 
}

QString Parser::get_working_path(const QString & file) {
	return "file://" + prec + Parser::get_working_path() + "/" + file; 
}

QString Parser::search(const QString& word) const {
	QString currentline("");
	QString result("");
	QFile file(m_filein);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	QTextStream flux(&file);
	while(!flux.atEnd()) {
		currentline = flux.readLine();
		if(currentline.contains(word, Qt::CaseInsensitive))
			result += split_line(currentline); 	
	}
	return result;
}

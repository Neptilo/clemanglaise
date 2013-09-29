#include <iostream>

#include <QDir>
#include <QFile>
#include <QTextStream>

#include "Parser.h"

Parser::Parser(const QString& srcDst, QObject* parent, QString file_in, QString file_out):QObject(parent) {
	m_srcDst = srcDst;
    m_filein = m_srcDst + "/" + file_in;
    m_fileout = m_srcDst + "/" + file_out;
	QDir dir(m_srcDst);
	if (!dir.exists()) {
		dir.mkpath(".");
	}
	
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
		flux << 1 << " : Ball : Balle, Ballon : n" << endl;
	}
	
}

Parser::~Parser()
{
    //dtor
}

QString Parser::getFilein() const {
    return m_filein;
}

QString Parser::getSrcDst() const {
	return m_srcDst;
}

QString Parser::getFileout() const{
    return m_fileout;
}

QString Parser::getThemeFile() {
	return "themes";
}

QString Parser::getTheme(const int & id) {
	QString theme("");
	QStringList list = getline(id, getThemeFile()).split(QRegExp("\\s*:\\s*"));
    if (list.count()>1) {
		theme = list.at(1);
	}
	return theme;
}

QString Parser::getline(const unsigned int & number, const QString & filename) {
    QString currentline("");
    unsigned int i(0);
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	QTextStream flux(&file);
	while( i<number && !flux.atEnd()) {
		i++;
		currentline = flux.readLine();
	}
    return currentline;
}

unsigned int Parser::nblines(const QString& files) const {
    QString s;
    unsigned int count(0);
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return -1;
	QTextStream flux(&file);

	while(!flux.atEnd()) {
		flux.readLine();
		++count;
	}
    return count;
}

QString Parser::getRandomLine(const QString& files) const {
    unsigned int line;
    unsigned int lines(nblines(files));
    QString const nomFichier(files);
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	QTextStream flux(&file);
	line = (lines > 0)? rand() % lines + 1 : 0;
	file.close();
	return getline(line, files);
}

void Parser::appendInFile(const QString& text, const QString& files) {
	QFile file(files);
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return;
	// Creation of QTextStream from QFile object
	QTextStream flux(&file);
	// choose codec UTF-8
	flux.setCodec("UTF-8");
	// write lines into file
    int id = getline(nblines(files), files).split(QRegExp("\\s*:\\s*")).at(0).toInt();
	flux << ++id << " : " << text;
	emit appendDone();
}

void Parser::deleteLineId(const int & id, const QString& files) {
	QString currentline("");
	bool ok = false;
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream flux0(&file);
	QStringList file_list = flux0.readAll().split(endline);
	file.close();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream flux(&file);
	while(!flux.atEnd() && !ok) {
		currentline = flux.readLine();
		if(currentline.split(QRegExp("\\s*:\\s*")).at(0).toInt() == id){
			file_list.removeOne(currentline);
			ok = true;
		}
	}
	file.close();
	file.resize(0);
	
	QString text("");
	for(int i=0, l = file_list.size(); i<l; i++) {
		text += (i==l-1)?file_list.at(i) : file_list.at(i) + endline;
	} 

	writeInFile(text, files); 
}

void Parser::writeInFile(const QString& text, const QString & files) {
	QFile file(files);
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

void Parser::parse(const QString &files) {
	QString real_text = split_line(getRandomLine(files));
	writeInFile(real_text, m_fileout);
}

QString Parser::split_line(QString line) const {
    QStringList text = line.split(QRegExp("\\s*:\\s*"));
	QString defaultText("");
	int l(text.size());
	int MAX(10);
	QStringList temp;
	for(int i=0; i<l; i++){
		temp << text[i];
	}
	for(int j = l; j<MAX; j++){
		temp << defaultText;
	}
	/*
	 * temp[0] = id
	 * temp[1] = name
	 * temp[2] = meaning
	 * temp[3] = nature
	 * temp[4] = comment
	 * temp[5] = example
	 * temp[6] = id_theme
	 * temp[7] = pronunciation
	 * temp[8] = score
	 * temp[9] = theme
	 */  
    	
	QString real_text("");
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

QString Parser::search(const QString& word, const QString& files) const {
	QString currentline("");
	QString result("");
	QFile file(files);
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

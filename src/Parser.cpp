#include <iostream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "Parser.h"

Parser::Parser(const QString& srcDst, QString file_in, QString file_out, QString score_file, QObject* parent): QObject(parent) {
	m_srcDst = srcDst;
	m_filein = m_srcDst + "/" + file_in;
	m_fileout = m_srcDst + "/" + file_out;
	m_scorefile = m_srcDst + "/" + score_file;
	QDir dir(m_srcDst);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	QFile file(m_filein);
	if (!file.exists() || file.size()==0) {
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

	QFile file2(m_scorefile);
	if (!file2.exists() || file2.size()==0) {
		//Opening file in write only mode
		if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		// Creation of QTextStream from QFile's object
		QTextStream flux(&file2);
		// choose corresponding codec: UTF-8
		flux.setCodec("UTF-8");
		// write in file
		flux << 1 << " : 0 : 0" << endl;
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

QString Parser::getScoreFile() const{
	return m_scorefile;
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
	flux << ++id << " : " << text << endl;
	emit appendDone();
}

void Parser::addInFile(const QString& text, const QString& files) {
	QFile file(files);
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return;
	// Creation of QTextStream from QFile object
	QTextStream flux(&file);
	// choose codec UTF-8
	flux.setCodec("UTF-8");
	// write lines into file
	flux << text << endl;
	emit appendDone();
}

void Parser::deleteLineId(const int & id, const QString& files) {
	QString currentline("");
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream flux0(&file);
	QStringList file_list = flux0.readAll().split(endline);
	for (int i=0, l = file_list.size(); i<l; i++){
		currentline = file_list[i];
		if(currentline.split(QRegExp("\\s*:\\s*")).at(0).toInt() == id){
			file_list.removeOne(currentline);
			break;
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

QString Parser::getLineId(const int &id, const QString& files) {
	QString currentline("");
	bool ok = false;
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";

	QTextStream flux(&file);
	while(!flux.atEnd()&& !ok) {
		currentline = flux.readLine();
		if(currentline.split(QRegExp("\\s*:\\s*")).at(0).toInt() == id){
			ok =true;
		}
	}
	return currentline;
}


int Parser::getLastId(const QString& files)
{
	QString currentline("");
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return 0;

	QTextStream flux(&file);
	while(!flux.atEnd())
		currentline = flux.readLine();

	return currentline.split(QRegExp("\\s*:\\s*")).at(0).toInt();

}
void Parser::updateLineId(const int &id, const QString& new_text, const QString& files) {
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream flux(&file);
	QStringList file_list = flux.readAll().split(endline);
	int textid;
	for (int i=0, l = file_list.size(); i<l; i++){
		QString & currentline = file_list[i];//la modif de l'un => modif de l'autre
		if((textid=currentline.split(QRegExp("\\s*:\\s*")).at(0).toInt()) == id){
			currentline = QString::number(id) + " : " + new_text;	
			break;
		}
	}
	file.close();
	file.resize(0);
	QString text("");
	for(int i=0, l = file_list.size(); i<l; i++) {
		text += (i==l-1)?file_list.at(i) : file_list.at(i) + endline;
	}

	writeInFile(text, files);
	emit appendDone();
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

QString Parser::split_line(QString line, const QString& delim) const {
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
		real_text += temp[k] + delim;
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

void Parser::parse(const QString& files) {
	float min (0.99);
	QList<int> min_line_list;
	QString currentline;
	QFile file(files);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream flux(&file);
	while(!flux.atEnd()){
		currentline = flux.readLine();
		QStringList list = currentline.split(QRegExp("\\s*:\\s*"));
		int l = list.size();
		float idx = (l>8)?list.at(8).toFloat():0;
		if (idx < min) {
			min = idx;
			min_line_list.clear();
			min_line_list << list.at(0).toInt();
		} else if (idx == min) {
			min_line_list << list.at(0).toInt();
		}
	}
	file.close();

	int id = rand() % min_line_list.size();
	QString real_text = split_line(getLineId(min_line_list[id], files));
	writeInFile(real_text, m_fileout);

}

void Parser::update_score(const int& id, const int& correct) {
	int total = getCellId(id, 1, getScoreFile()).toInt() + 1;
	int nbcorrect = getCellId(id, 2, getScoreFile()).toInt() + correct;
	float score = (float)nbcorrect / (float)(total +1);
	QString new_score_text = QString::number(total) + " : " + QString::number(nbcorrect);
	updateLineId(id,new_score_text, getScoreFile());

	QString line = split_line(getLineId(id, getFilein()), " : ");
	QStringList list = line.split(QRegExp("\\s*:\\s*"));
	QString new_score = QString::number(score, 'f', 2); //arrondi à 2 chiffres après la virgule

	list.replace(8,new_score);	
	QString text("");
	for(int i=1, l = list.size(); i<l; i++) {
		text += (i==l-1)?list.at(i) : list.at(i) + " : ";
	}
	updateLineId(id, text,getFilein());
}

QString Parser::getCellId(const int& id, const int& nb, const QString& files) {
	QStringList list = getLineId(id, files).split(QRegExp("\\s*:\\s*"));
	return (list.size()>nb)?list.at(nb):"";
}

QString Parser::getScoreId(const int& id) {
	return getCellId(id, 8,getFilein());
}



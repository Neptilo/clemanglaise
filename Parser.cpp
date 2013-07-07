#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "Parser.h"
Parser::Parser(QString file_in, QString file_out) {
    m_filein = file_in;
    m_fileout = file_out;

	QFile file(m_filein);
	if (!file.exists()) {
		//Ouverture du fichier en écriture seule
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		// Création d'un objet QTextStream à partir de notre objet QFile
		QTextStream flux(&file);
		// On choisit le codec correspondant au jeu de caractère que l'on souhaite ; ici, UTF-8
		flux.setCodec("UTF-8");
		// Écriture des différentes lignes dans le fichier
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

void Parser::writeInFile(const QString& text) {
	QFile file(m_fileout);
	//Ouverture du fichier en écriture seule
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	// Création d'un objet QTextStream à partir de notre objet QFile
	QTextStream flux(&file);
	// On choisit le codec correspondant au jeu de caractère que l'on souhaite ; ici, UTF-8
	flux.setCodec("UTF-8");
	// Écriture des différentes lignes dans le fichier
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
	return Parser::get_working_path() + slash + file; 
}

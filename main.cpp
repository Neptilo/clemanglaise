#include <ctime>
#include <string>
#include <iostream>
#if defined(Q_OS_WIN)
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <QApplication>
#include <QPushButton>
#include <QObject>

#include "homewindow.h"
#include "networkreplyreader.h"

using namespace std;

string tr(const char* s){
#if defined(Q_OS_WIN) && !defined(__CYGWIN__)
    return QObject::tr(s).toLocal8Bit().constData();
#else
    return QObject::tr(s).toUtf8().constData();
#endif
}

int main(int argc, char *argv[])
{

    // Read command line arguments
    bool use_password(false);
    string password;
    for(int i=1; i<argc; i++){
        string arg = argv[i];
        if(arg == "-p" || arg == "--password"){
            use_password = true;
            if((++i)<argc){
                password = argv[i];
            }else{
                cout << tr("Enter password: ");

                // Hide tty input and read it
#if defined(Q_OS_WIN) && !defined(__CYGWIN__)
               /* HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
                DWORD mode = 0;
                GetConsoleMode(hStdin, &mode);
                SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
                cin >> password;
                SetConsoleMode(hStdin, mode);*/
#else
                termios oldt;
                tcgetattr(STDIN_FILENO, &oldt);
                termios newt = oldt;
                newt.c_lflag &= ~ECHO;
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                cin >> password;
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                cout << endl;
#endif

            }
        }else if(arg == "-h" || arg == "-?" || arg == "--help"){
            cout << tr("Usage: ") << argv[0] << tr(" [OPTION]") << endl << endl;
            cout << "\t-h, -?, --help\t\t\t" << tr("Display this help and exit") << endl;
            cout << "\t-p, --password " << tr("[PASSWORD]\tPassword to connect as administrator. If password is not given it's asked from the tty.") << endl;
            return 0;
        }else{
            cerr << tr("unknown option '") << arg << tr("'") << endl;
            cerr << tr("Try '") << argv[0] << " --help" << tr("' for more information") << endl;
            return 1;
        }
    }

    QApplication a(argc, argv); // Needs to be done before using any signals

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    srand(time(0));

    if(use_password){

        // Connect as administrator
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif
        post_data.addQueryItem("password", password.c_str());
        const QUrl url("http://neptilo.com/php/clemanglaise/login.php");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkAccessManager* nam = new QNetworkAccessManager(); // TODO: To delete later
        nam->setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
        nam->cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.
        NetworkReplyReader* reply_reader = new NetworkReplyReader(); // TODO: To delete later

        // Will show confirmation when loading of reply is finished
        QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), reply_reader, SLOT(read_reply(QNetworkReply*)));

        // Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        nam->post(request, post_data.encodedQuery());
#else
        nam->post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif

        // Then we wait for the NetworkReplyReader to answer yes.
    }else{
        HomeWindow* w = new HomeWindow(false);
        w->show();
    }

	return a.exec();
}

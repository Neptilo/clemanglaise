#include <ctime>
#include <string>
#include <iostream>

#include <QApplication>
#if defined(Q_OS_WIN) // Q_OS_WIN may only exist after the first Qt inclusion
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <QFontDatabase>
#include <QPushButton>
#include <QObject>

#if defined(Q_OS_ANDROID)
#include "AndroidStyle.h"
#endif
#include "HomeView.h"
#include "NetworkReplyReader.h"
#include "string_utils.h"

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

    srand(time(nullptr));

    // Before the conditional because it has to be still there during the
    // execution of the app after the conditional.
    NetworkReplyReader reply_reader(nullptr);

    if (!QSslSocket::supportsSsl()){
        cerr << tr("The SSL libraries could not be loaded. Please make sure "
                   "to follow the setup instructions in the README.") << endl;
        return 2;
        // To show the version of the SSL libraries currently loaded,
        // use QSslSocket::sslLibraryBuildVersionString()
        // and QSslSocket::sslLibraryVersionString().
    }

    if(use_password){

        // Connect as administrator
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif
        post_data.addQueryItem("password", password.c_str());
        const QUrl url("https://neptilo.com/php/clemanglaise/login.php");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        // Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        nam.post(request, post_data.encodedQuery());
#else
        QNetworkReply* reply = NetworkReplyReader::nam->post(
                    request, post_data.query().toUtf8());
#endif

        // Will show confirmation when loading of reply is finished
        QObject::connect(reply, SIGNAL(finished()),
                         &reply_reader, SLOT(read_reply()));

        // Then we wait for the NetworkReplyReader to answer yes.
    }else{
        HomeView *w = new HomeView(false);
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        w->show();
    }

    return a.exec();
}

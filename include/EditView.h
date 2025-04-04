#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPushButton>
#include <QFormLayout>
#include <QComboBox>
#include <QListWidget>
#include <QTextEdit>

#include "CheckableComboBox.h"
#include "Test.h"
#ifndef Q_OS_WASM
#include "DatabaseManager.h"
#endif

class EditView : public QWidget{
    Q_OBJECT

public:
    EditView(Test *test,
             const QString &title,
             const QHash<QString, QString> &default_values,
             const QString &OK_button_value,
             const QString &php_filename,
             const QString &success_message,
         #ifndef Q_OS_WASM
             DatabaseManager *database_manager,
         #endif
             QWidget *parent = nullptr);
    ~EditView();
    void read_reply(QString reply_string);
    void disable_edition(bool ok);

private:
    QList<int> selected_tags;
    QLabel* title;
    QLabel* status;
    QComboBox* nature_edit;
    QComboBox *gender_edit;
    CheckableComboBox* tags_box;

    QLineEdit* word_edit;
    QLineEdit* meaning_edit;
    QLineEdit* pronunciation_edit;
    QTextEdit* comment_edit;
    QTextEdit* example_edit;
    QTextEdit* hint_edit;
    QPushButton* OK_button;
    QPushButton* continue_button;
    QFormLayout* layout;
    QString php_filename;
    QHash<QString, QString> default_values;
    // list of tags
    // Elements at even indices are the tag ids and elements at odd indices are their corresponding names.
    QStringList reply_list;
    QString success_message;
#ifndef Q_OS_WASM
    DatabaseManager *database_manager;
#endif
    Test *test;
    // This method is called after a word has been successfully added or edited.
    // It updates attributes and the UI to allow the user to add another word. In particular it prepares for an "add" behavior instead of "update".
    void prepare_to_continue();

public slots:
    void edit_word();
    void read_reply();
    void show_confirmation();
#ifndef Q_OS_WASM
    void show_confirmation(bool success);
#endif
    void reset();
    void find_tags();

private slots:
    void update_gender(int index);
    void update_selected_tags(QModelIndex top_left, QModelIndex bottom_right);
};
#endif // EDITVIEW_H

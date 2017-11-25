#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTextCodec>
#include <QtGui>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void New();
    void OpenFile();
    void OpenFileTxt();
    void SaveAsFile();
    void About();
    void Message();
    void Copy();
    void Cut();
    void Paste();
    void Undo();
    void Redo();
    void Format();
    void KeyGeneration();

protected:
    void closeEvent(QCloseEvent *event);

private:


    void createActions();
    void createMenus();

    QString fileName;

    QTextEdit *editor;

    QMenu *FileMenu;
    QMenu *EditMenu;
    QMenu *HelpMenu;
    QMenu *CryptMenu;

    QAction *NewAct;
    QAction *OpenFileAct;
    QAction *OpenFileTxtAct;
    QAction *SaveAsFileAct;
    QAction *ExitAct;
    QAction *AboutAct;
    QAction *CopyAct;
    QAction *CutAct;
    QAction *PasteAct;
    QAction *UndoAct;
    QAction *RedoAct;
    QAction *FormatAct;
    QAction *KeyGenerationAct;
};

#endif // MAINWINDOW_H

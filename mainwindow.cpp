#include "RSA.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();



    editor = new QTextEdit(this);
    setCentralWidget(editor);

    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);

    setWindowTitle(tr("RSA Crypt"));
    setMinimumSize (160, 160);
    resize(700, 300);
}

/////////////////////////////////////////////////////////////////////////
//Основные Функции//
//Функция открытия зашифрованного файла
void MainWindow::OpenFile()
{

    if (editor->document()->isModified())
        Message();
    fileName = "";
    //путь и имя файла для открытия
    fileName = QFileDialog::getOpenFileName(this,
                         tr("Открытие"), QString(),
                         ("Document (*.RSA);; All Files (*.*)"));
    if (!fileName.isEmpty())
    {
        //путь и имя файла с закрытым ключом
        QString KeyName = QFileDialog::getOpenFileName(this,
                                                       tr("Открытие"), QString(),
                                                       ("Document (*.Key);; All Files (*.*)"));
        if (!KeyName.isEmpty())
        {
            Decoding(fileName, KeyName);
            QString fileNametmp = fileName + ".tmp";
            QFile file(fileNametmp);
            if ( file.open( QIODevice::ReadOnly ) )
            {
                    QTextStream stream( &file );
                    editor->setText( stream.readAll() );
                    //удаление временного файла
                    QFile::remove(fileNametmp);
                    file.remove();
            }
        }
    }
}
//Функция открытия обычного файла
void MainWindow::OpenFileTxt()
{
    if (editor->document()->isModified())
        Message();
    fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
                         tr("Открытие"), QString(),
                         ("Document (*.txt);; All Files (*.*)"));
    if (!fileName.isEmpty())
    {
        QFile file( fileName );
        if ( file.open( QIODevice::ReadOnly ) )
        {
                QTextStream stream( &file );
                editor->setText( stream.readAll() );
        }
        file.close();
    }
}

//Функция сохранения файла по выбору
void MainWindow::SaveAsFile()
{
    fileName = "";
    // путь и имя файла для сохранения
    fileName = QFileDialog::getSaveFileName(this,
                         tr("Сохранить как"), QString(),
                         ("Document (*.RSA);; All Files (*.*)"));
    if (!fileName.isEmpty())
    {
        QString s;
        s = editor->toPlainText().toLocal8Bit();
        //путь и имя файла с открытым ключом
        QString KeyName = QFileDialog::getOpenFileName(this,
                             tr("Открытие"), QString(),
                             ("Document (*.Key);; All Files (*.*)"));
        if (!KeyName.isEmpty())
            Encrypting(fileName, s, KeyName);
    }
}
//Функция создания нового файла
void MainWindow::New()
{

    if (editor->document()->isModified())
        Message();
    editor->clear();
    fileName = "";
}
//Фунция вывода на экран сообщения о создателях программы
void MainWindow::About()
{
    QMessageBox::information(this, tr("О программе"),
                             tr("Эту программу создал<p>"
                                "<b>Дмитрий Шаблов</b>"), QMessageBox::Yes);
}
//Событие закрытие окна
void MainWindow::closeEvent(QCloseEvent *event)
{
    int message = QMessageBox::question(
            this , tr("RSA Crypt"), tr("Выйти из программы?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    if (message == QMessageBox::Yes)
    {
        event->accept();
        if (editor->document()->isModified())
            Message();
    }
    if (message == QMessageBox::No)
    {
        event->ignore();
    }
}
//Сообщение о сохранении
void MainWindow::Message()
{
    int message = QMessageBox::question(
            this , tr("RSA Crypt"), tr("Сохранить Файл?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    if (message == QMessageBox::No)
    {

    }
    else
    {
        SaveAsFile();
    }
}
//Функция копирования текста
void MainWindow::Copy()
{
   editor->copy();
}
//Функция вырезания текста
void MainWindow::Cut()
{
   editor->cut();
}
//Функция вставки текста
void MainWindow::Paste()
{
   editor->paste();
}
//Функция возвращение на один шаг назад
void MainWindow::Undo()
{
    editor->undo();
}
//Функция возвращение на один шаг вперед
void MainWindow::Redo()
{
    editor->redo();
}
///////////////////////////////////////////////////////////////////////////
//Фуннкции изменения формата текста//
void MainWindow::Format()
{
    QFont font;
    bool ok;
    font = QFontDialog::getFont(&ok, this);
    editor->setFont(font);
}
///////////////////////////////////////////////////////////////////////////
//Кнопки//
void MainWindow::createActions()
{
    //Меню Файловой системы с открытием, сохранением и т.д.
    //Кнопка открытия зашифрованного файла
    OpenFileAct = new QAction(tr("&Open"), this);
    OpenFileAct->setShortcuts(QKeySequence::Open);
    OpenFileAct->setStatusTip(tr("Open a file"));
    connect(OpenFileAct, SIGNAL(triggered()), this, SLOT(OpenFile()));
    //
    //Кнопка открытия обычного файла
    OpenFileTxtAct = new QAction(tr("Open ordinary"), this);
    connect(OpenFileTxtAct, SIGNAL(triggered()), this, SLOT(OpenFileTxt()));
    //
    //Кнопка сохранения как
    SaveAsFileAct = new QAction(tr("S&ave As"), this);
    SaveAsFileAct->setShortcuts(QKeySequence::SaveAs);
    SaveAsFileAct->setStatusTip(tr("SaveAs a file"));
    connect(SaveAsFileAct, SIGNAL(triggered()), this, SLOT(SaveAsFile()));
    //
    //Кнопка выхода
    ExitAct = new QAction(tr("E&xit"), this);
    ExitAct->setShortcuts(QKeySequence::Quit);
    ExitAct->setStatusTip(tr("Exit the application"));
    connect(ExitAct, SIGNAL(triggered()), this, SLOT(close()));
    //
    //Кнопка создания нового файла
    NewAct = new QAction(tr("&New"), this);
    NewAct->setShortcuts(QKeySequence::New);
    NewAct->setStatusTip(tr("Create a new file"));
    connect(NewAct, SIGNAL(triggered()), this, SLOT(New()));
    //
    //Меню помощи
    //Кнопка о программе
    AboutAct = new QAction(tr("About"), this);
    AboutAct->setShortcuts(QKeySequence::HelpContents);
    AboutAct->setStatusTip(tr("About this programm"));
    connect(AboutAct, SIGNAL(triggered()), this, SLOT(About()));
    //
    //
    //Меню редактирования текста
    //Кнопка копирования
    CopyAct = new QAction(tr("Copy"), this);
    CopyAct->setShortcuts(QKeySequence::Copy);
    CopyAct->setStatusTip(tr("Copy a text"));
    connect(CopyAct, SIGNAL(triggered()), this, SLOT(Copy()));
    //Кнопка вырезания
    CutAct = new QAction(tr("Cut"), this);
    CutAct->setShortcuts(QKeySequence::Cut);
    CutAct->setStatusTip(tr("Cut a text"));
    connect(CutAct, SIGNAL(triggered()), this, SLOT(Cut()));
    //Кнопка вставки
    PasteAct = new QAction(tr("Paste"), this);
    PasteAct->setShortcuts(QKeySequence::Paste);
    PasteAct->setStatusTip(tr("Paste a text"));
    connect(PasteAct, SIGNAL(triggered()), this, SLOT(Paste()));
    //Кнопка возвращения на один шаг назад
    UndoAct = new QAction(tr("Undo"), this);
    UndoAct->setShortcuts(QKeySequence::Undo);
    UndoAct->setStatusTip(tr("Undo"));
    connect(UndoAct, SIGNAL(triggered()), this, SLOT(Undo()));
    //Кнопка возвращения на один шаг вперед
    RedoAct = new QAction(tr("Redo"), this);
    RedoAct->setShortcuts(QKeySequence::Redo);
    RedoAct->setStatusTip(tr("Redo"));
    connect(RedoAct, SIGNAL(triggered()), this, SLOT(Redo()));
///////////////////////////////////////////////////////////////////
    //Кнопка изменения формата текста//
    FormatAct = new QAction(tr("&Format"), this);
    FormatAct->setStatusTip(tr("Format"));
    connect(FormatAct, SIGNAL(triggered()), this, SLOT(Format()));
///////////////////////////////////////////////////////////////////
    //Кнопка создания ключей//
    KeyGenerationAct = new QAction(tr("Generation"), this);
    KeyGenerationAct->setStatusTip(tr("Генерация ключей"));
    connect(KeyGenerationAct, SIGNAL(triggered()), this, SLOT(KeyGeneration()));
}
///////////////////////////////////////////////////////////////////
//Верхнее меню//
void MainWindow::createMenus()
{
    FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(NewAct);
    FileMenu->addAction(OpenFileAct);
    FileMenu->addAction(OpenFileTxtAct);
    FileMenu->addAction(SaveAsFileAct);
    FileMenu->addSeparator();
    FileMenu->addAction(ExitAct);

    EditMenu = menuBar()->addMenu((tr("&Edit")));
    EditMenu->addAction(UndoAct);
    EditMenu->addAction(RedoAct);
    EditMenu->addSeparator();
    EditMenu->addAction(CopyAct);
    EditMenu->addAction(CutAct);
    EditMenu->addAction(PasteAct);
    EditMenu->addSeparator();
    EditMenu->addAction(FormatAct);

    CryptMenu = menuBar()->addMenu("Crypt");
    CryptMenu->addAction(KeyGenerationAct);

    HelpMenu = menuBar()->addMenu(tr("&Help"));
    HelpMenu->addAction(AboutAct);
}

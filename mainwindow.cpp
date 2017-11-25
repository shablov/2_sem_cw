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
//�������� �������//
//������� �������� �������������� �����
void MainWindow::OpenFile()
{

    if (editor->document()->isModified())
        Message();
    fileName = "";
    //���� � ��� ����� ��� ��������
    fileName = QFileDialog::getOpenFileName(this,
                         tr("��������"), QString(),
                         ("Document (*.RSA);; All Files (*.*)"));
    if (!fileName.isEmpty())
    {
        //���� � ��� ����� � �������� ������
        QString KeyName = QFileDialog::getOpenFileName(this,
                                                       tr("��������"), QString(),
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
                    //�������� ���������� �����
                    QFile::remove(fileNametmp);
                    file.remove();
            }
        }
    }
}
//������� �������� �������� �����
void MainWindow::OpenFileTxt()
{
    if (editor->document()->isModified())
        Message();
    fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
                         tr("��������"), QString(),
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

//������� ���������� ����� �� ������
void MainWindow::SaveAsFile()
{
    fileName = "";
    // ���� � ��� ����� ��� ����������
    fileName = QFileDialog::getSaveFileName(this,
                         tr("��������� ���"), QString(),
                         ("Document (*.RSA);; All Files (*.*)"));
    if (!fileName.isEmpty())
    {
        QString s;
        s = editor->toPlainText().toLocal8Bit();
        //���� � ��� ����� � �������� ������
        QString KeyName = QFileDialog::getOpenFileName(this,
                             tr("��������"), QString(),
                             ("Document (*.Key);; All Files (*.*)"));
        if (!KeyName.isEmpty())
            Encrypting(fileName, s, KeyName);
    }
}
//������� �������� ������ �����
void MainWindow::New()
{

    if (editor->document()->isModified())
        Message();
    editor->clear();
    fileName = "";
}
//������ ������ �� ����� ��������� � ���������� ���������
void MainWindow::About()
{
    QMessageBox::information(this, tr("� ���������"),
                             tr("��� ��������� ������<p>"
                                "<b>������� ������</b>"), QMessageBox::Yes);
}
//������� �������� ����
void MainWindow::closeEvent(QCloseEvent *event)
{
    int message = QMessageBox::question(
            this , tr("RSA Crypt"), tr("����� �� ���������?"),
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
//��������� � ����������
void MainWindow::Message()
{
    int message = QMessageBox::question(
            this , tr("RSA Crypt"), tr("��������� ����?"),
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
//������� ����������� ������
void MainWindow::Copy()
{
   editor->copy();
}
//������� ��������� ������
void MainWindow::Cut()
{
   editor->cut();
}
//������� ������� ������
void MainWindow::Paste()
{
   editor->paste();
}
//������� ����������� �� ���� ��� �����
void MainWindow::Undo()
{
    editor->undo();
}
//������� ����������� �� ���� ��� ������
void MainWindow::Redo()
{
    editor->redo();
}
///////////////////////////////////////////////////////////////////////////
//�������� ��������� ������� ������//
void MainWindow::Format()
{
    QFont font;
    bool ok;
    font = QFontDialog::getFont(&ok, this);
    editor->setFont(font);
}
///////////////////////////////////////////////////////////////////////////
//������//
void MainWindow::createActions()
{
    //���� �������� ������� � ���������, ����������� � �.�.
    //������ �������� �������������� �����
    OpenFileAct = new QAction(tr("&Open"), this);
    OpenFileAct->setShortcuts(QKeySequence::Open);
    OpenFileAct->setStatusTip(tr("Open a file"));
    connect(OpenFileAct, SIGNAL(triggered()), this, SLOT(OpenFile()));
    //
    //������ �������� �������� �����
    OpenFileTxtAct = new QAction(tr("Open ordinary"), this);
    connect(OpenFileTxtAct, SIGNAL(triggered()), this, SLOT(OpenFileTxt()));
    //
    //������ ���������� ���
    SaveAsFileAct = new QAction(tr("S&ave As"), this);
    SaveAsFileAct->setShortcuts(QKeySequence::SaveAs);
    SaveAsFileAct->setStatusTip(tr("SaveAs a file"));
    connect(SaveAsFileAct, SIGNAL(triggered()), this, SLOT(SaveAsFile()));
    //
    //������ ������
    ExitAct = new QAction(tr("E&xit"), this);
    ExitAct->setShortcuts(QKeySequence::Quit);
    ExitAct->setStatusTip(tr("Exit the application"));
    connect(ExitAct, SIGNAL(triggered()), this, SLOT(close()));
    //
    //������ �������� ������ �����
    NewAct = new QAction(tr("&New"), this);
    NewAct->setShortcuts(QKeySequence::New);
    NewAct->setStatusTip(tr("Create a new file"));
    connect(NewAct, SIGNAL(triggered()), this, SLOT(New()));
    //
    //���� ������
    //������ � ���������
    AboutAct = new QAction(tr("About"), this);
    AboutAct->setShortcuts(QKeySequence::HelpContents);
    AboutAct->setStatusTip(tr("About this programm"));
    connect(AboutAct, SIGNAL(triggered()), this, SLOT(About()));
    //
    //
    //���� �������������� ������
    //������ �����������
    CopyAct = new QAction(tr("Copy"), this);
    CopyAct->setShortcuts(QKeySequence::Copy);
    CopyAct->setStatusTip(tr("Copy a text"));
    connect(CopyAct, SIGNAL(triggered()), this, SLOT(Copy()));
    //������ ���������
    CutAct = new QAction(tr("Cut"), this);
    CutAct->setShortcuts(QKeySequence::Cut);
    CutAct->setStatusTip(tr("Cut a text"));
    connect(CutAct, SIGNAL(triggered()), this, SLOT(Cut()));
    //������ �������
    PasteAct = new QAction(tr("Paste"), this);
    PasteAct->setShortcuts(QKeySequence::Paste);
    PasteAct->setStatusTip(tr("Paste a text"));
    connect(PasteAct, SIGNAL(triggered()), this, SLOT(Paste()));
    //������ ����������� �� ���� ��� �����
    UndoAct = new QAction(tr("Undo"), this);
    UndoAct->setShortcuts(QKeySequence::Undo);
    UndoAct->setStatusTip(tr("Undo"));
    connect(UndoAct, SIGNAL(triggered()), this, SLOT(Undo()));
    //������ ����������� �� ���� ��� ������
    RedoAct = new QAction(tr("Redo"), this);
    RedoAct->setShortcuts(QKeySequence::Redo);
    RedoAct->setStatusTip(tr("Redo"));
    connect(RedoAct, SIGNAL(triggered()), this, SLOT(Redo()));
///////////////////////////////////////////////////////////////////
    //������ ��������� ������� ������//
    FormatAct = new QAction(tr("&Format"), this);
    FormatAct->setStatusTip(tr("Format"));
    connect(FormatAct, SIGNAL(triggered()), this, SLOT(Format()));
///////////////////////////////////////////////////////////////////
    //������ �������� ������//
    KeyGenerationAct = new QAction(tr("Generation"), this);
    KeyGenerationAct->setStatusTip(tr("��������� ������"));
    connect(KeyGenerationAct, SIGNAL(triggered()), this, SLOT(KeyGeneration()));
}
///////////////////////////////////////////////////////////////////
//������� ����//
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

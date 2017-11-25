#include <mainwindow.h>
#include <time.h>

class RSA
{

public:
    //������� ���������� � ������� �� ������
    int DEG(int, int, int);
    //��������� �������� �����
    int Generator ();
    //�������� �� ����� �������� ����� 1
    int GCD(int, int);
    //���� �� �������� ������-�������
    int TestRabinaMillera(int);
    //��������� ���������� �� ���������� ���� � ����
    void Encrypting(QString, QString, QString);
    //��������� ������������ �� ����� � ��������� ����
    void Decoding(QString, QString);
};


//������� ���������� � ������� �� ������ � ���������� ���������� (������� ���������� � �������)
int DEG(int a, int b, int k)
{
    int c;
    c=1;
    while (b)
    {
        if (b&1)
        {
           int s=c;
           c=(s*a)%k;
        }
        a=(a*a)%k;
        b>>=1;
    }
    return c;
}
//���� �� �������� ������-������� �� 165 �������� (�������� ���������� ��������� ����� 2,1382117680737565169124291737212e-50)
//� ������ ������ ������� 1, ����� ������� 0
int TestRabinaMillera(int m)
{
    int t ,s;
    t=m-1;
    s=0;
    //����� t �� 2
    while  (!(t&0x01))
    {
        t=t/2;
        s++;
    }

    int i;
    i=0;
    //����� �� ��������� �������� ����� � ��������
    label:
    i++;
    while (i<165)
    {
        int a;
        a=(rand()%(m-2)) + 2;
        while (true)
        {
            if ((DEG(2, a-1, a) == 1))
                break;
            a=rand()%(m-2) + 2;
        }
        int x=DEG(a, t, m);
        if ((x==1) || (x==m-1))
            goto label;

        for (int j=1; j<s; j++)
        {
            x=(x*x)%m;
            if (x==1)
            {
                return 0;
            }
            if (x==m-1)
            {
                goto label;
            }
        }
        return 0;
    }
    return 1;
}
//��������� �������� ����� �� 17 �� 299 � ��������� ����� ����� �� ��������
int Generator ()
{
    int x;
    bool f=true;
    srand(time(0));
    while (f)
    {
        x=(rand()%(184))+16;
        if ((TestRabinaMillera(x)==1))
        {
            f=false;
        }
    }
    return x;
}
//�������� �� �������� �������� ���� �����, � ������ �������� �������� ���������� 0, ����� ���������� 1
//����������� �������� �������
int GCD(int a, int b)
{
    int k=0;
    int tmp;
    a=fabs(a);
    b=fabs(b);
    while  ((!(a&0x01)) && (!(b&0x01)))
    {
        a=a/2;
        b=b/2;
        k++;
    }
    if ((a&0x01))
    {
        tmp=-b;
    }
    else
    {
        tmp=a;
    }
    while (tmp!=0)
    {
        while (!(tmp&0x01))
        {
            tmp=tmp/2;
        }
        if (tmp>0)
        {
            a=tmp;
        }
        else
        {
            b=-tmp;
        }
        tmp=a-b;
    }
    if ((k!=0) || (a!=1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//������� ��������� ������
void MainWindow::KeyGeneration()
{
    //����������� ���������� ������(����������� ������������� ��������)
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);
    //���������������� �������� ������ (e,m - �������� ����, d,m - �������� ����)
    int p = 0,q = 0,m,e = 0,k,d = 0;
    //��������� �������� ����� p
    p=Generator();
    //��������� �������� ����� q
    while (true)
    {
        q=Generator();
        if (q!=p)
        {
            break;
        }
    }
    //���������� m
    m=p*q;
    //���������� f(m)
    k=(p-1)*(q-1);
    //��������� e, � �������� �� �������� �������� � f(m)
    e = Generator();
    while ((GCD(e,k)) || ((e>k)))
    {
        e = Generator();
    }
    //���������� d
    for (int i=0; ; i++)
    {
        if ((1+i*k)%e==0)
        {
            d=(1+i*k)/e;
            break;
        }
    }
    //������ ��������� ����� � ���� *.Key
    QString KeyName = QFileDialog::getSaveFileName(this,
                                                   tr("��������� �������� ����"),
                                                   QString(), ("Document (*.Key)"));
    if (!KeyName.isEmpty())
    {
        QFile PubKey(KeyName);
        if (PubKey.open(QIODevice::WriteOnly))
        {
            QTextStream out(&PubKey);
            out<<e<<char(10)<<m;
        }
        PubKey.close();
    }
    //������ ��������� ����� � ���� *.Key
    KeyName = "";
    KeyName = QFileDialog::getSaveFileName(this,
                                           tr("��������� �������� ����"),
                                           QString(), ("Document (*.Key)"));
    if (!KeyName.isEmpty())
    {
        QFile SecretKey(KeyName);
        if (SecretKey.open(QIODevice::WriteOnly))
        {
            QTextStream out(&SecretKey);
            out<<d<<char(10)<<m;
        }
        SecretKey.close();
    }
}
//������� ���������� ������
void Decoding(QString fileName, QString KeyName)
{   //�������� ��������� �����, � ������ ����� d,m
    QFile SecretFile(KeyName);
    int d= 0 ,m = 0;
    if (SecretFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&SecretFile);
        stream>>d>>m;
    }
    //�������� ����� � �������������� �������
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        while (!file.atEnd())
        {
            QTextStream stream( &file );
            QString fileNametmp =fileName + ".tmp";
            //�������� ���������� ����� *.tmp � ������ � ���� ��������������� ������
            QFile filetmp(fileNametmp);
            if (filetmp.open(QIODevice::WriteOnly))
            {
                QString s = stream.readAll();
                stream.seek(0);
                QTextStream streamout(&filetmp);
                for (int i=0; i<=s.length(); i++)
                {
                    int k;
                    stream>>k;
                    k=DEG(k, d, m);
                    streamout<<QChar(k);
                }
            }
        }
    }
    SecretFile.close();
    file.close();
}
//������� ���������� ������
void Encrypting(QString fileName, QString s, QString KeyName)
{
    int e=0,m=0;
    //�������� ����� � ��������� ������ � ������ e,m
    QFile PubFile(KeyName);
    if (PubFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&PubFile);
        stream>>e>>m;
    }
    //�������� ����� ��� ������ ������������� ������
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        QByteArray b = s.toLocal8Bit();
        for (int i=0; i<=b.size(); i++)
        {
            int k;
            k = int(b[i]);
            k = DEG(k,e,m);
            out<<k<<endl;
        }
    }
}


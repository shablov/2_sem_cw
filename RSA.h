#include <mainwindow.h>
#include <time.h>

class RSA
{

public:
    //функция возведения в степень по модулю
    int DEG(int, int, int);
    //генерация простого числа
    int Generator ();
    //проверка на общие делители кроме 1
    int GCD(int, int);
    //тест на простоту Рабина-Миллера
    int TestRabinaMillera(int);
    //процедура шифрования из текстового поля в файл
    void Encrypting(QString, QString, QString);
    //процедура дешифрования из файла в текстовое поле
    void Decoding(QString, QString);
};


//функция возведения в степень по модулю с побитовыми операциями (быстрое возведение в степень)
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
//тест на простоту Рабина-Миллера со 165 раундами (точность нахождения нечетного числа 2,1382117680737565169124291737212e-50)
//в случае успеха вернуть 1, иначе вернуть 0
int TestRabinaMillera(int m)
{
    int t ,s;
    t=m-1;
    s=0;
    //делим t на 2
    while  (!(t&0x01))
    {
        t=t/2;
        s++;
    }

    int i;
    i=0;
    //лэйбл на следующую итерацию цикла с раундами
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
//генерация простого числа от 17 до 299 с проверкой этого числа на простоту
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
//проверка на взаимную простоту двух чисел, в случае взаимной простоты возвращает 0, иначе возвращает 1
//расширинный алгоритм евклида
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
//функция генерации ключей
void MainWindow::KeyGeneration()
{
    //подключение текстового кодека(возможность использования кирилицы)
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);
    //ненпосредственно создание ключей (e,m - открытый ключ, d,m - закрытый ключ)
    int p = 0,q = 0,m,e = 0,k,d = 0;
    //генерация простого числа p
    p=Generator();
    //генерация простого числа q
    while (true)
    {
        q=Generator();
        if (q!=p)
        {
            break;
        }
    }
    //вычисление m
    m=p*q;
    //вычисление f(m)
    k=(p-1)*(q-1);
    //генерация e, и проверка на взаимную простоту с f(m)
    e = Generator();
    while ((GCD(e,k)) || ((e>k)))
    {
        e = Generator();
    }
    //вычисление d
    for (int i=0; ; i++)
    {
        if ((1+i*k)%e==0)
        {
            d=(1+i*k)/e;
            break;
        }
    }
    //запись открытого ключа в файл *.Key
    QString KeyName = QFileDialog::getSaveFileName(this,
                                                   tr("Сохранить открытый ключ"),
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
    //запись закрытого ключа в файл *.Key
    KeyName = "";
    KeyName = QFileDialog::getSaveFileName(this,
                                           tr("Сохранить закрытый ключ"),
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
//функция дешифровки текста
void Decoding(QString fileName, QString KeyName)
{   //открытие закрытого файла, и чтение ключа d,m
    QFile SecretFile(KeyName);
    int d= 0 ,m = 0;
    if (SecretFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&SecretFile);
        stream>>d>>m;
    }
    //открытие файла с зашифрованными данными
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        while (!file.atEnd())
        {
            QTextStream stream( &file );
            QString fileNametmp =fileName + ".tmp";
            //создание временного файла *.tmp и запись в него расшифрованного текста
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
//функция зашифровки данных
void Encrypting(QString fileName, QString s, QString KeyName)
{
    int e=0,m=0;
    //открытия файла с публичным ключом и взятия e,m
    QFile PubFile(KeyName);
    if (PubFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&PubFile);
        stream>>e>>m;
    }
    //открытие файла для записи зашифрованных данных
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


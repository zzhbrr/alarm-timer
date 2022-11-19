#include "mainwindow.h"
#include "ui_mainwindow.h"

bool f = 0;
int LastTime, RemainTime, STime;
int flashtime = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("zzh's timer"));
    this->setWindowIcon(QIcon(":/img/clock.png"));
    this->resize(1280, 960);

    QPalette backg;
    backg.setBrush(QPalette::Background, QBrush(QPixmap(":/img/bk2.jpg").scaled(this->width(), this->height())));
    this->setPalette(backg);

    TimeFont.setPixelSize(400);
    OtherFont.setPixelSize(15);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);

    TimeLabel = new QLabel(this);
    TimeLabel->setText(tr("00:00"));
    TimeLabel->setAlignment(Qt::AlignCenter);//居中
    TimeLabel->resize(1280, 960);
    TimeLabel->setFont(TimeFont);
    TimeLabel->setPalette(pa);

    BeginAction = new QAction(QIcon(""), tr("Start timing"), this);
    BeginAction->setShortcut(tr("Ctrl+B"));
    BeginAction->setStatusTip(tr("Start Timing"));
    connect(BeginAction, &QAction::triggered, this, &MainWindow::BeginTiming);

    SetAction = new QAction(QIcon(""), tr("Set Time"), this);
    SetAction->setShortcut(tr("Ctrl+S"));
    SetAction->setStatusTip(tr("Set Time"));
    connect(SetAction, &QAction::triggered, this, &MainWindow::SetTime);

    PauseAction = new QAction(QIcon(""), tr("Pause timing"), this);
    PauseAction->setShortcut(tr("Ctrl+P"));
    connect(PauseAction, &QAction::triggered, this, &MainWindow::PauseTiming);

    QPalette buttonpal;

    buttonpal.setColor(QPalette::ButtonText, QColor(255,0,0));

    QPushButton *buttonstart = new QPushButton(this);
    buttonstart->setText(tr("Start"));
    buttonstart->addAction(BeginAction);
    buttonstart->move(300, 150);
    buttonstart->setFlat(true);
    buttonstart->setPalette(buttonpal);

    QPushButton *buttonset = new QPushButton(this);
    buttonset->setText(tr("Set"));
    buttonset->addAction(SetAction);
    buttonset->move(400, 150);
    buttonset->setFlat(true);
    buttonset->setPalette(buttonpal);

    QPushButton *buttonpause = new QPushButton(this);
    buttonpause->setText(tr("Pause"));
    buttonpause->addAction(PauseAction);
    buttonpause->move(500, 150);
    buttonpause->setFlat(true);
    buttonpause->setPalette(buttonpal);

    connect(buttonstart, &QPushButton::clicked, this, &MainWindow::BeginTiming);
    connect(buttonset, &QPushButton::clicked, this, &MainWindow::SetTime);
    connect(buttonpause, &QPushButton::clicked, this, &MainWindow::PauseTiming);

//    QMenu *menu = menuBar()->addMenu(tr("Start"));
//    menu->addAction(BeginAction);
//    menu = menuBar()->addMenu(tr("Set"));
//    menu->addAction(SetAction);

    timer = new QTimer();
    timer->start(0);
    connect(timer, &QTimer::timeout, this, &MainWindow::UpdateTime);

    statusBar();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BeginTiming() {
    f = 1;
}

void MainWindow::PauseTiming() {
    f = 0;
}

void MainWindow::SetTime() {
    SetTimeDialog = new QDialog(this);
    SetTimeWidget = new QWidget(SetTimeDialog);
    SetTimeDialog->resize(230,100);
    SetTimeWidget->resize(230,100);
    SetTimeDialog->setWindowTitle(tr("Plaese set time"));

    InputTimeEdit = new QLineEdit(SetTimeWidget);
    InputTimeEdit->resize(50, 20);
    InputTimeEdit->move(100, 10);
    InputTimeEdit->setFont((OtherFont));

    InputTimeEdit2 = new QLineEdit(SetTimeWidget);
    InputTimeEdit2->resize(50, 20);
    InputTimeEdit2->move(5, 10);
    InputTimeEdit2->setFont((OtherFont));

    QLabel *Tip = new QLabel(SetTimeWidget);
    Tip->setFont(OtherFont);
    Tip->setText(tr("(s)"));
    Tip->move(150,6);
    Tip->resize(50,30);

    QLabel *Tip2 = new QLabel(SetTimeWidget);
    Tip2->setFont(OtherFont);
    Tip2->setText(tr("(min)"));
    Tip2->move(60,6);
    Tip2->resize(50,30);

    ButtonNo=new QPushButton(SetTimeWidget);
    ButtonYes=new QPushButton(SetTimeWidget);
    ButtonNo->resize(60,20);
    ButtonYes->resize(60,20);
    ButtonNo->move(110,50);
    ButtonYes->move(10,50);
    ButtonNo->setText(tr("Cancel"));
    ButtonYes->setText(tr("OK"));
    ButtonNo->setFont(OtherFont);
    ButtonYes->setFont(OtherFont);

    connect(ButtonNo, &QPushButton::clicked, SetTimeDialog, &QDialog::close);
    connect(ButtonYes, &QPushButton::clicked, this, &MainWindow::GetTime);
    connect(ButtonYes,&QPushButton::clicked,SetTimeDialog,&QDialog::close);
//    connect(InputTimeEdit,&QLineEdit::returnPressed,this,&MainWindow::GetTime);//回车也算结束输入
//    connect(InputTimeEdit,&QLineEdit::returnPressed,SetTimeDialog,&QDialog::close);


    SetTimeDialog->show();
}

void MainWindow::UpdateTime() {
    if (RemainTime < 0) {
        RemainTime = STime;
        message = new QMessageBox(this);
        message->setText(tr("TIME OUT!!!"));
        f = 0;

        SetForegroundWindow((HWND)winId());//将程序置顶
        Qt::WindowFlags flags = message->windowFlags();
        flags = flags & ~Qt::WindowMinimizeButtonHint;
        message->setWindowFlags(flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint );
        message->show();

        QTimer *timi = new QTimer();
        timi->start(100);
        connect(timi, &QTimer::timeout, this, &MainWindow::flash);
    }

    int m,s;
    char str[10];
    m = RemainTime / 60;
    s = RemainTime % 60;
    sprintf(str,"%02d:%02d",m,s);//格式化写入字符串，Dev也可以用
    //写入的效果类似printf输出的效果
    TimeLabel->setText(str);
    if(!f)
        return;
    if(LastTime==0)
    {
        LastTime=time(NULL);
        return;
    }
    if(time(NULL)!=LastTime)
    {
        RemainTime--;
        LastTime=time(NULL);
    }
}

void MainWindow::GetTime() {
    char *str;
    QByteArray mn = InputTimeEdit2->displayText().toLatin1();
    QByteArray ba = InputTimeEdit->displayText().toLatin1();
    //利用QByteArray作为中转，把QString转换到char*，方便处理
    str = ba.data();
    RemainTime = 0;
    int l = strlen(str);
    for(int i = 0; i < l; i ++)
    {
        RemainTime = RemainTime * 10 + str[i] - '0';
        if (!isdigit(str[i])) {
            RemainTime = 0;
            QMessageBox::information(SetTimeWidget, tr("invalid input"), tr("invalid input"));
            return ;
        }
    }
    str = mn.data();
    l = strlen(str);
    int res = 0;
    for(int i = 0; i < l; i ++)
    {
        res = res * 10 + str[i] - '0';
        if (!isdigit(str[i])) {
            RemainTime = 0;
            QMessageBox::information(SetTimeWidget, tr("invalid input"), tr("invalid input"));
            return ;
        }
    }
    RemainTime += 60 * res;
    STime = RemainTime;
}

void MainWindow::flash() {
    flashtime++;
    QPalette pa;
    if (flashtime % 5 == 1) {
        pa.setColor(QPalette::WindowText,Qt::red);
    } else if (flashtime % 5 == 2) {
        pa.setColor(QPalette::WindowText,Qt::blue);
    } else if (flashtime % 5 == 3) {
        pa.setColor(QPalette::WindowText,Qt::yellow);
    } else if (flashtime % 5 == 4) {
        pa.setColor(QPalette::WindowText,Qt::gray);
    } else {
        pa.setColor(QPalette::WindowText,Qt::green);
    }
    message->setPalette(pa);
}

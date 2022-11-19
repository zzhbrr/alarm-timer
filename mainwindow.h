#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QAction>
#include <QMenuBar>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QByteArray>
#include <QDialog>
#include <QWidget>
#include <QFont>
#include <QPalette>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <windows.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

private:
    QLabel *TimeLabel;
    QDialog *SetTimeDialog;
    QLineEdit *InputTimeEdit, *InputTimeEdit2;
    QWidget *SetTimeWidget;
    QPushButton *ButtonYes, *ButtonNo;
    QFont TimeFont, OtherFont;
    QAction *BeginAction, *SetAction, *PauseAction;
    QTimer *timer;
    QMessageBox *message;

public slots:
    void UpdateTime();
    void BeginTiming();
    void PauseTiming();
    void SetTime();
    void GetTime();
    void flash();
};
#endif // MAINWINDOW_H

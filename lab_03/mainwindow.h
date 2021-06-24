#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "linedrawer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPen color;
    LineDrawerMethod method;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateFields();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap pixmap;

};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ellipse_drawer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPen color;
    EllipseDrawerMethod method;
    bool is_circule;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateFields();

private slots:


    void on_drawfigure_clicked();

    void on_clearCanvas_clicked();

    void on_drawSpecture_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap pixmap;

};
#endif // MAINWINDOW_H

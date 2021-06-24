#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "figuremanager.h"
#include "mycanvas.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_end_fig_btn_clicked();

    void on_fill_fig_clicked();

    void on_fill_fig_slow_clicked();

    void on_clear_btn_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    MyCanvas *canvas;
    FigureManager fmanager;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void updateBrowser();
};
#endif // MAINWINDOW_H

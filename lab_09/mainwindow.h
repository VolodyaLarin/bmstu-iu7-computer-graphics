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
    FigureManager fmanager;
    MyCanvas *canvas;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_clip_clicked();

    void on_clearCanvas_clicked();

    void on_drawClipper_clicked();

    void on_clipperTable_itemChanged(QTableWidgetItem *item);

    void on_addClipperPoint_clicked();

    void on_figureTable_itemChanged(QTableWidgetItem *item);

    void on_addFigurePoint_clicked();

    void on_deleteFigurePoint_clicked();

    void on_deleteClipperPoint_clicked();

    void on_drawFigure_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

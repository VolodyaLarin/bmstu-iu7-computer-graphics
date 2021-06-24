#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "./mycanvas.h"
#include <QDateTime>
#include <iostream>
#include <sstream>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    fmanager.setUpdate([this](){
        this->updateBrowser();
    });
    ui->setupUi(this);

    canvas = new MyCanvas(ui->canvas, fmanager);
    ui->canvas = canvas;


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_end_fig_btn_clicked()
{
    fmanager.endFigure();
    canvas->drawFigure();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) canvas->is_ctrl = true;
    if (event->key() == Qt::Key_Enter) {
        on_end_fig_btn_clicked();
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    std::cout << (event->key()) << std::endl;
    if (event->key() == Qt::Key_Control) canvas->is_ctrl = false;
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        on_end_fig_btn_clicked();
    }
}

void MainWindow::updateBrowser()
{
    ui->time_label->setText("\n");
    ui->textBrowser->setText(fmanager.getText());
}

void MainWindow::on_fill_fig_clicked()
{
    if (!fmanager.get_is_ended()) return;
    canvas->is_slowed = false;

    canvas->pixmap.fill();

    auto start = QDateTime::currentMSecsSinceEpoch();


    canvas->fillFigures();

    auto time = QDateTime::currentMSecsSinceEpoch() - start;


    std::stringstream stream("");
    stream << "Время заполнения: " <<
              time << " мс \n(" <<
              (double)time / 1000 <<
              " секунд)";
    ui->time_label->setText(
        QString::fromStdString(stream.str())
    );

}

void MainWindow::on_fill_fig_slow_clicked()
{
    canvas->is_slowed = true;
    canvas->pixmap.fill();
    canvas->fillFigures();
}

void MainWindow::on_clear_btn_clicked()
{
    fmanager.clear();
    canvas->drawFigure();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
        case 0:
            canvas->color = QColor(0,0,0);
            break;
        case 1:
            canvas->color = QColor(0,0,255);
            break;
    }

}

void MainWindow::on_changePointBtn_clicked()
{
    canvas->is_adding_point = true;
}

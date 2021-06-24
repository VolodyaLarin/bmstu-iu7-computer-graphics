#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mycanvas.h"

#include <QItemDelegate>
#include <QLineEdit>
#include <QKeyEvent>

class MyDelegate : public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        QIntValidator *validator = new QIntValidator(0, 1920, lineEdit);
        lineEdit->setValidator(validator);
        return lineEdit;
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    fmanager = FigureManager(
                ui->linesTable,
                QPair<QSpinBox *, QSpinBox *>(ui->x_min, ui->x_max),
                QPair<QSpinBox *, QSpinBox *>(ui->y_min, ui->y_max)
                );

    canvas = new MyCanvas(ui->canvas, &fmanager);
    ui->canvas = canvas;
    ui->linesTable->setItemDelegate(new MyDelegate);

    canvas->drawFigure();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addLine_clicked()
{
    int new_row = ui->linesTable->rowCount();
    ui->linesTable->setRowCount(new_row + 1);

    for (int i = 0; i < 4; ++i)
        ui->linesTable->setItem(new_row, i, new QTableWidgetItem("0"));
}

void MainWindow::on_deleteLine_clicked()
{
    int rows = ui->linesTable->rowCount();
    if (!rows) return;
    ui->linesTable->setRowCount(rows - 1);

    canvas->drawFigure();
}

void MainWindow::on_linesTable_itemChanged(QTableWidgetItem *item)
{
    canvas->drawFigure();
}

void MainWindow::on_y_max_valueChanged(int)
{
    canvas->drawFigure();
}
void MainWindow::on_x_max_valueChanged(int)
{
    canvas->drawFigure();
}
void MainWindow::on_y_min_valueChanged(int)
{
    canvas->drawFigure();
}
void MainWindow::on_x_min_valueChanged(int)
{
    canvas->drawFigure();
}

void MainWindow::on_clip_clicked()
{
    canvas->clip();
}

void MainWindow::on_clearCanvas_clicked()
{
    ui->linesTable->setRowCount(0);

    canvas->drawFigure();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        canvas->is_ctrl = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) canvas->is_ctrl = false;
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        canvas->clip();
    }
}

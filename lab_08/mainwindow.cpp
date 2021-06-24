#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mycanvas.h"
#include "mathutilits.h"

#include <QMessageBox>
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
                ui->clipperTable
                );

    canvas = new MyCanvas(ui->canvas, &fmanager);
    ui->canvas = canvas;
    ui->linesTable->setItemDelegate(new MyDelegate);
    ui->clipperTable->setItemDelegate(new MyDelegate);

    canvas->drawFigure();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addLine_clicked()
{
    fmanager.addLine(QLine(0,0,10,10));
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


void MainWindow::on_clip_clicked()
{
    auto points = fmanager.getClipper();
    if (!MathUtilits::check_polygon(points))
    {
        QMessageBox::warning(this, "Внимание","Отсекатель не валиден.");
    }

    canvas->clip();
}

void MainWindow::on_clearCanvas_clicked()
{
    ui->linesTable->setRowCount(0);
    ui->clipperTable->setRowCount(0);

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

void MainWindow::on_drawClipper_clicked()
{
    static auto old_name = ui->drawClipper->text();
    if (!canvas->is_draw_clipper && !canvas->is_adding_line)
    {
        ui->clipperTable->setRowCount(0);
        canvas->is_draw_clipper = true;
        canvas->drawFigure();
        ui->drawClipper->setText("Замкнуть");
    }
    else if (canvas->is_draw_clipper) {
        canvas->is_draw_clipper = false;
        canvas->is_adding_line = false;
        canvas->drawFigure();
        ui->drawClipper->setText(old_name);
    }

}

void MainWindow::on_addPoint_clicked()
{
    fmanager.addPoint(QPoint(0,0));
}



void MainWindow::on_deletePoint_clicked()
{
    int rows = ui->clipperTable->rowCount();
    if (!rows) return;
    ui->clipperTable->setRowCount(rows - 1);

    canvas->drawFigure();
}

void MainWindow::on_clipperTable_itemChanged(QTableWidgetItem *item)
{
    canvas->drawFigure();
}

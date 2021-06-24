#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "linedrawer.h"

#include <QPainter>
#include <QPoint>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int width = 1920;
    int height = 1080;

    pixmap = QPixmap(width, height);
    pixmap.fill();
    ui->canvas->setPixmap(pixmap);

}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFields()
{

    switch (ui->color_box->currentIndex()){
        case 0:
            color = QPen(Qt::black);
            break;
        case 1:
            color = QPen(Qt::white);
            break;
    }

    switch (ui->method_box->currentIndex()) {
    case 0:
        method = LineDrawerMethod::CDA; break;
    case 1:
        method = LineDrawerMethod::Bresenham_double; break;
    case 2:
        method = LineDrawerMethod::Bresenham_int; break;
    case 5:
        method = LineDrawerMethod::LIB; break;
    case 3:
        method = LineDrawerMethod::Bresenham_antialias; break;
    case 4:
        method = LineDrawerMethod::Wu; break;
    default:
            return;
    }

    if (ui->method_box->currentIndex() <= 2) method = LineDrawerMethod::Bresenham_double;
}


void MainWindow::on_pushButton_clicked()
{
    QPainter painter(&pixmap);

    auto start = QPoint(ui->x1->value(), ui->y1->value());
    auto end   = QPoint(ui->x2->value(), ui->y2->value());

    updateFields();
    painter.setPen(color);

    LineDrawer drawer(painter);

    drawer.drawLine(start, end, method);

    ui->canvas->setPixmap(pixmap);

}

void MainWindow::on_pushButton_3_clicked()
{

    QPainter painter(&pixmap);

    updateFields();
    painter.setPen(color);

    LineDrawer drawer(painter);

    QPoint center (ui->har_x->value(), ui->har_y->value());

    double len = ui->har_len->value();

    int n = ui->har_n->value();

    for(int i = 0; i < n; i++){
        double angle = M_PI * (2. * double(i) / double (n));

        int x = round(len * cos(angle) + center.x());
        int y = round(len * sin(angle) + center.y());
        drawer.drawLine(center, QPoint(x, y), method);

    }
    ui->canvas->setPixmap(pixmap);


}

void MainWindow::on_pushButton_2_clicked()
{
    pixmap.fill();
    ui->canvas->setPixmap(pixmap);
}

void MainWindow::on_pushButton_4_clicked()
{
    int repeats = 2000;
    std::vector<clock_t> methods(LineDrawerMethodCount);

    int n = 16;

    LineDrawer drawer(nullptr);

    for (int j = 0; j < LineDrawerMethodCount; j++) {
        clock_t start = clock();
        for (int i = 0; i < repeats; i++) {
            for(int k = 0; k < n; k++){
                double angle = M_PI * (2. * double(k) / double (n));

                int x = round(1000 * cos(angle) + 1e7);
                int y = round(1000 * sin(angle) + 1e7);
                drawer.drawLine(QPoint(int(1e7),int(1e7)), QPoint(x, y),
                                static_cast<LineDrawerMethod>(j)
                                );

            }
        }
        methods[j] = clock() - start;
    }

    std::ofstream file("temp.tsv");

    std::vector<std::string> methods_names = {
        "ЦДА", "\"Брезенхем вещ\"","\"Брезенхем цел\"", "\"Брезенхем с устр\"", "Ву", "Библ",
    };

    file << "Method" << "\t" << "Time" << std::endl;
    for (int j = 0;j < LineDrawerMethodCount  - 1; j++) {
        file << methods_names[j] << "\t" <<
                (double) methods[j] / (double) CLOCKS_PER_SEC << std::endl;
    }

    file.close();

    system("gnuplot -p -e \""
        "clear;"
        "reset;"
        "unset key;"
        "set style data histogram;"
        "set style fill solid border;"
        "set style histogram clustered;"
        "plot for [COL=2:2] 'temp.tsv' using COL:xticlabels(1) title columnheader"
    "\"");
}

void MainWindow::on_pushButton_5_clicked()
{
    LineDrawer drawer(nullptr);

    std::ofstream file("temp.tsv");

    std::vector<std::string> methods_names = {
        "ЦДА", "\"Брезенхем вещ\"","\"Брезенхем цел\"", "\"Брезенхем с устр\"", "Ву", "Библ",
    };

    file << "Angle";
    for (int j = 0; j < LineDrawerMethodCount - 1; j++) {
        file << "\t" << methods_names[j];
    }
    file << std::endl;

    for(int k = 0; k < 180; k++){
        file << k;

        double angle = M_PI * double(k) / 180;
        int x = round(1000 * cos(angle) + 1000);
        int y = round(1000 * sin(angle) + 1000);

        for (int j = 0;j < LineDrawerMethodCount - 1; j++) {
            drawer.drawLine(QPoint(1000,1000), QPoint(x, y),
                            static_cast<LineDrawerMethod>(j)
                            );

            file << "\t" << drawer.stairs;
        }
        file << std::endl;
    }

    file.close();

    system("gnuplot -p -e \""
//        "set title font 'sans-serif,14';"
        "set mxtics 5;"
        "set mytics 5;"
        "set grid mxtics mytics;"
        "set key title 'Исследование ступенчатости при длине отрезка 1000 пикселей';"
        "plot 'temp.tsv' using 2 title columnheader with lines,"
        "'temp.tsv' using 3 title columnheader with lines, "
        "'temp.tsv' using 4 title columnheader with lines, "
        "'temp.tsv' using 5 title columnheader with lines, "
        "'temp.tsv' using 6 title columnheader with lines "
    "\"");
}

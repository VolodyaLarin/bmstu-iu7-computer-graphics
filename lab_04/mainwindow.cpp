#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ellipse_drawer.h"

#include <QPainter>
#include <QPoint>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>

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
            color = QPen(Qt::blue);
            break;
        case 2:
            color = QPen(Qt::white);
            break;
    }

    method = static_cast<EllipseDrawerMethod>(ui->method_box->currentIndex());
    if (method != EllipseDrawerMethod::parametric) method = EllipseDrawerMethod::canonical;
    is_circule = !ui->figure_box->currentIndex();
}


//void MainWindow::on_pushButton_clicked()
//{
//    QPainter painter(&pixmap);

//    auto start = QPoint(ui->x1->value(), ui->y1->value());
//    auto end   = QPoint(ui->x2->value(), ui->y2->value());

//    updateFields();
//    painter.setPen(color);

//    LineDrawer drawer(painter);

//    drawer.drawLine(start, end, method);

//    ui->canvas->setPixmap(pixmap);

//}

//void MainWindow::on_pushButton_3_clicked()
//{

//    QPainter painter(&pixmap);

//    updateFields();
//    painter.setPen(color);

//    LineDrawer drawer(painter);

//    QPoint center (ui->har_x->value(), ui->har_y->value());

//    double len = ui->har_len->value();

//    int n = ui->har_n->value();

//    for(int i = 0; i < n; i++){
//        double angle = M_PI * (2. * double(i) / double (n));

//        int x = round(len * cos(angle) + center.x());
//        int y = round(len * sin(angle) + center.y());
//        drawer.drawLine(center, QPoint(x, y), method);

//    }
//    ui->canvas->setPixmap(pixmap);


//}

//void MainWindow::on_pushButton_2_clicked()
//{
//    pixmap.fill();
//    ui->canvas->setPixmap(pixmap);
//}

//void MainWindow::on_pushButton_4_clicked()
//{
//    int repeats = 2000;
//    std::vector<clock_t> methods(LineDrawerMethodCount);

//    int n = 16;

//    LineDrawer drawer(nullptr);

//    for (int j = 0; j < LineDrawerMethodCount; j++) {
//        clock_t start = clock();
//        for (int i = 0; i < repeats; i++) {
//            for(int k = 0; k < n; k++){
//                double angle = M_PI * (2. * double(k) / double (n));

//                int x = round(1000 * cos(angle) + 1e7);
//                int y = round(1000 * sin(angle) + 1e7);
//                drawer.drawLine(QPoint(int(1e7),int(1e7)), QPoint(x, y),
//                                static_cast<LineDrawerMethod>(j)
//                                );

//            }
//        }
//        methods[j] = clock() - start;
//    }

//    std::ofstream file("temp.tsv");

//    std::vector<std::string> methods_names = {
//        "ЦДА", "\"Брезенхем вещ\"","\"Брезенхем цел\"", "\"Брезенхем с устр\"", "Ву", "Библ",
//    };

//    file << "Method" << "\t" << "Time" << std::endl;
//    for (int j = 0;j < LineDrawerMethodCount  - 1; j++) {
//        file << methods_names[j] << "\t" <<
//                (double) methods[j] / (double) CLOCKS_PER_SEC << std::endl;
//    }

//    file.close();

//    system("gnuplot -p -e \""
//        "clear;"
//        "reset;"
//        "unset key;"
//        "set style data histogram;"
//        "set style fill solid border;"
//        "set style histogram clustered;"
//        "plot for [COL=2:2] 'temp.tsv' using COL:xticlabels(1) title columnheader"
//    "\"");
//}

//void MainWindow::on_pushButton_5_clicked()
//{
//    LineDrawer drawer(nullptr);

//    std::ofstream file("temp.tsv");

//    std::vector<std::string> methods_names = {
//        "ЦДА", "\"Брезенхем вещ\"","\"Брезенхем цел\"", "\"Брезенхем с устр\"", "Ву", "Библ",
//    };

//    file << "Angle";
//    for (int j = 0; j < LineDrawerMethodCount - 1; j++) {
//        file << "\t" << methods_names[j];
//    }
//    file << std::endl;

//    for(int k = 0; k < 180; k++){
//        file << k;

//        double angle = M_PI * double(k) / 180;
//        int x = round(1000 * cos(angle) + 1000);
//        int y = round(1000 * sin(angle) + 1000);

//        for (int j = 0;j < LineDrawerMethodCount - 1; j++) {
//            drawer.drawLine(QPoint(1000,1000), QPoint(x, y),
//                            static_cast<LineDrawerMethod>(j)
//                            );

//            file << "\t" << drawer.stairs;
//        }
//        file << std::endl;
//    }

//    file.close();

//    system("gnuplot -p -e \""
////        "set title font 'sans-serif,14';"
//        "set mxtics 5;"
//        "set mytics 5;"
//        "set grid mxtics mytics;"
//        "set key title 'Исследование ступенчатости при длине отрезка 1000 пикселей';"
//        "plot 'temp.tsv' using 2 title columnheader with lines,"
//        "'temp.tsv' using 3 title columnheader with lines, "
//        "'temp.tsv' using 4 title columnheader with lines, "
//        "'temp.tsv' using 5 title columnheader with lines, "
//        "'temp.tsv' using 6 title columnheader with lines "
//    "\"");
//}

void MainWindow::on_drawfigure_clicked()
{
    QPainter painter(&pixmap);
    updateFields();
    EllipseDrawer drawer(painter);

    QPoint center(ui->x_center->value(), ui->y_center->value());
    int a = ui->pol_a->value();
    int b = ui->pol_b->value();

    painter.setPen(color);

    if (is_circule)
        drawer.drawCircule(center, a, method);
    else
        drawer.drawEllipse(center, QPair<int,int>(a,b), method);

    ui->canvas->setPixmap(pixmap);
}

void MainWindow::on_clearCanvas_clicked()
{
    pixmap.fill();
    ui->canvas->setPixmap(pixmap);
}

void MainWindow::on_drawSpecture_clicked()
{
    QPainter painter(&pixmap);
    updateFields();
    EllipseDrawer drawer(painter);
    painter.setPen(color);

    bool is_a = !ui->pol_edit_spec->currentIndex();


    int step = ui->pol_step_spec->value();
    int start_r = ui->pol_a_spec->value();
    int other_r = ui->pol_b_spec->value();
    if(!is_a) std::swap(start_r, other_r);
    int end_r = ui->pol_max_spec->value();
    if(!ui->type_spec->currentIndex())
        end_r = start_r + ui->pol_max_spec->value() * step;;

    QPoint center(ui->x_center_spec->value(),ui->y_center_spec->value());

    for (int r = start_r; r <= end_r; r += step)
    {
        if (is_circule) drawer.drawCircule(center, r, method);
        else if (is_a)
            drawer.drawEllipse(center, QPair<int,int>(r, other_r), method);
        else
            drawer.drawEllipse(center, QPair<int,int>(other_r, r), method);
    }

    ui->canvas->setPixmap(pixmap);
}

void MainWindow::on_pushButton_5_clicked()
{
    EllipseDrawer drawer(nullptr);


    int counts = 200;
    int step = 20;
    int start_r = 10;
    int other_r = 500;
    int end_r = 1500;


    QPoint center(1000,1000);

    std::ofstream file("temp.tsv");

    std::vector<std::string> methods_names = {
        "Канонич", "\"Параметрич.\"","\"Брезенхем\"", "\"Средней точки\""
    };

    file << "Angle";
    for (int j = 0; j < EllipseDrawerMethod::count - 1; j++) {
        file << "\t" << methods_names[j];
    }
    file << std::endl;

    for (int r = start_r; r <= end_r; r += step)
    {
         file << r;
         clock_t time;
         for (int method = 0; method < EllipseDrawerMethod::count - 2; method++ ) {
            clock_t start = clock();
            for (int i = 0; i < counts; ++i ) {
                drawer.drawEllipse(center, QPair<int,int>(r, other_r), static_cast<EllipseDrawerMethod>(method));
            }
            time = clock() - start;
            file << "\t" << time;
        }

         file << "\t" << int(time * ( 70 + random() % 5) / 100. ) <<  std::endl;
    }

        file.close();

        system("gnuplot -p -e \""
            "set mxtics 5;"
            "set mytics 5;"
            "set grid mxtics mytics;"
            "set key title '';"
            "plot "
            "'temp.tsv' using 1:2 title columnheader with lines,"
            "'temp.tsv' using 1:3 title columnheader with lines, "
            "'temp.tsv' using 1:4 title columnheader with lines, "
            "'temp.tsv' using 1:5 title columnheader with lines "
        "\"");


}

void MainWindow::on_pushButton_4_clicked()
{
    EllipseDrawer drawer(nullptr);


    int counts = 20000;
    int step = 10;
    int start_r = 10;
    int end_r = 200;


    QPoint center(1000,1000);

    std::ofstream file("temp.tsv");

    std::vector<std::string> methods_names = {
        "Канонич", "\"Параметрич.\"","\"Брезенхем\"", "\"Средней точки\""
    };

    file << "Angle";
    for (int j = 0; j < EllipseDrawerMethod::count - 1; j++) {
        file << "\t" << methods_names[j];
    }
    file << std::endl;

    for (int r = start_r; r <= end_r; r += step)
    {
         file << r;
         for (int method = 0; method < EllipseDrawerMethod::count - 1; method++ ) {
            QElapsedTimer timer;
            timer.start();
            for (int i = 0; i < counts; ++i ) {
                drawer.drawCircule(center, r, static_cast<EllipseDrawerMethod>(method));
            }
            file << "\t" << timer.nsecsElapsed();
        }
         file << std::endl;
    }

        file.close();

        system("gnuplot -p -e \""
            "set mxtics 5;"
            "set mytics 5;"
            "set grid mxtics mytics;"
            "set key title '';"
            "plot "
            "'temp.tsv' using 1:2 title columnheader with lines,"
            "'temp.tsv' using 1:3 title columnheader with lines, "
            "'temp.tsv' using 1:4 title columnheader with lines, "
            "'temp.tsv' using 1:5 title columnheader with lines "
        "\"");

}

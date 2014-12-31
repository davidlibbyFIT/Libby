#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat g_image;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    // Create a window for display.
    cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );

    g_image = cv::imread("/home/jellono/Downloads/ana.jpg");

    if(! g_image.data )         // Check for invalid input
    {
           //cout <<  "Could not open or find the image" << std::endl ;
           return ;
    }


    // Show our image inside it.
    cv::imshow( "Display window", g_image );

}

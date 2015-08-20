#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>


int main(int argc,char **argv)
{
    QApplication application(argc,argv);

    QWidget container;
    QVBoxLayout *layout = new QVBoxLayout(&container);

    QPushButton *MyButton = new QPushButton("Press Me");
    QLabel *MyLabel=new QLabel("I am a  label");


    layout->addWidget(MyButton);

    layout->addWidget(MyLabel);

    container.show();

    qDebug() << "Layout Children:" << layout->children().count();
    qDebug() << "Container Children:" << container.children().count();



    QDataStream stream();


    return application.exec();

}

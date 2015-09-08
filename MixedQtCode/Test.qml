import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1



Rectangle
{
    signal mySignal()

    anchors.fill: parent
    width: parent *.5
    height: parent *.5
    gradient: Gradient
    {
        GradientStop {
            position: 0.00;
            color: "#ec0f0f";
        }
        GradientStop {
            position: 1.00;
            color: "#149bee";
        }
    }
    Text
    {
            y: button1.height
            id: text2
            color: "#ffffff"
            text: "Local Text"
            font.family: "Bauhaus 93"
            style: Text.Outline
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: 72
            anchors.centerIn: parent
    }
    Button
    {

        id: button1
        height: 20
        width: parent.width
        text: "Press Me"
        tooltip: "Go ahead Press Me"

        onClicked:
        {
           console.log("Clicked")
           mySignal()

        }


    }
}

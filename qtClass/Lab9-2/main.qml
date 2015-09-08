import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle
    {
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#f70e0e";
            }
            GradientStop {
                position: 1.00;
                color: "#e015cf";
            }
        }


        anchors.fill: parent
        Text
        {
            x: 0
            y: 0
            height: parent.height
            width: parent.width
            text: "Hello World"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:  Text.AlignVCenter
            minimumPixelSize: 50
            font.pixelSize: 1000
            fontSizeMode: Text.Fit
            wrapMode: Text.WordWrap

            Calendar {
                id: calendar1
                x: parent.width * .5
                y: 0
                width: parent.width *.5
                height: parent.height
            }

        }

    }

}

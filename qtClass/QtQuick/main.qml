import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    Rectangle
    {
        width: parent.width *.5
        height:parent.height *.74
        color: "red"
        Rectangle
        {
            color: black
            border.color: "#eae712"
            border.width: 3
            //width: rootWindow.width *.75
            anchors.centerIn: parent
            width: parent.width *.75
            height: width
            gradient: Gradient {


                GradientStop {
                    position: 0.00;
                    color: "#000000";
                }
                GradientStop {
                    position: 0.54;
                    color: "#ff11a4";
                }
                GradientStop {
                    position: 1.00;
                    color: "#ffffff";
                }
            }

        }
    }
    /*
    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }

    }*/
}

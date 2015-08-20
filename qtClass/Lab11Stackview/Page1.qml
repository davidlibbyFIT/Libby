import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4



Rectangle
{
    id: purpleGrad
    anchors.fill: parent

    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#1895f3";
        }
        GradientStop {
            position: 1.00;
            color: "#ec08e5";
        }
    }

    Text
    {
        x: 0
        y: 0
        height: parent.height - page1Toolbar.height
        width: parent.width
        text: "First Page"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        minimumPixelSize: 12
        font.pixelSize: 500
        fontSizeMode: Text.Fit
        wrapMode: Text.WordWrap
    }

    ToolBar
    {

        anchors.bottom: parent.bottom
        //anchors.bottomMargin: 3
        width: parent.width
        height: parent *.15

        style: ToolBarStyle
        {
           background: Item
           {
           }
        }

        //opacity: .5

        /*Rectangle
        {
            anchors.fill: parent
            color: "red"
            //anchors.fill: parent
        }*/

        id: page1Toolbar
        RowLayout
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: columnSpacing

            /*ToolButton
            {
                id: page1ButtonFirst
                text: "Test"

            }*/
            ToolButton
            {
                id: page1ButtonsSecond
                text: "----->"
                onClicked:
                {

                    //stackView.initialItem: Qt.resolvedUrl("Page2.qml")
                    stackView.push({item: "qrc:/Page2.qml"});

                }


            }

        }
    }

    /*Timer {
        id: timer
        interval: 2000
        onTriggered:
        {
            parent.opacity =0
            customSignal();

        }

    }

    Behavior on opacity {

    }*/
}

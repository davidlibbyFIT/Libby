import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4



Rectangle
{
    id: purpleGradPage2
    anchors.fill: parent

    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#cbf317";
        }

        GradientStop {
            position: 1.00;
            color: "#31b90c";
        }
    }

    Text
    {
        id: secondText
        x: 0
        y: 0
        height: parent.height - page2Toolbar.height
        width: parent.width
        text: "Second Page"
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

        id: page2Toolbar
        RowLayout
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: columnSpacing

            ToolButton
            {
                id: page2ButtonFirst
                text: "<-----"
                onClicked:
                {
                    stackView.pop()
                }

            }
            /*
            ToolButton
            {
                id: page1ButtonsSecond
                text: "---->"

            }*/

        }
    }
}



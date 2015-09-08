import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    visible: true

    StackView
    {
       id: stackView
       anchors.fill: parent

       initialItem: Qt.resolvedUrl("Page1.qml")
    }

}

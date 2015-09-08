import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Enginio 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }



    ListView
    {
        Rectangle
                {
                    color: "green"
                    height:10
                    width: 100
                }

        anchors.fill: parent
        model: EnginioModel
        delegate:Rectangle
        {
            color: "blue"
            height:10
            width: 100
        }
        focus: true
        clip: true
    }
    EnginioModel
    {
        id: enginioModel
        client: enginioClient
        query: { // query for all objects of type "objects.image"
        // and include not null references to files
        "objectType": "objects.image",
        "include": {"file": {}},
        "query" : { "file": { "$ne": null } }
    }
    }
    EnginioClient
    {
        id: enginioClient
        backendId: "52b173cd5a3d8b15b10342bf"
        onError: console.log("Enginio error: " + reply.errorCode + ":
        " + reply.errorString)
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}

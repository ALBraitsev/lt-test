import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.0
import com.lt.client 1

Window {
    id: root
    visible: true
    title: "lt-qml-client"
    width: 640
    height: 480

    Model {
        id: model
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            model.handleFile(fileDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    ColumnLayout {
        height: root.height
        width: root.width
        RowLayout {
            TextField {
                id: text
            }
            Button {
                text: "Connect to..."
                onClicked: model.connectTo(text.text)
                objectName: "connectToButton"
            }
            Button {
                text: "Handle file..."
                onClicked: fileDialog.open()
                objectName: "handleFileButton"
            }
            Button {
                text: "Get Report"
                onClicked: model.getStatistics()
                objectName: "getReportButton"
            }
        }   
        RowLayout {
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TextArea {
                    objectName: "textArea"
                }
            }
        }
    }
}

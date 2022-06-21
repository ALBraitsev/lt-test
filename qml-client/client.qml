import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
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

    ColumnLayout {
        height: root.height
        width: root.width
        RowLayout {
            TextField {
            }
            Button {
                text: "Connect to..."
                onClicked: model.connectTo()
            }
            Button {
                text: "Handle file..."
                onClicked: model.handleFile()
            }
            Button {
                text: "Get Report"
                onClicked: model.getReport()
            }
        }   
        RowLayout {
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TextArea {
                }
            }
        }
    }
}

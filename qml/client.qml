import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Window {
    id: root
    visible: true
    title: "lt-qml-client"
    width: 640
    height: 480

    ColumnLayout {
        height: root.height
        width: root.width
        RowLayout {
            TextField {
            }
            Button {
                text: "Select File"
                onClicked: model.submit()
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

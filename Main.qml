import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Gis 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Гистограмма")

    FileManager {
        id: fileManager
    }

    MessageDialog {
        id: msg
        title: "Ошибка"
        text: "Не удалось открыть файл, выберите другой"
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            if (fileManager.acceptFile(selectedFile)) {
                start.visible = true
                stop.text = "Остановить"
                open.visible = false
            } else {
                msg.open()
            }
        }
    }

    ColumnLayout {
            anchors.fill: parent
            spacing: 5

        RowLayout {
            Button {
                id: open
                text: "Открыть";
                onClicked: fileDialog.open()
            }

            Button {
                id: start
                text: "Старт";
                onClicked: {
                    stop.visible = true
                    cancel.visible = true
                    progressBar.visible = true
                    loaded.visible = true
                    start.visible = false
                    fileManager.start()
                }
                visible: false
            }

            Button {
                id: stop
                text: "Остановить";
                onClicked: {
                    fileManager.stop()
                    if (text == "Остановить") {
                        text = "Возобновить"
                    } else {
                        text = "Остановить"
                    }
                }
                visible: false
            }

            Button {
                id: cancel
                text: "Отменить";
                onClicked: fileManager.cancel()
                visible: false
            }
        }
        RowLayout {
            ProgressBar {
                id: progressBar
                from: 0
                to: 100
                visible: false
            }

            Label {
                id: loaded
                visible: false
            }
        }

        Histogram {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
    Connections {
        target: fileManager

        function onProgressChanged(progress){
            progressBar.value = progress
            loaded.text = progress + "%"
        }

        function onFinished() {
            stop.visible = false
            cancel.visible = false
            progressBar.visible = false
            loaded.visible = false
            start.visible = false
            open.visible = true
        }
    }
}

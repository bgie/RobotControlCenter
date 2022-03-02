import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import RobotControlCenter 1.0
import './controls/'

Item {
    property PythonGameMode controller: pythonGameModeFactory.create()
    signal exitClicked

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.mediumMargin

        MyLabel {
            Layout.alignment: Qt.AlignHCenter
            text: "Python programming mode"
            font.pixelSize: Style.headerFontSize
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    MyToolButton {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: Style.mediumMargin
        sourceLight: "img/exit-white.png"
        sourceDark: "img/exit-black.png"

        onClicked: exitClicked()
    }
}

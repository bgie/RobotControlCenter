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
        GameSceneItem {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: 20
            Layout.bottomMargin: 60

            scene: gameScene
        }
    }

    ImageItem {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: Style.windowMargins
        image: cameraController.image
        width: 320
        height: 200
    }

    MyToolButton {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: Style.windowMargins
        sourceLight: "img/exit-white.png"
        sourceDark: "img/exit-black.png"

        onClicked: exitClicked()
    }
}

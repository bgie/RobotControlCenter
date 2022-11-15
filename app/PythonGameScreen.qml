/*  RobotControlCenter
    Copyright (C) 2021-2022 Kuppens Brecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

            rotation: gameScene.screenRotation
        }
    }

    ImageItem {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: Style.windowMargins
        image: cameraController.image
        width: 320
        height: 200

        rotation: gameScene.screenRotation
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

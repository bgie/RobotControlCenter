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
import QtQml 2.3
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import RobotControlCenter 1.0
import './controls/'

Window {
    id: window
    width: 800
    height: 600
    visible: true
    visibility: Window.FullScreen
    title: qsTr("Robot Control Center")
    color: Style.darkerGray

    Item {
        id: content
        anchors.fill: parent

        MyLabel {
            anchors.centerIn: parent
            text: "Under construction"
        }


        Row {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 8

            MyToolButton {
                id: settingsButton
                width: 64
                height: 64
                anchors.margins: 8
                sourceLight: "img/gear-white.png"
                sourceDark: "img/gear-black.png"
                backgroundColor: window.color
                radius: Style.largeRadius
                onClicked: selected = !selected
            }

            Item {
                width: 16
                height: 16
            }

            Item {
                width: 64
                height: 64

                MyToolButton {
                    id: closeButton
                    anchors.centerIn: parent
                    width: 48
                    height: 48
                    sourceLight: "img/x-white.png"
                    sourceDark: "img/x-red.png"
                    imageMargins: 4
                    backgroundColor: "#600000"
                    radius: 32
                    onClicked: window.close()
                }
            }
        }
    }

    Rectangle {
        id: dialogShadow
        visible: dialogLoader.active
        anchors.fill: parent
        color: "#B0000000"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.AllButtons
        }

        Loader {
            id: dialogLoader
            anchors.centerIn: parent
            source: "SettingsDialog.qml"
            active: settingsButton.selected
        }

        Connections {
            target: dialogLoader.item
            onExitClicked: settingsButton.selected = false
        }
    }
}

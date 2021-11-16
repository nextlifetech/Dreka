import QtQuick 2.12
import QtQuick.Layouts 1.12
import Industrial.Controls 1.0 as Controls
import Dreka 1.0

import "Map"
import "Routes"
import "Waypoints"
import "Vehicles"

Controls.ApplicationWindow {
    id: main

    readonly property real availableHeight: main.height - map.controlHeight - menuBar.height -
                                            Controls.Theme.margins * 2

    visible: true
    width: 1280
    height: 768

    ClipboardController {
        id: clipboard
    }

    CesiumMap {
        id: map
        anchors.fill: parent
    }

    MapMenu {
        id: mapMenu
        anchors.fill: map
    }

    WaypointView {
        id: waypointEdit
        anchors.fill: map
    }

    RowLayout {
        id: menuBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: Controls.Theme.margins
        spacing: Controls.Theme.spacing

        Repeater {
            model: layout.menu

            Loader { source: modelData }
        }

        RoutesView {
            id: routes
        }
    }

    VehiclesView {
        id: dashboard
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: Controls.Theme.margins
    }
}

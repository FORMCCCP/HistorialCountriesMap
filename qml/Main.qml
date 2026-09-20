import QtQuick
import QtQuick.Controls
import HistorialCountriesMaps 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    MapView{
        anchors.fill: parent
        controller: Maincontroller
    }
}

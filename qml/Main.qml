import QtQuick
import QtQuick.Controls
import HistorialCountriesMaps 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("历史政治地图")

    Component.onCompleted: {
        if(!Maincontroller.loadRegions(Maincontroller.DasePath() + "/dataBase.db")){
            console.warn("数据加载失败")
        }else{
            console.log("数据加载成功")
        }
    }

    MapView{
        anchors.fill: parent
        controller: Maincontroller
    }
}

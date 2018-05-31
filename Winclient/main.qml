import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import  learnsafe.winclient 1.0
ApplicationWindow {
    id: appWind
    visible: true
    width: 640
    height: 480
    title: qsTr("LearnSafe Win Client")

    WinClient{
        id:winclient
    }

    SwipeView{
        id:view
        currentIndex: tabBar.currentIndex
        anchors.fill: appWind


        Registration {

        }

        Monitor {
        }

        Library{

        }
    }


   /*PageIndicator{

        id:mainView
        interactive: true
        count:view.count
        currentIndex: view.currentIndex

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

    }*/

    footer: TabBar {
        id: tabBar
        currentIndex: view.currentIndex

        TabButton {
            text: qsTr("Registration")
        }
        TabButton {
            text: qsTr("Monitor")
        }

        TabButton {
            text: qsTr("Library")
        }
    }





}

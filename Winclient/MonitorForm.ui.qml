import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {

    width: appWind.width
    height: appWind.height

    header: Label {
        text: "Monitor"
        wrapMode: Text.WordWrap
    }

    ListView{
        id:monitorList
        implicitWidth: 250
        implicitHeight: 250
        clip:true

        model:ListModel{
            ListElement{
                text:"Block On Capture"
                done:true
            }
            ListElement{
                text:"Auto Exit"
                done:false
            }
            ListElement{
                text:"Deactive"
                done:true
            }
        }

        delegate: RowLayout{
            width:monitorList.width
            CheckBox{
            checked: model.done
            }
            TextField{
                text:model.text
            Layout.fillWidth: true
            }
        }

       }
}

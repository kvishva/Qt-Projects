import QtQuick 2.7
import QtQuick.Controls 2.2

Page {

    width: appWind.width
    height: appWind.height

    header: Label {
        text: "Registration"
        wrapMode: Text.WordWrap
    }

    Text {
        id: textRegistration
        x: 116
        y: 73
        width: 63
        height: 23
        text: "Registration"
        font.family: "Times New Roman"
        wrapMode: Text.WordWrap
        font.pixelSize: 12
    }

    Text {
        id: textServerName
        x: 136
        y: 108
        width: 79
        height: 20
        text: qsTr("Server Name")
        font.pixelSize: 12
    }

    Text {
        id: textSchoolName
        x: 136
        y: 160
        width: 80
        text: qsTr("School Name")
        font.pixelSize: 12
    }

    Text {
        id: textDomain
        x: 136
        y: 217
        width: 80
        text: qsTr("Domain Name")
        font.pixelSize: 12
    }

    Text {
        id: textPort
        x: 136
        y: 262
        width: 80
        text: qsTr("Port No")
        font.pixelSize: 12
    }

    TextEdit {
        id: editServerName
        x: 243
        y: 108
        width: 80
        height: 20
        color: "#ff0000"

        text: winclient.serverName
        activeFocusOnPress: true
        cursorVisible: true
        textFormat: Text.AutoText
        clip: true
        z: 0
        transformOrigin: Item.Center

        selectionColor: "#804d00"
        font.pixelSize: 12

        onTextChanged: winclient.serverName = text
        focus: true

        Keys.onTabPressed: editSchoolName.cursorVisible = true

        cursorPosition: text.length
        //Keys.onPressed: cursorVisible = true
    }

    TextEdit {
        id: editSchoolName
        x: 243
        y: 160
        width: 80
        height: 20
        color: "#ff0000"
        text: winclient.schoolName
        clip: true
        font.pixelSize: 12
        onTextChanged: winclient.schoolName = text
        focus: false
        Keys.onTabPressed: editDomainName.cursorVisible = true

        cursorPosition: text.length
        cursorVisible: false
    }

    TextEdit {
        id: editDomainName
        x: 243
        y: 217
        width: 225
        height: 20
        color: "#ff0000"
        text: winclient.domainName
        clip: true
        //qsTr("http://development.learnsafe.com")
        font.pixelSize: 12
        onTextChanged: winclient.domainName = text
    }

    TextEdit {
        id: editPort
        x: 243
        y: 258
        width: 80
        height: 20
        color: "#ff0000"
        text: winclient.portNumber
        clip: true
        font.pixelSize: 12
        onTextChanged: winclient.portNumber = text
    }

    Rectangle {
        id: rectButton
        x: 324
        y: 331
        width: 82
        height: 36
        color: "#ffffff"
        radius: 4
        border.color: "#00ff00"
        border.width: 4
        objectName: "rectButtReg"

        Text {
            id: textButton
            x: rectButton.x
            y: rectButton.y
            width: rectButton.width
            height: rectButton.height
            anchors.fill: rectButton
            text: qsTr("Register")
            verticalAlignment: Text.AlignVCenter
            font.family: "Arial"
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        signal qmlSignal(string msg)
    }

    MouseArea {
        id: mouseAreaButton
        x: rectButton.x
        y: rectButton.y
        width: rectButton.width
        height: rectButton.height
        anchors.fill: rectButton
        hoverEnabled: true
    }

    Connections {
        target: mouseAreaButton
        onClicked: rectButton.qmlSignal("qml")
    }

    Rectangle {
        id: rectEditServer
        x: 242
        y: 108
        width: 80
        height: 20
        color: "#f9f8f8"
        radius: 2
        z: -1
        border.color: "#504141"
        clip: true
    }

    Rectangle {
        id: rectEditSchool
        x: 243
        y: 160
        width: 80
        height: 20
        color: "#ffffff"
        radius: 2
        border.color: "#605151"
        z: -1
        clip: true
    }

    Rectangle {
        id: rectEditDomain
        x: 243
        y: 217
        width: 225
        height: 20
        color: "#ffffff"
        radius: 1
        clip: true
        z: -1
        border.color: "#826b6b"
    }

    Rectangle {
        id: rectEditPort
        x: 243
        y: 258
        width: 80
        height: 20
        color: "#ffffff"
        radius: 2
        clip: true
        border.color: "#907070"
        z: -1
    }

    Connections {
        target: editServerName
        onFocusChanged: print("clicked")
    }
}

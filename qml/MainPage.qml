import QtQuick 2.0
import Sailfish.Silica 1.0

import org.askpassword 1.0

Page {
    id: main

    Rectangle {
        color: "black"
        anchors.fill: parent
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: Column {
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                title: qsTr("Sailfish OS")
            }

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                height: implicitHeight + Theme.paddingLarge
                color: Theme.highlightColor
                font.family: Theme.fontFamilyHeading
                font.pixelSize: Theme.fontSizeLarge
                text: qsTr("Password(s) requested")
                visible: listView.count > 0
                wrapMode: Text.WordWrap
            }
        }

        delegate: ListItem {
            contentHeight: itemsCol.height + Theme.paddingLarge

            Column {
                id: itemsCol

                anchors.verticalCenter: parent.verticalCenter
                spacing: Theme.paddingMedium
                width: main.width - 2*Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin

                Label {
                    text: title
                    truncationMode: TruncationMode.Fade
                    width: parent.width
                }

                Label {
                    color: Theme.secondaryColor
                    horizontalAlignment: Text.AlignRight
                    text: description
                    truncationMode: TruncationMode.Fade
                    width: parent.width
               }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("PasswordPage.qml"),
                               {
                                   "passtask": PasswordTaskList.get(getId),
                                   "mainPage": main
                               });
            }
        }

        model: PasswordTaskList

        BusyLabel {
            text: qsTr("Starting, please wait")
            running: listView.count == 0
        }
    }
}

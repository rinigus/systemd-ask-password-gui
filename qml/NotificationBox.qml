import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    id: main

    anchors.bottom: parent.bottom
    anchors.bottomMargin: Theme.paddingLarge

    // From Silica notification background
    color: Qt.tint(
               Theme.rgba(Theme.overlayBackgroundColor, Theme.opacityOverlay),
               Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity))
    height: label.height + 2*Theme.paddingLarge
    radius: Theme.paddingMedium
    visible: label.text
    width: parent.width - 2*x
    x: Theme.horizontalPageMargin

    Label {
        id: label
        anchors.centerIn: parent
        font.pixelSize: Theme.fontSizeSmall
        horizontalAlignment: Text.AlignHCenter
        width: parent.width - 2*Theme.paddingLarge
        wrapMode: Text.WordWrap
    }

    Timer {
        id: timer
        interval: 3000
        onTriggered: label.text = ""
    }

    MouseArea {
        anchors.fill: parent
        onClicked: label.text = ""
    }

    onVisibleChanged: {
        if (visible) {
            timer.interval = 6000;
            timer.restart();
        }
        else timer.stop();
    }

    function hide() {
        label.text = "";
        timer.stop();
    }

    function show(text, interval) {
        timer.stop();
        timer.interval = interval;
        label.text = text;
        timer.restart();
    }
}

import QtQuick 2.0
import Sailfish.Silica 1.0

import org.askpassword 1.0

Page {
    id: page

    // provided on construction as a part of push
    property var mainPage
    property PasswordTask passtask

    // filled at Component.onCompleted
    property string description
    property string title


    Rectangle {
        color: "black"
        anchors.fill: parent
    }

    Column {
        id: column
        anchors.top: page.top

        width: page.width
        spacing: Theme.paddingLarge

        Item {
            height: Theme.paddingSmall/10
            width: parent.width
        }

        Label {
            anchors.left: parent.left
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin
            color: Theme.highlightColor
            horizontalAlignment: Text.AlignRight
            font.family: Theme.fontFamilyHeading
            font.pixelSize: Theme.fontSizeLarge
            text: title
            wrapMode: Text.WordWrap
        }

        Label {
            anchors.left: parent.left
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin
            color: Theme.secondaryHighlightColor
            height: implicitHeight + Theme.paddingLarge
            horizontalAlignment: Text.AlignRight
            font.family: Theme.fontFamilyHeading
            font.pixelSize: Theme.fontSizeMedium
            text: description
            wrapMode: Text.WordWrap
        }

        PasswordField {
            id: input
            focus: true
            text: ""
            onActiveFocusChanged: focus = true
        }
    }

    Keyboard {
        id: kbd
        anchors.bottom: parent.bottom
        keyHeight: Math.min(input.height * 2, Theme.itemSizeMedium,
                            (page.height - column.y - column.height - Theme.paddingLarge) / 5)
        width: parent.width

        onBackspace: {
            var position = input.cursorPosition;
            if (position < 1) return;
            input.text = input.text.substring(0, position-1) +
                    input.text.substring(position, input.text.length);
            input.cursorPosition = position - 1;
        }

        onClicked: {
            var position = input.cursorPosition;
            input.text = input.text.substring(0, position) + txt +
                    input.text.substring(position, input.text.length);
            input.cursorPosition = position + 1;
        }

        onEnter: passtask.set(input.text)
    }

    Component.onCompleted: {
        // as properties are non-NOTIFYable, just assign the values
        description = passtask.description;
        title = passtask.title;
    }

    onPasstaskChanged: {
        if (passtask == null && pageStack.currentPage == page)
            pageStack.pop(mainPage);
    }
}

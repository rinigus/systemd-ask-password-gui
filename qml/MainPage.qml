import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    Rectangle {
        color: "black"
        anchors.fill: parent
    }

    Column {
        id: column
        anchors.top: page.top

        width: page.width
        spacing: Theme.paddingLarge

        PageHeader {
            title: qsTr("Password requested")
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
    }

}

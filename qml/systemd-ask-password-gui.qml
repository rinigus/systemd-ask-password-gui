import QtQuick 2.0
import Sailfish.Silica 1.0

import org.askpassword 1.0

ApplicationWindow
{
    initialPage: Component { MainPage { } }
    cover: undefined

    NotificationBox {
        id: passwordSentNotice

        Connections {
            target: PasswordTaskList
            onPasswordFailed: passwordSentNotice.show(noticeText, 3000)
            onPasswordSent: passwordSentNotice.show(noticeText, 5000)
        }
    }
}

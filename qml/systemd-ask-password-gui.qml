import QtQuick 2.0
import Sailfish.Silica 1.0

import org.askpassword 1.0

ApplicationWindow
{
    initialPage: Component { MainPage { } }
    cover: undefined

    NotificationBox {
        id: passwordSentNotice
        property string passid
    }

    Connections {
        target: PasswordTaskList
        onNewPassword: {
            if (passwordSentNotice.passid === passid) {
                passwordSentNotice.show(qsTr("Wrong password: %1").arg(title),
                                        3000);
            }
        }

        onPasswordSent: {
            passwordSentNotice.passid = passid;
            passwordSentNotice.show(noticeText, 6000);
        }
    }
}

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>

#include <sailfishapp.h>

#include "taskwatcher.h"
#include "passwordtasklist.h"

int main(int argc, char *argv[])
{
  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  TaskWatcher watcher(app.data());

  QScopedPointer<QQuickView> v;
  v.reset(SailfishApp::createView());
  //QQmlContext *rootContext = v->rootContext();

  qmlRegisterType<PasswordTask>("org.askpassword", 1, 0, "PasswordTask");

  qmlRegisterSingletonType<PasswordTaskList>("org.askpassword", 1, 0, "PasswordTaskList", [](QQmlEngine *, QJSEngine *) -> QObject * {
      return static_cast<QObject *>(PasswordTaskList::instance());
  });

  v->setSource(SailfishApp::pathTo("qml/systemd-ask-password-gui.qml"));
  v->show();

  return app->exec();
}

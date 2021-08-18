#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QQuickView>

#include <sailfishapp.h>

#include "taskwatcher.h"

int main(int argc, char *argv[])
{
  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  TaskWatcher watcher(app.data());

  QScopedPointer<QQuickView> v;
  v.reset(SailfishApp::createView());
  //QQmlContext *rootContext = v->rootContext();

  v->setSource(SailfishApp::pathTo("qml/systemd-ask-password-gui.qml"));
  v->show();

  return app->exec();
}

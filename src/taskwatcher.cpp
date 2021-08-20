#include "taskwatcher.h"
#include "passwordtasklist.h"

#include <QDebug>
#include <QDirIterator>

TaskWatcher::TaskWatcher(QObject *parent) : QObject(parent)
{
  QString d(QStringLiteral("/run/systemd/ask-password"));

  if (!m_watcher.addPath(d))
    qCritical() << "Failed to register directory watching service";

  qDebug() << "Watched directories" << m_watcher.directories();

  connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &TaskWatcher::onDirChanged);
  onDirChanged(d); // initialize
}

void TaskWatcher::onDirChanged(const QString &path)
{
  QDirIterator dir(path, {"ask.*"});
  QSet<QString> current;
  while (dir.hasNext())
    {
      QString next = dir.next();

      if (!PasswordTaskList::instance()->contains(next))
        PasswordTaskList::instance()->add(next);

      current.insert(next);
    }

  PasswordTaskList::instance()->removeMissing(current);
}


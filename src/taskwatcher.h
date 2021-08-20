#ifndef TASKWATCHER_H
#define TASKWATCHER_H

#include <QObject>

#include <QFileSystemWatcher>
#include <QSet>
#include <QString>

class TaskWatcher : public QObject
{
  Q_OBJECT
public:
  explicit TaskWatcher(QObject *parent = nullptr);

signals:

protected:
  void processTask(const QString &path);

  void onDirChanged(const QString &path);

private:
  QFileSystemWatcher m_watcher;
};

#endif // TASKWATCHER_H

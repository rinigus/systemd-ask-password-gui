#ifndef PASSWORDTASKLIST_H
#define PASSWORDTASKLIST_H

#include <QHash>
#include <QObject>
#include <QSet>
#include <QSharedPointer>

#include "passwordtask.h"

class PasswordTaskList : public QObject
{
  Q_OBJECT
public:
  explicit PasswordTaskList(QObject *parent = nullptr);

  bool add(QString inifname);
  bool contains(QString inifname) const;
  void removeMissing(QSet<QString> current);
  void remove(QString inifname);

signals:

private:
  QHash<QString, QSharedPointer<PasswordTask>> m_tasks;
};

#endif // PASSWORDTASKLIST_H

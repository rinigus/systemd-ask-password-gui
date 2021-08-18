#include "passwordtasklist.h"

#include <QStringList>

PasswordTaskList::PasswordTaskList(QObject *parent) : QObject(parent)
{
}

bool PasswordTaskList::add(QString inifname)
{
  QSharedPointer<PasswordTask> p(new PasswordTask(inifname, this));
  if (!(*p)) return false;
  m_tasks[p->ini()] = p;
  return true;
}

bool PasswordTaskList::contains(QString inifname) const
{
  return m_tasks.contains(inifname);
}

void PasswordTaskList::remove(QString inifname)
{
  m_tasks.remove(inifname);
}

void PasswordTaskList::removeMissing(QSet<QString> current)
{
  QStringList keys = m_tasks.keys();
  for (QString k: keys)
    {
      if (!current.contains(k))
        m_tasks.remove(k);
    }
}

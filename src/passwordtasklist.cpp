#include "passwordtasklist.h"

#include <QStringList>

#include <algorithm>

PasswordTaskList* PasswordTaskList::s_instance = nullptr;

PasswordTaskList::PasswordTaskList()
{
  connect(this, &PasswordTaskList::passwordSent, this, &PasswordTaskList::onPasswordSent);
}

PasswordTaskList* PasswordTaskList::instance()
{
  if (!s_instance) s_instance = new PasswordTaskList();
  return s_instance;
}

bool PasswordTaskList::add(QString inifname)
{
  QSharedPointer<PasswordTask> p(new PasswordTask(inifname, this));
  if (!(*p)) return false;
  m_tasks[p->ini()] = p;
  sort();

  // check if password was sent earlier. if it was then
  // consider it failed
  if (m_tasks_sent.contains(p->id()))
    emit passwordFailed(tr("Password failed: %1").arg(p->title()));
  return true;
}

bool PasswordTaskList::contains(QString inifname) const
{
  return m_tasks.contains(inifname);
}

void PasswordTaskList::onPasswordSent(QString passid, QString /*noticeText*/)
{
  m_tasks_sent.insert(passid);
}

void PasswordTaskList::removeMissing(QSet<QString> current)
{
  QStringList keys = m_tasks.keys();
  for (QString k: keys)
    {
      if (!current.contains(k))
        m_tasks.remove(k);
    }

  if (m_tasks.size() != keys.size()) sort();
}

void PasswordTaskList::sort()
{
  beginResetModel();
  m_tasks_sorted = m_tasks.keys();
  std::sort(m_tasks_sorted.begin(), m_tasks_sorted.end(),
            [this](QString i, QString j)
  {
    QString v1 = m_tasks[i]->id();
    QString v2 = m_tasks[j]->id();
    return v1 < v2;
  });
  endResetModel();
}

PasswordTask* PasswordTaskList::get(QString inifname)
{
  return m_tasks.value(inifname, QSharedPointer<PasswordTask>()).data();
}

// Model API
QHash<int, QByteArray> PasswordTaskList::roleNames() const
{
  return {
      { RoleNames::GetIdRole, QByteArrayLiteral("getId") },
      { RoleNames::TitleRole, QByteArrayLiteral("title") },
      { RoleNames::DescriptionRole, QByteArrayLiteral("description") },
    };
}

QVariant PasswordTaskList::data(const QModelIndex &index, int role) const
{
  const int row = index.row();
  if (!index.isValid() || row < 0 || row >= m_tasks.size())
    return {};

  const QString getid = m_tasks_sorted[row];
  const QSharedPointer<PasswordTask> p = m_tasks[ getid ];
  switch (role) {
    case RoleNames::GetIdRole: return getid;
    case RoleNames::TitleRole: return p->title();
    case RoleNames::DescriptionRole: return p->description();
    }

  return {};
}

int PasswordTaskList::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : m_tasks_sorted.size();
}

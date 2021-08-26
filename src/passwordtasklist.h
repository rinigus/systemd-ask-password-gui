#ifndef PASSWORDTASKLIST_H
#define PASSWORDTASKLIST_H

#include <QAbstractListModel>
#include <QHash>
#include <QObject>
#include <QSet>
#include <QSharedPointer>

#include "passwordtask.h"

class PasswordTaskList : public QAbstractListModel
{
  Q_OBJECT

  enum RoleNames { GetIdRole = Qt::UserRole + 1,
                   TitleRole,
                   DescriptionRole };


private:
  explicit PasswordTaskList();

public:
  bool add(QString inifname);
  bool contains(QString inifname) const;
  void removeMissing(QSet<QString> current);

  Q_INVOKABLE PasswordTask* get(QString inifname);

  // Model API
  QHash<int, QByteArray> roleNames() const override;
  QVariant data(const QModelIndex &index, int role) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

signals:
  void newPassword(QString passid, QString title);
  void passwordRemoved(QString passid);
  void passwordSent(QString passid, QString noticeText);

public: // static
  static PasswordTaskList* instance();

private:
  void sort();

private:
  QHash<QString, QSharedPointer<PasswordTask>> m_tasks;
  QStringList m_tasks_sorted;

private: // static
  static PasswordTaskList *s_instance;
};

#endif // PASSWORDTASKLIST_H

#ifndef PASSWORDTASK_H
#define PASSWORDTASK_H

#include <QObject>

#include <QString>

typedef uint64_t usec_t;

class PasswordTask : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString id READ id)

public:
  PasswordTask(QObject *parent = nullptr);
  PasswordTask(QString inifname, QObject *parent = nullptr);
  ~PasswordTask();

  operator bool() const;
  bool expired() const;
  double dtExpiration() const;

  QString id() const { return m_id; }
  QString ini() const { return m_ini; }

private:
  int     m_pid{-1};
  int     m_echo{0};
  usec_t  m_not_after{0};
  QString m_id;
  QString m_ini;
  QString m_message;
  QString m_socket;
};

#endif // PASSWORDTASK_H

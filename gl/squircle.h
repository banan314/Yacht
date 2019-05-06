#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QQuickItem>
#include <QWidget>

class Squircle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
public:
    Squircle();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void paint();
    void cleanup();
    void sync();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QOpenGLShaderProgram *m_program;

    qreal m_t;
    qreal m_thread_t;
};

#endif // SQUIRCLE_H

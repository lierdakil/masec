#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    explicit MainWindow(QString filename, QWidget *parent = 0);
    virtual ~MainWindow();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H

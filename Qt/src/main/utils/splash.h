#pragma once

#include <QtGui/QDialog>

class SplashDlg : public QDialog
{
    Q_OBJECT

public:
    static SplashDlg* getInstance();

signals:
    void exitSplash();

public slots:
    void splash();

protected:
    virtual bool event ( QEvent * event );

private:
    SplashDlg(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~SplashDlg();

private:
};

#pragma once

#include <QtGui/QDialog>
#include <QFileInfoList>

class QLabel;
class CBottomBarMgr;
class CDataManager;
class CUserNoticeReport;
class QTimer;
class CPublicNotice : public QDialog
{
    Q_OBJECT

public:
    enum NOTICE_TYPE{
        IMAGE_TYPE,
        NOTCIE_CONNECT_TYPE,
        USER_INFO_TYPE,

        NONE_TYPE
    };
    static CPublicNotice* getInstance();

signals:
    void exitSplash();

public slots:
    void splash();
    void stopSplash();

protected:
    virtual bool event ( QEvent * event);

private slots:
    void onTimerOut();

private:
    CPublicNotice(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~CPublicNotice();

    void initUI();

    void createNotice();
    bool getNoticeConnects();
    void showNotice(int index);
    void clearNotice();

    void createNoticeReport();
    bool getNoticeReportInfos();
    void showNoticeReport(int index);
    void clearNoticeReport();

    bool getImagesResource();
    void showImage(int index);
    void clearImage();

    void getNoticeResource();

    void shouDefaultStyle();

private:
    QLabel              *m_topBar;                                      // 标题
    QLabel              *m_titleLabel;                                  // 公告标题
    QLabel              *m_contentLabel;                                // 公告内容区
    CBottomBarMgr       *m_bottomBar;
    CDataManager        *m_dataMgr;
    CUserNoticeReport   *m_pUserNoticeReport;

    QTimer              *m_pTimer;
    QFileInfoList       m_imageInfoList;

    NOTICE_TYPE         m_noticeType;
    int                 m_noticeIndex;                                  // 公告索引 三种类型都可以使用
};

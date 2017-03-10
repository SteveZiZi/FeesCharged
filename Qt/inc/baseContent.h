#pragma once

#include <QtGui/QWidget>


class CBaseContentPrivate;
class CBaseContent : public QWidget {
    Q_OBJECT
public:
    CBaseContent(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CBaseContent();

    virtual void initUI();

    void setContentCaption(const QString &caption);
    void setContentIcon(QIcon *icon);

    bool setMenuCaption(int item, const QString &caption);

    /*
     *  @Func:  setMenuContentPage
     *  @Desc:  ���ò˵����Ӧ��ʾ��ҳ��
     *  
     *  @Param[in]:
     *      item:   �˵���ţ�0~3
     *      pageId: content������ҳ��ID��, -1����ЧID�ţ��ڲ˵���Ч����£������л�ҳ��
     *  
     *  @Return:
     *      true:   ���óɹ�
     *      false:  ����ʧ�ܣ�������itemԽ����
     */
    bool setMenuContentPage(int item, int pageId);
    
    //���²˵������ͼ��
    void updateMenuInfo();
    // ���ð�ť�Ƿ���ʾ
    void setBtnVisiable(bool flag);

protected:
    void keyPressEvent(QKeyEvent *e);

    /*
     *  @Func:  OnF1Press ~ OnF4Press
     *  @Desc:  ��Ӧ��Ӧ�˵����µ��¼�
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   �˵������ɣ���ʹ�����˲˵���Ӧ���л�ҳ��Ҳ������ʾҳ��
     *      false:  �����˲˵���Ӧ���л�ҳ�潫����ʾ��ҳ��
     */
    virtual bool OnF1Press() {return false;}
    virtual bool OnF2Press() {return false;}
    virtual bool OnF3Press() {return false;}
    virtual bool OnF4Press() {return false;}

protected slots:
    void menu1Pressed();
    void menu2Pressed();
    void menu3Pressed();
    void menu4Pressed();

private:
    void btnLayout();

protected:
    CBaseContentPrivate* m_data;
};


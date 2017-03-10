#pragma once


//////////////////////////////////////////////////////////////////////////
// 窗口定位坐标
//////////////////////////////////////////////////////////////////////////
#define SCREEN_POS_L                    0
#define SCREEN_POS_T                    0
#define SCREEN_SIZE_W                   800
#define SCREEN_SIZE_H                   480

// topbar
#define TOP_BAR_POS_L                   0
#define TOP_BAR_POS_T                   0
#define TOP_BAR_SIZE_W                  SCREEN_SIZE_W
#define TOP_BAR_SIZE_H                  38

// bottombar
#define BOTTOM_BAR_POS_L                0
#define BOTTOM_BAR_POS_T                442
#define BOTTOM_BAR_SIZE_W               SCREEN_SIZE_W
#define BOTTOM_BAR_SIZE_H              (SCREEN_SIZE_H-BOTTOM_BAR_POS_T)

//naviMenu
#define NAVI_MENU_SIZE_W                112
#define NAVI_MENU_SIZE_H               (SCREEN_SIZE_H-TOP_BAR_SIZE_H-BOTTOM_BAR_SIZE_H-2)
#define NAVI_MENU_POS_L                (SCREEN_SIZE_W-NAVI_MENU_SIZE_W)
#define NAVI_MENU_POS_T                (TOP_BAR_SIZE_H+1)

#define NAVI_MENU_ITEM_H                100                              /*  单个导航菜单的高度          */

// body
#define CONTENT_POS_L                  (0)
#define CONTENT_POS_T                  (NAVI_MENU_POS_T)
#define CONTENT_SIZE_W                 (SCREEN_SIZE_W-NAVI_MENU_SIZE_W)
#define CONTENT_SIZE_H                 (NAVI_MENU_SIZE_H)


/*
 *  color
 */
#define TABLE_LINE_CLR          qRgb(74, 108, 150)



#define _create_label(label, label_caption, x, y)\
    label = new QLabel(this);\
    label->setText(label_caption);\
    label->move(x, y)

#define _create_comboBox(comboBox, comboBoxName, x, y)\
    comboBox = new MComboBox(this);\
    comboBox->setObjectName(comboBoxName);\
    comboBox->move(x, y)

#define _create_lineEdit_float(dataEdit, dataEditName, data, prec, unit, x, y) \
    dataEdit = new MDataEdit(data, 0, 'f', prec, unit, this);\
    dataEdit->setObjectName(dataEditName);\
    dataEdit->move(x, y)

#define _create_lineEdit_int(dataEdit, dataEditName, data, unit, x, y) \
    dataEdit = new MDataEdit(data, 0, 10, unit, this);\
    dataEdit->setObjectName(dataEditName);\
    dataEdit->move(x, y)



// 用户卡
#define USER_MAIN_PAGE_ID            0x1000

#define USER_CHARGE_PAGE_ID          0x1100

#define USER_QUERY_PAGE_ID           0x1200
#define USER_QUERY_CURRENT_ELEC_ID   0x1210
#define USER_QUERY_HISTORY_ELEC_ID   0x1220
#define USER_ELEC_RECORD_ID          0x1230

#define USER_REPAY                   0x1300

#define USER_NOTICE_ID               0x1400

// 维护卡
#define MANAGER_MAIN_PAGE_ID         0x2000

#define MANAGER_MAINTAIN_PAGE_ID     0x2100
#define MANAGER_TERMINAL_INFO_ID     0x2110
#define MANAGER_PARAM_SETTING_ID     0x2120
#define MANAGER_SYSTEM_PARAM_ID      0x2121
#define MANAGER_SYSTEM_MAINTENACE_ID 0x2122

#define MANAGER_POWER_MONITOR_ID     0x2200

// Msz
#define JUST_FOR_DEMO                1
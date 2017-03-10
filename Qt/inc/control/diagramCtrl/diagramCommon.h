#pragma once


#include <QtCore/QRect>

///
/// @brief
///     控件的背景颜色
///
#define WFG_CTRL_BKGND_COLOR    qRgb(204,255,255)            ///< 黑色

///
/// @brief
///     字体的默认设置，包括字体样式以及标题、X-Y标尺刻度、波形描述信息和游标描述信息的字体大小和颜色
///
#define WFG_FONT_STYLE             "微软雅黑"
#define WFG_TITLE_FONT_SIZE        10
#define WFG_TITLE_FONT_COLOR       qRgb(0,0,0)      ///< 白色
#define WFG_SCALE_FONT_SIZE        10
#define WFG_SCALE_FONT_COLOR       qRgb(0,0,0)      ///< 暖灰色
#define WFG_WAVEDESC_FONT_SIZE     15
#define WFG_CURSORDESC_FONT_SIZE   10
#define WFG_CURSORDESC1_FONT_COLOR qRgb(255,140,0)        ///< 深橙色
#define WFG_CURSORDESC2_FONT_COLOR qRgb(220,20,60)        ///< 腥红

#define WFG_LINE_SOLID              1

///
/// @brief
///     波形显示区域的默认设置：边框和内部网格的线条的类型、宽度和颜色
///
#define WFG_WAVE_AREA_BORDER_STYLE Qt::SolidLine
#define WFG_WAVE_AREA_BORDER_WIDTH 1
#define WFG_WAVE_AREA_GRID_STYLE   Qt::DashLine
#define WFG_WAVE_AREA_GRID_WIDTH   0
#define WFG_WAVE_AREA_GRID_COLOR   qRgb(128,128,105)   ///< 冷灰色

#define WFG_WAVE_LINE_STYLE        Qt::SolidLine
#define WFG_WAVE_LINE_WIDTH        1

namespace MDIAGRAM {

    enum {
        BACKGROUND_ID       = 0x00000000,
        SCALE_ID            = 0x01000000,
        DESCRIPTION_ID      = 0x02000000,

        FOREGROUND_ID       = 0x81000000,
        TREND_ID            = 0x82000000,
        GRID_ID             = 0x83000000,

        USER_ID             = 0xE0000000,

        NULL_COMPONENT_ID   = 0xFFFFFFFF
    };

    enum {
        NEGATIVE = 1 << 0,      //正向坐标
        POSITIVE = 1 << 1       //负向坐标
    };


    struct Margin
    {
        int left;
        int right;
        int top;
        int bottom;
        Margin(int x, int y) {
            left = right = x;
            top = bottom = y;
        }
        Margin(int l, int r, int t, int b) {
            left = l; right = r;
            top = t; bottom = b;
        }
        Margin() {
            left = right = 0;
            top = bottom = 0;
        }
        void setMargin(int x, int y) {
            left = right = x;
            top = bottom = y;
        }
        void setMargin(int l, int t, int r, int b) {
            left = l; right = r;
            top = t; bottom = b;
        }
    };

    struct ScaleInfo
    {
        int gridCountX;                                             //  XY轴点的个数
        int gridCountY;
        int intervalX;                                              //  刻度间隔
        int intervalY;
        int divIntervalX;                                           //  小刻度尺  
        int divIntervalY;
        float fScaleX;                                              //  刻度尺的比例
        float fScaleY;

        Margin  magin;
        QRect   rectClient;

        int axisDirX;                                                //  X Y轴方向  
        int axisDirY;

        QPoint GetCenterPoint() {
            QPoint center;
            if (axisDirY == POSITIVE)
                center.setY(rectClient.bottom()-magin.bottom);
            else if(axisDirY == NEGATIVE) 
                center.setY(rectClient.top()+magin.top);
            else if (axisDirY == (NEGATIVE|POSITIVE))
                center.setY(rectClient.height()/2+8);
            else
                Q_ASSERT(false);

            if (axisDirX == POSITIVE)
                center.setX(rectClient.left()+magin.left);
            else if(axisDirX == NEGATIVE) 
                center.setX(rectClient.right()-magin.right);
            else if (axisDirX == (NEGATIVE|POSITIVE))
                center.setX(rectClient.width()/2-10);
            else
                Q_ASSERT(false);

            return center;
        }

        //y轴的总长度
        int GetWidth() {
            return (rectClient.width() - (magin.left+magin.right) - 10);
        }
        //x轴的总长度
        int GetHeight() {
            return (rectClient.height() - (magin.top + magin.bottom) - 10);
        }

        // x轴刻度间的像素点
        float GetSpanWidthX() {
            return (float)(rectClient.width() - (magin.left+magin.right) - 10) / (float)gridCountX;
        }
        // y轴刻度间的像素点
        float GetSpanWidthY() {
            return (float)(rectClient.height() - (magin.top + magin.bottom) - 10) / (float)gridCountY;
        }
    };

};



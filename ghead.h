//=================================
//创建：wangzhengwei
//时间：2012-06-08
//描述：主要用于定义全局的宏、数据类型、变量等
//          这里同时包含了一些常用的头文件
//=================================
#ifndef __GHEAD_H__
#define __GHEAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string>
#include <strings.h>
#include <vector>
#include <QDebug>

using namespace std;
#define MAIN_MENU_COUNT      5
#define SECOND_MENU_COUNT    6

//部分函数
inline int Round(float f_) { return int (f_ + 0.5);}

//液晶屏尺寸变化时，修改这两个宏
#define LCD_W  800.0
#define LCD_H  600.0

#define W2LCD(x)  Round((x) * LCD_W / 800.0)
#define H2LCD(y)   Round((y) * LCD_H / 600.0)

#define  g_pi   (3.1415926f / 180.0f)

//浮点数比较宏定义
#define FLT_EPSILON     0.00001f
#define DB_EPSILON      0.0000001f

#define FLT_EZ(a) (a <= FLT_EPSILON && a >= -FLT_EPSILON)
#define FLT_E(a, b) (FLT_EZ(a -b))
#define FLT_L(a, b)  (a < b)
#define FLT_LE(a, b) (FLT_L(a, b) || FLT_E(a, b))
#define FLT_G(a, b) (a > b)
#define FLT_GE(a, b) (FLT_G(a, b) || FLT_E(a, b))

#define DB_EZ(a) (a <= DB_EPSILON && a >= -DB_EPSILON)
#define DB_E(a, b) (DB_EZ(a-b))
#define DB_L(a, b)  (a < b)
#define DB_LE(a, b) (DB_L(a, b) || DB_E(a, b))
#define DB_G(a, b) (a > b)
#define DB_GE(a, b) (DB_G(a, b) || DB_E(a, b))

#define MAX(a, b)  ((a) >= (b) ? (a) : (b))
#define MIN(a, b)  ((a) <= b ? (a) : (b))


//界面颜色 在状态相关
enum WIND_STATE
{
        INVALID_STATE = -1,
        NORMAL_WIND = 0,
        ACTIVE_WIND,
        STATE_COUNT,
};

enum LANG
{
       LANG_CHN = 0,
       LANG_ENG,
       LANG_NUM,
};


#define COLOR_F  30


#define DISABLE_BK_COLOR  QColor(212, 207, 178, COLOR_F)
#define DISABLE_FG_COLOR  QColor(152, 151, 148, COLOR_F)


#define RECT_RADIUS   0  //图形的圆角
#define MENU_FONT_SIZE  20 //菜单字体

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0])) //数组个数


//保存全局的类指针，方便统一释放
extern QList<QObject*> g_pListObject;

#define  AXIS_NUM  6
#define  PAGE_PARAM_COUNT  50
#define  MAX_LINE_READ  256
#define MAX_FILE_NAME_SIZE  128

#endif

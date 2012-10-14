#ifndef  __GPAGE_H__
#define  __GPAGE_H__

//体系单位定义
#define UNIT_MM   0
#define UNIT_INC    1

//
#define MAX_LAW_QTY  1024       //最大聚焦法则个数
#define MAX_GROUP_LAW_QTY  256  //组内最大聚焦法则数
#define MAX_CHANEL_ELEM  32    //最大通道数

#define MAX_GROUP_QTY  25    //最大组数

#define GRP_PA_MODE       0      //PA UT1 UT2探头契块模式
#define GRP_UT1_MODE     1
#define GRP_UT2_MODE     2

#define UT_CONVESION_MODE 0   //UT1时，对应的选择
#define UT_TOFD_MOD       1


#define WELD_EDIT_SEL    0     //探头的选择和自定义
#define WELD_EDIT_DEF    1

#define PA_LAW_SECTORAL  0  //聚焦法则的扇形
#define PA_LAW_LINEAR       1  //线性

#define PA_LAW_SOUNDPATH  0 //声程
#define PA_LAW_TRUEDEPTH   1//真实深度

#define PULSE_TXRX_PC      0  //收发模式
#define PULSE_TXRX_PE      1

const double g_dClickStep4[4] = {0.01, 0.1, 1.0, 10.0};
const double g_dClickStep3[3] = {0.01, 0.1, 1.0};
const float     g_fClickStep4[4] = {0.1, 1.0, 10.0, 50.0};
const float     g_fClickStep3[3] = {0.1, 1.0, 10.0};
const float     g_fClickStep2[2] = {0.1, 1.0};
const int        g_iClickStep4[4] = {1, 5, 10, 50};
const int        g_iClickStep3[3] = {1, 5, 10};
const int        g_iClickStep2[2] = {1, 5};
#endif // GPAGE_H

#ifndef __ADDRDEF_H__
#define __ADDRDEF_H__
//kongzhicanshu dizi
namespace CTL_PARAM_ADDR
{

    enum CTR_PARAM
    {
        ADDR_CTR_BASE = 2048,
        AXIS_X_POS = ADDR_CTR_BASE,
        AXIS_Y_POS,
        AXIS_Z_POS,
        AXIS_A_POS,
        AXIS_B_POS,
        AXIS_U_POS,
//=================================
        RUN_STATUS = 2560,
        ALRAM_STATUS,
        CTRL_CMD = RUN_STATUS + 6,
        CTRL_PARAM0,
        CTRL_PARAM1,
        CTRL_PARAM2,
        CTRL_PARAM3,
//===================================
        IOPORT_INPUT0 = RUN_STATUS + 16,
        IOPORT_INPUT1,
        IOPORT_OUTPUT0,
        IOPORT_OUTPUT1,
//==========================
        DIRLL0_CUR_ROW = RUN_STATUS + 32,
        DIRLL0_CUR_POS,
        DIRLL1_CUR_ROW,
        DIRLL1_CUR_POS,
        DIRLL2_CUR_ROW,
        DIRLL2_CUR_POS,
        DIRLL3_CUR_ROW,
        DIRLL3_CUR_POS,
        DIRLL4_CUR_ROW,
        DIRLL4_CUR_POS,
//==================================
        ROW_QTY = RUN_STATUS + 154,
        ROW0_HOLE_QTY,
        ROW1_HOLE_QTY,
        ROW2_HOLE_QTY,
        ROW3_HOLE_QTY,
        ROW4_HOLE_QTY,
        ROW5_HOLE_QTY,
        ROW6_HOLE_QTY,
        ROW7_HOLE_QTY,
        ROW8_HOLE_QTY,
        ROW9_HOLE_QTY,
        ROW10_HOLE_QTY,
        ROW11_HOLE_QTY,
    };
};

//tongyong bianji canshu dizi
namespace GEN_PARAM
{
    enum SYS_PARAM
    {
        //=============
        DRILL_AXIS_QTY = 0,
        RUN_QUERY_TIME,
        //================
        OFFSET_LIMIT = 512,
        AXIS_LINMIT_XP = OFFSET_LIMIT,
        AXIS_LINMIT_YP,
        AXIS_LINMIT_ZP,
        AXIS_LINMIT_AP,
        AXIS_LINMIT_BP,
        AXIS_LINMIT_UP,
        AXIS_LINMIT_XN ,
        AXIS_LINMIT_YN,     
        AXIS_LINMIT_ZN,        
        AXIS_LINMIT_AN,        
        AXIS_LINMIT_BN,     
        AXIS_LINMIT_UN,        
        //==============
        OFFSET_SPEED = 1024,
        AXIS_SPPED_X = OFFSET_SPEED,
        AXIS_SPPED_Y,
        AXIS_SPPED_Z,
        AXIS_SPPED_A,
        AXIS_SPPED_B,
        AXIS_SPPED_U,
        MANUAL_SPPED,
        STEP_LENGHT,
    };
};

//zuankong canshu dizi
namespace HOLE_POS
{
    enum HOLE_POS_XY
    {
        ROW_MAX = 100,
        HOLE_ROW_MAX = 128,
        HOLE_POS_BASE = 3072,
        ADDR_ROW0 = HOLE_POS_BASE,
    };
};

namespace CMD_VAL
{
    enum CMD
    {

    };
};
#endif // ADDRDEF_H

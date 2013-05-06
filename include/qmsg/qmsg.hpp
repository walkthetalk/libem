#pragma once

#include "qtype.hpp"

/***********************************************************************************************************************
* 消息频道定义
***********************************************************************************************************************/
#define QWSC_ORGNIZIATION "jilong"


#define QWSC_MODULE_APP "app"
#define QWSC_MODULE_SRV "srv"

#define QWSC_CHANNEL_GUI QWSC_ORGNIZIATION##"/"##QWSC_MODULE_APP##"/"##"gui"
#define QWSC_CHANNEL_FUSION QWSC_ORGNIZIATION##"/"##QWSC_MODULE_SRV##"/"##"fusion"


/***********************************************************************************************************************
* 消息类型定义
***********************************************************************************************************************/
#define MSG_START_FUSION         "MSG_START_FUSION"
#define MSG_STOP_FUSION          "MSG_STOP_FUSION"
#define MSG_START_HEAT           "MSG_START_HEAT"
#define MSG_STOP_HEAT            "MSG_STOP_HEAT"
#define MSG_MOVE_MOTOR           "MSG_MOVE_MOTOR"
#define MSG_STOP_MOTOR           "MSG_STOP_MOTOR"
#define MSG_MOVE_CAMERA          "MSG_MOVE_CAMERA"
#define MSG_START_TEST1          "MSG_START_TEST1"
#define MSG_SET_DISPLAY_MODE     "MSG_SET_DISPLAY_MODE"

/***********************************************************************************************************************
* 加热模块数据定义
***********************************************************************************************************************/

/* 光纤材料类型 */
typedef enum tagMaterialType
{
        MATERIAL_STANDARD = 0,                           /* Standard */
        MATERIAL_MICRO_250 = 1,                          /* Micro_250 */
        MATERIAL_MICRO_400 = 2,                          /* Micro_400 */
        MATERIAL_MICRO_900 = 3,                          /* Micro_900 */

        MATERIAL_NULL
}MATERIAL_TYPE;

/*光纤长度类型*/
typedef enum tagFibreLenType
{
        FIBRE_LEN_40MM = 0,                              /* 40mm */
        FIBRE_LEN_60MM = 1,                              /* 60mm */

        FIBRE_LEN_NULL
}FIBRE_LEN_TYPE;


/***********************************************************************************************************************
* 熔接模块数据定义
***********************************************************************************************************************/

/* 熔接模式 */
typedef enum tagWeldPattern
{
        WELD_PATTERN_AUTO,
        WELD_PATTERN_CALIBRATE,
        WELD_PATTERN_NORMAL,
        WELD_PATTERN_SPECIAL,
        WELD_PATERN_BLANK
}WELD_PATTERN_E;


/* 光纤类型 */
typedef enum tagFibreType
{
        FIBRE_TYPE_SM,
        FIBRE_TYPE_DS,
        FIBRE_TYPE_NZ,
        FIBRE_TYPE_NM,
        FIBRE_TYPE_LF_LF,
        FIBRE_TYPE_RS_RS,
        FIBRE_TYPE_RCH_RCH,
        FIBRE_TYPE_HI_HI,
        FIBRE_TYPE_FX_FX,
        FIBRE_TYPE_SM080,
        FIBRE_TYPE_SM80125,
        FIBRE_TYPE_TR_TR,
        FIBRE_TYPE_TRU_TRU,
        FIBRE_TYPE_FR_FR,
        FIBRE_TYPE_MT_MT,
        FIBRE_TYPE_ULA,
        FIBRE_TYPE_LA_LA,
        FIBRE_TYPE_SS_SS,

        FIBRE_TYPE_END
}FIBRE_TYPE_E;

/* 光纤对齐方式 */
typedef enum tagFibreAlignment
{
        FIBRE_ALIGN_FINE_CORE,                          /* 精细对芯 */
        FIBRE_ALIGN_CLADDING,                           /* 包层 */
        FIBRE_ALIGN_CORE,                               /* 纤芯 */
        FIBRE_ALIGN_MANUAL                              /* 手动 */
}FIBRE_ALIGNMENT_E;



typedef enum tagFibreShiftCompensantion
{
        FIBRE_SHIFT_COMPENSANTION_AUTOMATIC            /* 纤芯偏移补偿功能:自动 */
}FIBRE_SHIFT_COMPENSANTION_E;


/* 光纤算好估计方式 */
typedef enum tagFibreLossEstimation
{
        FIBRE_LOSS_ESTIMATION_DELICATE,                 /* 精细估算 */
        FIBRE_LOSS_ESTIMATION_CLADDING,                 /* 包层估算 */
        FIBRE_LOSS_ESTIMATION_CORE,                     /* 纤芯估算 */
        FIBRE_LOSS_ESTIMATION_OFF                       /* 关闭 */
}FIBRE_LOSS_ESTIMATION_E;


/* 图像坐标显示方式 */
typedef enum tagCoordinate
{
        COOR_X,                                         /* X */
        COOR_Y,                                         /* Y */
        COOR_X1Y,                                       /* X/Y */
        COOR_X2Y,                                       /* X|Y */
        COOR_X3Y,                                       /* X=>Y */
        COOR_X4Y                                        /* Y=>X */
}COORDINATE_E;


/**
 * \brief service state
 */
enum class svc_state_t{
    fs_reset,
    fs_idle,
    fs_clring,
    ///
    heat_idle
};

/**
 * \brief motor id
 */
enum motorId_t {
    LZ = 0,	// left z
    RZ,	// right z
    X,	// x
    Y,	// y
    NUM	// total number
};

typedef enum
{
    DISPLAY_MODE_X  = 0x0,	/// only x
    DISPLAY_MODE_Y  = 0x1,	/// only y
    DISPLAY_MODE_TB = 0x2,	/// top <-> bottom
    DISPLAY_MODE_LR = 0x3,	/// left <-> right
    DISPLAY_MODE_N  = 0x4	/// no
} DISPLAY_MODE_E;


/* 加热参数 */
typedef struct tagHeatPara                                 /* 加热参数 */
{
    MATERIAL_TYPE eMaterialType;                       /* 材料类型 */
    FIBRE_LEN_TYPE eFibreLenType;                      /* 长度类型 */
    BOOL bHeatControl;                                 /* 是否使用加热控制 */
    ULONG ulHeatMinute;                                /* 加热时间 */
    ULONG ulHeatTemperature;                           /* 加热温度 */
    ULONG ulHeatFinishTemperature;                     /* 结束温度 */
}HEAT_PARA_S;

/* 熔接参数 */
typedef struct tagWeldPara
{
    WELD_PATTERN_E eWeldPattern;                    /* 熔接模式 */
    FIBRE_TYPE_E eFibreType;                        /* 光纤类型 */
    FIBRE_ALIGNMENT_E eFibreAlignment;              /* 光纤对准选择 */
    BOOL bXImageFocus;                              /* x图像聚焦 */
    BOOL bYImageFocus;                              /* y图像聚焦 */
    FIBRE_SHIFT_COMPENSANTION_E eFibreShift;        /* 纤芯偏移补偿功能 */
    BOOL bDischargeStrengthAdjustment;              /* 放电强度自动调整 */
    BOOL bTensionSet;                               /* 拉力测试 */
    FLOAT fCutAngleLimit;                           /* 切割角限定 */
    FLOAT fLossLimit;                               /* 损耗限定 */
    FLOAT fFibreAngleLimit;                         /* 纤芯角限定 */
    ULONG ulCleanDischargeTime;                     /* 清洁放电时间 */
    ULONG ulFibreIntervalSetup;                     /* 光纤间隙设定 */
    LONG  lWeldPosSetup;                            /* 熔接位置设定 */
    ULONG ulFibrePreWeldingStrength;                /* 光纤预熔强度 */
    ULONG ulFibrePreWeldingTime;                    /* 光纤预熔时间 */
    ULONG ulFibreOverlapSetup;                      /* 熔接重叠量设定 */
    ULONG ulDischarge1Strength;                     /* 放电1强度 */
    ULONG ulDischarge1Time;                         /* 放电1时间 */
    ULONG ulDischarge2Strength;                     /* 放电2强度 */
    ULONG ulDischarge2LastTime;                     /* 放电2持续时间 */
    ULONG ulDischarge2StartTime;                    /* 放电2接通时间 */
    ULONG ulDischarge2StopTime;                     /* 放电2关闭时间 */
    ULONG ulExtraManualDischargeTime;               /* 手动补充放电定时 */
    BOOL bConeWelding;                              /* 锥形熔接 */
    ULONG ulConeWeldingWaitTime;                    /* 锥形熔接等待时间 */
    ULONG ulConeWeldingSpeed;                       /* 锥形熔接速度 bit */
    ULONG ulConeWeldingStretchLength;               /* 锥形熔接拉伸长度 um */
    FIBRE_LOSS_ESTIMATION_E eLossEstimationMode;    /* 算好估算方式 */
    FLOAT fLeftFibreMFD;                            /* 左纤模场直径(MFD)设定 */
    FLOAT fRightFibreMFD;                           /* 右纤模场直径(MFD)设定 */
    FLOAT fLeastLoss;                               /* 最小损耗 dB */
    FLOAT fRateOfSyntropyBending;                   /* 纤芯同向弯曲系数 */
    FLOAT fRateOfReverseBending;                    /* 纤芯反向弯曲系数 */
    FLOAT fRateOfMFDDeviation;                      /* 模场直径(MFD)失配系数 */

/* 熔接操作选项 */
    /* 操作选项 */
    BOOL bAutoStart;                                /* 自动开始:打开或关闭 */
    BOOL bStop1;                                    /* 停止1 */
    BOOL bStop2;                                    /* 停止1 */

    /* 数据显示 */
    BOOL bCutAngle;                                 /* 切割角度显示 */
    BOOL bOffsetData;                               /* 偏移数据显示 */

    /* 忽略选项 */
    BOOL bCut;                                      /* 切割 */
    BOOL bLoss;                                     /* 损耗 */
    BOOL bFibreCoreAngle;                           /* 纤芯角 */
    BOOL bBubble;                                   /* 气泡 */
    BOOL bThick;                                    /* 粗 */
    BOOL bThin;                                     /* 细 */

    /* 放电补偿 */
    BOOL bAirPressure;                              /* 气压 */
    BOOL bTemperature;                              /* 温度 */

    /* 光纤图像显示 */
    COORDINATE_E eGap;                              /* 间隙设定 */
    COORDINATE_E eStop1;                            /* 暂停1 */
    COORDINATE_E eAlign;                            /* 对准 */
    COORDINATE_E eStop2;                            /* 暂停2 */
    COORDINATE_E eDischarge;                        /* 放电 */
    COORDINATE_E eLossEstimation;                   /* 损耗估算 */

    /* 其他 */
    BOOL bFibreAutoFeed;                            /* 光纤自动推进 */
    BOOL bBadCutSurface;                            /* 切割端面不良 */
    BOOL bAutoAlignAfterStop;                       /* 暂停后自动对齐 */
    ULONG bManualDischargeTimes;                    /* 手动补充放电次数限定 */

}WELD_PARA_S;

//camera window info
typedef struct tagCameraPara
{
    bool is_pos_x;
    int row;
    int column;
}CAMERA_PARA_S;

typedef struct tagMotorPara
{
    int motorId;
    bool direction;
}MotorPara_S;

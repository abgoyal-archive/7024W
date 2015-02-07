#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov5648mipiraw.h"
#include "camera_info_ov5648mipiraw.h"
#include "camera_custom_AEPlinetable.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        69333,    // i4R_AVG
        13497,    // i4R_STD
        79267,    // i4B_AVG
        20579,    // i4B_STD
        {  // i4P00[9]
            4836667, -1290000, -990000, -920000, 3873333, -393333, -46667, -1613333, 4220000
        },
        {  // i4P10[9]
            1388580, -1026039, -358398, -299465, 138017, 161448, -116584, 528610, -412025
        },
        {  // i4P01[9]
            1180928, -814905, -356933, -360887, 184352, 176535, -107721, -113539, 221259
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1136,    // u4MinGain, 1024 base = 1x
            16384,    // u4MaxGain, 16x
            51,    // u4MiniISOGain, ISOxx  
            64,    // u4GainStepUnit, 1x/8 
            35,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            35,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            35,    // u4CapExpUnit 
            15,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            28,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            FALSE,    // bEnableCaptureThres
            FALSE,    // bEnableVideoThres
            FALSE,    // bEnableStrobeThres
            47,    // u4AETarget
            47,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -10,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            64,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            10,//18,    // u4FlatnessThres    // 10 base for flatness condition.
            75    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                762,    // i4R
                512,    // i4G
                619    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                10,    // i4X
                -380    // i4Y
            },
            // Horizon
            {
                -408,    // i4X
                -336    // i4Y
            },
            // A
            {
                -304,    // i4X
                -337    // i4Y
            },
            // TL84
            {
                -191,    // i4X
                -327    // i4Y
            },
            // CWF
            {
                -125,    // i4X
                -408    // i4Y
            },
            // DNP
            {
                -11,    // i4X
                -248    // i4Y
            },
            // D65
            {
                77,    // i4X
                -217    // i4Y
            },
            // DF
            {
                18,    // i4X
                -317    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                -96,    // i4X
                -368    // i4Y
            },
            // Horizon
            {
                -485,    // i4X
                -210    // i4Y
            },
            // A
            {
                -386,    // i4X
                -240    // i4Y
            },
            // TL84
            {
                -274,    // i4X
                -261    // i4Y
            },
            // CWF
            {
                -233,    // i4X
                -357    // i4Y
            },
            // DNP
            {
                -79,    // i4X
                -235    // i4Y
            },
            // D65
            {
                14,    // i4X
                -230    // i4Y
            },
            // DF
            {
                -71,    // i4X
                -310    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                869,    // i4R
                512,    // i4G
                845    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                565,    // i4G
                1546    // i4B
            },
            // A 
            {
                535,    // i4R
                512,    // i4G
                1221    // i4B
            },
            // TL84 
            {
                616,    // i4R
                512,    // i4G
                1032    // i4B
            },
            // CWF 
            {
                751,    // i4R
                512,    // i4G
                1054    // i4B
            },
            // DNP 
            {
                706,    // i4R
                512,    // i4G
                728    // i4B
            },
            // D65 
            {
                762,    // i4R
                512,    // i4G
                619    // i4B
            },
            // DF 
            {
                805,    // i4R
                512,    // i4G
                768    // i4B
            }
        },
        // Rotation matrix parameter
        {
            16,    // i4RotationAngle
            246,    // i4Cos
            71    // i4Sin
        },
        // Daylight locus parameter
        {
            -232,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -324,    // i4RightBound
            -974,    // i4LeftBound
            -175,    // i4UpperBound
            -275    // i4LowerBound
            },
            // Warm fluorescent
            {
            -324,    // i4RightBound
            -974,    // i4LeftBound
            -275,    // i4UpperBound
            -395    // i4LowerBound
            },
            // Fluorescent
            {
            -129,    // i4RightBound
            -324,    // i4LeftBound
            -162,    // i4UpperBound
            -309    // i4LowerBound
            },
            // CWF
            {
            -129,    // i4RightBound
            -324,    // i4LeftBound
            -309,    // i4UpperBound
            -407    // i4LowerBound
            },
            // Daylight
            {
            39,    // i4RightBound
            -129,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Shade
            {
            399,    // i4RightBound
            39,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            50,    // i4RightBound
            -129,    // i4LeftBound
            -310,    // i4UpperBound
            -415    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            399,    // i4RightBound
            -974,    // i4LeftBound
            0,    // i4UpperBound
            -415    // i4LowerBound
            },
            // Daylight
            {
            64,    // i4RightBound
            -129,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Cloudy daylight
            {
            164,    // i4RightBound
            -11,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Shade
            {
            264,    // i4RightBound
            -11,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Twilight
            {
            -129,    // i4RightBound
            -289,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Fluorescent
            {
            64,    // i4RightBound
            -374,    // i4LeftBound
            -180,    // i4UpperBound
            -407    // i4LowerBound
            },
            // Warm fluorescent
            {
            -286,    // i4RightBound
            -486,    // i4LeftBound
            -180,    // i4UpperBound
            -407    // i4LowerBound
            },
            // Incandescent
            {
            -286,    // i4RightBound
            -486,    // i4LeftBound
            -150,    // i4UpperBound
            -310    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            730,    // i4R
            512,    // i4G
            669    // i4B
            },
            // Cloudy daylight
            {
            808,    // i4R
            512,    // i4G
            557    // i4B
            },
            // Shade
            {
            846,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Twilight
            {
            620,    // i4R
            512,    // i4G
            899    // i4B
            },
            // Fluorescent
            {
            725,    // i4R
            512,    // i4G
            871    // i4B
            },
            // Warm fluorescent
            {
            586,    // i4R
            512,    // i4G
            1283    // i4B
            },
            // Incandescent
            {
            527,    // i4R
            512,    // i4G
            1210    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            6267    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5799    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1355    // i4OffsetThr
            },
            // Daylight WB gain
            {
            700,    // i4R
            512,    // i4G
            723    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -499,    // i4RotatedXCoordinate[0]
                -400,    // i4RotatedXCoordinate[1]
                -288,    // i4RotatedXCoordinate[2]
                -93,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature



#pragma once

//{{NO_DEPENDENCIES}}
// Microsoft Developer Studio generated include file.
// Used by Gayka.rc
//

#define MAX_PATH       260
#define MB_ICONERROR   0x00000010L
#define MB_ICONWARNING 0x00000030L

#define CM_GAYKA5915 1
#define CM_BOLT7787  2

#define CM_CFG          900
#define CM_HELP         901
#define CM_INSERT       902
#define CM_SET_FAVORITE 903
#define CM_DEL_FAVORITE 904

#define ID_DB_NOT_FOUND_ERROR 350
#define ID_DB_ERROR           351

// BaseMaсroParam.drawType команды видов
#define ID_VIEW       101 // вид основной        // ID_VID
#define ID_SEC        102 // вид основной разрез // ID_SEC
#define ID_TOP        103 // вид сверху
#define ID_TOP_SEC    104 // вид сверху разрез
#define ID_LEFT       105 // вид слева
#define ID_LEFT_SEC   106 // вид слева разрез
#define ID_RIGHT      107 // вид справа
#define ID_RIGHT_SEC  108 // вид справа разрез
#define ID_BOTTOM     109 // вид снизу
#define ID_BOTTOM_SEC 110 // вид снизу разрез
#define ID_SIDE       111 // вид спереди
#define ID_SIDE_SEC   112 // вид спереди разрез
#define ID_BACK       113 // вид сзади
#define ID_BACK_SEC   114 // вид сзади разрез

#define ID_TOPVID  ID_TOP
#define ID_SIDEVID ID_LEFT
#define ID_VIDSEC  ID_SEC

#define ID_SIMPLE 200 // Упрощенно
#define ID_AXIS   201 // Рисовать ось

#define ID_LEADER_CONNECT  300
#define ID_LEADER_CREATE   301
#define ID_ERROR_LEADER    302
#define ID_INDICATE_LEADER 303
#define ID_ERROR_2DDOC     304

#define IDP_LEADER_BUTTONS 1002 // "Создать новую"
#define IDP_POS_LEADER     1003 // "Ввод линии выноски"

#define IDP_ELEMENT_PARAM 4000
#define IDP_GAYKA5915     4001
#define IDP_BOLT7787      4002

#define IDP_HATCH_PARAM    5000
#define IDP_HATCHANG_ANGLE 5001
#define IDP_HATCHANG_STEP  5002
#define IDP_DIAM           5003
#define IDP_REFLECTION     5004
#define IDP_ISP            5005
#define IDP_FLAGS          5006
#define IDP_SIMPLE         5007
#define IDP_SPC_OBJ        5008

// Шрифтовые иконки
#define IDF_VIEW        0xe00b
#define IDF_LEFT        0xe00c
#define IDF_TOP         0xe00d
#define IDF_SEC         0xe00e
#define IDF_G_I1        0xe00f
#define IDF_G_I2        0xe010
#define IDF_SIMPLE      0xe011
#define IDF_AXIS        0xe012
#define IDF_G_STEP      0xe013
#define IDF_KEY         0xe014
#define IDF_BOLT_VIEW   0xe015
#define IDF_BOLT_TOP    0xe016
#define IDF_BOLT_BOTTOM 0xe017


//#define IDP_BMP_AXIS                    ID_AXIS
#define IDP_ICO_AXIS ID_AXIS
//#define IDP_BMP_KEY                     6002
//#define IDP_BMP_G_VIEW                  6003
//#define IDP_BMP_G_LEFT                  6004
//#define IDP_BMP_G_TOP                   6005
//#define IDP_BMP_G_SEC                   6006
//#define IDP_BMP_G_I1                    6007
//#define IDP_BMP_G_I2                    6008
//#define IDP_BMP_G_SIMPLE                6009
//#define IDP_BMP_G_STEP                  6010
#define IDP_ICO_KEY      6002
#define IDP_ICO_G_VIEW   6003
#define IDP_ICO_G_LEFT   6004
#define IDP_ICO_G_TOP    6005
#define IDP_ICO_G_SEC    6006
#define IDP_ICO_G_I1     6007
#define IDP_ICO_G_I2     6008
#define IDP_ICO_G_SIMPLE 6009
#define IDP_ICO_G_STEP   6010

#define IDP_HOTPNT1_BMP 6011
#define IDP_HOTPNT2_BMP 6012
#define IDP_HOTPNT3_BMP 6013
#define IDP_HOTPNT4_BMP 6014

#define IDP_HOTPNT1C_BMP 6016
#define IDP_HOTPNT2C_BMP 6017
#define IDP_HOTPNT3C_BMP 6018
#define IDP_HOTPNT4C_BMP 6019

#define IDP_HOTPNT1S_BMP 6021
#define IDP_HOTPNT2S_BMP 6022
#define IDP_HOTPNT3S_BMP 6023
#define IDP_HOTPNT4S_BMP 6024

#define IDP_C_VCHATCH 7000
#define IDS_C_VCHATCH 7001 // "Параметры штриховки";

#define IDS_PARAM_CAPTION 8000 // "Параметры"
#define IDS_VALUE_CAPTION 8001 // "Значения"
#define IDS_PITCH         8002 // "Шаг резьбы"
//#define IDS_DIM_KEY                     IDP_BMP_KEY // "Размер под ключ"
#define IDS_DIM_KEY     IDP_ICO_KEY // "Размер под ключ"
#define IDS_HEAD_H      8003        // "Высота головки"
#define IDS_MASSA_1000  8004        // "Масса 1000 шт"
#define IDS_CUPNUT_STR1 8005        // "D Диметр колпачка, мм"
#define IDS_CUTGAY_STR9 8006        // "e Диаметр описанной окружности, мм"
#define IDS_CUPNUT_STR2 8007        // "h1 Высота под ключ, мм"
#define IDS_CUPNUT_STR3 8008        // "l Глубина отверстия, мм"
#define IDS_CUPNUT_STR4 8009        // "l1 Длина резьбы, мм"
#define IDS_DIM_KEY_2   8010        // "Дополнительный размер под ключ"

#define MENU_G5915_1 8023
#define MENU_G5915_2 8024
#define MENU_G5915_3 8025

#define IDC_0_MENU_G5915 8030
#define IDC_1_MENU_G5915 8031
#define IDC_2_MENU_G5915 8032
#define IDC_3_MENU_G5915 8033
#define IDC_4_MENU_G5915 8034
#define IDC_5_MENU_G5915 8035
#define IDC_6_MENU_G5915 8036
#define IDC_7_MENU_G5915 8037
#define IDC_8_MENU_G5915 8038
#define IDC_9_MENU_G5915 8039

#define ID_TREE_MENU          8026
#define ID_TREE_FAVORITE_MENU 8027

#define DATABASE_TOOLBAR      9000
#define FAVORITE_TOOLBAR      9001
#define GAYKA5915_BAR         9002
#define CONTEXT_BAR_COUNT     3
#define END_OF_RESOURCE_TABLE 0xffff


#define ID_FIRST_HT 2000

#define IDB_ID_VIEW    1101 // ID_VIEW
#define IDB_ID_VIDSEC  1102 // ID_VIDSEC
#define IDB_ID_TOPVID  1103 // ID_TOP
#define IDB_ID_SIDEVID 1105 // ID_LEFT
#define IDB_ID_SIMPLE  1200 // ID_SIMPLE   // Упрощенный вид
#define IDB_ID_AXIS    1201 // ID_AXIS     // Рисовать ось

//#define MENU_POS_LEADER                 10009
//#define GAIKA_POPUPMENU                 10010
#define CURSOR_ROTATE   10011
#define CURSOR_DIAMETER 10012
#define CURSOR_LENGHT   10013

#define ID_LIBID 11111

// Reserve /*Резерв*/
#define IDS_COMMAND_NUMBER 12000 // Номер команды = %d
#define IDS_REFLECTION_SEP 12001 // Отображение
#define IDS_GOST           12002 // ГОСТ
#define IDS_SLIDEBOX       12007 // Окно просмотра

//Bolt
#define IDS_BOLT7787_CAPTION 15001
#define IDS_HEAD_DIAM        15002
#define IDS_HEAD_HEIGHT      15003
#define IDS_THREAD_LENGTH    15004

// Окно свойств свойства сортируются в порядке возростания идентификаторов
#define ID_PROP_BOLT_PARAM      15005
#define ID_PROP_DIAMETER        15006
#define ID_PROP_LENGTH          15007
#define ID_PROP_BOLT_VIEW_PARAM 15008
#define ID_PROP_BOLT_VIEW       15009
#define IDP_BOLT_VIEW           15010
#define IDP_BOLT_TOP            15011
#define IDP_BOLT_BOTTOM         15012

#define IDC_DIAMETER 15013
#define IDC_LENGTH   15016
#define IDC_AXIS     15020

// Окно свойств свойства сортируются в порядке возростания идентификаторов
#define ID_PROP_ELEMENT_PARAM 20000 // Параметры элемента
#define ID_PROP_DIAM          20001 // Диаметр резьбы
#define ID_PROP_ISPOLN        20002 // Исполнение
#define ID_PROP_VIEW_PARAM    20003 // Группа Вид
#define ID_PROP_VIEW          20004 // Вид
#define ID_PROP_SIMPLE        20005 // Упрощенное отображение
#define ID_PROP_AXIS          20006 // Рисовать ось
#define ID_PROP_HATCH_PARAM   20007 // Параметры штриховки
#define ID_PROP_HATCH_STEP    20008 // Шаг штриховки
#define ID_PROP_HATCH_ANGLE   20009 // Угол штриховки

#define ID_PROP_OTHER_PARAM     20100 // Дополнительные параметры
#define ID_PROP_MASSA_1000      20199 // Масса 100 шт
#define ID_PROP_OTHER_PARAM_END 20200 // Занять диапазон под дополнительные параметры

// ExternalBaseElement
#define IDS_XML_TREE_HAT        20300 // Дерево XML шапка
#define IDS_XML_TREE_NAIL       20301 // Дерево XML хвост
#define IDS_XML_TREE_FASTENER   20302 // Дерево XML "Крепеж"
#define IDS_XML_TREE_PARTFAMILY 20303 // Дерево XML PartFamily (отдельный элемент дерева)
#define IDS_FAVORITES           20304 // Избранное
#define IDS_FASTENER            20305 // Крепеж
#define IDS_FASTENER_ELMEMENT   20306 // Крепежный элемент
#define IDS_ARMATURE            20307 // Арматура
#define IDS_GAYKA5915           20308 // Гайка ГОСТ 5915-70
#define IDS_GAYKA5915_ID        20309 // Gayka GOST 5915-70
#define IDS_BOLT7787            20310 // Болт ГОСТ 7787-81
#define IDS_BOLT7787_ID         20311 // Bolt GOST 7787-81
#define IDS_LOCATION            20312 // Размещение


// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE 15000
#define _APS_NEXT_COMMAND_VALUE  35000
#define _APS_NEXT_CONTROL_VALUE  15000
#define _APS_NEXT_SYMED_VALUE    15000
#endif
#endif

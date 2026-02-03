////////////////////////////////////////////////////////////////////////////////
//
//  Boltmain.cpp
//  Файл описывает класс BoltMain - класс болта ГОСТ 7787-81, и наследуемые от
//  него классы Bolt7786 (болты ГОСТ 7786-81 и ГОСТ 17673-81), Bolt7802 (ГОСТ 7802-81),
//  Вolt7801 (ГОСТ 7781-81), Bolt7783 (ГОСТ 7783-81), Bolt7785 (ГОСТ 7785-81)
//
//  BoltMainStruct - Основная структура данных болта
//  BoltMain       - Болт ГОСТ 7787-81, Базовый класс для остальных болтов
//  Внеклассные функции
//
////////////////////////////////////////////////////////////////////////////////
#include "Gayka1_pch.h"

//
#include <memory>

#include "Doc2DDispatcher.h"
#include "Bolt.h"
#include "math.h"
#include "ParameterObject.h"
#include "DataBase.h"
#include <format>
#include "BaseProcess.h"
#include <KompasLibraryActions.h>
#include <variant>

extern ksapi::IApplication * application;
extern int GetDoubleColumnDataCallback(void * result, int count, char ** rowData, char **);

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
BoltMainStruct::BoltMainStruct(unsigned short _gost)
{
  gost = _gost; // ГОСТ
  Init();
}


//------------------------------------------------------------------------------
/**
  Конструктор копирования
*/
//---
BoltMainStruct::BoltMainStruct(const BoltMainStruct & boltSt)
{
  *this = boltSt;
}


//------------------------------------------------------------------------------
/**
  Оператор =.
*/
//---
const BoltMainStruct & BoltMainStruct::operator=(const BoltMainStruct & boltSt)
{
  l = boltSt.l;           // длина стержня
  dr = boltSt.dr;         // Диаметр резьбы
  b = boltSt.b;           // Длина резьбы
  m = boltSt.m;           // масса 1000 болтов
  d = boltSt.d;           // диаметр головки
  k = boltSt.k;           // высота головки
  kr = boltSt.kr;         // резьба на b
  param1 = boltSt.param1; // размер стороны подголовка или высота уса
  param2 = boltSt.param2; // радиус сферы или угол alpha
  v = boltSt.v;           // ширина уса или высота подголовка для ГОСТ 7802
  h = boltSt.h;           // высота подголовка ГОСТ 7783
  d1 = boltSt.d1;         // диаметр подголовка ГОСТ 7783
  p = boltSt.p;           // Шаг резьбы
  dp = boltSt.dp;         // Диаметр конца
  rp = boltSt.rp;         // радиус под головкой
  z4 = boltSt.z4;         // величина фаски
  f = boltSt.f;           // Битовые маски
  gost = boltSt.gost;     // ГОСТ
  perf = boltSt.perf;     // исполнение

  return *this;
}


int GetAllBoltDataCallback(void * result, int count, char ** rowData, char **)
{
  BoltMainStruct * resultData = reinterpret_cast<BoltMainStruct *>(result);

  if (resultData)
  {
    resultData->l = std::stof(rowData[1]);
    resultData->dr = std::stof(rowData[2]);
    resultData->b = std::stof(rowData[3]);
    resultData->m = std::stof(rowData[4]);
    resultData->z4 = std::stof(rowData[5]);
    resultData->dp = std::stof(rowData[6]);
  }

  return 0;
}


//------------------------------------------------------------------------------
/**
  Инициализация параметров болта
*/
//---
void BoltMainStruct::Init()
{
  l = 90;     // длина стержня
  dr = 10;    // Диаметр резьбы
  p = 1.5;    // Шаг резьбы
  kr = 1;     // резьба на b
  dp = 7;     // Диаметр конца
  z4 = 1.5;   // величина фаски
  f = 0;      // Битовые маски
  rp = 0.4f;  // радиус под головкой
  b = 26;     // Длина резьбы
  param1 = 0; //
  param2 = 0; //
  v = 0;      //
  h = 0;      //
  d1 = 0;     //
  b1 = 0;     //
  perf = 1;   // исполнение

  switch (gost)
  {
    case 7786:
    {
      m = 48.26f;  // масса 1000 болтов
      d = 18;      // диаметр головки
      k = 8;       // высота головки
      param1 = 10; // размер стороны подголовка
      break;
    }

    case 7783:
    {
      m = 53.80f;     // масса 1000 болтов
      d = 20;         // диаметр головки
      k = 7;          // высота головки
      param1 = 2.7f;  // высота уса
      param2 = 10.5f; // радиус сферы
      v = 3.2f;       // ширина уса
      h = 5;          // высота подголовка
      d1 = 10;        // диаметр подголовка или стержня
      break;
    }

    case 7785:
    {
      m = 49.45f;    // масса 1000 болтов
      d = 20;        // диаметр головки
      k = 5.5;       // высота головки
      param1 = 4, 2; // высота уса
      param2 = 90;   // угол alpha
      v = 3.2f;      // ширина уса
      break;
    }

    case 17673:
    {
      m = 55.98f;  // масса 1000 болтов
      d = 23;      // диаметр головки
      k = 11;      // высота головки
      param1 = 10; // размер стороны подголовка
      break;
    }

    case 7801:
    {
      m = 54.30f;  // масса 1000 болтов
      d = 23;      // диаметр головки
      k = 5;       // высота головки
      param1 = 5;  // высота уса
      param2 = 18; // радиус сферы
      break;
    }

    case 7802:
    {
      m = 55, 53;  // масса 1000 болтов
      d = 23;      // диаметр головки
      k = 5, 0;    // высота головки
      param1 = 10; // размер стороны подголовка
      param2 = 18; // радиус сферы
      v = 6;       // высота подголовка
      break;
    }

    default:
    {
      m = 50.04f; // масса 1000 болтов
      d = 17;     // диаметр головки
      k = 10.5;   // высота головки
      break;
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
//
// Внеклассные функции
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
  Мочка пересечения фаски и линии обозначения резьбы
*/
//---
void IntersectFaceThread(const BoltMainStruct & bolt, int & kp, double & xp, double & yp)
{
  float pThread = bolt.dr * 0.5f - MODSTEP_REAL * bolt.p; // координата у линии обозначения резьбы
  float endThr = bolt.l - bolt.b;                         // координата конца резьбы
  math::IntersectLinSLinS(bolt.l - bolt.z4, bolt.dr * 0.5, bolt.l, bolt.dp * 0.5, endThr, pThread, bolt.l, pThread, kp, xp, yp);
}


//------------------------------------------------------------------------------
/**
  Рисует половинку стержня для болтов ГОСТ 7786, 7802 и 7787.
*/
//---
void BoltBarDraw(Doc2DDispatcher & emulator, const BoltMainStruct & bolt, const int sign, bool apl = false)
{
  float radThr = sign * bolt.dr * 0.5f;                 // радиус резьбы
  float lengthBar = bolt.l - bolt.z4;                   // длина стержня без фаски
  float headRest = bolt.gost == 7802 ? bolt.v : bolt.k; // высота подголовка
  float radDp = sign * bolt.dp * 0.5f;                  // радиус резьбы без фаски

  emulator.CreateLineSegment(lengthBar, radThr, bolt.l, radDp, BASE_LINE);    // фаска
  emulator.CreateLineSegment(bolt.l, radDp, bolt.l, 0, BASE_LINE);            // конец болта
  emulator.CreateLineSegment(headRest, radThr, lengthBar, radThr, BASE_LINE); // стержень без фаски

  if (!apl)
  {
    emulator.CreateLineSegment(lengthBar, radThr, lengthBar, 0, BASE_LINE); // конец стержня без фаски

    float endThr = bolt.l - bolt.b; // координата конца резьбы
    if (bolt.kr)
      emulator.CreateLineSegment(endThr, radThr, endThr, 0, BASE_LINE); // граница резьбы

    float pThread = radThr - sign * MODSTEP_REAL * bolt.p; // координата у линии обозначения резьбы
    int kp = 0;
    double xp = 0.0, yp = 0.0;
    ::IntersectFaceThread(bolt, kp, xp, yp);

    if (kp)
      emulator.CreateLineSegment(endThr, pThread, xp, pThread, SLIM_LINE); // обозначение резьбы
  }
}


//------------------------------------------------------------------------------
/**
  Пересечение грани подголовка и фаски для болтов ГОСТ 7786, 7802.
*/
//---
void IntersectHeadRestFaset(const BoltMainStruct & bolt, int & kp1, double & xp1, double & yp1)
{
  float headRest = bolt.gost == 7802 ? bolt.v : bolt.k;           // высота подголовка
  float radThr = bolt.dr * 0.5f;                                  // радиус резьбы
  float sideHead = static_cast<float>(bolt.param1 * ::sqrt(0.5)); // сторона подголовка

  math::IntersectLinLin(headRest /*коор-та x*/, radThr /*коор-та у*/, 120 /*угол*/, // первая линия
                        0 /*коор-та x*/, sideHead /*коор-та у*/, 0 /*угол*/,        // вторая линия
                        kp1, xp1, yp1);                                             // возвращяемые значения
}


//------------------------------------------------------------------------------
/**
  Рисует границу стержня, фаску и обзначение резьбы для болтов ГОСТ 7801 и 7783.
*/
//---
void BoltBarThrDraw(Doc2DDispatcher & emulator, const BoltMainStruct & bolt)
{
  float lengthBar = bolt.l - bolt.z4; // длина стержня без фаски
  float radThr = bolt.dr * 0.5f;      // радиус резьбы
  float mRadThr = -radThr;

  emulator.CreateLineSegment(lengthBar, radThr, lengthBar, mRadThr, BASE_LINE); // конец стержня без фаски

  float endThr = bolt.l - bolt.b;                                         // координата конца резьбы
  emulator.CreateLineSegment(endThr, radThr, endThr, mRadThr, BASE_LINE); // граница резьбы

  float radDp = bolt.dp * 0.5f;                                            // радиус резьбы без фаски
  emulator.CreateLineSegment(lengthBar, radThr, bolt.l, radDp, BASE_LINE); // фаска вверху

  float mRadDp = -radDp;
  emulator.CreateLineSegment(bolt.l, radDp, bolt.l, mRadDp, BASE_LINE);      // конец болта
  emulator.CreateLineSegment(lengthBar, mRadThr, bolt.l, mRadDp, BASE_LINE); // фаска внизу

  float pThread = radThr - MODSTEP_REAL * bolt.p; // координата у линии обозначения резьбы
  int kp;
  double xp, yp;
  ::IntersectFaceThread(bolt, kp, xp, yp);

  if (kp)
  {
    emulator.CreateLineSegment(endThr, pThread, xp, pThread, SLIM_LINE);   // обозначение резьбы вверху
    emulator.CreateLineSegment(endThr, -pThread, xp, -pThread, SLIM_LINE); // обозначение резьбы внизу
  }
}


//------------------------------------------------------------------------------
/**
  Рисует оси в основном виде для болтов ГОСТ 7801 и 7783.
*/
//---
void BoltAxisDraw(Doc2DDispatcher & emulator, const BoltMainStruct & bolt)
{
  if (!(bolt.f & AXIS_OFF)) // оси
    emulator.CreateLineSegment(-bolt.k - AXIS_END, 0, (bolt.f & DRAW_ST_OF) ? AXIS_END : bolt.l + AXIS_END, 0, AXIS_LINE);
}


//------------------------------------------------------------------------------
/**
  Рисует стержень в виде снизу.
*/
//---
void BoltBarBottomDraw(Doc2DDispatcher & emulator, const BoltMainStruct & bolt)
{
  // тело стержня
  float radiusThread = bolt.dr * 0.5f;
  emulator.CreateCircle(0 /*x центра*/, 0 /*y центра*/, radiusThread /*радиус*/, BASE_LINE);

  // обозначение резьбы (тонкой линией)
  float stepThd = radiusThread - MODSTEP_REAL * bolt.p;
  emulator.CreateArcByPoint(0 /*x центра*/, 0 /*y центра*/, stepThd /*радиус*/, bolt.p /*x нач. точки*/, stepThd /*y нач. точки*/,
                            -stepThd /*x кон. точки*/, bolt.p /*y кон. точки*/, true /*направление*/, SLIM_LINE);
}


//------------------------------------------------------------------------------
/**
  Конструктор. Тело конструктора выполняется только для болта ГОСТ 7787-81
  для остальных болтов конструктор пуст
*/
//---
BoltMain::BoltMain(unsigned short gost)
  : ParameterObject(CM_BOLT7787)
  , m_bolt(gost)
{
  if (m_bolt.gost == 7787)
    InitBolt();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
BoltMain::BoltMain(ksapi::IMacroObjectPtr & macroObj)
  : ParameterObject(CM_BOLT7787, macroObj)
  , m_bolt(7787)
{
  if (!macro::GetMacroParam(*this, macroObj))
  {
    InitBolt();
  }
  else
  {
    if (!m_baseParam.version)
      m_baseParam.version = 1;
  }
}


//------------------------------------------------------------------------------
/**
  Конструктор копирования
*/
//---
BoltMain::BoltMain(const BoltMain & other)
  : ParameterObject(CM_BOLT7787)
{
  Assign(other);
}

//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
BoltMain::~BoltMain()
{
}


//------------------------------------------------------------------------------
// Инициализация параметров болта
// ---
void BoltMain::InitBolt()
{
  m_baseParam.drawType = ID_VIEW; // Тип отображения
  angle = 0;                      // Угол
}


//-------------------------------------------------------------------------------
// Копирование
// ---
void BoltMain::Assign(const BoltMain & other)
{
  memcpy(&m_baseParam, &other.m_baseParam, sizeof(m_baseParam)); // Параметры макрообъекта
  memcpy(&m_bolt, &other.m_bolt, sizeof(m_bolt));                // Параметры гайки ГОСТ 5915
  m_flagMode = other.m_flagMode;
}


//------------------------------------------------------------------------------
// создание контура для очистки области под элементом
// ---
void BoltMain::CreateClearGroup()
{
  float radiusHead = m_bolt.d * 0.5f;
  dispatcher.CreateCircle(0 /*x центра*/, 0 /*у центра*/, radiusHead + ADD_DIM /*радиус*/, BASE_LINE); //Головка
}


//------------------------------------------------------------------------------
// чтение БД для составного элемента AssmElement
// ---
bool BoltMain::ReadElemBase(float d, float l, bool forAssmElem)
{
  return FillStruct(forAssmElem, d, l);
}


//-------------------------------------------------------------------------------
//
// ---
void BoltMain::SetDrawType(short drawType)
{
  m_baseParam.drawType = static_cast<unsigned short>(drawType == ID_BOTTOM ? ID_BOTTOM : drawType);
}


//------------------------------------------------------------------------------
/**
  рисовать ось
*/
//---
void BoltMain::SetAxis(bool v)
{
  if (v)
    m_bolt.f &= static_cast<short>(~AXIS_OFF); // Снять флаг
  else
    m_bolt.f |= AXIS_OFF; // Установить флаг
}


//------------------------------------------------------------------------------
/**
  Номер атрибута
*/
//---
ksapi::NumericId BoltMain::AttrNumber()
{
  return 313277777065;
}


//------------------------------------------------------------------------------
/**
  Заполнение объекта СП
*/
//---
bool BoltMain::FillSpcObj(ksapi::ISpecificationBaseObjectPtr & obj1)
{
  std::int32_t uBuf;
  ksapi::ISpecificationColumnsPtr columns = obj1->GetColumns();
  ksapi::ISpecificationColumnPtr column = columns->GetColumn(ksSColumnName, 1, 0);
  ksapi::ISpecificationColumnItemsPtr columnItems = column->GetColumnItems();
  ksapi::ISpecificationColumnItemPtr item;

  if (columnItems)
  {
    item = columnItems->GetItem(1);
    if (item)
      item->SetVisible(false); // Выключим исполнение

    item = columnItems->GetItem(3); // Изменим диаметр
    if (item)
      item->SetValue(static_cast<double>(m_bolt.dr));

    item = columnItems->GetItem(4);
    if (item)
      item->SetVisible(false);

    item = columnItems->GetItem(5);
    if (item)
      item->SetVisible(false); // выключим шаг

    item = columnItems->GetItem(6);
    if (item)
      item->SetVisible(false); // выключим поле допуска

    uBuf = static_cast<std::int32_t>(m_bolt.l);

    item = columnItems->GetItem(6);
    if (item)
      item->SetValue(uBuf); //изменим длину

    item = columnItems->GetItem(9);
    if (item)
      item->SetVisible(false); // выключим класс прочности

    item = columnItems->GetItem(10);
    if (item)
      item->SetVisible(false); // выключим материал

    item = columnItems->GetItem(11);
    if (item)
      item->SetVisible(false); // выключим покрытие

    item = columnItems->GetItem(12);
    if (item)
      item->SetValue(::LoadStr(IDS_GOST));

    item = columnItems->GetItem(13);
    if (item)
      item->SetValue(L"7787");

    item = columnItems->GetItem(15);
    if (item)
      item->SetValue(L"81");

    columns = obj1->GetAdditionalColumns();

    if (columns)
    {
      column = columns->GetColumn(ksSColumnMass, 1, 0);
      if (column)
      {
        columnItems = column->GetColumnItems();
        if (columnItems)
          columnItems->GetItem(0)->SetValue(m_bolt.m);
      }
    }
  }


  return true;
}


//------------------------------------------------------------------------------
/**
  Получить парметры для элемента XML дерева
*/
//---
void BoltMain::GetXMLPartFamilyParam(std::wstring & name, std::wstring & id)
{
  name = ::LoadStr(IDS_BOLT7787);
  id = ::LoadStr(IDS_BOLT7787_ID);
}


//------------------------------------------------------------------------------
/**
  Создаёт группу объектов
*/
//---
void BoltMain::GetGroup()
{
  switch (m_baseParam.drawType)
  {
    case ID_BOTTOM:
      BottomDraw();
      break; // вид снизу
    case ID_TOP:
      TopDraw();
      break; // вид ссверху
    default:
      ViewDraw();
      break; // вид
  }
}


//------------------------------------------------------------------------------
/**
  Рисование оконтовки головки для болтов
*/
//---
void HalfSideHead(Doc2DDispatcher & emulator, float x, float y, bool apl = false)
{
  emulator.CreateLineSegment(0, 0, 0, y, BASE_LINE);
  emulator.CreateLineSegment(0, y, x, y, BASE_LINE);

  if (!apl)
    emulator.CreateLineSegment(x, y, x, 0, BASE_LINE);
}


//------------------------------------------------------------------------------
/**
  Рисование половины основного вида болта:
  downHalf = false - верхняя часть, true - нижняя
*/
//---
void BoltMain::ViewHalfDraw(bool downHalf, bool apl)
{
  int sign = downHalf ? -1 : 1;

  float radThr = m_bolt.dr * 0.5f; // радиус резьбы
  float radHead = m_bolt.d * 0.5f; // радиус головки

  int kp;                                                                           // кол-во точек пересечения
  double xp, yp;                                                                    // координаты точки пересечения
  math::IntersectLinLin(m_bolt.k /*коор-та x*/, radThr /*коор-та у*/, 160 /*угол*/, // первая линия
                        0 /*коор-та x*/, radHead /*коор-та у*/, 0 /*угол*/,         // вторая линия
                        kp, xp, yp);                                                // возвращяемые значения

  if (kp)
  {
    radHead *= sign;
    radThr *= sign;

    dispatcher.CreateLineSegment(xp, radHead, m_bolt.k, radThr, BASE_LINE); // подголовник
    if (!apl)
      dispatcher.CreateLineSegment(m_bolt.k, radThr, m_bolt.k, 0, BASE_LINE); // граница подголовка

    ::HalfSideHead(dispatcher, static_cast<float>(xp), radHead, apl); // оконтовка головки
    ::BoltBarDraw(dispatcher, m_bolt, sign, apl);                     // стержень
  }
}


//------------------------------------------------------------------------------
/**
  Рисование вида сверху
*/
//---
void BoltMain::TopDraw()
{
  float radiusHead = m_bolt.d * 0.5f;

  // Отрисовка апликативной заливки
  dispatcher.CreateColoring(WORKWINDOWCOLOR);
  dispatcher.CreateCircle(0 /*x центра*/, 0 /*у центра*/, radiusHead /*радиус*/, BASE_LINE); // Головка
  dispatcher.CloseFilling();

  // Отрисовка основной графики
  dispatcher.CreateCircle(0 /*x центра*/, 0 /*у центра*/, radiusHead /*радиус*/, BASE_LINE); // Головка

  if (!(m_bolt.f & AXIS_OFF)) // Оси
  {
    radiusHead += AXIS_END;
    dispatcher.CreateLineSegment(radiusHead /*х нач. точки*/, 0 /*у нач. точки*/, -radiusHead /*х кон. точки*/,
                                 0 /*у кон. точки*/, AXIS_LINE);
    dispatcher.CreateLineSegment(0, radiusHead, 0, -radiusHead, AXIS_LINE);
  }
}


//------------------------------------------------------------------------------
/**
  Рисование основного вида болта
*/
//---
void BoltMain::ViewDraw()
{
  // Отрисовка апликативной заливки
  dispatcher.CreateColoring(WORKWINDOWCOLOR);
  ViewHalfDraw(false /*downHalf*/, true /*apl*/); // верхняя часть
  ViewHalfDraw(true /*downHalf*/, true /*apl*/);  // Нижняя часть
  dispatcher.CloseFilling();
  // Отрисовка основной графики
  ViewHalfDraw(false /*downHalf*/); // верхняя часть
  ViewHalfDraw(true /*downHalf*/);  // Нижняя часть

  if (!(m_bolt.f & AXIS_OFF)) // оси
    dispatcher.CreateLineSegment(-AXIS_END, 0, m_bolt.l + AXIS_END, 0, AXIS_LINE);
}


//------------------------------------------------------------------------------
/**
  Рисование вида снизу
*/
//---
void BoltMain::BottomDraw()
{
  TopDraw();                               // Головка
  ::BoltBarBottomDraw(dispatcher, m_bolt); // стержень
}


//------------------------------------------------------------------------------
/**
  Перезаполняет структуру данными из базы в соответствии со значением длины и диаметра
*/
//---
bool BoltMain::FillStruct(bool forAssmElem, float d, float l)
{
  bool failDB = true; // предполагаем ошибку

  // Считываем из файла строку с форматом запроса к БД для выборки по полям
  // длина (L) и диаметр резьбы (dr)
  if (DataBase * db = DataBase::GetDBConnection())
  {
    BoltMainStruct tmpBolt(m_bolt);

    // в формируем строку запрса с текущими значениями длины и диам.резьбы
    std::string expression = forAssmElem ? std::format("BOLT_7787.L>={:.2f} AND BOLT_7787.dr={:.2f}", l, d)
                                         : std::format("BOLT_7787.L={:.2f} AND BOLT_7787.dr={:.2f}", m_bolt.l, m_bolt.dr);

    std::string query("SELECT BOLT_7787.*, R_SH_K.z4, R_SH_K.dp FROM ");
    query.append(std::format("BOLT_7787 INNER JOIN R_SH_K on BOLT_7787.dr = R_SH_K.dr WHERE {} LIMIT 1", expression));

    if (db->ExecuteQuery(query, GetAllBoltDataCallback, &tmpBolt))
    {
      m_bolt = tmpBolt;

      // переписываем поле b определяя истинную длину резьбы, т.к. она зависит от
      // длины болта и от значения поля kr
      switch (m_bolt.gost)
      {
        case 7786:
        case 17673:
        case 7785:
          m_bolt.b = m_bolt.kr == 1 ? (m_bolt.l > 120) ? m_bolt.gost == 7785 ? tmpBolt.h : tmpBolt.param2 : tmpBolt.b
                                    : m_bolt.l - m_bolt.k;
          break; // длина резьбы

        case 7801:
        {
          m_bolt.b = m_bolt.kr == 1 ? (m_bolt.l > 120) ? tmpBolt.h : tmpBolt.b : m_bolt.l - m_bolt.param1; // длина резьбы

          m_bolt.rp = m_bolt.v; // радиус под головкой
          m_bolt.v = 0;
          break;
        }

        case 7802:
        {
          m_bolt.b = m_bolt.kr == 1 ? (m_bolt.l > 120) ? tmpBolt.d1 : tmpBolt.b : m_bolt.l - m_bolt.v; // длина резьбы

          m_bolt.rp = m_bolt.h; // радиус под головкой
          m_bolt.h = 0;
          break;
        }

        case 7783:
          m_bolt.b = m_bolt.kr == 1 ? (m_bolt.l > 120) ? tmpBolt.b1 : tmpBolt.b : m_bolt.l - m_bolt.h;
          break; // длина резьбы
      }

      failDB = false; // все получилось без ошибки
    }
  }

  return !failDB;
}


extern void * GetModule();


//------------------------------------------------------------------------------
/**
  Параметры для Hot точек
*/
//---
bool BoltMain::GetHotPoint(ksapi::IObject2DHotPoint & point, int index)
{
  bool res = false;
  bool stDraw = m_baseParam.drawType != ID_BOTTOM && m_baseParam.drawType != ID_TOP; // true - стержень рисуется
  ResourceModule module = GetModule();

  switch (index)
  {
    case 0: // точка привязки
    {
      point.SetPoint(0, 0);
      point.SetText(L"0");
      point.SetStandartIcon(ksHotPointEnum::ksHPRing);
      res = true;
      break;
    }
    case 1: // Угол поворота
    {
      point.SetPoint(stDraw ? m_bolt.l / 3 : m_bolt.d * 0.5, 0);
      point.SetCursor(CURSOR_ROTATE, module);
      point.SetText(L"A");
      res = true;
      break;
    }
    case 2: // Изменение диаметра
    {
      point.SetPoint(stDraw ? m_bolt.l : 0, m_bolt.dr * 0.5);
      point.SetCursor(CURSOR_DIAMETER, module);
      point.SetText(L"Dr");
      res = true;
      break;
    }
    case 3: // Длина стержня болта
    {
      if (stDraw)
      {
        point.SetPoint(m_bolt.l, 0);
        point.SetCursor(CURSOR_LENGHT, module);
        point.SetText(L"L");
        res = true;
      }
      break;
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Текст для Hot точек
*/
//---
std::wstring BoltMain::GetCursorTextEx(int index)
{
  std::wstring name;

  switch (index)
  {
    case 3:
    {
      name = std::format(L"L = {:.2f}", m_bolt.l);
      break;
    }
    case 2:
    {
      name = std::format(L"Dr = М{:.2f}", m_bolt.dr);
      break;
    }
    case 1:
    {
      name = std::format(L"A = {:.2f}", fmod(angle, 360.0));
      break;
    }
    case 0:
    {
      name = std::format(L"X = {:.2f}\nY = {:.2f}", x, y);
      break;
    }
  }

  return name;
};


//------------------------------------------------------------------------------
/**
  Параметры для Hot точек
*/
//---
bool BoltMain::ChangeHotPointParam(double d, double l, int index)
{
  bool retVal = false;

  if (index == 3)
  {
    if (l > 0 && ::fabs(l - m_bolt.l) > 0.01)
      retVal = ReadElemBase(m_bolt.dr, static_cast<float>(l), true /*forAssmElem*/);
  }
  else
  {
    std::vector<double> diameters = GetDiamList();

    if (!diameters.empty())
    {
      // подобрать ближайший dR по d
      double dR = 0.0;
      double oldDR = m_bolt.dr;

      for (double diameter : diameters)
      {
        if (dR <= d)
          dR = diameter;
        else
          break;
      }

      if (::fabs(dR - oldDR) > 0.01)
        retVal = ReadElemBase(static_cast<float>(dR), m_bolt.l, true /*forAssmElem*/);
    }
  }

  return retVal;
}


//------------------------------------------------------------------------------
/**
  Изменение положения Hot точек
*/
//---
bool BoltMain::SetHotPoint(double newX, double newY, int index)
{
  double x;
  double y;
  double alpha;
  bool mirrorSymetry;

  if (macro::GetMacroPlacement(m_macroObject, x, y, alpha, mirrorSymetry))
  {
    switch (index)
    {
      case 0: // Хотточка перемещения
      case 1:
      {
        ParameterObject::SetHotPoint(newX, newY, index);
        break;
      }
      case 2:
      case 3:
      {
        if (ChangeHotPointParam(::fabs(newY * 2), newX, index))
        {
          RedrawMacro();
        }
        break;
      }
    }
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Выдать идентификатор свойства по индексу
*/
//---
int BoltMain::GetPropertyID(int index)
{
  int res = 0;
  switch (index)
  {
    case 0:
      res = ID_PROP_BOLT_PARAM;
      break; // Параметры элемента
    case 1:
      res = ID_PROP_DIAMETER;
      break; // Диаметр
    case 2:
      res = ID_PROP_LENGTH;
      break; // Длина
    case 3:
      res = ID_PROP_BOLT_VIEW_PARAM;
      break; // Параметры отображения
    case 4:
      res = ID_PROP_BOLT_VIEW;
      break; // Вид
    case 5:
      res = ID_PROP_AXIS;
      break; // Рисовать ось
    case 6:
      res = ID_PROP_OTHER_PARAM;
      break; // Справочные данные
    // Для примера свойств не описанных в ресурсах ----------------------
    case 7:
      res = ID_PROP_OTHER_PARAM + 4;
      break; // Диаметр головки
    case 8:
      res = ID_PROP_OTHER_PARAM + 5;
      break; // Высота головки
    case 9:
      res = ID_PROP_OTHER_PARAM + 6;
      break; // Длина резьбы
    // ------------------------------------------------------------------
    case 10:
      res = ID_PROP_MASSA_1000;
      break; // Масса 1000 шт
  }
  return res;
}

extern std::wstring GetPropertyName(int id);
extern void * GetModule();


//------------------------------------------------------------------------------
/**
  Добавить свойство в список свойств
*/
//---
bool BoltMain::GetProperty(int32_t index, IPropertyParam & propertyParam)
{
  propertyParam.SetId(GetPropertyID(index));

  if (propertyParam.GetId())
  {
    propertyParam.SetResModule(GetModule());

    switch (propertyParam.GetId())
    {
      case ID_PROP_BOLT_PARAM:      // Параметры элемента
      case ID_PROP_BOLT_VIEW_PARAM: // Параметры отображения
      case ID_PROP_OTHER_PARAM:     // Справочные данные
      {
        propertyParam.SetType(ksOPControlGroup); // Группа
        propertyParam.SetEnable(true);
        break;
      }
      case ID_PROP_DIAMETER: // Диаметр
      {
        propertyParam.SetType(ksOPControlListDouble); // Вещественный комбобокс
        propertyParam.SetValue(0.0);
        propertyParam.SetEnable(true);
        propertyParam.SetReadOnly(true);
        break;
      }
      case ID_PROP_LENGTH: // Длина
      {
        propertyParam.SetType(ksOPControlListDouble); // Вещественный комбобокс
        propertyParam.SetValue(0.0);
        propertyParam.SetEnable(true);
        propertyParam.SetReadOnly(true);
        break;
      }
      case ID_PROP_BOLT_VIEW: // Вид
      {
        propertyParam.SetType(ksOPControlFontImageList); // Список с иконками
        propertyParam.SetAdditionText(GetPropertyName(ID_PROP_BOLT_VIEW));
        propertyParam.SetValue(0);
        propertyParam.SetEnable(true);
        break;
      }
      case ID_PROP_AXIS: // Ось
      {
        propertyParam.SetType(ksOPControlListCheck); // Логическое значение Да \ Нет
        propertyParam.SetValue(false);
        propertyParam.SetEnable(true);
        break;
      }
    }

    if (propertyParam.GetId() > ID_PROP_OTHER_PARAM && propertyParam.GetId() < ID_PROP_OTHER_PARAM_END)
    {
      propertyParam.SetType(ksOPControlEditDouble); // Вещественный комбобокс
      propertyParam.SetValue(0.0);
      propertyParam.SetEnable(false); // Не редактируется

      int nameId = 0;
      switch (propertyParam.GetId())
      {
        case (ID_PROP_OTHER_PARAM + 4):
          nameId = IDS_HEAD_DIAM;
          break; // Диаметр головки
        case (ID_PROP_OTHER_PARAM + 5):
          nameId = IDS_HEAD_H;
          break; // Высота головки
        case (ID_PROP_OTHER_PARAM + 6):
          nameId = IDS_THREAD_LENGTH;
          break; // Длина резьбы
        case ID_PROP_MASSA_1000:
          nameId = IDS_MASSA_1000;
          break; // Масса 1000 шт
      }

      propertyParam.SetDisplayName(LoadStr(nameId));
    }
  }
  return !!propertyParam.GetId();
}


extern void * GetModule();


//------------------------------------------------------------------------------
/**
  Обновить свойства
*/
//---
bool BoltMain::UpdateProperty(IPropertyParam & propertyParam)
{
  // Возможно элемент уже изменили
  if (m_macroObject && propertyParam.GetId() == GetPropertyID(0))
    macro::GetMacroParam(*this, m_macroObject);

  propertyParam.SetResModule(GetModule());

  switch (propertyParam.GetId())
  {
    case ID_PROP_BOLT_VIEW: // Вид
    {
      std::int32_t view = IDP_BOLT_VIEW;
      switch (m_baseParam.drawType)
      {
        case ID_VIEW:
          view = IDP_BOLT_VIEW;
          break; // Вид
        case ID_TOP:
          view = IDP_BOLT_TOP;
          break; // Вид сверху
        case ID_BOTTOM:
          view = IDP_BOLT_BOTTOM;
          break; // Вид снизу
      }
      propertyParam.SetValue(view);
      break;
    }
    case ID_PROP_AXIS: // Ось
    {
      propertyParam.SetValue(GetAxis());
      break;
    }
    case ID_PROP_DIAMETER: // Диаметр
    {
      auto value = propertyParam.GetValue();

      // Повторно не перенакачиваем
      if (propertyParam.GetAdditionData().empty())
      {
        auto diams = GetDiamList();
        std::vector<PropertyValue> data(diams.begin(), diams.end());
        propertyParam.SetAdditionData(data);
      }

      propertyParam.SetValue(m_bolt.dr);
      break;
    }
    case ID_PROP_LENGTH: // Длина
    {
      auto value = propertyParam.GetValue();

      // Повторно не перенакачиваем
      if (propertyParam.GetAdditionData().empty())
      {
        auto lengthList = GetLengthList();
        std::vector<PropertyValue> data(lengthList.begin(), lengthList.end());
        propertyParam.SetAdditionData(data);
      }

      propertyParam.SetValue(m_bolt.l);
      break;
    }
    // Для примера свойств не описанных в ресурсах
    case (ID_PROP_OTHER_PARAM + 4):
      propertyParam.SetValue(m_bolt.d);
      break; // "Диаметр головки"
    case (ID_PROP_OTHER_PARAM + 5):
      propertyParam.SetValue(m_bolt.k);
      break; // "Высота головки"
    case (ID_PROP_OTHER_PARAM + 6):
      propertyParam.SetValue(m_bolt.b);
      break; // "Длина резьбы"
    case ID_PROP_MASSA_1000:
      propertyParam.SetValue(m_bolt.m);
      break; // "Масса 1000 шт"

    default:
      return false;
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Изменить свойство
*/
//---
bool BoltMain::ApplyProperty(IPropertyParam & propertyParam)
{
  bool res = false;
  auto value = propertyParam.GetValue();

  switch (propertyParam.GetId())
  {
    case ID_PROP_DIAMETER: // Диаметр
    case ID_PROP_LENGTH:   // Длина
    {
      std::int32_t controlType = propertyParam.GetId() == ID_PROP_DIAMETER ? IDC_DIAMETER : IDC_LENGTH;
      res = ChangeControlValue(controlType, value);
      break;
    }

    case ID_PROP_BOLT_VIEW: // Вид
    {
      short view = ID_VIEW;

      switch (value.GetInt())
      {
        case IDP_BOLT_VIEW:
          view = ID_VIEW;
          break; // Вид
        case IDP_BOLT_TOP:
          view = ID_TOPVID;
          break; // Вид сверху
        case IDP_BOLT_BOTTOM:
          view = ID_BOTTOM;
          break; // Вид снизу
      }
      m_baseParam.drawType = view;
      res = true;

      break;
    }
    case ID_PROP_AXIS: // Ось
    {
      SetAxis(value.GetBool());
      res = true;

      break;
    }
  }

  if (res)
    SetChanged();

  return res;
}


//------------------------------------------------------------------------------
/**
  Имя группы
*/
//---
std::wstring BoltMain::GetGroupName()
{
  return LoadStr(IDP_BOLT7787);
}


//------------------------------------------------------------------------------
/**
  Заполнение списка диаметров резьбы
*/
//---
void BoltMain::FillDiamList(ksapi::IPropertyListPtr & diamList)
{
  diamList->ClearList();
  std::vector<double> diams = GetDiamList();

  if (!diams.empty())
    diamList->AddDoubleValues(diams); // заносим его в список

  diamList->SetDoubleValue(m_bolt.dr); // Считываем значение диаметра из структуры и выделяем его в списке

  if (m_bolt.gost == 7787)
    diamList->SetEnable(false);
}


//------------------------------------------------------------------------------
/**
  Заполнение списка длин
*/
//---
void BoltMain::FillLengthList(ksapi::IPropertyListPtr & lengthList)
{
  // Очищаем содержимое списка "Длина"
  lengthList->ClearList();
  std::vector<double> lenghtList = GetLengthList();

  // Получаем длины
  if (!lenghtList.empty())
    // Заносим их в список
    lengthList->AddDoubleValues(lenghtList);

  // Считываем значение длины из структуры и выделяем ее в списке
  lengthList->SetDoubleValue(m_bolt.l);
}


//------------------------------------------------------------------------------
/**
  Получить массив диаметров
*/
//---
std::vector<double> BoltMain::GetDiamList()
{
  std::vector<double> diams;

  // Просматриваем все записи базы данных
  if (auto db = DataBase::GetDBConnection())
    db->ExecuteQuery("SELECT DISTINCT(dr) FROM BOLT_7787", GetDoubleColumnDataCallback, &diams);

  return diams;
}


//------------------------------------------------------------------------------
/**
  Получить массив длин
*/
//---
std::vector<double> BoltMain::GetLengthList()
{
  std::vector<double> lengthList;

  double lMin = MAX_LENGTH;
  double lMax = MIN_LENGTH;

  if (auto db = DataBase::GetDBConnection())
  {
    // Просматриваем все записи базы данных
    std::string query = std::format("SELECT L FROM BOLT_7787 WHERE dr = {}", m_bolt.dr);

    if (db->ExecuteQuery(query, GetDoubleColumnDataCallback, &lengthList))
    {
      if (!lengthList.empty())
      {
        // находим минимальное значение
        lMin = *std::min_element(lengthList.begin(), lengthList.end());
        // находим максимальное значение
        lMax = *std::max_element(lengthList.begin(), lengthList.end());
      }

      if (m_bolt.l < lMin)                   // если длина выходит за минимальную границу новых значений
        m_bolt.l = static_cast<float>(lMin); // присваиваем ей минимальное значение

      if (m_bolt.l > lMax)                   // если длина выходит за максимальную границу новых значений
        m_bolt.l = static_cast<float>(lMax); // присваиваем ей максимальное значение
    }
  }

  return lengthList;
}


//------------------------------------------------------------------------------
/**
  Перерисовка превью элемента
*/
//---
void BoltMain::RedrawPreview()
{
  BoltMain tmpBolt(*this);   // Создаем временный объект
  tmpBolt.GetParam().Init(); // Параметры по умолчанию
  tmpBolt.GetGroup();
}


//------------------------------------------------------------------------------
/**
  Нажатие на кнопку
*/
//---
bool BoltMain::ButtonClick(unsigned int buttonID)
{
  SetUnchanged();

  bool res = false;
  BaseMacroParam & baseParam = m_baseParam;

  switch (buttonID)
  {
    case IDP_BOLT_VIEW: // Вид
    {
      baseParam.drawType = ID_VIEW;
      SetChanged();
      res = true;
      break;
    }

    case IDP_BOLT_TOP: // Вид сверху
    {
      baseParam.drawType = ID_TOP;
      SetChanged();
      res = true;
      break;
    }

    case IDP_BOLT_BOTTOM: // Вид снизу
    {
      baseParam.drawType = ID_BOTTOM;
      SetChanged();
      res = true;
      break;
    }

    case ID_AXIS: // Рисовать ось
    {
      SetAxis(!GetAxis());
      SetChanged();
      res = true;
      break;
    }
  }

  return res;
}

bool BoltMain::ChangeControlValue(std::int32_t ctrlID, PropertyValue & newVal)
{
  bool res = false;

  switch (ctrlID)
  {
    case IDC_DIAMETER: // Диаметр
    {
      float diam = static_cast<float>(newVal.GetDouble());

      if (diam && ::fabs(diam - m_bolt.dr) > 0.001)
      {
        m_bolt.dr = diam;

        if (FillStruct()) // Считываем остальные данные из базы
        {
          SetChanged(); // надо перерисовать
          res = true;
        }
      }

      break;
    }

    case IDC_LENGTH: // Длина
    {
      float ln = static_cast<float>(newVal.GetDouble());

      if (ln && ::fabs(ln - m_bolt.l) > 0.001)
      {
        m_bolt.l = ln;

        if (FillStruct()) // Считываем остальные данные из базы
        {
          SetChanged(); // надо перерисовать
          res = true;
        }
      }

      break;
    }
  }

  return res;
}

// Панель свойств ##################################################################


//------------------------------------------------------------------------------
// Новый болт. Болт ГОСТ 7787-81
// ---
ParameterObject * NewBolt7787(ksapi::IMacroObjectPtr & obj)
{
  return new BoltMain(obj);
}

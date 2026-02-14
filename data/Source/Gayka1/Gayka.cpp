////////////////////////////////////////////////////////////////////////////////
//
// gayka.cpp - Конструкторский элемент на Visual C++ - гайка
//
////////////////////////////////////////////////////////////////////////////////
#include "Gayka1_pch.h"

#include <memory>

#include "Doc2DDispatcher.h"
#include "Gayka.h"
#include "math.h"
#include "ParameterObject.h"

#include "DataBase.h"
#include <format>
#include <filesystem>
#include <KompasLibraryActions.h>

bool messCommand = false;
bool messPrepare = false;
bool messSelect = false;
bool commandChange = false;

extern ksapi::IApplication * application;

//------------------------------------------------------------------------------
/**
  Отрисовка геометрии гайки (вид)
*/
//---
void Gayka_K(Doc2DDispatcher & emulator, float ls, float l, float d1, float s, float l1, float H, short j, int j1, float d2,
             int j2, bool apl)
{
  double x[9], y[9];
  double c, h1, rb;
  double xc2, yc2, xcbl, ycbl, xcbp, ycbp, ycml;
  // Координаты контура рез_части
  // j1=1 - контровочное отверстие, j1=0 - контровочного отверстия нет
  // j2=1 - исполнение 1, j2=2 - исполнение 2

  double D = s / math::CosD(30);
  double D2 = D * 0.5;
  double d22 = d2 * 0.5;

  c = (D2 - d22) * math::TanD(30);
  h1 = D2 * math::SinD(30);

  rb = (h1 * h1 + c * c) * 0.5 / c;

  x[1] = ls;
  y[1] = 0;
  if (j2 == 1)
  {
    x[2] = ls;
    y[2] = j * d22;
    x[3] = (ls + c);
    y[3] = j * D2;
    x[7] = ls + c;
    y[7] = j * h1;
  }
  else
  {
    x[2] = ls;
    y[2] = j * D2;
    x[7] = ls;
    y[7] = j * h1;
  }
  x[4] = ls + H - c;
  y[4] = j * D2;
  x[5] = ls + H;
  y[5] = j * d22;
  x[6] = ls + H;
  y[6] = 0;

  x[8] = ls + H - c;
  y[8] = j * h1;
  xc2 = ls + l;
  yc2 = j * (D2 - l1);

  xcbl = ls + rb;
  ycbl = 0;
  xcbp = ls + H - rb;
  ycbp = 0;
  ycml = j * ((D2 - h1) * 0.5 + h1);

  if (j2 == 1)
  {
    emulator.CreateLineSegment(x[1], y[1], x[2], y[2], 1);
    emulator.CreateLineSegment(x[2], y[2], x[3], y[3], 1);
    emulator.CreateLineSegment(x[3], y[3], x[4], y[4], 1);
    emulator.CreateLineSegment(x[4], y[4], x[5], y[5], 1);
    emulator.CreateLineSegment(x[5], y[5], x[6], y[6], 1);

    if (!apl)
    {
      emulator.CreateLineSegment(x[7], y[7], x[8], y[8], 1);
      emulator.CreateArcByPoint(xcbl, ycbl, rb, x[1], y[1], x[7], y[7], j == 1, 1);
      emulator.CreateArcByPoint(xcbp, ycbp, rb, x[6], y[6], x[8], y[8], j == -1, 1);
      emulator.CreateArcBy3Points(ls + c * 0.5, (D2 - (D2 - d22) * 0.5) * j, ls + H * 0.01, ycml, x[7], y[7], 1);
      emulator.CreateArcBy3Points(ls + H - c * 0.5, (D2 - (D2 - d22) * 0.5) * j, ls + H * 0.99, ycml, x[8], y[8], 1);
    }
  }
  else
  {
    emulator.CreateLineSegment(x[1], y[1], x[2], y[2], 1);
    emulator.CreateLineSegment(x[2], y[2], x[4], y[4], 1);
    emulator.CreateLineSegment(x[4], y[4], x[5], y[5], 1);
    emulator.CreateLineSegment(x[5], y[5], x[6], y[6], 1);

    if (!apl)
    {
      emulator.CreateLineSegment(x[7], y[7], x[8], y[8], 1);
      emulator.CreateArcByPoint(xcbp, ycbp, rb, x[6], y[6], x[8], y[8], j == -1, 1);
      emulator.CreateArcBy3Points(ls + H - c * 0.5, (D2 - (D2 - d22) * 0.5) * j, ls + H * 0.99, ycml, x[8], y[8], 1);
    }
  }

  if (!apl && (j1 == 1))
  {
    emulator.CreateCircle(xc2, yc2, d1 * 0.5, 1);
    emulator.CreateLineSegment(xc2 - 2, yc2, xc2 + 2, yc2, 2);
    emulator.CreateLineSegment(xc2, yc2 - 2, xc2, yc2 + 2, 2);
  }
}


//------------------------------------------------------------------------------
/**
  Отрисовка геометрии гайки (вид сбоку)
*/
//---
void Gayka_K_Side(Doc2DDispatcher & emulator, float ls, float s, float D, float d2, float H, int j, int j2, bool appl)
{
  float x, y;
  float x2, y2;
  // j2=1 - исполнение 1, j2=2 - упрощенное

  float c = static_cast<float>((D - d2) / 2 * math::TanD(30));

  y = j * s * 0.5f;
  if (j2 == 1)
  {
    x = ls + c;
    emulator.CreateLineSegment(ls, 0, ls, j * d2 * 0.5, 1);
    emulator.CreateLineSegment(ls, j * d2 * 0.5, x, y, 1);

    if (!appl)
      emulator.CreateArcBy3Points(ls + c, j * (s * 0.5), ls, s * 0.25 * j, ls + c, 0, 1);
  }
  else
  {
    x = ls;
    emulator.CreateLineSegment(x, 0, x, y, 1);
  }

  if (j2 == 3)
  {
    x2 = ls + H;
    y2 = y;
  }
  else
  {
    x2 = ls + H - c;
    y2 = j * d2 * 0.5f;
  }
  emulator.CreateLineSegment(x, y, x2, y, 1);

  if (j2 != 3)
  {
    emulator.CreateLineSegment(x2, y, ls + H, y2, 1);
    if (!appl)
      emulator.CreateArcBy3Points(ls + H - c, j * (s * 0.5), ls + H, s * 0.25 * j, ls + H - c, 0, 1);
  }

  emulator.CreateLineSegment(ls + H, y2, ls + H, 0, 1);

  if (!appl && j > 0)
    emulator.CreateLineSegment(x, 0, x2, 0, 1);
}


//------------------------------------------------------------------------------
/**
  Отрисовка геометрии гайки (вид - упрощенно)
*/
//---
void Gayka_K_Y(Doc2DDispatcher & emulator, float ls, GAYKA5915 * tmp, int j, bool appl)
{
  float h1;
  tmp->D = static_cast<float>(tmp->s / math::CosD(30));
  h1 = static_cast<float>(tmp->D * 0.5 * math::SinD(30));

  emulator.CreateLineSegment(ls, 0, ls, j * (tmp->D * 0.5), 1);
  emulator.CreateLineSegment(ls, j * (tmp->D * 0.5), ls + tmp->h, j * (tmp->D * 0.5), 1);
  emulator.CreateLineSegment(ls + tmp->h, j * (tmp->D * 0.5), ls + tmp->h, 0, 1);

  if (!appl)
    emulator.CreateLineSegment(ls, j * h1, ls + tmp->h, j * h1, 1);
}


//------------------------------------------------------------------------------
/**
  Отрисовка геометрии гайки ( пол-вида/пол-разреза )
*/
//---
void Gayka5915::Gayka_P_K(int j, bool appl)
{
  double c, dd, c1 = 0, c2 = 0;
  double y, x = 0, x1, x2, x3, y3, y2;
  double D2 = m_paramGayka.s / math::CosD(30) * 0.5;
  double d2 = m_paramGayka.d2 * 0.5;
  c = (D2 - d2) * math::TanD(30);

  if (!m_paramGayka.flags.isp && !m_baseParam.flags.flagSimp)
    y = j > 0 ? d2 : -d2;
  else
    y = j > 0 ? D2 : -D2;

  dispatcher.CreateLineSegment(0, 0, 0, y, 1);

  if (!m_paramGayka.flags.isp && !m_baseParam.flags.flagSimp)
  {
    x = c;
    dispatcher.CreateLineSegment(0, y, x, j * D2, 1);
  }
  dd = m_paramGayka.dr - 2 * MODSTEP_REAL * m_paramGayka.p;

  if (!m_baseParam.flags.flagSimp)
  {
    x3 = m_paramGayka.h - c;
    y3 = j * d2;
    y2 = j * m_paramGayka.da * 0.5;
  }
  else
  {
    x3 = m_paramGayka.h;
    y3 = j * D2;
    y2 = j * dd * 0.5;
  }

  dispatcher.CreateLineSegment(x, j * D2, x3, j * D2, 1);

  if (!m_baseParam.flags.flagSimp)
    dispatcher.CreateLineSegment(x3, j * D2, m_paramGayka.h, y3, 1);

  dispatcher.CreateLineSegment(m_paramGayka.h, y3, m_paramGayka.h, 0, 1);

  x1 = m_paramGayka.h;
  x2 = x1;
  if (!(m_baseParam.flags.flagSimp))
  {
    c1 = (m_paramGayka.da - dd) * 0.5;
    c2 = (m_paramGayka.da - m_paramGayka.dr) * 0.5;
    if (!m_paramGayka.flags.isp)
      x2 = x2 - c2;
  }


  if (!appl)
  {
    if (!m_paramGayka.flags.isp && !m_baseParam.flags.flagSimp)
    {
      x1 = x1 - c1;
      dispatcher.CreateLineSegment(m_paramGayka.h, j * m_paramGayka.da * 0.5, x1, j * 0.5 * dd, 1); //+
      dispatcher.CreateLineSegment(x1, j * dd * 0.5, x1, 0, 1);
    }

    if (!m_baseParam.flags.flagSimp)
    {
      dispatcher.CreateLineSegment(c1, j * 0.5 * dd, 0, j * 0.5 * m_paramGayka.da, 1); //+
      dispatcher.CreateLineSegment(c1, j * dd * 0.5, c1, 0, 1);                        //+
    }

    dispatcher.CreateLineSegment(x1, j * 0.5 * dd, c1, j * 0.5 * dd, 1); //+
    dispatcher.CloseFilling();

    dispatcher.CreateHatch(0, m_paramGayka.hatchAngle, m_paramGayka.hatchStep, 0, 0, 0);
    dispatcher.CreateLineSegment(0, y2, 0, y, 1);

    if (x != x3)
      dispatcher.CreateLineSegment(x, j * D2, x3, j * D2, 1);
    if (!(m_baseParam.flags.flagSimp))
      dispatcher.CreateLineSegment(x3, j * D2, m_paramGayka.h, y3, 1);

    if (!m_paramGayka.flags.isp && !m_baseParam.flags.flagSimp)
    {
      dispatcher.CreateLineSegment(0, y, x, j * D2, 1);
      dispatcher.CreateLineSegment(m_paramGayka.h, y3, m_paramGayka.h, j * m_paramGayka.da * 0.5, 1);
      dispatcher.CreateLineSegment(m_paramGayka.h, j * m_paramGayka.da * 0.5, x1, j * 0.5 * dd, 1);
    }
    else
      dispatcher.CreateLineSegment(m_paramGayka.h, y3, m_paramGayka.h, j * dd * 0.5, 1);

    dispatcher.CreateLineSegment(x1, j * 0.5 * dd, c1, j * 0.5 * dd, 1);
    if (!m_baseParam.flags.flagSimp)
      dispatcher.CreateLineSegment(c1, j * 0.5 * dd, 0, j * 0.5 * m_paramGayka.da, 1);

    dispatcher.CloseFilling();
    dispatcher.CreateLineSegment(c2, j * 0.5 * m_paramGayka.dr, x2, j * 0.5 * m_paramGayka.dr, 2);
  }
}


//------------------------------------------------------------------------------
/**
  Отрисовка геометрии гайки (сверху)
*/
//---
void Gayka5915::Gayka_Sverhu(bool appl)
{
  float D, s;
  float h1, dd;

  s = m_paramGayka.s * 0.5f;
  D = static_cast<float>(s / math::CosD(30));
  dd = m_paramGayka.dr - 2 * MODSTEP_REAL * m_paramGayka.p;
  h1 = static_cast<float>(D * math::SinD(30));

  dispatcher.CreateLineSegment(-s, h1, 0, D, 1);
  dispatcher.CreateLineSegment(0, D, s, h1, 1);
  dispatcher.CreateLineSegment(s, h1, s, -h1, 1);
  dispatcher.CreateLineSegment(s, -h1, 0, -D, 1);
  dispatcher.CreateLineSegment(0, -D, -s, -h1, 1);
  dispatcher.CreateLineSegment(-s, -h1, -s, h1, 1);

  if (!appl && !(m_baseParam.flags.flagSimp))
    dispatcher.CreateCircle(0, 0, m_paramGayka.d2 * 0.5, 1);
  dispatcher.CreateCircle(0, 0, dd * 0.5, 1);
  if (!appl)
  {
    dispatcher.CreateArcByPoint(0, 0, m_paramGayka.dr * 0.5, m_paramGayka.dr * 0.5 * math::SinD(15),
                                m_paramGayka.dr * 0.5 * math::CosD(15), m_paramGayka.dr * 0.5 * math::CosD(15),
                                -m_paramGayka.dr * 0.5 * math::SinD(15), false, 2);
  }

  if (!appl && m_baseParam.flags.flagAxis)
  {
    if (D >= 6)
    {
      dispatcher.CreateLineSegment(-AXIS_END - s, 0, s + AXIS_END, 0, AXIS_LINE);
      dispatcher.CreateLineSegment(0, -AXIS_END - D, 0, AXIS_END + D, AXIS_LINE);
    }
    else
    {
      dispatcher.CreateLineSegment(-AXIS_END - s, 0, s + AXIS_END, 0, AXIS_LINE);
      dispatcher.CreateLineSegment(0, -AXIS_END - D, 0, AXIS_END + D, AXIS_LINE);
    }
  }
}


int GetAllGaykaDataCallback(void * result, int count, char ** rowData, char **)
{
  GAYKA5915 * gayka = reinterpret_cast<GAYKA5915 *>(result);

  if (gayka)
  {
    gayka->dr = std::stof(rowData[1]);
    gayka->da = std::stof(rowData[6]);
    gayka->h = std::stof(rowData[7]);
    gayka->d2 = std::stof(rowData[8]);

    float p2 = std::stof(rowData[3]);
    float p1 = std::stof(rowData[2]);
    float s1 = std::stof(rowData[10]);

    if (gayka->flags.pitch)
      gayka->p = p2;
    else
      gayka->p = p1;

    gayka->flags.pitch_on = fabs(p2 - p1) > 0.001;
    gayka->flags.key_s_on = fabs(s1) > 0.001;

    if (!gayka->flags.key_s_on)
      gayka->flags.key_s = false;

    if (gayka->flags.key_s)
    {
      gayka->s = std::stof(rowData[10]);     // Размер под ключ
      gayka->D = std::stof(rowData[11]);     // Диаметр описанной окружности
      gayka->massa = std::stof(rowData[12]); // Масса
    }
    else
    {
      gayka->s = std::stof(rowData[4]);     // Рразмер под ключ
      gayka->D = std::stof(rowData[5]);     // Диаметр описанной окружности
      gayka->massa = std::stof(rowData[9]); // Масса
    }
  }

  return 0;
}


//------------------------------------------------------------------------------
/**
  Читать параметры гайки по диаметру
*/
//---
bool ReadGaykaBase(float d, GAYKA5915 * pGayka)
{
  bool res = false;

  if (pGayka)
  {
    if (DataBase * db = DataBase::GetDBConnection())
    {
      std::string query = "SELECT * FROM GAYKA_5915 WHERE dr=" + std::to_string(d);
      res = db->ExecuteQuery(query, GetAllGaykaDataCallback, pGayka);
    }
  }

  return res;
}


extern void * GetModule();


//------------------------------------------------------------------------------
/**
  Параметры для Hot точек
*/
//---
bool Gayka5915::GetHotPoint(ksapi::IObject2DHotPoint & point, int index)
{
  bool res = false;

  switch (index)
  {
    case 0: // Точка привязки
    {
      point.SetPoint(0, 0);
      point.SetText(L"0");
      point.SetStandartIcon(ksHotPointEnum::ksHPRing);
      res = true;
      break;
    }

    case 1: // Угол поворота
    {
      point.SetPoint(m_baseParam.drawType == ID_TOP ? (m_paramGayka.s * 0.5) : m_paramGayka.h, 0);
      point.SetCursor(CURSOR_ROTATE, GetModule());
      point.SetText(L"A");
      res = true;
      break;
    }

    case 2: // Изменение диаметра
    {
      point.SetPoint(0, m_paramGayka.dr / 2.0);
      point.SetCursor(CURSOR_DIAMETER, GetModule());
      point.SetText(L"Dr");
      res = true;
      break;
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Параметры для Hot точек
*/
//---
bool Gayka5915::ChangeHotPointParam(double d)
{
  bool res = false;

  if (DataBase * db = DataBase::GetDBConnection())
  {
    // Подобрать ближайший dR по d
    float dr = 0.0;
    float drOld = m_paramGayka.dr;

    for (auto diameter : GetDiamList())
    {
      if (dr <= d)
        dr = static_cast<float>(diameter);
      else
        break;
    }

    // Дочитать все параметры
    if (fabs(dr - drOld) > 0.01)
      res = ReadGaykaBase(dr, &m_paramGayka);
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Изменение положения Hot точек
*/
//---
bool Gayka5915::SetHotPoint(double newX, double newY, int index)
{
  switch (index)
  {
    case 0: // Хотточка перемещения
    case 1:
      ParameterObject::SetHotPoint(newX, newY, index);
      break;
    case 2:
    {
      if (ChangeHotPointParam(fabs(newY * 2)))
        RedrawMacro();

      break;
    }
  }
  return true;
}


//------------------------------------------------------------------------------
/**
  Редактирование и вставка объекта спецификации, если он был создан
*/
//---
bool Gayka5915::EditComplete(int index, bool success)
{
  if (index == 2 && success)
    DrawSpcObj();

  if (messPrepare)
    LibMessage(L"EditComplete");

  return true;
}


//------------------------------------------------------------------------------
/**
  Меню
*/
//---
IMenuPtr Gayka5915::GetMenu(int32_t index)
{
  int32_t menuId = 0;
  switch (index)
  {
    case -1:
      menuId = MENU_G5915_1;
      break;
    case 1:
      menuId = MENU_G5915_2;
      break;
    case 2:
      menuId = MENU_G5915_3;
      break;
    default:
      return nullptr;
  }

  IMenuPtr menu = application->CreateMenu();
  menu->Load(menuId, GetModule());
  return menu;
}


//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
bool Gayka5915::ExecuteCommand(int id)
{
  if (messCommand)
    ::LibMessage(std::format(L"{} = {}", LoadStr(IDS_COMMAND_NUMBER), id));

  switch (id)
  {
    case 1:
      messCommand = !messCommand;
      break;
    case 2:
      messPrepare = !messPrepare;
      break;
    case 3:
      messSelect = !messSelect;
      break;
    case 4:
      messSelect = messCommand = messPrepare = false;
      break;
    case 7:
      commandChange = !commandChange;
      break;
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Получить текст для отображения курсором
*/
//---
std::wstring Gayka5915::GetCursorTextEx(int index)
{
  std::wstring name;

  switch (index)
  {
    case 0: // Точка привязки
    {
      break;
    }
    case 1: // Угол поворота
    {
      name = std::format(L"A = {:.2f}", fmod(angle, 360.0));
      break;
    }
    case 2: // Изменение диаметра
    {
      name = std::format(L"D = {:.2f}\nH = {:.2f}", m_paramGayka.dr, m_paramGayka.h);
      break;
    }
  }
  return name;
};


//------------------------------------------------------------------------------
/**
  
*/
//---
void Gayka5915::GetCommandState(int32_t commanId, bool & enable, bool & checked)
{
  enable = commanId < 8035 || commanId == 8039;
}


//------------------------------------------------------------------------------
/**
  Подготовка к редактированию
*/
//---
bool Gayka5915::Prepare()
{
  if (messPrepare)
    ::LibMessage(L"Prepare");

  return true;
}


//------------------------------------------------------------------------------
/**
  Селектировали hot point
*/
//---
bool Gayka5915::SelectHotPoint(int index)
{
  if (messSelect)
    ::LibMessage(std::format(L"Select Index HotPoint = {}", index));

  return true;
}


//------------------------------------------------------------------------------
/**
  Расселектировали hot point
*/
//---
bool Gayka5915::UnselectHotPoint(int index)
{
  if (messSelect)
    ::LibMessage(std::format(L"Unselect Index HotPoint = {}", index));

  return true;
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Gayka5915::Gayka5915(ksapi::IMacroObjectPtr & macroObj)
  : ParameterObject(CM_GAYKA5915, macroObj)
{
  if (!macro::GetMacroParam(*this, macroObj))
  {
    m_baseParam.flags.flagAxis = 1;
    m_paramGayka.hatchAngle = 45;
    m_paramGayka.hatchStep = 2;
    InitParam();
    m_paramGayka.iFlags = 0;
  }
  else if (!m_baseParam.version)
    m_baseParam.version = 1;
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
Gayka5915::~Gayka5915()
{
}


//------------------------------------------------------------------------------
/**
  Инициализация параметров
*/
//---
void Gayka5915::InitParam()
{
  m_paramGayka.dr = 20;
  m_paramGayka.p = 2.5f;
  m_paramGayka.version = 1;
  m_paramGayka.indexMassa = 0;
  m_paramGayka.s = 30;
  m_paramGayka.D = 33;
  m_paramGayka.da = 21.6f;
  m_paramGayka.h = 16;
  m_paramGayka.d2 = 27.7f;
  m_paramGayka.classAccuracy = 2;
  m_paramGayka.massa = 71.44f;
}


//------------------------------------------------------------------------------
/**
  Копирование
*/
//---
void Gayka5915::Assign(const Gayka5915 & other)
{
  memcpy(&m_baseParam, &other.m_baseParam, sizeof(m_baseParam));    // Параметры макрообъекта
  memcpy(&m_paramGayka, &other.m_paramGayka, sizeof(m_paramGayka)); // Параметры гайки ГОСТ 5915
  m_flagMode = other.m_flagMode;
}


//------------------------------------------------------------------------------
/**
  Конструктор копирования
*/
//---
Gayka5915::Gayka5915(const Gayka5915 & other)
  : ParameterObject(CM_GAYKA5915)
{
  Assign(other);
}


//------------------------------------------------------------------------------
/**
  Номер атрибута
*/
//---
ksapi::NumericId Gayka5915::AttrNumber()
{
  return 297327484710;
}


//------------------------------------------------------------------------------
/**
  Заполнение объекта спецификации
*/
//---
bool Gayka5915::FillSpcObj(ksapi::ISpecificationBaseObjectPtr & obj1)
{
  ksapi::ISpecificationColumnsPtr columns = obj1->GetColumns();
  ksapi::ISpecificationColumnPtr column = columns->GetColumn(ksSColumnName, 1, 0);
  ksapi::ISpecificationColumnItemsPtr columnItems = column->GetColumnItems();
  ksapi::ISpecificationColumnItemPtr item;
  int spcValue;

  if (columnItems)
  {
    if (item = columnItems->GetItem(1))
    {
      // Исполнение
      if (!m_paramGayka.flags.isp)
      {
        item->SetVisible(false);
      }
      else
      {
        spcValue = 2;
        item->SetVisible(true);
        item->SetValue(spcValue);
      }
    }

    if (item = columnItems->GetItem(3))
      item->SetValue(m_paramGayka.dr);

    // Отследим мелкий шаг
    if (!m_paramGayka.flags.pitch)
    {
      // Выключить шаг и его разделитель
      if (item = columnItems->GetItem(4))
        item->SetVisible(false);

      if (item = columnItems->GetItem(5))
        item->SetVisible(false); // Шаг
    }
    else
    {
      if (item = columnItems->GetItem(4))
        item->SetVisible(true);

      if (item = columnItems->GetItem(5))
      {
        item->SetVisible(true); // Шаг
        item->SetValue(m_paramGayka.p);
      }
    }

    if (item = columnItems->GetItem(6))
      item->SetVisible(false);

    // Выключим класс прочности
    if (item = columnItems->GetItem(7))
      item->SetVisible(false);

    if (item = columnItems->GetItem(8))
      item->SetVisible(false);

    // Выключим покрытие
    if (item = columnItems->GetItem(9))
      item->SetVisible(false);

    spcValue = 5915;

    // ГОСТ
    if (item = columnItems->GetItem(11))
      item->SetValue(spcValue);

    if (columns = obj1->GetAdditionalColumns())
    {
      // Масса детали
      double massa = m_paramGayka.massa *
                     (!m_paramGayka.indexMassa       ? 1
                      : m_paramGayka.indexMassa == 1 ? 0.356
                                                     : 1.08) /
                     COUNT_MASSA;

      if (column = columns->GetColumn(ksSColumnMass, 1, 0))
      {
        if (columnItems = column->GetColumnItems())
          columnItems->GetItem(0)->SetValue(massa);
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
void Gayka5915::GetXMLPartFamilyParam(std::wstring & name, std::wstring & id)
{
  name = ::LoadStr(IDS_GAYKA5915);
  id = ::LoadStr(IDS_GAYKA5915_ID);
}


//------------------------------------------------------------------------------
/**
  Отрисовка геометрии
*/
//---
void Gayka5915::GetGroup()
{
  int k2 = int(m_paramGayka.flags.isp) + 1;

  switch (m_baseParam.drawType)
  {
    case ID_VIEW: // Вид
      if (!(m_baseParam.flags.flagSimp))
      {
        // Отрисовка апликативной заливки
        dispatcher.CreateColoring(WORKWINDOWCOLOR);
        Gayka_K(dispatcher, 0, 0, 0, m_paramGayka.s, 0, m_paramGayka.h, 1, 0, m_paramGayka.d2, k2, true);
        Gayka_K(dispatcher, 0, 0, 0, m_paramGayka.s, 0, m_paramGayka.h, -1, 0, m_paramGayka.d2, k2, true);
        dispatcher.CloseFilling();
        // Отрисовка основной графики
        Gayka_K(dispatcher, 0, 0, 0, m_paramGayka.s, 0, m_paramGayka.h, 1, 0, m_paramGayka.d2, k2, false);
        Gayka_K(dispatcher, 0, 0, 0, m_paramGayka.s, 0, m_paramGayka.h, -1, 0, m_paramGayka.d2, k2, false);
      }
      else
      {
        // Отрисовка апликативной заливки
        dispatcher.CreateColoring(WORKWINDOWCOLOR);
        Gayka_K_Y(dispatcher, 0, &m_paramGayka, 1, true);
        Gayka_K_Y(dispatcher, 0, &m_paramGayka, -1, true);
        dispatcher.CloseFilling();
        // Отрисовка основной графики
        Gayka_K_Y(dispatcher, 0, &m_paramGayka, 1, false);
        Gayka_K_Y(dispatcher, 0, &m_paramGayka, -1, false);
      }
      if (m_baseParam.flags.flagAxis)
        dispatcher.CreateLineSegment(-AXIS_END, 0, m_paramGayka.h + AXIS_END, 0, AXIS_LINE);
      break;
    case ID_LEFT: // Вид сбоку
      if (m_baseParam.flags.flagAxis)
        dispatcher.CreateLineSegment(-AXIS_END, 0, m_paramGayka.h + AXIS_END, 0, AXIS_LINE);
      if (m_baseParam.flags.flagSimp)
        k2 = 3;
      // Отрисовка апликативной заливки
      dispatcher.CreateColoring(WORKWINDOWCOLOR);
      Gayka_K_Side(dispatcher, 0, m_paramGayka.s, m_paramGayka.D, m_paramGayka.d2, m_paramGayka.h, 1, k2, true);
      Gayka_K_Side(dispatcher, 0, m_paramGayka.s, m_paramGayka.D, m_paramGayka.d2, m_paramGayka.h, -1, k2, true);
      dispatcher.CloseFilling();
      // Отрисовка основной графики
      Gayka_K_Side(dispatcher, 0, m_paramGayka.s, m_paramGayka.D, m_paramGayka.d2, m_paramGayka.h, 1, k2, false);
      Gayka_K_Side(dispatcher, 0, m_paramGayka.s, m_paramGayka.D, m_paramGayka.d2, m_paramGayka.h, -1, k2, false);
      break;
    case ID_TOP: // Вид ссверху
      // Отрисовка апликативной заливки
      dispatcher.CreateColoring(WORKWINDOWCOLOR);
      Gayka_Sverhu(true);
      dispatcher.CloseFilling();
      // Отрисовка основной графики
      Gayka_Sverhu(false);
      break;
    case ID_SEC: // Пол-вида/пол-разреза
      if (!(m_baseParam.flags.flagSimp))
      {
        // Отрисовка апликативной заливки
        dispatcher.CreateColoring(WORKWINDOWCOLOR);
        Gayka_K(dispatcher, 0, 0, 0, m_paramGayka.s, 0, m_paramGayka.h, 1, 0, m_paramGayka.d2, k2, true);
        Gayka_P_K(-1, true);
        dispatcher.CloseFilling();
        // Отрисовка основной графики
        Gayka_K(dispatcher, 0, 0, 0, m_paramGayka.s, 0, m_paramGayka.h, 1, 0, m_paramGayka.d2, k2, false);
        Gayka_P_K(-1, false);
      }
      else
      {
        // Отрисовка апликативной заливки
        dispatcher.CreateColoring(WORKWINDOWCOLOR);
        Gayka_K_Y(dispatcher, 0, &m_paramGayka, 1, true);
        Gayka_P_K(-1, true);
        dispatcher.CloseFilling();
        // Отрисовка основной графики
        Gayka_K_Y(dispatcher, 0, &m_paramGayka, 1, false);
        Gayka_P_K(-1, false);
      }
      if (m_baseParam.flags.flagAxis)
        dispatcher.CreateLineSegment(-AXIS_END, 0, m_paramGayka.h + AXIS_END, 0, AXIS_LINE);
      else
        dispatcher.CreateLineSegment(0, 0, m_paramGayka.h, 0, 1);

      break;
  }
}


//------------------------------------------------------------------------------
/**
  Перерисовка изображения в окне предварительного просмотра
*/
//---
void Gayka5915::RedrawPreview()
{
  Gayka5915 tmp(*this);
  tmp.InitParam();
  tmp.GetGroup();
}


//------------------------------------------------------------------------------
/**
  Виртуальные функции для работы с элементом
*/
//---
void Gayka5915::FillDiamList(IPropertyListPtr & diamList)
{
  std::vector<double> diameters = GetDiamList();

  if (!diameters.empty())
    diamList->AddDoubleValues(diameters);

  diamList->SetDoubleValue(m_paramGayka.dr);
}


extern int GetDoubleColumnDataCallback(void * result, int count, char ** rowData, char **);

//------------------------------------------------------------------------------
/**
  Список диаметров для комбобокса
*/
//---
std::vector<double> Gayka5915::GetDiamList()
{
  std::vector<double> diams;

  if (DataBase * db = DataBase::GetDBConnection())
    db->ExecuteQuery("SELECT dr FROM GAYKA_5915", GetDoubleColumnDataCallback, &diams);

  return diams;
}


//------------------------------------------------------------------------------
/**
  Выдать идентификатор свойства по индексу
*/
//---
int Gayka5915::GetPropertyID(int index)
{
  int res = 0;
  switch (index)
  {
    case 0:
      res = ID_PROP_ELEMENT_PARAM;
      break; // Параметры элемента
    case 1:
      res = ID_PROP_DIAM;
      break; // Диаметр резьбы
    case 2:
      res = ID_PROP_ISPOLN;
      break; // Исполнение
    case 3:
      res = ID_PROP_VIEW_PARAM;
      break; // Параметры отображения
    case 4:
      res = ID_PROP_VIEW;
      break; // Вид
    case 5:
      res = ID_PROP_SIMPLE;
      break; // Упрощенное отображение
    case 6:
      res = ID_PROP_AXIS;
      break; // Рисовать ось
    case 7:
      res = ID_PROP_HATCH_PARAM;
      break; // Параметры штриховки
    case 8:
      res = ID_PROP_HATCH_STEP;
      break; // Шаг штриховки
    case 9:
      res = ID_PROP_HATCH_ANGLE;
      break; // Угол штриховки
    case 10:
      res = ID_PROP_OTHER_PARAM;
      break; // Группа Дополнительные параметры
    // Для примера свойств не описанных в ресурсах
    case 11:
      res = ID_PROP_OTHER_PARAM + 1;
      break; // IDS_PITCH      Шаг резьбы
    case 12:
      res = ID_PROP_OTHER_PARAM + 2;
      break; // IDS_DIM_KEY    Размер под ключ
    case 13:
      res = ID_PROP_OTHER_PARAM + 3;
      break; // IDS_HEAD_H     Высота головки
    case 14:
      res = ID_PROP_MASSA_1000;
      break; // IDS_MASSA_1000 Масса 1000 шт
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Добавить свойство в список свойств
*/
//---
bool Gayka5915::GetProperty(int32_t index, IPropertyParam & propertyParam)
{
  bool res = ParameterObject::GetProperty(index, propertyParam);

  if (propertyParam.GetId() > ID_PROP_OTHER_PARAM && propertyParam.GetId() < ID_PROP_OTHER_PARAM_END)
  {
    propertyParam.SetType(ksOPControlEditDouble); // Вещественный комбобокс
    propertyParam.SetValue(0.0);
    propertyParam.SetEnable(false); // Не редактируется

    int nameId = 0;
    switch (propertyParam.GetId())
    {
      case (ID_PROP_OTHER_PARAM + 1):
        nameId = IDS_PITCH;
        break; // Шаг резьбы
      case (ID_PROP_OTHER_PARAM + 2):
        nameId = IDS_DIM_KEY;
        break; // Размер под ключ
      case (ID_PROP_OTHER_PARAM + 3):
        nameId = IDS_HEAD_H;
        break; // Высота головки
      case ID_PROP_MASSA_1000:
        nameId = IDS_MASSA_1000;
        break; // Масса 1000 шт
    }

    propertyParam.SetDisplayName(LoadStr(nameId));
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Обновить параметры свойства
*/
//---
bool Gayka5915::UpdateProperty(IPropertyParam & propertyParam)
{
  bool res = ParameterObject::UpdateProperty(propertyParam);

  if (!res)
  {
    switch (propertyParam.GetId())
    {
      case ID_PROP_DIAM: // Диаметр резьбы
      {
        auto value = propertyParam.GetValue();

        // Повторно не перенакачиваем
        if (propertyParam.GetAdditionData().empty())
        {
          auto diams = GetDiamList();
          std::vector<PropertyValue> data(diams.begin(), diams.end());
          propertyParam.SetAdditionData(data);
        }

        propertyParam.SetValue(static_cast<double>(GetParamGayka().dr));
        res = true;
        break;
      }
      case ID_PROP_ISPOLN: // Исполнение
      {
        propertyParam.SetValue(GetParamGayka().flags.isp ? IDP_ICO_G_I2 : IDP_ICO_G_I1);
        res = true;
        break;
      }
      // Для примера свойств не описанных в ресурсах
      case (ID_PROP_OTHER_PARAM + 1):
        propertyParam.SetValue(m_paramGayka.p);
        break; // IDS_PITCH       Шаг резьбы
      case (ID_PROP_OTHER_PARAM + 2):
        propertyParam.SetValue(m_paramGayka.s);
        break; // IDS_DIM_KEY     Размер под ключ
      case (ID_PROP_OTHER_PARAM + 3):
        propertyParam.SetValue(m_paramGayka.h);
        break; // IDS_HEAD_H      Высота головки
      case ID_PROP_MASSA_1000:
        propertyParam.SetValue(m_paramGayka.massa);
        break; // IDS_MASSA_1000  Масса 1000 шт
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Изменить свойство
*/
//---
bool Gayka5915::ApplyProperty(IPropertyParam & propertyParam)
{
  bool res = ParameterObject::ApplyProperty(propertyParam);

  if (!res)
  {
    switch (propertyParam.GetId())
    {
      case ID_PROP_DIAM: // Диаметр резьбы
      {
        auto value = propertyParam.GetValue();
        res = ChangeControlValue(IDP_DIAM, value);
        break;
      }

      case ID_PROP_ISPOLN: // Исполнение
      {
        auto value = propertyParam.GetValue();

        m_paramGayka.flags.isp = value.GetInt() == IDP_ICO_G_I2;
        res = true;

        break;
      }
    }

    if (res)
      SetChanged();
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Имя группы
*/
//---
std::wstring Gayka5915::GetGroupName()
{
  return LoadStr(IDP_GAYKA5915);
}


//------------------------------------------------------------------------------
/**
  Нажатие на кнопку
*/
//---
bool Gayka5915::ButtonClick(unsigned int buttonID)
{
  SetUnchanged();

  bool res = false;

  switch (buttonID)
  {
    // Отображение
    case ID_VIEW: // Вид
    case ID_LEFT: // Cбоку
    case ID_TOP:  // Cверху
    case ID_SEC:  // Вид/разрез
      m_baseParam.drawType = static_cast<short>(buttonID);
      SetChanged();
      res = true;
      break;

    case ID_SIMPLE: // Упрощенно
      m_baseParam.flags.flagSimp = !m_baseParam.flags.flagSimp;
      SetChanged();
      res = true;
      break;

    case ID_AXIS: // Рисовать ось
      m_baseParam.flags.flagAxis = !m_baseParam.flags.flagAxis;
      SetChanged();
      res = true;
      break;

    case IDP_ICO_G_I1: // Исполнение 1
      m_paramGayka.flags.isp = 0;
      SetChanged(); // Нужно перерисовать и перезаполнить грид
      break;

    case IDP_ICO_G_I2: // Исполнение 2
      m_paramGayka.flags.isp = 1;
      SetChanged(); // Нужно перерисовать и перезаполнить грид
      break;

    case IDP_ICO_G_STEP: // Шаг
      m_paramGayka.flags.pitch = !m_paramGayka.flags.pitch;
      SetChanged(); // Нужно перерисовать и перезаполнить грид
      break;

    case IDP_ICO_KEY: // Размер поз ключ
      m_paramGayka.flags.key_s = !m_paramGayka.flags.key_s;
      SetChanged(); // Нужно перерисовать и перезаполнить грид
      break;
  }

  return res;
}

bool Gayka5915::ChangeControlValue(std::int32_t ctrlID, PropertyValue & newVal)
{
  bool res = false;

  switch (ctrlID)
  {
    case IDP_DIAM: // Диаметр
    {
      float diam = static_cast<float>(newVal.GetDouble());

      if (diam && ::fabs(diam - m_paramGayka.dr) > 0.001)
      {
        m_paramGayka.dr = diam;
        res = ReadGaykaBase(m_paramGayka.dr, &m_paramGayka);
      }

      break;
    }
  }

  if (res)
    SetChanged(); // надо перерисовать

  return res;
}

//------------------------------------------------------------------------------
/**
  Новая гайка
*/
//---
ParameterObject * NewGayka5915(ksapi::IMacroObjectPtr & obj)
{
  return new Gayka5915(obj);
}

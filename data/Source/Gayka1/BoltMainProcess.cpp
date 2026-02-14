#include "Gayka1_pch.h"
#include "BoltMainProcess.h"
#include "BaseProcess.h"
#include "ParameterObject.h"
#include "Bolt.h"


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
BoltMainProcess::BoltMainProcess(BoltMain & bolt)
  : ParameterObjectProcess(bolt)
  , m_bolt(bolt)
{
}


//------------------------------------------------------------------------------
/**
  Нажатие на кнопку
*/
//---
bool BoltMainProcess::ButtonClick(std::int32_t buttonID)
{
  bool res = m_bolt.ButtonClick(buttonID);

  if (Changed())
    Draw1();

  return !res ? ParameterObjectProcess::ButtonClick(buttonID) : res;
}


//------------------------------------------------------------------------------
/**
  Вывод контролов
*/
//---
void BoltMainProcess::ShowControls()
{
  // Комбобокс диаметров резьбы
  if (ksapi::IPropertyListPtr diamList = CreateList())
  {
    InitPropertyControl(diamList, IDC_DIAMETER);
    m_bolt.FillDiamList(diamList);
  }

  // Комбобокс длин
  if (ksapi::IPropertyListPtr lengthList = CreateList())
  {
    InitPropertyControl(lengthList, IDC_LENGTH);
    m_bolt.FillLengthList(lengthList);
  }

  // Отображение
  CreateSeparator(ksSeparatorDownName);

  if (ksapi::IPropertyMultiButtonPtr buttons = CreateMultiButton(ksRadioButton))
  {
    InitPropertyControl(buttons, IDP_REFLECTION);

    BaseMacroParam & baseParam = m_bolt.GetBaseParam();
    AddButton2(buttons, IDP_BOLT_VIEW, IDF_BOLT_VIEW, baseParam.drawType == ID_VIEW);       // Кнопка вид
    AddButton2(buttons, IDP_BOLT_TOP, IDF_BOLT_TOP, baseParam.drawType == ID_TOP);          // Кнопка сверху
    AddButton2(buttons, IDP_BOLT_BOTTOM, IDF_BOLT_BOTTOM, baseParam.drawType == ID_BOTTOM); // Кнопка снизу
  }

  // Рисовать ось
  CreateSeparator(ksSeparatorDownName);
  if (ksapi::IPropertyMultiButtonPtr axisBtn = CreateMultiButton(ksCheckButton))
  {
    InitPropertyControl(axisBtn, IDC_AXIS);
    AddButton2(axisBtn, ID_AXIS, 0xe018, m_bolt.GetAxis());
  }

  // Создавать объект спецификации
  ParameterObjectProcess::ShowControls();
}


//------------------------------------------------------------------------------
/**
  Изменить значение контрола
*/
//---
bool BoltMainProcess::ChangeControlValue(ksapi::IPropertyControl & ctrl)
{
  bool res = false;
  BoltMainStruct & bolt = m_bolt.GetParam();

  switch (ctrl.GetId())
  {
    case IDC_DIAMETER: // Диаметр
    {
      if (ksapi::IPropertyEditPtr realEdit = &ctrl)
      {
        float diam = static_cast<float>(realEdit->GetDoubleValue());
        if (::fabs(diam - bolt.dr) > 0.001)
        {
          bolt.dr = diam;

          ksapi::IPropertyListPtr lengthList(GetPropertyControl(IDC_LENGTH));
          m_bolt.FillLengthList(lengthList); // Перезаполняем список "Длина"

          if (m_bolt.FillStruct()) // Считываем остальные данные из базы
          {
            SetChanged(); // надо перерисовать
            res = true;
          }
        }
      }
      break;
    }

    case IDC_LENGTH: // Длина
    {
      if (ksapi::IPropertyListPtr realList = &ctrl)
      {
        float ln = static_cast<float>(realList->GetDoubleValue());
        if (::fabs(ln - bolt.l) > 0.001)
        {
          bolt.l = ln;

          if (m_bolt.FillStruct()) // Считываем остальные данные из базы
          {
            SetChanged(); // надо перерисовать
            res = true;
          }
        }
      }
      break;
    }

    default:
      res = ParameterObjectProcess::ChangeControlValue(ctrl);
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Заполнение грида параметров
*/
//---
void BoltMainProcess::ShowParam()
{
  BoltMainStruct & bolt = m_bolt.GetParam();
  AddDoubleToGrig(IDS_HEAD_DIAM, bolt.d);     // Диаметр головки
  AddDoubleToGrig(IDS_HEAD_HEIGHT, bolt.k);   // Высота головки
  AddDoubleToGrig(IDS_THREAD_LENGTH, bolt.b); // Длина резьбы
  AddDoubleToGrig(IDS_MASSA_1000, bolt.m);    // Масса 1000 шт
}

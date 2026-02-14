#include "Gayka1_pch.h"
#include "Gayka5915Process.h"
#include "Gayka.h"


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Gayka5915Process::Gayka5915Process(Gayka5915 & gayka)
  : ParameterObjectProcess(gayka)
  , m_gayka(gayka)
{
}


//------------------------------------------------------------------------------
/**
  Заполнение грида параметров
*/
//---
void Gayka5915Process::ShowParam()
{
  GAYKA5915 & gaykaParam = m_gayka.GetParamGayka();
  AddDoubleToGrig(IDS_PITCH, gaykaParam.p);          // Шаг резьбы
  AddDoubleToGrig(IDS_DIM_KEY, gaykaParam.s);        // Размер под ключ
  AddDoubleToGrig(IDS_HEAD_H, gaykaParam.h);         // Высота головки
  AddDoubleToGrig(IDS_MASSA_1000, gaykaParam.massa); // Масса 1000 шт
}


//------------------------------------------------------------------------------
/**
  Нажатие на кнопку
*/
//---
bool Gayka5915Process::ButtonClick(std::int32_t buttonID)
{
  bool oldSec = m_gayka.IsHatchEnable(); // Изменен тип отображения

  if (m_gayka.ButtonClick(buttonID))
  {
    // Если переключили тип отрисовки на штриховку
    bool enable = m_gayka.IsHatchEnable();

    if (oldSec != enable)
    {
      SetControlVisible(IDP_HATCHANG_ANGLE, enable);
      SetControlVisible(IDP_HATCHANG_STEP, enable);
    }
  }

  bool changed = Changed();

  if (changed)
    Draw1();
  else
    ParameterObjectProcess::ButtonClick(buttonID);

  return changed;
}


//------------------------------------------------------------------------------
/**
  Вывод контролов
*/
//---
void Gayka5915Process::ShowControls()
{
  // Комбобокс диаметров резьбы
  if (ksapi::IPropertyListPtr diamList = CreateList())
  {
    InitPropertyControl(diamList, IDP_DIAM);
    m_gayka.FillDiamList(diamList);
  }

  // Отображение
  ksapi::IPropertySeparatorPtr sep(CreateSeparator(ksSeparatorDownName));

  if (sep)
    sep->SetName(::LoadStr(IDS_REFLECTION_SEP));

  if (ksapi::IPropertyMultiButtonPtr buttons = CreateMultiButton(ksRadioButton))
  {
    InitPropertyControl(buttons, IDP_REFLECTION);

    BaseMacroParam & baseParam = m_gayka.GetBaseParam();
    // Кнопка вид
    AddButton2(buttons, ID_VIEW, IDF_VIEW, baseParam.drawType == ID_VIEW);
    // Кнопка сбоку
    AddButton2(buttons, ID_LEFT, IDF_LEFT, baseParam.drawType == ID_LEFT);
    // Кнопка сверху
    AddButton2(buttons, ID_TOP, IDF_TOP, baseParam.drawType == ID_TOP);
    // Кнопка Вид/разрез
    AddButton2(buttons, ID_SEC, IDF_SEC, baseParam.drawType == ID_SEC);
  }

  // Исполнение
  if (ksapi::IPropertyMultiButtonPtr ispButtons = CreateMultiButton(ksRadioButton))
  {
    InitPropertyControl(ispButtons, IDP_ISP);

    GAYKA5915 & paramGayka = m_gayka.GetParamGayka();
    // Кнопка исполнение 1
    AddButton2(ispButtons, IDP_ICO_G_I1, IDF_G_I1, !paramGayka.flags.isp);
    // Кнопка исполнение 2
    AddButton2(ispButtons, IDP_ICO_G_I2, IDF_G_I2, paramGayka.flags.isp);
  }

  // Упрощенно
  if (ksapi::IPropertyMultiButtonPtr simpleButtons = CreateMultiButton(ksCheckButton))
  {
    InitPropertyControl(simpleButtons, IDP_SIMPLE);

    BaseMacroParam & baseParam = m_gayka.GetBaseParam();
    AddButton2(simpleButtons, ID_SIMPLE, IDF_SIMPLE, baseParam.flags.flagSimp);
    AddButton2(simpleButtons, ID_AXIS, IDF_AXIS, baseParam.flags.flagAxis);
  }

  CreateSeparator(ksSeparatorDownName);

  // Дополнительные параметры
  if (ksapi::IPropertyMultiButtonPtr paramButtons = CreateMultiButton(ksCheckButton))
  {
    InitPropertyControl(paramButtons, IDP_FLAGS);

    GAYKA5915 & paramGayka = m_gayka.GetParamGayka();
    // Шаг мелкий
    AddButton2(paramButtons, IDP_ICO_G_STEP, IDF_G_STEP, paramGayka.flags.pitch, paramGayka.flags.pitch_on);
    // Размер под ключ
    AddButton2(paramButtons, IDP_ICO_KEY, IDF_KEY, !paramGayka.flags.key_s, paramGayka.flags.key_s_on);
  }

  // Параметры штриховки
  ParameterObjectProcess::ShowControls();
}


//------------------------------------------------------------------------------
/**
  Изменить значение контрола
*/
//---
bool Gayka5915Process::ChangeControlValue(ksapi::IPropertyControl & ctrl)
{
  bool res = ParameterObjectProcess::ChangeControlValue(ctrl);

  if (!res)
  {
    switch (ctrl.GetId())
    {
      case IDP_DIAM: // Диаметр
      {
        if (ksapi::IPropertyListPtr realList = &ctrl)
        {
          float diam = static_cast<float>(realList->GetDoubleValue());
          GAYKA5915 & gaykaParam = m_gayka.GetParamGayka();

          if (::fabs(diam - gaykaParam.dr) > 0.001)
          {
            res = true;
            gaykaParam.dr = diam;

            if (ReadGaykaBase(gaykaParam.dr, &gaykaParam))
            {
              if (ksapi::IPropertyMultiButtonPtr control = GetPropertyControl(IDP_FLAGS))
              {
                control->SetButtonEnable(IDP_ICO_G_STEP, gaykaParam.flags.pitch_on);
                control->SetButtonEnable(IDP_ICO_KEY, gaykaParam.flags.key_s_on);
                control->SetButtonChecked(IDP_ICO_KEY, gaykaParam.flags.key_s);
              }
            }
          }
        }
        break;
      }
    }

    if (res)
      SetChanged(); // надо перерисовать
  }

  return res;
}

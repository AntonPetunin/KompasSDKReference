#include "Gayka1_pch.h"
#include "BaseProcess.h"
#include "ParameterObject.h"


extern ksapi::IApplication * application;


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
BaseProcess::BaseProcess(const BaseProcess & other)
  : m_rowIndex(other.m_rowIndex)
  , m_propTabs(other.m_propTabs)
  , m_curentCollection(other.m_curentCollection)
  , m_propGrid(other.m_propGrid)
  , m_slideBox(other.m_slideBox)
{
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
BaseProcess::BaseProcess()
{
  m_parent = application ? application->GetActiveDocument().Get() : nullptr;
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
BaseProcess::~BaseProcess()
{
}


//------------------------------------------------------------------------------
/**
  Получить родительский документ
*/
//---
ksapi::IKompasDocument * BaseProcess::GetParentDocument() const
{
  return m_parent;
}

//------------------------------------------------------------------------------
/**
  Накачка контрола общими параметрами
*/
//---
void BaseProcess::InitPropertyControl(ksapi::IPropertyControlPtr control, std::int32_t ctrlID, bool enable /*= true*/,
                                      bool visible /*= true*/)
{
  if (control)
  {
    control->SetName(::LoadStr(ctrlID));
    control->SetId(ctrlID);
    control->SetEnable(enable);
    control->SetVisible(visible);
  }
}


//------------------------------------------------------------------------------
/**
  Добавление закладки
*/
//---
bool BaseProcess::CreateTab(std::int32_t tabID, bool visible /*= true*/, bool active /*= false*/)
{
  bool res = false;

  if (tabID)
  {
    if (m_propTabs)
    {
      std::wstring tmpBstr(LoadStr(tabID));

      if (ksapi::IPropertyTabPtr propTab = m_propTabs->Add(tmpBstr))
      {
        m_curentCollection = propTab->GetPropertyControls();

        if (!visible)
          propTab->SetVisible(visible);

        res = true;
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Добавление комбобокса вещественных значений
*/
//---
ksapi::IPropertyListPtr BaseProcess::CreateList(double minVal /*= 0*/, double maxVal /*= 0*/)
{
  ksapi::IPropertyListPtr realList;

  if (m_curentCollection)
  {
    realList = m_curentCollection->Add(ksControlListReal);

    if (realList)
    {
      if (minVal != maxVal)
        realList->SetValueRange(minVal, maxVal);

      realList->SetReadOnly(true);
    }
  }

  return realList;
}


//------------------------------------------------------------------------------
/**
  Добавление редактора вещественных параметров
*/
//---
ksapi::IPropertyEditPtr BaseProcess::CreateRealEdit(double value, double minValue, double maxValue, ControlTypeEnum controlType)
{
  ksapi::IPropertyEditPtr realList;

  if (m_curentCollection)
  {
    realList = m_curentCollection->Add(controlType);

    if (realList)
    {
      if (minValue != maxValue)
        realList->SetValueRange(minValue, maxValue);

      realList->SetDoubleValue(value);
    }
  }

  return realList;
}


//------------------------------------------------------------------------------
/**
  Добавление сепаратора
*/
//---
ksapi::IPropertySeparatorPtr BaseProcess::CreateSeparator(SeparatorTypeEnum separatorType)
{
  ksapi::IPropertySeparatorPtr rSepar;

  if (m_curentCollection)
    rSepar = m_curentCollection->Add(ksControlSeparator);

  return rSepar;
}


//------------------------------------------------------------------------------
/**
  Добавление CheckBox-а
*/
//---
ksapi::IPropertyCheckBoxPtr BaseProcess::CreateCheckBox(bool checked)
{
  ksapi::IPropertyCheckBoxPtr check;

  if (m_curentCollection)
  {
    check = m_curentCollection->Add(ksControlCheckBox);

    if (check)
      check->SetBoolValue(checked);
  }

  return check;
}


//------------------------------------------------------------------------------
/**
  Добавление группы кнопок
*/
//---
ksapi::IPropertyMultiButtonPtr BaseProcess::CreateMultiButton(ButtonTypeEnum buttonType)
{
  ksapi::IPropertyMultiButtonPtr buttons;

  if (m_curentCollection)
  {
    buttons = m_curentCollection->Add(ksControlMultiButton);

    if (buttons)
      buttons->SetButtonsType(buttonType);
  }

  return buttons;
}


//------------------------------------------------------------------------------
/**
  Получить указатель на Control по его идентификатору (для изменения параметров контрола)
*/
//---
ksapi::IPropertyControlPtr BaseProcess::GetPropertyControl(int ctrlID)
{
  ksapi::IPropertyControlPtr control;

  if (m_propTabs)
  {
    for (std::int32_t i = 0, c = m_propTabs->GetCount(); i < c && !control; i++)
    {
      ksapi::IPropertyTabPtr tab(m_propTabs->GetItem(i));
      ksapi::IPropertyControlsPtr ctrls(tab ? tab->GetPropertyControls() : nullptr);

      if (ctrls)
      {
        std::wstring name(LoadStr(ctrlID));
        control = ctrls->GetItemByName(name);
      }
    }
  }

  return control;
}


//------------------------------------------------------------------------------
/**
  Изменить состояние видимости контрола
*/
//---
void BaseProcess::SetControlVisible(std::int32_t ctrlID, bool enabled /*= true*/)
{
  if (ksapi::IPropertyControlPtr control = GetPropertyControl(ctrlID))
    control->SetVisible(enabled);
}


//------------------------------------------------------------------------------
/**
  Добавление строкового значения в грид
*/
//---
void BaseProcess::AddStringToGrig(std::int32_t paramID, std::wstring value)
{
  if (m_propGrid)
  {
    m_propGrid->SetCellText(m_rowIndex, 1, value);
    m_propGrid->SetCellText(m_rowIndex++, 0, LoadStr(paramID)); // Не переставлять так задумано
  }
}


//------------------------------------------------------------------------------
/**
  Добавление числового значения в грид
*/
//---
void BaseProcess::AddDoubleToGrig(std::int32_t paramID, double value)
{
  if (m_propGrid)
  {
    m_propGrid->SetCellText(m_rowIndex, 1, std::to_wstring(value)); // Выводим значение
    m_propGrid->SetCellText(m_rowIndex++, 0, LoadStr(paramID));     // Имя параметра
  }
}


//------------------------------------------------------------------------------
/**
  Добавление кнопки в группу
*/
//---
void BaseProcess::AddButton(ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, bool cheched /*= false*/,
                            bool enable /*= true*/)
{
  AddButton2(buttons, btnID, btnID, cheched, enable);
}


//------------------------------------------------------------------------------
/**
  Добавление кнопки в группу
*/
//---
void BaseProcess::AddButton2(ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, std::int32_t bmpID,
                             bool cheched /*= false*/, bool enable /*= true*/)
{
  if (buttons)
  {
    buttons->AddButton(btnID, bmpID, -1);
    buttons->SetButtonEnable(btnID, enable);
    buttons->SetButtonChecked(btnID, cheched);
    buttons->SetButtonTips(btnID, LoadStr(btnID));
    buttons->SetButtonIconFont(btnID, GetLibraryIconFont());
  }
}


//------------------------------------------------------------------------------
/**
  Обработчик нажатия кнопок
*/
//---
bool BaseProcess::OnButtonClick(std::int32_t buttonId)
{
  if (buttonId == pbHelp && GetProcessObject())
    OpenHelp(GetProcessObject()->GetCommand());
  else if (ButtonClick(buttonId))
    RedrawPhantom();

  return true;
}


//------------------------------------------------------------------------------
/**
  Обработчик изменения значения контрола
*/
//---
void BaseProcess::OnChangeControlValue(ksapi::IPropertyControlPtr ctrl)
{
  if (ctrl)
  {
    if (ChangeControlValue(*ctrl))
    {
      Draw1();
      RedrawPhantom();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Обработчик нажатия кнопки контрола
*/
//---
void BaseProcess::OnControlCommand(ksapi::IPropertyControlPtr control, std::int32_t buttonId)
{
  if (ButtonClick(buttonId))
    RedrawPhantom();
}


//------------------------------------------------------------------------------
/**
  Вызов справки
*/
//---
bool BaseProcess::OnCommandHelp(std::int32_t buttonID)
{
  if (GetProcessObject())
    OpenHelp(GetProcessObject()->GetCommand());

  return true;
}


//------------------------------------------------------------------------------
/**
  Обработчик изменения положения объекта
*/
//---
bool BaseProcess::OnPlacementChanged(double x, double y, double angle, bool dynamic)
{
  PlacementChanged(x, y, angle, dynamic);
  return true;
}


//------------------------------------------------------------------------------
/**
  Обработчик завершения процесса
*/
//---
bool BaseProcess::OnEndProcess()
{
  bool res = EndProcess();
  if (res)
    delete this;

  return res;
}

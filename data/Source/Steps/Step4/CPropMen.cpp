////////////////////////////////////////////////////////////////////////////////
//
// CPropMen.cpp - Класс для работы с панелью свойств
//
////////////////////////////////////////////////////////////////////////////////
#include "Step4_pch.h"

#include "CPropMen.h"
#include "Events/PropertyManagerEvent.h"
#include "Resource.h"
#include "Step4.h"

extern ksapi::IApplication * ksAppl;

//------------------------------------------------------------------------------
/**
  Вывод сообщения
*/
//---
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons,
                        bool showModal)
{
  std::int32_t res = 0;

  if (ksAppl && !message.empty()) // строка передана
    res = ksAppl->ShowMessageBox(message, LoadStr(IDR_LIBID), dlgType, dlgButtons, showModal);

  return res;
}


//------------------------------------------------------------------------------
/**
  Cообщение об ошибке
*/
//---
std::int32_t ErrorMessage(const std::wstring & message)
{
  std::int32_t res = 0;

  if (ksAppl && !message.empty()) // строка передана
    res = ksAppl->ShowMessageBox(message, LoadStr(IDR_LIBID), ksMessageWarning, ksButtonSetOk, false);

  return res;
}


/// Результат диалога
enum DialogResult
{
  Positive = 6,
  Negative = 7,
  Cancel = 2
};


//------------------------------------------------------------------------------
/**
  Диалог Да|Нет|Отмена
*/
//---
std::int32_t YesNoMessage(const std::wstring & message)
{
  std::int32_t res = LibMessage(message, ksMessageWarning, ksButtonSetYesNoCancel);
  return res == DialogResult::Positive ? 1 : res == DialogResult::Negative ? 0 : -1;
}


//------------------------------------------------------------------------------
/**
  Вывод сообщения
*/
//---
std::int32_t LibMessage(std::int32_t strId, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  return LibMessage(LoadStr(strId), dlgType, dlgButtons, showModal);
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerObject::PropertyManagerObject()
  : m_rowIndex(0)
  , m_flagMode(0)
{
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
PropertyManagerObject::~PropertyManagerObject()
{
  m_paramGrid = nullptr;        // Грид для вывода дополнительных параметров элемента
  m_slideBox = nullptr;         // Окно просмотра
  m_curentCollection = nullptr; // Коллекция контролов
  m_propTabs = nullptr;         // Коллекция закладок
  m_procParam = nullptr;        // Параметры процесса
  m_rowIndex = 0;               // Индекс строки
}


//------------------------------------------------------------------------------
/**
  Создать Сontrol заданного типа
*/
//---
ksapi::IPropertyControlPtr PropertyManagerObject::CreateControl(ControlTypeEnum conType)
{
  return m_curentCollection ? m_curentCollection->Add(conType) : nullptr;
}


//------------------------------------------------------------------------------
/**
  Найти контрол
*/
//---
ksapi::IPropertyControlPtr PropertyManagerObject::GetPropertyControl(long ctrlID)
{
  ksapi::IPropertyControlPtr control;

  if (ksapi::IPropertyTabsPtr tabs = m_procParam ? m_procParam->GetPropertyTabs() : nullptr)
  {
    for (std::int32_t i = 0, c = tabs->GetCount(); i < c && control; i++)
    {
      ksapi::IPropertyTabPtr tab(tabs->GetItem(i));

      if (ksapi::IPropertyControlsPtr ctrls = tab ? tab->GetPropertyControls() : nullptr)
        control = ctrls->GetItemByName(LoadStr(ctrlID));
    }
  }

  return control;
}


//------------------------------------------------------------------------------
/**
  Добавить параметр в грид
*/
//---
void PropertyManagerObject::AddStringToGrig(std::int32_t paramID, const std::wstring & value)
{
  if (m_paramGrid)
  {
    m_paramGrid->SetCellText(m_rowIndex, 1, value);
    // Не переставлять так задумано
    m_paramGrid->SetCellText(m_rowIndex++, 0, LoadStr(paramID));
  }
}


//------------------------------------------------------------------------------
/**
  Добавить параметр в грид
*/
//---
void PropertyManagerObject::AddDoubleToGrig(std::int32_t paramID, double value)
{
  if (m_paramGrid)
  {
    // Выводим значение
    m_paramGrid->SetCellText(m_rowIndex, 1, std::to_wstring(value));
    // Имя параметра
    m_paramGrid->SetCellText(m_rowIndex++, 0, LoadStr(paramID));
  }
}


//------------------------------------------------------------------------------
/**
  Создать закладку
*/
//---
bool PropertyManagerObject::CreateTab(std::int32_t tabID, bool visible, bool active)
{
  bool res = false;

  if (tabID)
  {
    if (m_propTabs)
    {
      if (ksapi::IPropertyTabPtr propTab = m_propTabs->Add(LoadStr(tabID)))
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
  Создать комбобокс
*/
//---
ksapi::IPropertyListPtr PropertyManagerObject::CreateList(double minVal, double maxVal)
{
  ksapi::IPropertyListPtr realList;

  if (m_curentCollection)
  {
    if (realList = m_curentCollection->Add(ksControlListReal))
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
  Создать поле для ввода вещественных значений
*/
//---
ksapi::IPropertyEditPtr PropertyManagerObject::CreateEditReal(double minVal, double maxVal)
{
  ksapi::IPropertyEditPtr realEdit;

  if (m_curentCollection)
  {
    if (realEdit = m_curentCollection->Add(ksControlEditReal))
    {
      if (minVal != maxVal)
        realEdit->SetValueRange(minVal, maxVal);
    }
  }
  return realEdit;
}


//------------------------------------------------------------------------------
/**
  Добавление SpinEdit
*/
//---
ksapi::IPropertySpinEditPtr PropertyManagerObject::CreateSpinList(double minVal, double maxVal)
{
  ksapi::IPropertySpinEditPtr realList;

  if (m_curentCollection)
  {
    if (realList = m_curentCollection->Add(ksControlSpinInt))
    {
      if (minVal != maxVal)
        realList->SetValueRange(minVal, maxVal);

      realList->SetReadOnly(false);
    }
  }

  return realList;
}


//------------------------------------------------------------------------------
/**
  Добавление CheckBox-а
*/
//---
ksapi::IPropertyCheckBoxPtr PropertyManagerObject::CreateCheckBox(bool checked)
{
  ksapi::IPropertyCheckBoxPtr check;

  if (m_curentCollection)
  {
    if (check = m_curentCollection->Add(ksControlCheckBox))
      check->SetBoolValue(checked);
  }

  return check;
}


//------------------------------------------------------------------------------
/**
  Установить доступность контрола
*/
//---
void PropertyManagerObject::SetControlEnable(long ctrlID, bool enabled)
{
  if (ksapi::IPropertyControlPtr control = GetPropertyControl(ctrlID))
    control->SetEnable(enabled);
}


//------------------------------------------------------------------------------
/**
  Инициализация параметров процесса
*/
//---
bool PropertyManagerObject::InitProcessParam()
{
  bool res = true;

  if (ksAppl && !m_procParam)
  {
    res = false;
    // Создаем панель свойств
    m_procParam = ksAppl->CreateProcessParam();
    m_procParam->SetAutoReduce(false);
    m_procParam->SetCaption(LoadStr(IDS_PROCESS));
    m_procParam->SetSpecToolbar(pnEscHelp);
    m_procParam->SetEnableUndoRedo(true);

    // Подписываемся на события панели свойств
    new PropertyManagerEvent(m_procParam, *this);

    if (ksapi::IPropertyTabsPtr tabs = m_procParam->GetPropertyTabs())
    {
      //Добавляем закладку
      if (ksapi::IPropertyTabPtr tab = tabs->Add(LoadStr(IDS_TABS)))
      {
        // Получаем коллекцию контролов
        m_curentCollection = tab->GetPropertyControls();
        ShowControls();
        res = true;
      }
    }
  }

  return res;
}

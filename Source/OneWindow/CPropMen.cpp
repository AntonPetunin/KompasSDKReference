////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "OneWindow_pch.h"

#include "CPropMen.h"
#include "Resource.h"
#include "OneWindow.h"
#include "Events/OneWindowEvents.h"

extern ksapi::IApplication * application;
extern std::wstring handlerOwner;

//------------------------------------------------------------------------------
// Вывод сообщения
// ---
int LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum buttonsType, bool showModal)
{
  int res = 0;

  if (application && !str.empty())
    application->ShowMessageBox(str, LoadStr(IDR_LIBID), dlgType, buttonsType, showModal);

  return res;
}


//------------------------------------------------------------------------------
// Вывод сообщения
// ---
int LibMessage(int strId, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum buttonsType, bool showModal)
{
  return LibMessage(LoadStr(strId), dlgType, buttonsType, showModal);
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
  Диалог да/нет/отмена
*/
//---
int YesNoMessage(const std::wstring & message, bool question)
{
  int res = LibMessage(message, question ? ksMessageQuestion : ksMessageWarning, ksButtonSetYesNoCancel, false);
  return int(res == Positive);
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerObject::PropertyManagerObject()
  : m_procParam(nullptr)        // Параметры процесса
  , m_propTabs(nullptr)         // Коллекция закладок
  , m_curentCollection(nullptr) // Коллекция контролов
  , m_flagMode(0)
  , m_procPropMngEvent(0)
{
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
PropertyManagerObject::~PropertyManagerObject()
{
  m_curentCollection = nullptr; // Коллекция контролов
  m_propTabs = nullptr;         // Коллекция закладок
  m_procParam = nullptr;        // Параметры процесса
}


//------------------------------------------------------------------------------
/**
  Инициализация параметров процесса
*/
//---
bool PropertyManagerObject::InitProcessParam(std::int32_t toolBarID, SpecPropertyToolBarEnum toolBarType, std::int32_t firstTabID)
{
  bool res = false;

  if (application)
  {
    try
    {
      // Получаем параметры процессы
      m_procParam = application->CreateProcessParam();

      if (m_procParam)
      {
        m_events.Add(new PropertyManagerEvent(handlerOwner, *m_procParam, *this));

        // Получаем интерфейс панели свойств
        m_procParam->SetSpecToolbar(toolBarType);    // Тип кнопок на панели свойств
        m_procParam->SetCaption(LoadStr(toolBarID)); // Устанавливаем заголовок панели свойств
        m_procParam->SetAutoReduce(!m_flagMode);     // Автосоздание объекта

        if (m_flagMode)
          m_procParam->SetDefaultControlFix(ksAllFix);

        // Получаем коллекцию закладок панели свойств
        m_propTabs = m_procParam->GetPropertyTabs();
        // Создаем закладку параметров элемента
        if (firstTabID)
          CreateTab(firstTabID, true, true);

        res = true;
      }
    }
    catch (std::wstring mes)
    {
      LibMessage(mes, ksMessageError); // Вывод сообщений о ошибке
    }
  }
  return res;
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
      ksapi::IPropertyTabPtr propTab = m_propTabs->Add(LoadStr(tabID));

      if (propTab)
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
void PropertyManagerObject::InitPropertyControl(ksapi::IPropertyControl * control, std::int32_t ctrlID, bool enable,
                                                PropertyControlNameVisibility nameVisibility, bool visible)
{
  if (control)
  {
    control->SetName(LoadStr(ctrlID));
    control->SetId(ctrlID);
    control->SetEnable(enable);
    control->SetVisible(visible);
  }
}


//------------------------------------------------------------------------------
/**
  Создать едит вещественных значений
*/
//---
ksapi::IPropertyEditPtr PropertyManagerObject::CreateEditReal(double minVal, double maxVal)
{
  ksapi::IPropertyEditPtr realEdit;

  if (m_curentCollection)
  {
    realEdit = m_curentCollection->Add(ksControlEditReal);

    if (realEdit)
    {
      if (minVal != maxVal)
        realEdit->SetValueRange(minVal, maxVal);
    }
  }

  return realEdit;
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
  Добавление комбобокса строковых значений
*/
//---
ksapi::IPropertyListPtr PropertyManagerObject::CreateStringList(std::wstring matVal)
{
  ksapi::IPropertyListPtr realList;

  if (m_curentCollection)
  {
    if (realList = m_curentCollection->Add(ksControlListStr))
      realList->SetReadOnly(true);
  }

  return realList;
}


//------------------------------------------------------------------------------
/**
  Создать сепаратор
*/
//---
ksapi::IPropertySeparatorPtr PropertyManagerObject::CreateSeparator(SeparatorTypeEnum type)
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
ksapi::IPropertyCheckBoxPtr PropertyManagerObject::CreateCheckBox(bool checked)
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
  Создать набор кнопок
*/
//---
ksapi::IPropertyMultiButtonPtr PropertyManagerObject::CreateMultiButton(ButtonTypeEnum type)
{
  ksapi::IPropertyMultiButtonPtr buttons;

  if (m_curentCollection)
  {
    buttons = m_curentCollection->Add(ksControlMultiButton);

    if (buttons)
      buttons->SetButtonsType(type);
  }

  return buttons;
}


//------------------------------------------------------------------------------
/**
  Добавить кнопку в набор
*/
//---
void PropertyManagerObject::AddButton(const ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, bool cheched,
                                      bool enable)
{
  AddButton2(buttons, btnID, btnID, cheched, enable);
}


//------------------------------------------------------------------------------
/**
  Добавить кнопку в набор
*/
//---
void PropertyManagerObject::AddButton2(const ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, std::int32_t bmpID,
                                       bool cheched, bool enable)
{
  if (buttons)
  {
    buttons->AddButton(btnID, bmpID, -1);
    buttons->SetButtonEnable(btnID, enable);
    buttons->SetButtonChecked(btnID, cheched);
    buttons->SetButtonTips(btnID, LoadStr(btnID));
  }
}


//------------------------------------------------------------------------------
/**
  Добавить кнопку в набор
*/
//---
void PropertyManagerObject::EndProcess()
{
  m_events.RemoveWithUnadvise(m_procPropMngEvent);
  m_procPropMngEvent = 0;
  m_curentCollection = nullptr; // Коллекция контролов
  m_propTabs = nullptr;         // Коллекция закладок
  m_procParam = nullptr;        // Параметры процесса
}


//------------------------------------------------------------------------------
/**
  Найти контрол
*/
//---
ksapi::IPropertyControlPtr PropertyManagerObject::GetPropertyControl(int ctrlID)
{
  ksapi::IPropertyControlPtr control;

  if (m_propTabs)
  {
    for (std::int32_t i = 0, c = m_propTabs->GetCount(); i < c && !control; i++)
    {
      ksapi::IPropertyTabPtr tab(m_propTabs->GetItem(i));
      ksapi::IPropertyControlsPtr ctrls(tab ? tab->GetPropertyControls() : nullptr);

      if (ctrls)
        control = ctrls->GetItemByName(LoadStr(ctrlID));
    }
  }

  return control;
}


//------------------------------------------------------------------------------
/**
  Установить доступность контрола
*/
//---
void PropertyManagerObject::SetControlEnable(std::int32_t ctrlID, bool enabled)
{
  if (ksapi::IPropertyControlPtr control = GetPropertyControl(ctrlID))
    control->SetEnable(enabled);
}

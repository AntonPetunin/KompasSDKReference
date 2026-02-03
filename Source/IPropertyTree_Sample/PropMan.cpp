////////////////////////////////////////////////////////////////////////////////
//
// PropMen.cpp - Класс для работы с панелью свойств
//
////////////////////////////////////////////////////////////////////////////////
#include "PropMan.h"

#include "IPropertyTree_Sample.h"

#include <KsAPI.h>


//----------------------------------------------------------------------------------------------
// Конструктор
// ---
PropertyManagerObject::PropertyManagerObject()
  : m_procParam(nullptr)        // Параметры процесса
  , m_propTabs(nullptr)         // Коллекция закладок
  , m_curentCollection(nullptr) // Коллекция контролов
  , m_paramGrid(nullptr)        // Грид для вывода дополнительных параметров элемента
  , m_slideBox(nullptr)         // Окно просмотра
  , m_rowIndex(0)               // Индекс строки
  , m_posLiaderParam(nullptr)
  , m_flagMode(0)
{
}


//----------------------------------------------------------------------------------------------
// Деструктор
// ---
PropertyManagerObject::~PropertyManagerObject()
{
  m_paramGrid = nullptr;        // Грид для вывода дополнительных параметров элемента
  m_slideBox = nullptr;         // Окно просмотра
  m_curentCollection = nullptr; // Коллекция контролов
  m_propTabs = nullptr;         // Коллекция закладок
  m_procParam = nullptr;        // Параметры процесса
  m_posLiaderParam = nullptr;
  m_rowIndex = 0; // Индекс строки
}


//----------------------------------------------------------------------------------------------
// Инициализация параметров процесса
// ---
bool PropertyManagerObject::InitProcessParam()
{
  return false;
}


//----------------------------------------------------------------------------------------------
// Добавить параметр в грид
// ---
void PropertyManagerObject::AddStringToGrig(int32_t paramID, const std::wstring & value)
{
  if (m_paramGrid)
  {
    m_paramGrid->SetCellText(m_rowIndex, 1, value);
    m_paramGrid->SetCellText(m_rowIndex++, 0, ::LoadStr(paramID)); // Не переставлять так задумано
  }
}


//----------------------------------------------------------------------------------------------
// Добавить параметр в грид
// ---
void PropertyManagerObject::AddDoubleToGrig(int32_t paramID, double value)
{
  if (m_paramGrid)
  {
    m_paramGrid->SetCellText(m_rowIndex, 1, std::to_wstring(value)); // Выводим значение
    m_paramGrid->SetCellText(m_rowIndex++, 0, ::LoadStr(paramID));   // Имя параметра
  }
}


//----------------------------------------------------------------------------------------------
// Создать закладку
// ---
bool PropertyManagerObject::CreateTab(int32_t tabID, bool visible)
{
  bool res = false;

  if (tabID)
  {
    if (m_propTabs)
    {
      IPropertyTabPtr propTab = m_propTabs->Add(::LoadStr(tabID));
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


//----------------------------------------------------------------------------------------------
// Создать комбобокс
// ---
void PropertyManagerObject::InitPropertyControl(IPropertyControlPtr & control, int32_t ctrlID, bool enable, bool visible)
{
  if (control)
  {
    control->SetName(LoadStr(ctrlID));
    control->SetId(ctrlID);
    control->SetEnable(enable);
    control->SetVisible(visible);
  }
}


//----------------------------------------------------------------------------------------------
// Создать едит вещественных значений
// ---
IPropertyEditPtr PropertyManagerObject::CreateEditReal(double minVal, double maxVal)
{
  IPropertyEditPtr realEdit;

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


//----------------------------------------------------------------------------------------------
// Создать комбобокс
// ---
IPropertyListPtr PropertyManagerObject::CreateList(double minVal, double maxVal)
{
  IPropertyListPtr realList;

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


//----------------------------------------------------------------------------------------------
// Добавление комбобокса строковых значений
// ---
IPropertyListPtr PropertyManagerObject::CreateStringList(std::wstring matVal)
{
  IPropertyListPtr realList;

  if (m_curentCollection)
  {
    realList = m_curentCollection->Add(ksControlListStr);
    if (realList)
      realList->SetReadOnly(true);
  }

  return realList;
}


//----------------------------------------------------------------------------------------------
// Создать сепаратор
// ---
IPropertySeparatorPtr PropertyManagerObject::CreateSeparator()
{
  IPropertySeparatorPtr rSepar;

  if (m_curentCollection)
    rSepar = m_curentCollection->Add(ksControlSeparator);

  return rSepar;
}


//----------------------------------------------------------------------------------------------
// Добавление CheckBox-а
// ---
IPropertyCheckBoxPtr PropertyManagerObject::CreateCheckBox(bool checked)
{
  IPropertyCheckBoxPtr check;

  if (m_curentCollection)
  {
    check = m_curentCollection->Add(ksControlCheckBox);
    if (check)
      check->SetBoolValue(checked);
  }

  return check;
}


//----------------------------------------------------------------------------------------------
// Добавление SpinEdit /*Голованов Е*/
// ---
IPropertySpinEditPtr PropertyManagerObject::CreateSpinList(double minVal, double maxVal)
{
  IPropertySpinEditPtr realList;

  if (m_curentCollection)
  {
    realList = m_curentCollection->Add(ksControlSpinInt);
    if (realList)
    {
      if (minVal != maxVal)
        realList->SetValueRange(minVal, maxVal);
      realList->SetReadOnly(false);
    }
  }

  return realList;
}


//----------------------------------------------------------------------------------------------
// Создать набор кнопок
// ---
IPropertyMultiButtonPtr PropertyManagerObject::CreateMultiButton(ButtonTypeEnum type)
{
  IPropertyMultiButtonPtr buttons;

  if (m_curentCollection)
  {
    buttons = m_curentCollection->Add(ksControlMultiButton);
    if (buttons)
      buttons->SetButtonsType(type);
  }

  return buttons;
}


//----------------------------------------------------------------------------------------------
// Добавить кнопку в набор
// ---
void PropertyManagerObject::AddButton(IPropertyMultiButtonPtr & buttons, int32_t btnID, bool cheched, bool enable)
{
  AddButton2(buttons, btnID, btnID, cheched, enable);
}


//----------------------------------------------------------------------------------------------
// Добавить кнопку в набор
// ---
void PropertyManagerObject::AddButton2(IPropertyMultiButtonPtr & buttons, int32_t btnID, int32_t bmpID, bool checked, bool enable)
{
  if (buttons)
  {
    buttons->AddButton(btnID, bmpID, -1);
    buttons->SetButtonEnable(btnID, enable);
    buttons->SetButtonChecked(btnID, checked);
    buttons->SetButtonTips(btnID, LoadStr(btnID));
  }
}


//----------------------------------------------------------------------------------------------
// Добавить кнопку в набор
// ---
void PropertyManagerObject::EndProcess()
{
  BaseEvent::TerminateEvents();
}


//----------------------------------------------------------------------------------------------
// Найти контрол
// ---
IPropertyControlPtr PropertyManagerObject::GetPropertyControl(int32_t ctrlID)
{
  IPropertyControlPtr control;

  if (m_propTabs)
    for (int32_t i = 0, c = m_propTabs->GetCount(); i < c && !control; i++)
    {
      IPropertyTabPtr tab(m_propTabs->GetItem(i));
      IPropertyControlsPtr ctrls(tab ? tab->GetPropertyControls() : nullptr);

      if (ctrls)
        control = ctrls->GetItemByName(::LoadStr(ctrlID));
    }

  return control;
}


//----------------------------------------------------------------------------------------------
// Установить доступность контрола
// ---
void PropertyManagerObject::SetControlEnable(int32_t ctrlID, bool enabled)
{
  IPropertyControlPtr control(GetPropertyControl(ctrlID));
  if (control)
    control->SetEnable(enabled);
}


////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerEvent - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
PropertyManagerEvent::PropertyManagerEvent(const IPropertyManagerPtr & manager, PropertyManagerObject & obj)
  : BaseEvent(manager)
  , m_obj(obj)
{
  Advise();
}


//-------------------------------------------------------------------------------
//
// ---
PropertyManagerEvent::~PropertyManagerEvent()
{
  Unadvise();
}


//-------------------------------------------------------------------------------
// Подписаться на события.
// ---
void PropertyManagerEvent::Advise()
{
  if (IPropertyManagerPtr manager = m_container)
  {
    std::wstring handlerOwner = L"IPropertyTree_Sample";
    IPropertiesManagerEventsPtr events = manager->Events();

    if (!events)
      return;

    events->AddButtonClickHandler(handlerOwner, [this](int32_t buttonId) { return this->ButtonClick(buttonId); });
    events->AddControlCommandHandler(handlerOwner, [this](const IPropertyControlPtr & control, int32_t buttonId)
                                     { this->ControlCommand(control, buttonId); });
    events->AddButtonUpdateHandler(handlerOwner, [this](int32_t buttonId, bool & enable, int & checked)
                                   { return this->ButtonUpdate(buttonId, enable, checked); });
    events->AddProcessActivateHandler(handlerOwner, [this]() { return this->ProcessActivate(); });
    events->AddProcessDeactivateHandler(handlerOwner, [this]() { return this->ProcessDeactivate(); });
    events->AddUserMenuCommandHandler(handlerOwner, [this](const IPropertyControlPtr & control, int32_t menuId)
                                      { this->UserMenuCommand(control, menuId); });
  }
}


//-------------------------------------------------------------------------------
// Отписаться от событий.
// ---
void PropertyManagerEvent::Unadvise()
{
  if (IPropertyManagerPtr manager = m_container)
    manager->Events()->RemoveAllHandlers(L"IPropertyTree_Sample");
}


//-----------------------------------------------------------------------------
// Нажатие кнопки.
// ---
bool PropertyManagerEvent::ButtonClick(int32_t buttonId)
{
  return m_obj.OnButtonClick(buttonId);
}


//-----------------------------------------------------------------------------
// Нажатие кнопки контрола.
// ---
void PropertyManagerEvent::ControlCommand(const IPropertyControlPtr & control, int32_t buttonId)
{
  m_obj.OnButtonClick(buttonId);
}


//-----------------------------------------------------------------------------
// Установка состояния кнопки спецпанели.
// ---
bool PropertyManagerEvent::ButtonUpdate(int32_t buttonId, bool & enable, int & checked)
{
  return true;
}


//-----------------------------------------------------------------------------
// Начало процесса.
// ---
bool PropertyManagerEvent::ProcessActivate()
{
  return true;
}


//-----------------------------------------------------------------------------
// Завершение процесса.
// ---
bool PropertyManagerEvent::ProcessDeactivate()
{
  return true;
}


//-----------------------------------------------------------------------------
// Нажатие кнопки пользовательского меню.
// ---
void PropertyManagerEvent::UserMenuCommand(const IPropertyControlPtr & control, int32_t menuId)
{
  if (control)
  {
    int32_t ctrlId = control->GetId();
    m_obj.OnControlCommand(ctrlId, menuId);
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// CPropMen.cpp - Класс для работы с панелью свойств
//
////////////////////////////////////////////////////////////////////////////////

#include "CPropMen.h"

#include "MacroObj_pch.h"
#include <Events/PropertyManagerEvent.h>

extern ksapi::IApplication * ksAppl;
std::wstring handlerOwner = L"MacroObj_Lib";

//------------------------------------------------------------------------------
// Вывод сообщения
// ---
std::int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  if (!str.empty() && ksAppl)
    // текст сообщения, заголовок, параметры
    return ksAppl->ShowMessageBox(str, LoadStr(IDR_LIBID), dlgType, dlgButtons, showModal);

  return 0;
}


//------------------------------------------------------------------------------
// Вывод сообщения
// ---
std::int32_t LibMessage(std::int32_t strId, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  return ::LibMessage(LoadStr(strId), dlgType, dlgButtons, showModal);
}


//----------------------------------------------------------------------------------------------
// Конструктор
// ---
PropertyManagerObject::PropertyManagerObject()
  : m_rowIndex(0) // Индекс строки
  , m_flagMode(0)
  , m_objectIndex(0)
{
}

//----------------------------------------------------------------------------------------------
// Деструктор
// ---
PropertyManagerObject::~PropertyManagerObject()
{
  m_paramGrid = nullptr;        // Грид для вывода дополнительных параметров элемента
  m_curentCollection = nullptr; // Коллекция контролов
  m_propTabs = nullptr;         // Коллекция закладок
  m_procParam = nullptr;        // Параметры процесса
  m_rowIndex = 0;               // Индекс строки
}

//----------------------------------------------------------------------------------------------
// Установить параметры Сontrol'а
// ---
void PropertyManagerObject::InitPropertyControl(const ksapi::IPropertyControlPtr & control, std::int32_t ctrlID, bool enable,
                                                bool visible)
{
  if (control)
  {
    std::wstring tmpBstr(LoadStr(ctrlID));
    control->SetName(tmpBstr);
    control->SetId(ctrlID);
    control->SetEnable(enable);
    control->SetVisible(visible);
  }
}

//-------------------------------------------------------------------------------
// Создать Сontrol заданного типа
// ---
ksapi::IPropertyControlPtr PropertyManagerObject::CreateControl(ControlTypeEnum conType)
{
  return m_curentCollection ? m_curentCollection->Add(conType) : nullptr;
}


ksapi::IPropertyControlPtr PropertyManagerObject::GetPropertyControl(std::int32_t ctrlID)
{
  ksapi::IPropertyControlPtr control;

  if (ksapi::IPropertyTabsPtr tabs = m_procParam ? m_procParam->GetPropertyTabs() : nullptr)
  {
    for (std::int32_t i = 0, c = tabs->GetCount(); i < c && !control; i++)
    {
      ksapi::IPropertyTabPtr tab(tabs->GetItem(i));
      ksapi::IPropertyControlsPtr ctrls(tab ? tab->GetPropertyControls() : nullptr);

      if (ctrls)
      {
        std::wstring name(::LoadStr(ctrlID));
        control = ctrls->GetItemByName(name);
      }
    }
  }

  return control;
}


//----------------------------------------------------------------------------------------------
// Добавить параметр в грид
// ---
void PropertyManagerObject::AddStringToGrig(std::int32_t paramID, const std::wstring & value)
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
void PropertyManagerObject::AddDoubleToGrig(std::int32_t paramID, double value)
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
bool PropertyManagerObject::CreateTab(std::int32_t tabID, bool visible, bool active)
{
  bool res = false;

  if (tabID)
  {
    if (m_propTabs)
    {
      std::wstring tmpBstr(::LoadStr(tabID));
      ksapi::IPropertyTabPtr propTab = m_propTabs->Add(tmpBstr);

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


//----------------------------------------------------------------------------------------------
// Создать едит вещественных значений
// ---
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


//----------------------------------------------------------------------------------------------
// Добавление SpinEdit /*Голованов Е*/
// ---
ksapi::IPropertySpinEditPtr PropertyManagerObject::CreateSpinList(double minVal, double maxVal)
{
  ksapi::IPropertySpinEditPtr realList;

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
// Добавление CheckBox-а
// ---
ksapi::IPropertyCheckBoxPtr PropertyManagerObject::CreateCheckBox(bool checked)
{
  ksapi::IPropertyCheckBoxPtr check(nullptr);

  if (m_curentCollection)
  {
    check = m_curentCollection->Add(ksControlCheckBox);

    if (check)
      check->SetBoolValue(checked);
  }

  return check;
}


//----------------------------------------------------------------------------------------------
// Установить доступность контрола
// ---
void PropertyManagerObject::SetControlEnable(std::int32_t ctrlID, bool enabled)
{
  if (ksapi::IPropertyControlPtr control = GetPropertyControl(ctrlID))
    control->SetEnable(enabled);
}

//------------------------------------------------------------------------------
// Инициализация параметров процесса
// ---
bool PropertyManagerObject::InitProcessParam()
{
  bool res = true;

  if (!m_procParam)
  {
    res = false;

    // Создаем панель свойств
    m_procParam = ksAppl ? ksAppl->CreateProcessParam() : nullptr;

    if (m_procParam)
    {
      m_procParam->SetAutoReduce(false);
      m_procParam->SetCaption(LoadStr(IDS_CAPTION));
      m_procParam->SetSpecToolbar(pnEnterEscHelp);
      m_procParam->SetEnableUndoRedo(true);
      // Подписываемся на события панели свойств
      new PropertyManagerEvent(m_procParam, *this);

      if (ksapi::IPropertyTabsPtr tabs = m_procParam->GetPropertyTabs())
      {
        //Добавляем закладку
        if (ksapi::IPropertyTabPtr tab = tabs->Add(LoadStr(IDS_TAB_NAME)))
        {
          // Получаем коллекцию контролов
          m_curentCollection = tab->GetPropertyControls();
          ShowControls();
          res = true;
        }
      }
    }
  }

  return res;
}

// Завершить процесс редактирования
void PropertyManagerObject::EndProcess()
{
  // Отписываемся от событий
  if (!m_procParam)
    return;

  if (ksapi::IPropertiesManagerEventsPtr events = m_procParam->Events())
    events->RemoveAllHandlers(handlerOwner);
}

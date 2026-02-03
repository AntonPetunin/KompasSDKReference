/////////////////////////////////////////////////////////////////////
//
// PropMen.cpp - Класс для работы с панелью свойств
//
////////////////////////////////////////////////////////////////////////////////
#include <PropertyManager/PropertyManager.h>

#include <UserSymbolWithLeader_pch.h>

using namespace ksapi;

extern IApplication * kompasApp;                       // Интерфейс Приложения Компас
extern std::wstring LoadResourceString(int32_t strId); // Загрузить строку из файла ресурсов.


//----------------------------------------------------------------------------------------------
/**
  Конструктор.
*/
// ---
PropertyManagerObject::PropertyManagerObject()
  : m_rowIndex(0)
  , m_doc(kompasApp ? kompasApp->GetActiveDocument() : nullptr)
{
}

//----------------------------------------------------------------------------------------------
/**
  Инициализация параметров процесса.
*/
// ---
bool PropertyManagerObject::InitProcessParam()
{
  // Получаем параметры процессы.
  m_procParam = kompasApp->CreateProcessParam();
  if (!m_procParam)
    return false;

  // Подписываемся на события процесса.
  new PropertyManagerEvent(m_procParam, *this);
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerEvent - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------
/**
  Конструктор с параметрами процесса.
*/
// ---
PropertyManagerEvent::PropertyManagerEvent(const IProcessParamPtr & manager, PropertyManagerObject & obj)
  : ABaseEvent(manager)
  , m_obj(obj)
{
  Advise();
}

//----------------------------------------------------------------------------------------------
/**
  Конструктор с панелью свойств.
*/
// ---
PropertyManagerEvent::PropertyManagerEvent(const IPropertyManagerPtr & manager, PropertyManagerObject & obj)
  : ABaseEvent(manager)
  , m_obj(obj)
{
  Advise();
}

//----------------------------------------------------------------------------------------------
/**
  Деструктор.
*/
// ---
PropertyManagerEvent::~PropertyManagerEvent()
{
  delete &m_obj;
}


//-----------------------------------------------------------------------------
// prButtonClick - Нажатие кнопки.
// ---
bool PropertyManagerEvent::ButtonClick(int32_t buttonId)
{
  return m_obj.OnButtonClick(buttonId);
}


//-----------------------------------------------------------------------------
// prChangeControlValue - Событие изменения значения контрола.
// ---
void PropertyManagerEvent::ChangeControlValue(const IPropertyControlPtr & control)
{
  m_obj.OnChangeControlValue(control);
}


//-----------------------------------------------------------------------------
// prControlCommand - Нажатие кнопки контрола.
// ---
void PropertyManagerEvent::ControlCommand(const IPropertyControlPtr & control, int32_t buttonId)
{
}


//-----------------------------------------------------------------------------
// prButtonUpdate - Обновление состояния кнопки.
// ---
bool PropertyManagerEvent::ButtonUpdate(int32_t buttonId, bool & enable, int & checked)
{
  return m_obj.OnButtonUpdate(buttonId, enable, checked);
}


//-----------------------------------------------------------------------------
// prProcessActivate - Активизация процесса.
// ---
void PropertyManagerEvent::ProcessActivate()
{
  m_obj.ProcessActivate();
}


//-----------------------------------------------------------------------------
// prProcessDeactivate - Деактивизация процесса.
// ---
void PropertyManagerEvent::ProcessDeactivate()
{
}


//-----------------------------------------------------------------------------
//
// ---
void PropertyManagerEvent::EditFocus(const IPropertyControlPtr & control, bool setFocus)
{
  m_obj.OnEditFocus(control, setFocus);
}


//-----------------------------------------------------------------------------
//
// ---
void PropertyManagerEvent::SelectItem(const IPropertyControlPtr & control, int32_t index, bool select)
{
  m_obj.OnSelectItem(control, index, select);
}


//-----------------------------------------------------------------------------
//
// ---
void PropertyManagerEvent::UserMenuCommand(const IPropertyControlPtr & control, int32_t menuId)
{
  m_obj.OnEditFocus(control, true);
}


//-----------------------------------------------------------------------------
// CLLBACK для получения типа контекстного меню
// ---
ksProcessContextMenuType PropertyManagerEvent::GetContextMenuType(int32_t screenX, int32_t screenY)
{
  return m_obj.GetContextMenuType(screenX, screenY);
}


// "CLLBACK для накачки контекстной панели
void PropertyManagerEvent::FillContextPanel(const IProcessContextPanelPtr & contextPanel)
{
  m_obj.FillContextPanel(contextPanel);
}


//-----------------------------------------------------------------------------
//
// ---
void PropertyManagerEvent::DoubleClick(const IPropertyControlPtr & control, int32_t itemIndex)
{
  m_obj.OnProcItemDblClick(control, itemIndex);
}


void PropertyManagerEvent::CheckItem(const IPropertyControlPtr & control, int32_t index, bool check)
{
  m_obj.OnCheckItem(control, index, check);
}

//-------------------------------------------------------------------------------
// Подписаться на события
// ---
void PropertyManagerEvent::Advise()
{
  IPropertyManagerPtr propManager = m_container;
  IProcessParamPtr procParam = m_container;

  if (propManager || procParam)
  {
    IPropertiesManagerEventsPtr events = propManager ? propManager->Events() : procParam->Events();

    if (!events)
      return;

    events->AddButtonClickHandler(klibName, [this](int32_t buttonId) { return ButtonClick(buttonId); });
    events->AddChangeControlValueHandler(klibName, [this](const IPropertyControlPtr & control) { ChangeControlValue(control); });
    events->AddControlCommandHandler(klibName, [this](const IPropertyControlPtr & control, int32_t buttonId)
                                     { ControlCommand(control, buttonId); });
    events->AddButtonUpdateHandler(klibName, [this](int32_t buttonId, bool & enable, int & checked)
                                   { return ButtonUpdate(buttonId, enable, checked); });
    events->AddProcessActivateHandler(klibName, [this]() { ProcessActivate(); });
    events->AddProcessDeactivateHandler(klibName, [this]() { ProcessDeactivate(); });
    events->AddEditFocusHandler(klibName,
                                [this](const IPropertyControlPtr & control, bool setFocus) { EditFocus(control, setFocus); });
    events->AddSelectItemHandler(klibName, [this](const IPropertyControlPtr & control, int32_t index, bool select)
                                 { SelectItem(control, index, select); });
    events->AddUserMenuCommandHandler(klibName, [this](const IPropertyControlPtr & control, int32_t menuId)
                                      { UserMenuCommand(control, menuId); });
    events->AddGetContextMenuTypeHandler(klibName, [this](int32_t screenX, int32_t screenY)
                                         { return GetContextMenuType(screenX, screenY); });
    events->AddFillContextPanelHandler(klibName,
                                       [this](const IProcessContextPanelPtr & contextPanel) { FillContextPanel(contextPanel); });
    events->AddDoubleClickItemHandler(klibName, [this](const IPropertyControlPtr & control, int32_t itemIndex)
                                      { DoubleClick(control, itemIndex); });
    events->AddCheckItemHandler(klibName, [this](const IPropertyControlPtr & control, int32_t index, bool check)
                                { CheckItem(control, index, check); });
  }
}

//-------------------------------------------------------------------------------
// Отписаться от событий
// ---
void PropertyManagerEvent::Unadvise()
{
  IPropertyManagerPtr propManager = m_container;
  IProcessParamPtr procParam = m_container;

  if (propManager || procParam)
  {
    if (IPropertiesManagerEventsPtr events = propManager ? propManager->Events() : procParam->Events())
      events->RemoveAllHandlers(klibName);
  }
}

////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Реализация класса обработчика событий от менеджера панели свойств
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "PropertyManagerEvent.h"
#include <MacroObj_pch.h>
#include <PropManager/CPropMen.h>

extern ksapi::IApplication * ksAppl;
extern std::wstring handlerOwner;


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerEvent::PropertyManagerEvent(ksapi::IProcessParamPtr & processParam, PropertyManagerObject & obj)
  : m_processParam(processParam)
  , m_obj(obj)
{
  AdviseEvents();
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
PropertyManagerEvent::~PropertyManagerEvent()
{
}


//------------------------------------------------------------------------------
/**
  Выполнить команду библиотеки по id-команды
*/
//---
void ExecuteLibCommand(std::int32_t command)
{
  ksapi::ILibraryManagerPtr manager(ksAppl ? ksAppl->GetLibraryManager() : nullptr);
  ksapi::IProceduresLibrariesPtr coll(manager ? manager->GetProceduresLibraries() : nullptr);

  if (ksapi::IProceduresLibraryPtr lib = coll ? coll->GetItemByName(::LoadStr(IDR_LIBID)) : nullptr)
    lib->Execute(command, true /*post*/, nullptr);
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки спецпанели
*/
//---
bool PropertyManagerEvent::ButtonClick(std::int32_t buttonID)
{
  return m_obj.ButtonClick(buttonID);
}


//------------------------------------------------------------------------------
/**
  Изменение значения контрола
*/
//---
bool PropertyManagerEvent::ChangeControlValue(const ksapi::IPropertyControlPtr & ctrl)
{
  ksapi::IPropertySpinEditPtr realEdit = ctrl;
  m_obj.OnChangeControlValue(ctrl->GetId(), realEdit->GetDoubleValue());
  return true;
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки контрола
*/
//---
bool PropertyManagerEvent::ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID)
{
  return m_obj.ControlCommand(ctrl, buttonID);
}


//------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели
*/
//---
bool PropertyManagerEvent::ButtonUpdate(std::int32_t buttonId, bool & enable, std::int32_t & checked)
{
  return m_obj.ButtonUpdate(buttonId, checked, enable);
}


//------------------------------------------------------------------------------
/**
  CALLBACK для накачки контекстной панели
*/
//---
bool PropertyManagerEvent::FillContextPanel(const ksapi::IProcessContextPanelPtr & contextPanel)
{
  return m_obj.FillContextPanel(contextPanel);
}


//------------------------------------------------------------------------------
/**
  CALLBACK для получения типа контекстного меню
*/
//---
ksProcessContextMenuType PropertyManagerEvent::GetContextMenuType(std::int32_t screenX, std::int32_t screenY)
{
  return m_obj.GetContextMenuType(screenX, screenY);
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void PropertyManagerEvent::AdviseEvents()
{
  if (!m_processParam)
    return;

  ksapi::IPropertiesManagerEventsPtr events = m_processParam->Events();

  if (!events)
    return;

  events->AddButtonClickHandler(handlerOwner, [this](std::int32_t buttonID) { return ButtonClick(buttonID); });
  events->AddChangeControlValueHandler(handlerOwner,
                                       [this](const ksapi::IPropertyControlPtr & ctrl) { return ChangeControlValue(ctrl); });
  events->AddControlCommandHandler(handlerOwner, [this](const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID)
                                   { return ControlCommand(ctrl, buttonID); });
  events->AddButtonUpdateHandler(handlerOwner, [this](std::int32_t buttonId, bool & enable, std::int32_t & checked)
                                 { return ButtonUpdate(buttonId, enable, checked); });
  events->AddFillContextPanelHandler(handlerOwner, [this](const ksapi::IProcessContextPanelPtr & contextPanel)
                                     { FillContextPanel(contextPanel); });
  events->AddGetContextMenuTypeHandler(handlerOwner, [this](std::int32_t screenX, std::int32_t screenY)
                                       { return GetContextMenuType(screenX, screenY); });
  //events->AddCommandHelpHandler(handlerOwner, BIND_MY_METHOD(PropertyManagerEvent::CommandHelp));
}

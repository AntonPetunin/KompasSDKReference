////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "PropertyManagerEvent.h"
#include "Step4_pch.h"
#include "../CPropMen.h"

std::wstring handlerOwner = L"ste4_lib";

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerEvent::PropertyManagerEvent(ksapi::IProcessParamPtr & processParam, PropertyManagerObject & obj)
  : m_obj(obj)
  , m_processParam(processParam)
{
  AdviseEvents();
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void PropertyManagerEvent::AdviseEvents()
{
  if (m_processParam)
  {
    ksapi::IPropertiesManagerEventsPtr events = m_processParam->Events();

    if (!events)
      return;

    events->AddButtonUpdateHandler(handlerOwner, [this](int32_t buttonId, bool & enable, std::int32_t & checked)
                                   { return ButtonUpdate(buttonId, enable, checked); });
    events->AddButtonClickHandler(handlerOwner, [this](std::int32_t buttonID) { return ButtonClick(buttonID); });
  }
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки
*/
//---
bool PropertyManagerEvent::ButtonUpdate(int32_t buttonId, bool & enable, std::int32_t & checked)
{
  return m_obj.ButtonUpdate(buttonId, checked, enable);
}


//------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели
*/
//---
bool PropertyManagerEvent::ButtonClick(std::int32_t buttonID)
{
  return m_obj.ButtonClick(buttonID);
}

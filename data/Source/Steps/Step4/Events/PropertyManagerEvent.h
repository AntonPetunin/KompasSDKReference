////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <KsAPI.h>

class PropertyManagerObject;

////////////////////////////////////////////////////////////////////////////////
//
/// Обработчик событий от менеджера панели свойств
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerEvent
{
protected:
  PropertyManagerObject & m_obj; ///< Указатель на подписанный объект

private:
  ksapi::IProcessParamPtr & m_processParam; ///< Параметры процесса на которые подписываемся

public:
  /// Конструктор
  PropertyManagerEvent(ksapi::IProcessParamPtr & process, PropertyManagerObject & obj);

  /// Подписаться на события
  void AdviseEvents();

  /// Нажатие кнопки
  bool ButtonClick(std::int32_t buttonID);
  /// Установка состояния кнопки спецпанели
  bool ButtonUpdate(std::int32_t buttonId, bool & enable, std::int32_t & checked);
};

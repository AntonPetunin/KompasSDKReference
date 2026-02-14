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
/// Обработчик событий от процесса 3D
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Process2DEvent
{
protected:
  PropertyManagerObject & m_obj; ///< Указатель на подписанный объект

private:
  ksapi::IProcess2DPtr & m_process; ///<Процесс на который подписываемся

public:
  /// Конструктор
  Process2DEvent(ksapi::IProcess2DPtr & process, PropertyManagerObject & obj, bool copyProc = false);

  /// Подписаться на события
  void AdviseEvents();

  /// Выполнить команду меню.
  bool ExecuteCommand(std::int32_t command);
  /// Изменено положения объекта
  bool PlacementChanged(double x, double y, double angle, bool dynamic);
  /// Отписаться от событий
  void UnadviseEvents();
  /// Окончание процесса.
  bool EndProcess();
};

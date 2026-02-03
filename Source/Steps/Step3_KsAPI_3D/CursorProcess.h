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

using PlacementChangedCall = std::function<bool(const ksapi::IKompasAPIObjectPtr & object)>;

////////////////////////////////////////////////////////////////////////////////
//
/// Процесс курсора
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class CursorProcess
{
private:
  ksapi::IProcessPtr m_process;
  PlacementChangedCall m_placementChanged; ///< Функция события PlacementChange.

public:
  CursorProcess() = default;
  ~CursorProcess() = default;

  /// Получить процесс.
  ksapi::IProcessPtr GetProcess();
  /// Запустить процесс выполняющий функцию.
  void Execute(PlacementChangedCall placementChanged);

private:
  /// Инициализировать процесс.
  void InitProcess();
  /// Подписаться на события.
  void Advice();
  /// Обработчик события "Измененилось положение".
  bool PlacementChanged(const ksapi::IKompasAPIObjectPtr & object);
  /// Обработчик события "Окончание процесс".
  bool EndProcess();
};

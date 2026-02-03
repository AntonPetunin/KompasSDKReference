////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

using PlacementChangedCall = std::function<void(double x, double y)>;

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
  ksapi::IProcessPtr m_process;            ///< Процесс
  PlacementChangedCall m_placementChanged; /// Функция события PlacementChange

public:
  /// Конструктор
  CursorProcess() = default;
  /// Деструктор
  ~CursorProcess() = default;

  /// Получить объект процесса
  ksapi::IProcessPtr & Get();
  /// Запустить процесс выполняющий функцию
  void Execute(PlacementChangedCall placementChanged);

private:
  /// Инициализировать процесс
  void InitProcess();
  /// Подписаться на события
  void Advice();
  /// Обработчик события "Измененилось положение"
  bool PlacementChanged(double x, double y, double angle, bool dynamic);
  /// Обработчик события "Окончание процесс"
  bool EndProcess();
};

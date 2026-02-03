////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Объявление класса обработчика событий от процесса 3D
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class PropertyManagerObject;

////////////////////////////////////////////////////////////////////////////////
//
/// Обработчик событий от процесса 3D
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Process3DEvent
{
private:
  PropertyManagerObject & m_obj;    ///< Указатель на подписанный объект
  ksapi::IProcess3DPtr & m_process; ///< Процесс 3D

public:
  /// Конструктор
  Process3DEvent(ksapi::IProcess3DPtr & process, PropertyManagerObject & obj, bool copyProc = false);
  /// Деструктор
  virtual ~Process3DEvent();

private:
  /// Выполнение команды
  bool ExecuteCommand(std::int32_t command);
  /// Фильтрация объетов
  bool FilterObjects(const ksapi::IKompasAPIObjectPtr & object);
  /// Измененилось положение
  bool PlacementChanged(const ksapi::IKompasAPIObjectPtr & object);
  /// Окончание процесса
  bool EndProcess();
  /// Запуск процесса
  bool Run();

private:
  /// Подписаться на события
  void AdviseEvents();
  /// Отписаться от событий
  void UnadviseEvents();
};

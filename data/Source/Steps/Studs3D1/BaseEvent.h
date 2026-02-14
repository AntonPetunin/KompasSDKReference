///////////////////////////////////////////////////////////////////////////////
//
// EventsList - Список подписчиков
// BaseEvent - Базовый клас для обработчиков событий в автоматизации
//
////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <KsAPI_fwd.h>

using namespace ksapi;

/////////////////////////////////////////////////////////////////////////////
//
// Базовый клас для автаматизационных обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
class BaseEvent
{
protected:
  static std::vector<BaseEvent *> m_eventList; // Список обработчиков событий
  IAPIObjectPtr m_container;                   // Источник событий

public:
  // Конструктор
  BaseEvent(const IAPIObjectPtr & container);
  // Диструктор
  virtual ~BaseEvent();

  static void TerminateEvents(void); // Отписать все события
  void RemoveThis();                 // Удалить себя из списка
  void Disconnect();                 // Рассоединиться
  void Clear();
  virtual void Unadvise() = 0; // Отписаться от событий

  static void TerminateEvents(const IAPIObjectPtr & container);
};

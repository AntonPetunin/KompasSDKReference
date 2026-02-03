/////////////////////////////////////////////////////////////////////////////
//
// ABaseEvent  - Базовый класс для обработчиков событий в автоматизации
//
////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

#include <KsAPI_fwd.h>

using namespace ksapi;

/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для автоматизационных обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
class ABaseEvent
{
protected:
  static std::vector<ABaseEvent *> m_eventList; ///< Список обработчиков событий
  IAPIObjectPtr m_container;                    ///< Источник событий
  IKompasDocumentPtr m_doc;                     ///< Документ
  int32_t m_objType;                            ///< Тип объекта
  IAPIObjectPtr m_obj3D;

public:
  // Конструктор
  ABaseEvent(const IAPIObjectPtr & object, const IKompasDocumentPtr & doc = nullptr, int32_t objType = -1,
             const IAPIObjectPtr & obj3D = nullptr);

  // Деструктор
  virtual ~ABaseEvent();

  void RemoveThis(); ///< Удалить себя из списка
  void Disconnect(); ///< Рассоединиться
  void Clear();
  virtual void Unadvise() = 0; ///< Отписаться от событий

  static void TerminateEvents(); ///< Отписать все события
};

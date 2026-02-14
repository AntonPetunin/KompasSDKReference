/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для обработчиков событий в автоматизации
//
/////////////////////////////////////////////////////////////////////////////
#include <Events/ABaseEvent.h>

#include <UserSymbolWithLeader_pch.h>


//-------------------------------------------------------------------------------
// Список подписчиков
// ---
std::vector<ABaseEvent *> ABaseEvent::m_eventList;


/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для автоматизационных обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
// Конструктор
// ---
ABaseEvent::ABaseEvent(const IAPIObjectPtr & object, const IKompasDocumentPtr & doc, int32_t objType, const IAPIObjectPtr & obj3D)
  : m_container(object) // Источник событий
  , m_doc(doc)          // Документ
  , m_objType(objType)  // Тип объекта
  , m_obj3D(obj3D)      // Объект для 3D
{
  m_eventList.push_back(this);
}


//-------------------------------------------------------------------------------
// Деструктор
// ---
ABaseEvent::~ABaseEvent()
{
  RemoveThis();
  m_container = nullptr;
  m_doc = nullptr;
  m_obj3D = nullptr;
}


//-------------------------------------------------------------------------------
// Удалить себя из списка
// ---
void ABaseEvent::RemoveThis()
{
  // Удалим себя из списка обработчиков событий
  auto pos = std::find(m_eventList.begin(), m_eventList.end(), this);
  if (pos != m_eventList.end())
  {
    m_eventList.erase(pos);
  }
}


//-------------------------------------------------------------------------------
//
// ---
void ABaseEvent::Clear()
{
  m_container = nullptr;
  m_doc = nullptr;
  m_obj3D = nullptr;
}


//-------------------------------------------------------------------------------
// Рассоединиться
// ---
void ABaseEvent::Disconnect()
{
  Unadvise();
  Clear();
}


//-----------------------------------------------------------------------------
// Отписать все события
// ---
void ABaseEvent::TerminateEvents()
{
  for (ABaseEvent * ev : m_eventList)
  {
    ev->Disconnect();
    delete ev;
  }
  m_eventList.clear();
}

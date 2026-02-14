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
#include <memory>

class PropertyManagerObject;
struct IOneWindowEvent;
class PropertyTreeEventCallbacks;
class DocumentFrameEventCallback;
class Document3DEvent;
class DocumentFrameEvent;
class PropertyTreeEvent;
class PropertyManagerEvent;
class LibObj;
class Process3DEvent;

////////////////////////////////////////////////////////////////////////////////
//
/// Событие библиотеки.
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct IOneWindowEvent
{
  virtual ~IOneWindowEvent() = default;
  /// Подписаться на события
  virtual void Advise() = 0;
  /// Отписаться от событий
  virtual void Unadvise() = 0;
  /// Получить hash события
  virtual size_t GetHash() = 0;
  /// Получить объект события
  virtual ksapi::IAPIObjectPtr GetEventObject() = 0;
};


////////////////////////////////////////////////////////////////////////////////
//
/// Индентификатор документа
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class DocumentIdentifier
{
private:
  size_t m_documentHansh; ///< Хэш документа

public:
  /// Конструктор с инициализацией hash
  DocumentIdentifier();
  /// Получить hash документа
  size_t GetDocumentHash() { return m_documentHansh; }
};


////////////////////////////////////////////////////////////////////////////////
//
/// Коллекция событий.
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class OWEventCollection
{
private:
  std::unordered_map<size_t, std::shared_ptr<IOneWindowEvent>> m_events; ///< Контейнер событий

public:
  /// Добавить событие в контейнер событий
  size_t Add(IOneWindowEvent * additionEvent);
  ///
  IOneWindowEvent * GetEvent(size_t eventHash);
  /// Удалить событие из контейнера и отписаться от него
  void RemoveWithUnadvise(size_t eventHash);
  /// Очистить контейнер событий
  void Clear();
};


////////////////////////////////////////////////////////////////////////////////
//
/// События менеджера свойств
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerEvent : public IOneWindowEvent,
                             public DocumentIdentifier
{
private:
  ksapi::IPropertyManagerPtr m_propMng; ///< Менеджер свойств
  ksapi::IProcessParamPtr m_procParam;  ///< Параметры процесса
  PropertyManagerObject * m_obj;        ///< Указатель на подписанный объект
  std::wstring m_subName;               ///< Имя подписчика

public:
  /// Конструктор
  PropertyManagerEvent(const std::wstring & subName, ksapi::IPropertyManager & manager, PropertyManagerObject & obj);
  /// Конструктор
  PropertyManagerEvent(const std::wstring & subName, ksapi::IProcessParam & procParam, PropertyManagerObject & obj);
  /// Деструктор
  ~PropertyManagerEvent();

#pragma region
  /// Подписаться на события
  virtual void Advise() override;
  /// Отписаться от событий
  virtual void Unadvise() override;
  /// Получить hash события
  virtual size_t GetHash() override;
  /// Получить объект события
  ksapi::IAPIObjectPtr GetEventObject() override;
#pragma endregion

public:
  /// Нажатие кнопки.
  bool ButtonClick(std::int32_t buttonID);
  /// Нажатие кнопки контрола
  bool ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID);
  /// Установка состояния кнопки спецпанели.
  bool ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable);
  /// Установка состояния кнопки спецпанели.
  bool CommandHelp(std::int32_t buttonID);
};


////////////////////////////////////////////////////////////////////////////////
//
/// События дерева
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyTreeEvent : public IOneWindowEvent,
                          public DocumentIdentifier
{
private:
  ksapi::IPropertyTreePtr m_tree;     /// Дерево
  PropertyTreeEventCallbacks * m_obj; ///< Объект для вызова обработчиков
  std::wstring m_subName;             ///< Имя подписчика

public:
  /// Конструктор
  PropertyTreeEvent(const std::wstring & subName, ksapi::IPropertyTree & tree, PropertyTreeEventCallbacks & obj);
  /// Деструктор
  ~PropertyTreeEvent();

  /// Подписаться на события
  virtual void Advise() override;
  /// Отписаться от событий
  virtual void Unadvise() override;
  /// Получить hash события
  virtual size_t GetHash() override;
  /// Получить объект события
  ksapi::IAPIObjectPtr GetEventObject() override;

protected:
  /// Событие начала переименования узла дерева.
  void BeginTextEdit(const ksapi::IPropertyTreeNodePtr & node);
  /// Событие завершения переименования узла дерева.
  void EndTextEdit(const ksapi::IPropertyTreeNodePtr & node);
  /// Событие "Узлы селектированы".
  void SelectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes);
  /// Событие "С узлов снято селектирование".
  void UnselectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes);
  /// Запрос контекстного меню для узла дерева.
  void ContextMenuRequest(const ksapi::IPropertyTreeNodePtr & node);
};


////////////////////////////////////////////////////////////////////////////////
//
/// События окна документа
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class DocumentFrameEvent : public IOneWindowEvent,
                           public DocumentIdentifier
{
private:
  ksapi::IDocumentFramePtr m_frame;   ///< Окно документа
  DocumentFrameEventCallback * m_obj; ///< Объект для вызова обработчиков
  std::wstring m_subName;             ///< Имя подписчика

public:
  /// Конструктор
  DocumentFrameEvent(const std::wstring & subName, ksapi::IDocumentFrame & frame, DocumentFrameEventCallback & obj);
  /// Деструктор
  ~DocumentFrameEvent();


public:
  /// Подписаться на события
  virtual void Advise() override;
  /// Отписаться от событий
  virtual void Unadvise() override;
  /// Получить hash события
  virtual size_t GetHash() override;
  /// Получить объект события
  ksapi::IAPIObjectPtr GetEventObject() override;

  /// Начало создания листа в контексте OpenGL
  virtual bool OnBeginPaintGL(const ksapi::IOpenGLObjectPtr & glObj, std::int32_t drawMode);
  /// Определение габаритов документа
  virtual bool OnAddGabarit(ksapi::IGabaritObjectPtr & gabObj);
  /// Закрытие окна
  virtual bool OnCloseFrame();
};


////////////////////////////////////////////////////////////////////////////////
//
/// События 3D-процесса
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Process3DEvent : public IOneWindowEvent,
                       public DocumentIdentifier
{
private:
  PropertyManagerObject * m_propMng; ///< Объект для вызова обработчиков
  ksapi::IProcess3DPtr m_process;    ///< Процесс 3D
  std::wstring m_subName;            ///< Имя подписчика

public:
  /// Конструктор
  Process3DEvent(const std::wstring & subName, PropertyManagerObject & propMng, ksapi::IProcess3D & process);
  /// Деструктор
  ~Process3DEvent();

  /// Подписаться на события
  virtual void Advise() override;
  /// Отписаться от событий
  virtual void Unadvise() override;
  /// Получить hash события
  virtual size_t GetHash() override;
  /// Получить объект события
  ksapi::IAPIObjectPtr GetEventObject() override;

  /// Обработчик события "Окончание процесса".
  virtual bool OnFilterObjects(const ksapi::IKompasAPIObjectPtr & object);
  /// Обработчик события "Фильтрация объетов".
  virtual bool OnEndProcess();
};

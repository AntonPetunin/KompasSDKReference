#pragma once

#include <KsAPI.h>

/// Вывод сообщения
std::int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
/// Вывод сообщения
std::int32_t LibMessage(std::int32_t strId, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);


////////////////////////////////////////////////////////////////////////////////
//
///
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerSubscriber
{
private:
  static PropertyManagerLayout m_layout; ///< Положение панели

private:
  ksapi::IKompasDocumentPtr m_document;   ///< Документ
  ksapi::IPropertyManagerPtr m_propMng;   ///< Менежер панели свойств
  ksapi::IPropertySlideBoxPtr m_slideBox; ///< Окно в котором будет отрисовываться текущий документ

private:
  ksapi::IDrawingObjectEventsPtr m_drawingObjEvents;   ///< События объекта 2D
  ksapi::IModelObjectEventsPtr m_modelObjEvents;       ///< События объекта 3D
  ksapi::ISpecificationObjectEventsPtr m_spcObjEvents; ///< События объекта спецификации

public:
  /// Конструктор
  PropertyManagerSubscriber(ksapi::IKompasDocument & document);

  /// Получить документ на который подписываемся
  ksapi::IKompasDocumentPtr GetDocument() { return m_document; }
  /// Подписка на события документа
  void SubscribeEvents(ksapi::IKompasDocument & document);
  /// Подписаться на события документа
  bool SubscribeOnDocument(const ksapi::IKompasDocumentPtr & doc);
  /// Подписаться на события 3D документа
  void SubscribeOnDocument3D(const ksapi::IKompasDocument3DPtr & doc);
  /// Подписаться на события 2D документа
  void SubscribeOnDocument2D(const ksapi::IKompasDocument2DPtr & doc);
  /// Подписаться на события объектов чертежа
  void SubscribeOnObject2D(const ksapi::IKompasDocument2DPtr & doc, DrawingObjectTypeEnum objType);
  /// Подписаться на события объектов детали
  void SubscribeOnObject3D(const ksapi::IKompasDocument3DPtr & doc, ksObj3dTypeEnum objType);
  /// Подписаться на события документа спецификации
  void SubscribeOnSpcDoc(const ksapi::ISpecificationDocumentPtr & doc);
  /// Подписаться на события объекта спецификации
  void SubscribeOnSpcObject(const ksapi::ISpecificationDocumentPtr & doc, ksSpecificationObjectTypeEnum objType);
  /// Подписаться на события описаний спецификации
  void SubscribeOnSpcDescription(const ksapi::ISpecificationDocumentPtr & doc);
  /// Подписаться на события штампа
  void SubscribeOnStamp(const ksapi::IKompasDocumentPtr & doc);
  /// Подписаться на события панели свойств и параметров процесса
  void SubscribeOnPropMng();

  /// Отписаться от событий
  void UnadviseEvents();
  /// Создать закладку и подписаться
  void CreateAndSubscriptionPropertyManager(bool mes);
  /// Остановить работу с панелью свойствт и отписаться
  bool ClosePropertyManager(bool mes);
  /// Обновление окна
  bool UpdateSlideBox();
  /// Обновление окна
  bool UpdateSlideBox(const ksapi::IKompasDocumentPtr & docRef);

private:
  /// Событие нажатие кнопки
  bool ButtonClick(std::int32_t buttonID);
  /// Событие изменения значения контрола
  bool ChangeControlValue(const ksapi::IPropertyControlPtr & ctrl);
  /// Нажатие кнопки контрола
  bool ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID);
  /// Установка состояния кнопки спецпанели.
  bool ButtonUpdate(int32_t buttonId, bool & enable, int & checked);
};

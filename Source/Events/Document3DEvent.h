#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// Document3DEvent  - обработчик событий 3D документа
//
////////////////////////////////////////////////////////////////////////////////
class Document3DEvent final : public BaseEvent
{
public:
  Document3DEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~Document3DEvent();

  virtual bool Advise() override;   // подписаться на получение событий
  virtual void UnAdvise() override; // отписаться от получения событий

  virtual std::wstring EventCaption() override { return L"Document3DEvent: " + GetDocumentParam(m_doc); };

  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");

#pragma region Обработчики событий 3D документа
  // Начало перестроения модели
  bool OnBeginRebuild();
  // Модель перестроена
  bool OnRebuild();
  // Начало выбора материала
  bool OnBeginChoiceMaterial();
  // Начало выбора обозначения
  bool OnBeginChoiceMarking();
  // Начало установки компонента в сборку (до диалога выбора имени)
  bool OnBeginSetPartFromFile();
  // Начало создания компонента в сборке (до диалога выбора имени)
  bool OnBeginCreatePartFromFile(bool isPart, const ksapi::IModelObjectPtr & object);
  // Добавлено новое исполнение
  void OnCreateEmbodiment(const std::wstring & marking);
  // Удалено исполнение
  void OnDeleteEmbodiment(const std::wstring & marking);
  // Исполение установлено текущим
  void OnChangeCurrentEmbodiment(const std::wstring & marking);
  // Начало выбора свойства
  bool OnBeginChoiceProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId);
  // Закончен выбор свойства
  bool OnChoiceProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId);
  // Начало отката дерева модели
  bool OnBeginRollbackFeatures();
  // Завершение отката дерева модели
  bool OnRollbackFeatures();
  // Начало переключения типа загрузки
  bool OnBeginLoadCombinationChange(int32_t index);
  // Завершение переключения типа загрузки
  bool OnLoadCombinationChange(int32_t index);
  // Начало удаления материала
  bool OnBeginDeleteMaterial();
  // Материал удален
  bool OnDeleteMaterial();
  // Начало удаления значения свойства
  bool OnBeginDeleteProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propId);
  // Завершение удаления свойства
  bool OnDeleteProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propId);
#pragma endregion
};

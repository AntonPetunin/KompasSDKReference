////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerObject - Класс для обслуживания панели свойств
// PropertyManagerEvent  - Класс событий менеджера свойств
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include <KsAPI.h>

#include "BaseEvent.h"

using namespace ksapi;

// Получить библиотечный шрифт
std::wstring GetLibraryIconFont();

////////////////////////////////////////////////////////////////////////////////
//
// Класс для обслуживания панели свойств
//
// ctrlID используется как идентивикатор контрола и как
// идентификатор для загрузки  ресурсов контрола
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject
{
protected:
  int m_flagMode;                          // Признак редактирования по дабл-клику
  IProcessParamPtr m_procParam;            // Параметры процесса
  IPropertyTabsPtr m_propTabs;             // Закладки
  IPropertyControlsPtr m_curentCollection; // Контролы текущей закладки панели свойств
  IPropertyGridPtr m_paramGrid;            // Грид для вывода дополнительных параметров элемента
  IPropertySlideBoxPtr m_slideBox;         // Окно просмотра
  int32_t m_rowIndex;                      // Индекс строки

public:
  PropertyManagerObject();
  virtual ~PropertyManagerObject();

  // Менеджер свойств ##################################################################
  // Событие prButtonClick - Нажатие кнопки.
  virtual void OnButtonClick(int32_t buttonID) {}
  // Событие prChangeControlValue - Изменение значения контрола
  virtual void OnChangeControlValue(const IPropertyControlPtr & control) {}

  // Инициализация параметров процесса
  bool InitProcessParam(int32_t toolBarID, SpecPropertyToolBarEnum toolBarType, int32_t firstTabID = 0);

  // Добавление закладки
  bool CreateTab(int32_t tabID, bool visible = true, bool active = false);
  // Добавление комбобокса вещественных значений
  IPropertyListPtr CreateRealList(double minVal = 0, double maxVal = 0);
  // Добавление комбобокса строковых значений
  IPropertyListPtr CreateStringList();
  // Добавление комбобокса целых значений
  IPropertyListPtr CreateIntList();
  // Добавление сепаратора
  IPropertySeparatorPtr CreateSeparator(SeparatorTypeEnum type);
  // Добавление CheckBox-а
  IPropertyCheckBoxPtr CreateCheckBox(bool checked);
  // Добавление группы кнопок
  IPropertyMultiButtonPtr CreateMultiButton(ButtonTypeEnum type);
  // Добавление кнопоки в группу
  void AddButton(IPropertyMultiButtonPtr & buttons, int32_t btnID, bool cheched = false, bool enable = true);
  // Добавление кнопоки в группу
  void AddButton2(IPropertyMultiButtonPtr & buttons, int32_t btnID, int32_t bmpID, bool cheched = false, bool enable = true);

  // Получить указатель на Control по его идентификатору (для изменения параметров контрола)
  IPropertyControlPtr GetPropertyControl(int32_t ctrlID);
  // Изменить состояние контрола
  void SetControlEnable(int32_t ctrlID, bool enabled = true);

  // Накачка контрола общими параметрами
  void InitPropertyControl(IPropertyControl * control, int32_t ctrlID, uint32_t hint = 0, uint32_t tips = 0, bool enable = true,
                           PropertyControlNameVisibility nameVisibility = ksNameAlwaysVisible, bool visible = true);

  // Виртуальная функция наполнения контролов элемента
  virtual void ShowControls() {}
  // Перерисовать фантом
  virtual void RedrawPhantom() {}

  // Количество параметров выводимых в гриде
  virtual int ParamCount() { return 0; }
  // Заполнение грида параметров
  virtual void ShowParam() {}
  // Идентификатор слайда
  virtual int32_t GetSlideID() { return 0; }

  // Функции для заполнения грида параметров
  // Добавление строкового значения
  void AddStringToGrig(int32_t paramID, const std::wstring & value);
  // Добавление числового значения
  void AddDoubleToGrig(int32_t paramID, double value);
  // ksFilterObjects - Фильтрация объектов
  virtual bool FilterObjects(const IModelObjectPtr & object) { return false; }
  // ksProcess3DCreateTakeObject - Событие создания объекта в подпроцессе.
  virtual bool CreateTakeObject(const IModelObjectPtr & object) { return false; }
  // ksPlacementChanged - Изменено положения объекта
  virtual bool PlacementChanged(const IModelObjectPtr & object) { return false; }
  // Завершить процесс редактирования
  virtual bool EndProcess();
  virtual void UnadviseEvents();
  // Менеджер свойств ##################################################################
};


////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerEvent - обработчик событий от менеджера панели свойств
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerEvent : public BaseEvent
{
protected:
  PropertyManagerObject & m_obj; // Указатель на подписанный объект

public:
  PropertyManagerEvent(const IProcessParamPtr & manager, PropertyManagerObject & obj);
  virtual ~PropertyManagerEvent();

public:
  // Подписаться на события
  void Advise();
  // Отписаться от событий
  virtual void Unadvise() override;

  // prChangeControlValue - Событие изменения значения контрола
  bool ButtonClick(int32_t buttonID);
  // prChangeControlValue - Событие изменения значения контрола
  void ChangeControlValue(const IPropertyControlPtr & control);
  // prControlCommand - Нажатие кнопки контрола
  void ControlCommand(const IPropertyControlPtr & control, int32_t buttonId);
  // Callback для получения типа контекстного меню
  ksProcessContextMenuType GetContextMenuType(int32_t screenX,
                                              int32_t screenY); // screenX, screenY - это экранные координаты
  // Callback для накачки контекстной панели
  void FillContextPanel(const IProcessContextPanelPtr & contextPanel);
};


////////////////////////////////////////////////////////////////////////////////
//
// Process3DEvent - обработчик событий от процесса 3D
//
////////////////////////////////////////////////////////////////////////////////
class Process3DEvent : public BaseEvent
{
protected:
  PropertyManagerObject & m_obj; // Указатель на подписанный объект
  // Attributes
public:
  Process3DEvent(const IProcess3DPtr & process, PropertyManagerObject & obj);
  virtual ~Process3DEvent();

public:
  // Подписаться на события
  void Advise();
  // Отписаться от событий
  virtual void Unadvise() override;

  // Operations
  // ksFilterObjects - Фильтрация объектов
  bool FilterObjects(const IKompasAPIObjectPtr & object);
  // ksProcess3DCreateTakeObject - Событие создания объекта в подпроцессе.
  bool CreateTakeObject(const IModelObjectPtr & object);
  //  ksPlacementChanged - Изменено положения объекта
  bool PlacementChanged(const IModelObjectPtr & object);
  // ksEndProcess - Окончание процесса.
  bool EndProcess();
  // ksStop - Остановка процесса
  bool Stop();
  // ksRun - Запуск процесса
  bool Run();
};


////////////////////////////////////////////////////////////////////////////////
//
/// Класс для реализации событий манипулятора
//
////////////////////////////////////////////////////////////////////////////////
class Process3DManipulatorsObject
{
public:
  // ksRotateManipulator - Поворот манипулятора
  virtual bool RotateManipulator(double x0, double y0, double z0, double axisX, double axisY, double axisZ, double angle,
                                 bool fromEdit)
  {
    return false;
  }
  // ksMoveManipulator   - Передвижение манипулятора
  virtual bool MoveManipulator(int32_t manipulatorId, double vX, double vY, double vZ, double delta, bool fromEdit)
  {
    return false;
  }
  // ksClickManipulatorPrimitive - клик по примитиву манипулятора
  virtual bool ClickManipulatorPrimitive(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType, bool doubleClick)
  {
    return false;
  }
  // ksBeginDragManipulator - начало перетаскивания манипулятора
  virtual bool BeginDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType) { return false; }
  // ksEndDragManipulator - окончание перетаскивания манипулятора
  virtual bool EndDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType) { return false; }

  virtual ~Process3DManipulatorsObject() {}
};


////////////////////////////////////////////////////////////////////////////////
//
// Process3DManipulatorsEvent - обработчик событий от манипуляторов 3D
//
////////////////////////////////////////////////////////////////////////////////
class Process3DManipulatorsEvent : public BaseEvent
{
protected:
  Process3DManipulatorsObject & m_obj; // Указатель на подписанный объект

public:
  Process3DManipulatorsEvent(const IManipulatorsPtr & manipulators, Process3DManipulatorsObject & obj);
  virtual ~Process3DManipulatorsEvent();

public:
  // Подписаться на события
  void Advise();
  // Отписаться от событий
  virtual void Unadvise() override;

  //Operations
  // ksRotateManipulator - Поворот манипулятора
  bool RotateManipulator(int32_t manipulatorId, double x0, double y0, double z0, double axisZX, double axisZXY, double axisZZ,
                         double angle, bool fromEdit);
  // ksMoveManipulator   - Передвижение манипулятора
  bool MoveManipulator(int32_t manipulatorId, double vX, double vY, double vZ, double delta, bool fromEdit);
  // ksClickManipulatorPrimitive - Клик по примитиву манипулятора
  bool ClickManipulatorPrimitive(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType, bool doubleClick);
  // ksBeginDragManipulator - Начало перетаскивания манипулятора
  bool BeginDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType);
  // ksEndDragManipulator   - Окончание перетаскивания манипулятора
  bool EndDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType);
};

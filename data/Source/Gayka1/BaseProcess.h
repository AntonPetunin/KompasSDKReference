#pragma once
#include <memory>
#include <unordered_map>

class ParameterObject;
class ProcessDispatcher;

////////////////////////////////////////////////////////////////////////////////
//
/// Базовый процесс
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class BaseProcess
{
protected:
  std::int32_t m_rowIndex;                        ///< Индекс строки
  ksapi::IPropertyTabsPtr m_propTabs;             ///< Закладки
  ksapi::IPropertyGridPtr m_propGrid;             ///< Грид для вывода дополнительных параметров элемента
  ksapi::IPropertyControlsPtr m_curentCollection; ///< Контролы текущей закладки панели свойств
  ksapi::IPropertySlideBoxPtr m_slideBox;         ///< Окно просмотра

private:
  ksapi::IKompasDocument * m_parent; ///< Родительский документ

public:
  /// Конструктор
  BaseProcess();
  /// Конструктор
  BaseProcess(const BaseProcess & other);
  /// Деструктор
  virtual ~BaseProcess();

  /// Установить индекса строки грида параметров
  void SetGridRowIndex(std::int32_t rowIndex) { m_rowIndex = rowIndex; }
  /// Получить индекса строки грида параметров
  std::int32_t GetGridRowIndex() const { return m_rowIndex; }
  /// Получить базовый объект процесса
  virtual ParameterObject * GetProcessObject() = 0;
  /// Запустить процесс
  virtual bool Run(bool modal, bool postMessage) = 0;
  /// Отписаться от событий
  virtual void UnadviseEvents() = 0;
  /// Получить родительский документ
  virtual ksapi::IKompasDocument * GetParentDocument() const;

#pragma region ControlsFunctions

public:
  /// Накачка контрола общими параметрами
  virtual void InitPropertyControl(ksapi::IPropertyControlPtr control, std::int32_t ctrlID, bool enable = true,
                                   bool visible = true) final;
  /// Добавление закладки
  virtual bool CreateTab(std::int32_t tabID, bool visible = true, bool active = false) final;
  /// Добавление комбобокса вещественных значений
  virtual ksapi::IPropertyListPtr CreateList(double minVal = 0, double maxVal = 0) final;
  /// Добавление редактора вещественных параметров
  virtual ksapi::IPropertyEditPtr CreateRealEdit(double value, double minValue, double maxValue, ControlTypeEnum controlType);
  /// Добавление сепаратора
  virtual ksapi::IPropertySeparatorPtr CreateSeparator(SeparatorTypeEnum separatorType) final;
  /// Добавление CheckBox-а
  virtual ksapi::IPropertyCheckBoxPtr CreateCheckBox(bool checked) final;
  /// Добавление группы кнопок
  virtual ksapi::IPropertyMultiButtonPtr CreateMultiButton(ButtonTypeEnum buttonType) final;
  /// Получить указатель на Control по его идентификатору (для изменения параметров контрола)
  virtual ksapi::IPropertyControlPtr GetPropertyControl(int ctrlID) final;
  /// Изменить состояние видимости контрола
  virtual void SetControlVisible(std::int32_t ctrlID, bool enabled = true) final;
  /// Добавление строкового значения в грид
  virtual void AddStringToGrig(std::int32_t paramID, std::wstring value) final;
  /// Добавление числового значения в грид
  virtual void AddDoubleToGrig(std::int32_t paramID, double value) final;
  /// Добавление кнопки в группу
  virtual void AddButton(ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, bool cheched = false, bool enable = true);
  /// Добавление кнопки в группу
  virtual void AddButton2(ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, std::int32_t bmpID, bool cheched = false,
                          bool enable = true);
  /// Вывод контролов
  virtual void ShowControls() = 0;

#pragma endregion

#pragma region View

public:
  /// Перерисовать фантом
  virtual void RedrawPhantom() {};
  /// Процесс создания элемента
  virtual void Draw1() {};
  /// Функции для вывода контрола параметров штриховки
  virtual void ShowParam() {};
  /// Показать контрол штриховки
  virtual void ShowHatchControl() {};

#pragma endregion

#pragma region HandlerFunctions

public:
  /// Нажатие на кнопку
  virtual bool ButtonClick(std::int32_t buttonID) { return false; }
  /// Изменить значение контрола
  virtual bool ChangeControlValue(ksapi::IPropertyControl & ctrl) { return false; };
  /// Изменено положения объекта
  virtual bool PlacementChanged(double x, double y, double angle, bool dynamic) = 0;
  /// Завершение процесса
  virtual bool EndProcess() = 0;

  /// Обработчик нажатия кнопки
  bool OnButtonClick(std::int32_t buttonId);
  /// Обработчик изменения значения контрола
  void OnChangeControlValue(ksapi::IPropertyControlPtr ctrl);
  /// Обработчик нажатия кнопки контрола
  void OnControlCommand(ksapi::IPropertyControlPtr control, std::int32_t buttonId);
  /// Вызов справки
  bool OnCommandHelp(std::int32_t buttonID);
  /// Обработчик изменения положения объекта
  bool OnPlacementChanged(double x, double y, double angle, bool dynamic);
  /// Обработчик завершения процесса
  bool OnEndProcess();
#pragma endregion
};

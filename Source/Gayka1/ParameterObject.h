////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Содержит базовый класс элемента
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Doc2DDispatcher.h"
#include "IHatchUtil.h"
#include "PropertyManagerObject.h"
#include "Resource.h"

#define LIB_HELP L"Gayka.chm" // файл помощи
#define LIB_LAT  L"Spc.lat"   // файл атрибутов
#define LIB_VER  02           // версия библиотеки

#define _SHOW_ERRORS // флаг - показывать сообщения об ошибках

#define BASE_LINE 1 // тип основной линии
#define SLIM_LINE 2 // тонкая линия
#define AXIS_LINE 3 // тип осевой линии
#define AXIS_END  3 // величина выступания осевой линии за габарит тела

// параметы штриховки
#define MIN_H_ANG        -360 // минимальный угол
#define MAX_H_ANG        360  // максимальный угол
#define MIN_H_STEP       0.01 // минимальный шаг
#define MAX_H_STEP       100  // максимальный шаг
#define MAX_COUNT_SPCOBJ 4    // максимальное число объектов СП за раз

#define EPSILON          0.001 // погрешность при сравнении с нулем типа float
#define STANDART_SECTION 25    // Номер раздела и подраздела спецификации

#define SPCOBJ_CREATE   true // признак подключения объекта к спецификации
#define COUNT_MASSA     1000 // количество элементов для которых задана масса
#define WORKWINDOWCOLOR -1   // признак назначенности цвета рабочего окна
#define MODSTEP_REAL    0.5412765877f

typedef unsigned char uint8;

struct RequestInfo;
struct HotPointDescription;
struct HotPointDescription1;
struct PropertyParam;
enum class ProcessState;

namespace ksapi
{
struct IPropertyParam;
struct IObject2DHotPoint;
} // namespace ksapi

////////////////////////////////////////////////////////////////////////////////
//
/// Базовые параметры объекта
/**
  \ingroup 
*/
////////////////////////////////////////////////////////////////////////////////
struct BaseMacroParam
{
  unsigned char version; // Версия элемента
  union
  {
    struct
    {
      uint8 flagAttr : 1; ///< 0x01 создать объект СП
      uint8 flagAxis : 1; ///< 0x02 рисовать ось
      uint8 flagSimp : 1; ///< 0x04 упрощенное изображение
    } flags;
    uint8 bitVector;
  };

  short drawType; ///< тип отображения
  BaseMacroParam();
  BaseMacroParam(BaseMacroParam & other);
};


////////////////////////////////////////////////////////////////////////////////
//
/// Базовый класс элемента
/**
  \ingroup Группа
*/
////////////////////////////////////////////////////////////////////////////////
class ParameterObject : public PropertyManagerObject
{
protected:
  Doc2DDispatcher dispatcher; ///< Диспетчер работы с документом

private:
  ksapi::ISpecificationBaseObjectsPtr m_spcBaseObjects; ///< Коллекция объектов спецификации
  ksapi::IDrawingGroupPtr m_phantomGroup;               ///< Фантомная группа

protected:
  bool m_changed;                       ///< Признак изменения параметров
  unsigned int command = 0;             ///< Номер команды
  ksapi::IMacroObjectPtr m_macroObject; ///< Макро объект
  ksapi::IMacroObjectPtr m_editMode;    ///< Признак редактирования по дабл-клику

  ksapi::ISpecificationBaseObjectPtr m_spcObject; ///< Объект спецификации
  ksapi::IPositionLeaderPtr oldPosLeader;
  ksapi::IPositionLeaderPtr posLeader;

  double x;           ///< X координата точки вставки макрообъекта
  double y;           ///< Y координата точки вставки макрообъекта
  double angle;       ///< Угол вставки макрообъекта
  bool mirrorSymetry; ///< Симметрия вставки макрообъекта

  /// Обязательно последний параметр т.к. начиная с него пишем область памяти в макро параметры
  BaseMacroParam m_baseParam; ///< Базовые параметры объекта

public:
  /// Конструктор
  ParameterObject(unsigned int command, ksapi::IMacroObjectPtr macroObj = nullptr);
  /// Конструктор
  ParameterObject(ParameterObject & other);

  /// оператор приведения по умолчанию к void*
  operator void *() { return &m_baseParam.version; }
  /// Оператор присваивания
  void Assign(const ParameterObject & other);

  /// Получить базовые параметры
  virtual BaseMacroParam & GetBaseParam() { return m_baseParam; }
  /// Получить флаг изменения параметров
  virtual bool Changed() { return m_changed; }
  /// Установить флаг изменения параметров true
  virtual void SetChanged() { m_changed = true; }
  /// Установить флаг изменения параметров false
  virtual void SetUnchanged() { m_changed = false; }
  /// Получить X координату
  virtual double GetX() const { return x; }
  /// Получить Y координату
  virtual double GetY() const { return y; }
  /// Получить угол
  virtual double GetAngle() const { return angle; }
  /// Получить зеркальную симметрию
  virtual bool GetMirrorSymetry() const { return mirrorSymetry; }
  /// Получить объект спецификации
  virtual ksapi::ISpecificationBaseObjectPtr GetSpcObject() const { return m_spcObject; }

  /// Признак редактирования по дабл-клику
  virtual bool IsEditMode() const { return !m_editMode.IsEmpty(); }
  /// Установить редактируемый объект
  virtual void SetEdit(ksapi::IMacroObjectPtr editMacroObj) { m_editMode = editMacroObj; }
  /// Создание геометрии элемента
  virtual void GetGroup() = 0;
  /// Отрисовка фантома
  virtual void DrawPhantom(double x, double y, double angle, bool dynamic);
  /// Перерисовать макро
  virtual void RedrawMacro();
  /// Заполнение объекта СП
  virtual bool FillSpcObj(ksapi::ISpecificationBaseObjectPtr & obj1) { return true; }
  /// Редактирование объекта спецификации
  virtual bool EditSpcObject() override;
  /// Отобразить диалог создани объекта спецификации
  virtual bool DrawSpcObj();
  /// Создание объектов спецификации
  virtual bool IsSpcObjCreate() { return true; }
  /// Изменено положения объекта
  virtual bool PlacementChanged(double _x, double _y, double _angle);
  /// Поиск линии-выноски
  virtual bool PosLeader(double x, double y, bool dynamic);
  /// Получить номер команды
  virtual int GetCommand() override { return command; }
  /// Cохраняет параметры эл-та в макро
  virtual void SetParam();
  /// Возвращает размер структуры параметров
  virtual int Size() { return sizeof(m_baseParam); }
  /// Вызывается после изменения типа отображения через меню во время процесса постановки в модель
  virtual ksapi::NumericId AttrNumber() { return 0; }
  /// В текущем режиме штриховка есть
  virtual bool IsHatchEnable() const override { return m_baseParam.drawType == ID_SEC; }
  /// Создать линию выноски
  virtual bool CreateLeader(ProcessState processState);
  /// Создание элемента
  virtual void Draw1(ksapi::IPhantom2D & phantom) override;
  /// Создание спецификации
  virtual ksapi::ISpecificationBaseObjectPtr EditSpcObj();
  /// Изменение положения объекта
  virtual void SetPlacement(double x, double y, double angle);
  /// Отрисовать позиционную линию выноски
  virtual bool DrawPosLeader();
  /// Версия
  virtual int GetVersion() { return 1; }
  /// Перерисовка превью элемента
  virtual ksapi::IDrawingGroupPtr GetPreviewGroup() override;
  /// Перерисовка превью элемента
  virtual void RedrawPreview() = 0;
  // Получить парметры для элемента XML дерева
  virtual void GetXMLPartFamilyParam(std::wstring & name, std::wstring & id) = 0;

#pragma region HotPoints

#pragma region ILibHPObject
  /// Параметры для Hot точек
  virtual bool GetHotPoint(ksapi::IObject2DHotPoint & point, int index) { return false; }
  /// Изменение положения Hot точек
  virtual bool SetHotPoint(double newX, double newY, int index);
  /// Подготовка к редактированию
  virtual bool Prepare() { return true; }
  /// Выполнение команды
  virtual bool ExecuteCommand(int id) { return true; }
  // Редактирование и вставка объекта спецификации, если он был создан
  virtual bool EditComplete(int index, bool success) { return true; }
  // Меню
  virtual IMenuPtr GetMenu(int32_t index) { return 0; }
  /// Параметры для Hot точек
  virtual bool ChangeHotPointParam(double d) { return true; };
#pragma endregion

#pragma region ILibHPObject1
  /// Текст для Hot точек
  virtual std::wstring GetCursorTextEx(int index) { return L""; }
  /// Получить состояние команды из меню.
  virtual void GetCommandState(int32_t commanId, bool & enable, bool & checked) {}
  virtual bool SelectHotPoint(int index) { return false; }
  virtual bool UnselectHotPoint(int index) { return false; }
#pragma endregion

#pragma endregion

#pragma region ButtonEnable
  /// Нажата ли кнопка
  virtual bool IsButtonEnable(long buttonID) { return true; }
  /// Нажата ли кнопка
  virtual bool IsButtonChecked(long buttonID);
#pragma endregion

#pragma region PropertyManager
  // Добавить свойство в список свойств
  virtual bool GetProperty(int32_t index, IPropertyParam & propertyParam);
  // Обновить свойства
  virtual bool UpdateProperty(IPropertyParam & propertyParam);
  // Изменить свойство
  virtual bool ApplyProperty(IPropertyParam & propertyParam);
  /// По индексу свойства получить идентификатор
  virtual int GetPropertyID(int index);
  /// Имя группы
  virtual std::wstring GetGroupName() { return L""; }
#pragma endregion

#pragma region HandlerFunctions
  /// Нажатие на кнопку
  virtual bool ButtonClick(unsigned int buttonID) = 0;
  /// Изменить значение контрола
  virtual bool ChangeControlValue(std::int32_t ctrlID, PropertyValue & newVal) = 0;
#pragma endregion
};

/// Возвращает полное имя файла
bool GetFullName(const std::wstring & inName, std::wstring & outName);
/// Сообшение о ошибке
void _MessageBoxResult();
// Получить библиотечный шрифт
std::wstring GetLibraryIconFont();

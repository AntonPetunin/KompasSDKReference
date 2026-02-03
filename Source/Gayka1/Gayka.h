////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Конструкторский элемент на Visual C++ - гайка
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ParameterObject.h"

class Doc2DDispatcher;
namespace prop
{
struct Params;
}

////////////////////////////////////////////////////////////////////////////////
//
/// Флаги гайки ГОСТ 5915
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct G5915Flags
{
  bool isp : 1;      ///< FALSE - исполнение 1,            TRUE - исполнение 2
  bool pitch : 1;    ///< FALSE - крупный шаг,             TRUE - мелкий шаг
  bool pitch_on : 1; ///< FALSE - есть только крупный шаг, TRUE - есть крупный и мелкий шаг
  bool key_s : 1;    ///< FALSE - обычный размер под ключ, TRUE - дополнительный размер под ключ,
  bool key_s_on : 1; ///< FALSE - греить "Дополнительные размеры под ключ"
};

////////////////////////////////////////////////////////////////////////////////
//
/// Параметры гайки ГОСТ 5915
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct GAYKA5915
{
  float dr; // Диаметр резьбы
  float s;  // Шаг резьбы ()
  float D;  // Размер под ключ
  float da;
  float h; // Высота головки
  float d2;
  float p;             // Шаг резьбы
  short classAccuracy; // Класс точности
  float hatchAngle;    // Угол штриховки
  float hatchStep;     // Шаг штриховки
  short version;       // Версия макрообъекта
  float massa;         // Масса
  short indexMassa;    // 0 - металл, 1 - алюмин сплав, 2 - латунь
  union
  {
    uint8 iFlags;
    G5915Flags flags; // Флаги
  };
  GAYKA5915() { memset(&dr, 0, sizeof(GAYKA5915)); }
};


////////////////////////////////////////////////////////////////////////////////
//
/// Класс гайки ГОСТ 5915
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Gayka5915 : public ParameterObject
{
protected:
  GAYKA5915 m_paramGayka; ///< Параметры гайки ГОСТ 5915

public:
  /// Конструктор
  Gayka5915(ksapi::IMacroObjectPtr & macroObj);
  /// Конструктор
  Gayka5915(const Gayka5915 & other);
  /// Деструктор
  virtual ~Gayka5915();

  /// Инициализация параметров
  void InitParam();
  /// Копирование
  void Assign(const Gayka5915 & other);
  /// Возврашает параметры гайки
  GAYKA5915 & GetParamGayka() { return m_paramGayka; }
  /// Отрисовка геометрии гайки (сверху)
  void Gayka_Sverhu(bool appl);
  /// Отрисовка геометрии гайки (пол-вида/пол-разреза)
  void Gayka_P_K(int j, bool appl);
  /// Виртуальные функции для работы с элементом
  void FillDiamList(IPropertyListPtr & diamList);
  /// Список диаметров для комбобокса
  std::vector<double> GetDiamList();

#pragma region ParameterObject
  /// создание геометрии элемента
  virtual void GetGroup() override;
  /// Размер параметров
  virtual int Size() override { return ParameterObject::Size() + sizeof(GAYKA5915); }
  /// Заполнение объекта спецификации
  virtual bool FillSpcObj(ksapi::ISpecificationBaseObjectPtr & obj1) override;
  // Получить парметры для элемента XML дерева
  virtual void GetXMLPartFamilyParam(std::wstring & name, std::wstring & id) override;
  /// Функции вызываемые из меню в процессе постановки эл-та в документ
  virtual ksapi::NumericId AttrNumber() override;
  /// true - в текущем режиме штриховка есть
  virtual bool IsSpcObjCreate() override { return true; };
  /// Отрисовать группу
  virtual void RedrawPreview() override;
  /// Количество параметров выводимых в гриде
  virtual int ParamCount() override { return 4; }
#pragma endregion

#pragma region IHatchUtil
  /// Получить угол штриховки
  virtual double GetHatchAngle() const override { return m_paramGayka.hatchAngle; }
  /// Получить шаг штриховки
  virtual double GetHatchStep() const override { return m_paramGayka.hatchStep; }
  /// Установить угол штриховки
  virtual void SetHatchAngle(double hatch) override { m_paramGayka.hatchAngle = static_cast<float>(hatch); }
  /// Установить шаг штриховки
  virtual void SetHatchStep(double step) override { m_paramGayka.hatchStep = static_cast<float>(step); }
  /// Если штриховка в одном из режимов есть то нужно вернуть true
  virtual bool IsHatchObject() const override { return true; }
#pragma endregion

#pragma region HotPoints

#pragma region ILibHPObject
  /// Параметры для Hot точек
  virtual bool GetHotPoint(ksapi::IObject2DHotPoint & point, int index) override;
  /// Изменение положения Hot точек
  virtual bool SetHotPoint(double newX, double newY, int index) override;
  /// Выполнение команды
  virtual bool ExecuteCommand(int id) override;
  /// Редактирование и вставка объекта спецификации, если он был создан
  virtual bool EditComplete(int index, bool success) override;
  /// Подготовка к редактированию
  virtual bool Prepare() override;
  /// Меню
  virtual IMenuPtr GetMenu(int32_t index) override;
  /// Параметры для Hot точек
  virtual bool ChangeHotPointParam(double d) override;
#pragma endregion

#pragma region ILibHPObject1
  /// Получить текст для отображения курсором
  virtual std::wstring GetCursorTextEx(int index) override;
  ///
  virtual void GetCommandState(int32_t commanId, bool & enable, bool & checked) override;
  /// Селектировали hot point
  virtual bool SelectHotPoint(int index) override;
  /// Расселектировали hot point
  virtual bool UnselectHotPoint(int index) override;
#pragma endregion

#pragma endregion

#pragma region PropertyManager
  /// Выдать идентификатор свойства по индексу
  virtual int GetPropertyID(int index) override;
  /// Добавить свойство в список свойств
  virtual bool GetProperty(int32_t index, IPropertyParam & propertyParam) override;
  /// Обновить параметры свойства
  virtual bool UpdateProperty(IPropertyParam & propertyParam) override;
  /// Изменить свойство
  virtual bool ApplyProperty(IPropertyParam & propertyParam) override;
  /// Имя группы
  virtual std::wstring GetGroupName() override;
#pragma endregion

#pragma region HandlerFunctions
  /// Нажатие на кнопку
  virtual bool ButtonClick(unsigned int buttonID) override;
  /// Изменить значение контрола
  virtual bool ChangeControlValue(std::int32_t ctrlID, PropertyValue & newVal) override;
#pragma endregion
};

/// Отрисовка геометрии гайки ( вид сбоку )
void Gayka_K_Side(Doc2DDispatcher & emulator, float ls, float s, float D, float d2, float H, int j, int j2, bool appl);
/// Отрисовка геометрии гайки ( вид - упрощенно )
void Gayka_K_Y(Doc2DDispatcher & emulator, float ls, GAYKA5915 * tmp, int j, bool appl);
/// Отрисовка геометрии гайки ( вид )
void Gayka_K(Doc2DDispatcher & emulator, float ls, float l, float d1, float s, float l1, float H, short j, int j1, float d2,
             int j2, bool appl);
/// Читать параметры гайки по диаметру
bool ReadGaykaBase(float d, GAYKA5915 * pGayka);

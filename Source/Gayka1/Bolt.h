////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Конструкторский элемент на Visual C++ - Болт
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ParameterObject.h"

#define MAX_LENGTH   260 ///< максимальная длина стержня
#define MIN_LENGTH   12  ///< мигнимальная длина стержня
#define LENGTH_STR   20  ///< длина строки принимаемой из базы данных
#define ADD_DIM      0.05
#define STR_SIZE_STD 32

#define AXIS_OFF   0x1  ///< 0 рисовать >0 не рисовать
#define DRAW_ST_OF 0x10 ///< 0 стержень у болта рисуется > 0 - рисовать только головку


////////////////////////////////////////////////////////////////////////////////
//
/// Основная структура данных болта
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct BoltMainStruct
{
  unsigned short gost; ///< номер госта
  unsigned short perf; ///< исполнение
  unsigned short f;    ///< битовые маски
  float z4;            ///< Величина фаски
  float dp;            ///< диаметр конца болта
  float l;             ///< длина болта (стержня)
  float dr;            ///< диаметр резьбы
  float b;             ///< длина резьбы
  float m;             ///< масса 1000 штук
  float d;             ///< диаметр головки
  float k;             ///< высота головки и/или подголовка
  short kr;            ///< 1 длина резьбы ниже ломманной(резьба только на b) ; 0 - выше ломанной (резьба до головки и  на в )
  float p;             ///< шаг резьбы
  float param1;        ///< высота уса или сторона квадратного подголовка
  float param2;        ///< радиус сферы или угол Alpha
  float v;             ///< ширина уса
  float h;             ///< высота подголовка
  float d1;            ///< диаметр подголовка или стержня
  float rp;            ///< радиус под головкой
  float b1;            ///< Длина при l>120 для ГОСТ 7783-81

  /// Конструктор
  BoltMainStruct() {}
  /// Конструктор
  BoltMainStruct(unsigned short _gost);
  /// Конструктор копий
  BoltMainStruct(const BoltMainStruct &);
  /// Оператор присваивания копированием
  const BoltMainStruct & operator=(const BoltMainStruct &);
  /// Инициализация параметров болта
  void Init();
};


////////////////////////////////////////////////////////////////////////////////
//
/// Болт ГОСТ 7787-81
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class BoltMain : public ParameterObject
{
public:
  BoltMainStruct m_bolt; ///< Структура данных

public:
  /// Конструктор
  BoltMain(unsigned short gost);
  /// Конструктор
  BoltMain(ksapi::IMacroObjectPtr & macroObj);
  /// Конструктор копирования
  BoltMain(const BoltMain & other);
  /// Деструктор
  ~BoltMain();

  /// Параметры элемента
  BoltMainStruct & GetParam() { return m_bolt; }
  /// Перезаполняет структуру
  bool FillStruct(bool forAssmElem = false, float d = 0, float l = 0);
  void Assign(const BoltMain & other);

public:
  /// Инициализация параметров болта
  void InitBolt();
  /// Рисует вид сверху
  virtual void TopDraw();
  /// Рисует основной вид
  virtual void ViewDraw();
  /// Рисует половину основного вида
  virtual void ViewHalfDraw(bool downHalf, bool apl = false);
  /// Рисует вид снизу
  virtual void BottomDraw();
  /// Заполнение списка диаметров резьбы
  void FillDiamList(ksapi::IPropertyListPtr & diamList);
  /// Заполнение списка длин
  void FillLengthList(ksapi::IPropertyListPtr & lengthList);
  /// Получить массив диаметров
  std::vector<double> GetDiamList();
  /// Получить массив длин
  std::vector<double> GetLengthList();
  /// Чтение БД
  virtual bool ReadElemBase(float d, float l, bool forAssmElem = true);
  /// Возвращает диаметр резьбы
  virtual float GetDiamThread() { return m_bolt.dr; }
  /// Допуск длины базового элемента = 2p(p - шаг резьба)
  virtual float GetToleranceLength() { return 2 * m_bolt.p; }
  /// Установить тип отображения болта
  virtual void SetDrawType(short drawType);
  /// Получить длину болта
  virtual float GetLengthBaseElem() { return m_bolt.l; }
  /// создание контура для очистки области под элементом
  virtual void CreateClearGroup();
  /// Получить диаметр фаски базового элемента
  virtual float GetFaceDiam() { return m_bolt.z4; }
  /// Получить текущий шаг базового элемента
  virtual float GetCurStep() { return m_bolt.p; }

#pragma region ParameterObject
  /// Получить группу
  virtual void GetGroup() override;
  /// Возвращает размер структуры параметров
  virtual int Size() override { return ParameterObject::Size() + sizeof(BoltMainStruct); }
  /// Номер атрибута
  virtual ksapi::NumericId AttrNumber() override;
  /// Заполнение объекта СП
  virtual bool FillSpcObj(ksapi::ISpecificationBaseObjectPtr & obj1) override;
  /// функция проверки подключения атрибутов или нет
  virtual bool IsSpcObjCreate() override { return true; }
  /// Получить парметры для элемента XML дерева
  virtual void GetXMLPartFamilyParam(std::wstring & name, std::wstring & id) override;
  /// Перерисовка превью элемента
  virtual void RedrawPreview() override;
#pragma endregion

#pragma region HotPoints

#pragma region ILibHPObject
  /// Параметры для Hot точек
  virtual bool GetHotPoint(ksapi::IObject2DHotPoint & point, int index) override;
  /// Изменение положения Hot точек
  virtual bool SetHotPoint(double newX, double newY, int index) override;
  /// Параметры для Hot точек
  virtual bool ChangeHotPointParam(double d, double l, int index);
#pragma endregion

#pragma region ILibHPObject1
  /// Текст для Hot точек
  virtual std::wstring GetCursorTextEx(int index) override;
#pragma endregion

#pragma endregion

#pragma region PropertyManager
  /// Выдать идентификатор свойства по индексу
  virtual int GetPropertyID(int index) override;
  /// Добавить свойство в список свойств
  virtual bool GetProperty(int32_t index, IPropertyParam & propertyParam) override;
  /// Обновить свойства
  virtual bool UpdateProperty(IPropertyParam & propertyParam) override;
  /// Изменить свойство
  virtual bool ApplyProperty(IPropertyParam & propertyParam) override;
  /// Имя группы
  virtual std::wstring GetGroupName() override;
#pragma endregion

#pragma region PropertyPanel
  /// Рисовать ось
  virtual void SetAxis(bool);
  /// Рисовать ось
  virtual bool GetAxis() { return !(m_bolt.f & AXIS_OFF); }
  /// Количество параметров выводимых в гриде
  virtual int ParamCount() override { return 4; }
#pragma endregion

#pragma region HandlerFunctions
  /// Нажатие на кнопку
  virtual bool ButtonClick(unsigned int buttonID) override;
  /// Изменить значение контрола
  virtual bool ChangeControlValue(std::int32_t ctrlID, PropertyValue & newVal) override;
#pragma endregion
};

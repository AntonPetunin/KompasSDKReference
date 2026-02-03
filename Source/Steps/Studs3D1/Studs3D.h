////////////////////////////////////////////////////////////////////////////////
//
// Studs3D.h - Крепежный элемент на Visual C++ - шпилька
// Структуры параметров и класс шпилек
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <KsAPI.h>

#include "PropMen.h"

using namespace ksapi;

//-------------------------------------------------------------------------------
// Маски настройки
// ---
#define AXIS_OFF     0x1    // false - рисовать, true - не рисовать
#define PITCH        0x2    // false - крупный шаг, true - мелкий шаг
#define SIMPLE       0x10   // false - нормальная отрисовка, true - упрощенная отрисовка
#define ISPOLN       0x80   // false - исполнение 1, true - исполнение 2
#define TAKEISPOLN   0x100  // false - не учитывать исполнение, true - учитывать исполнение
#define ALLST        0x4    // false - резьба на b, true - резьба до головки
#define KONTR        0x8    // false - обычная головка, true - головка под контровочное отверстие
#define SECOFF       0x10   // false - сечение работает, true - сечение засерить, для винтов, для трубопроводов
#define TAKEKONTR    0x20   // false - не учитывать контровку, true - учитыварь контровку
#define REVERCE_ON   0x20   // false - включить прямую, true - обратную отрисовку
#define TAKEPODG     0x40   // false - не учитывать подголовку, true - учитыварь бодголовку
#define LEFT_DRAW    0x40   // true - рисовать слева
#define FLAGLENGTH   0x200  // false - длина болта подобрана правильно, true - не правильно
#define ISPOLN3      0x200  // Для штифтов false - исполнений 2, true - исполнений 3
#define REVERCE_DRAW 0x200  // true - обратная отрисовка, false - прямая отрисовка
#define SBORSHAYBA   0x400  // false - гайка без шайбы, true - гайка вместе с шайбой
#define PITCHOFF     0x800  // false - есть крупный и мелкий шаг, true - есть только крупный
#define LENLEN       0x1000 // false - нормальная длина резьбы, true - удлиненная
#define LENLENOFF    0x2000 // false - не учитывать, true - учитывать

//-------------------------------------------------------------------------------
// Дополнительные маски
// ---
#define KEY_S_ON     0x1  // true - включены дополнитеоьные размеры под ключ, false - выключены
#define KOEFF_MAT_ON 0x4  // false - только металл, true другие материалы
#define KEY_S        0x2  // true - дополнительный размер под ключ, false - обычный размер под ключ
#define KEY_S_GRAY   0x8  // true - греить "Дополнитеоьные размеры под ключ"
#define DRAW_ST_OF   0x10 // false - стержень у болта рисуется true - рисовать только головку

//-------------------------------------------------------------------------------
// Насроечная информация
// ---
#define COUNT_MASSA      1000 // Количество деталей масса которых лежит в ГОСТ-е
#define STANDART_SECTION 25   // Номер раздела и подраздела спецификации
#define SPC_NAME         5    // Колонка наименования

#define EPSILON 0.001

#define STUDS_FILE       L"Stud.l3d"           // Библиотека моделей
#define STUDS_INPUT_PATH L"|Шпильки|Шпилька_1" // Путь внутри библиотеки моделей

#define SPCOBJ_CREATE true // Признак подключения объекта к спецификации
#define PARAM_COUNT   5    // Количество параметров выводимых в гриде

#define MAX_LENGTH 1000 // Максимальная длина стержня
#define MIN_LENGTH 0    // Минимальная длина стержня

////////////////////////////////////////////////////////////////////////////////
//
// Структуры параметров и класс шпилек
//
////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
//------------------------------------------------------------------------------
// Структура параметров шпилек
// ---
struct SHPEEL
{
  float d = 20;             // Диаметр резьбы
  float p1 = 2.5;           // Шаг резьбы
  float p2 = 1.5;           // Шаг резьбы
  float b1 = 20;            // Ввинчиваемый конец
  float c = 2.5;            // Размер фаски
  float l = 90;             // Длина шпильки
  float b = 46;             // Гаечный конец
  short f = 0 | TAKEISPOLN; // Битовые маски
  short klass = 2;          // Класс точности
  short gost = 22032;       // Номер ГОСТа
  short ver = 1;            // Версия макро
  float m1 = 245.9f;        // Масса 1 исполнение
  float m2 = 228.9f;        // Масса 2 исполнение
  short indexMassa = 0;     // 0 - металл 1 - алюмин сплав 3 - бронза 2 - латунь
};


//-------------------------------------------------------------------------------
// Макро параметры
// ---
struct BaseMakroParam
{
  float ang;
  unsigned short flagAttr;
  short drawType;
  __int8 typeSwitch; // Тип запроса положения базовой точки элемента
                     // 0 - точка и направление оси 0X ( Placement )
                     // 1 - точка, направление совпадает с осью 0X текущей СК ( Cursor )
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
//
// Класс шпилек
//
////////////////////////////////////////////////////////////////////////////////
class Shpeel : public PropertyManagerObject,
               public Process3DManipulatorsObject
{
private:
  IKompasDocument3DPtr m_doc3D; // Указатель на интерфейс текущего 3D-документа
  IPartPtr m_part;              // Деталь
  IModelObjectPtr m_takePoint;  // Точка 3D
  std::wstring m_fileName;      // Имя файла
  int m_flagMode;               // Тип редактирования
  bool m_changed = true;        // Признак изменения параметров
  std::vector<IModelObjectPtr> m_objects;

  ISpecificationBaseObjectsPtr m_spcBaseObjects;

  IPlacement3DManipulatorPtr m_manipulator; // Манипулятор
  IBaseManipulatorPtr m_baseMan;            // Базовый интерфейс манипулятора

  IProcess3DPtr m_process3D; // Копия процесса 3D
  IProcessPtr m_process;

  bool m_fixingPart = false;    // Флаг закрепления объекта
  IMateConstraints3DPtr m_mCol; // Коллекция сопряжений

  // Структуры par и tmp записываемые как UserParam
  BaseMakroParam m_par; // Макро параметры
  SHPEEL m_tmp;         // Структура параметров шпилек

public:
  Shpeel();
  virtual ~Shpeel();

  // Инициализация
  void Init();
  // При вставке из библиотеки моделей указывается полный путь к файлу библиотеки и путь внутри библиотеки моделей
  std::wstring GetFileName();
  // Редактируем переменные
  void SetParam(IPartPtr & pPart);
  // Размер данных
  int Size() { return sizeof(m_tmp) + sizeof(m_par); }
  // Редактирование объекта спецификации
  ISpecificationBaseObjectPtr EditSpcObj(ISpecificationBaseObjectPtr);
  // Функция проверки подключения атрибутов или нет
  bool IsSpcObjCreate() { return SPCOBJ_CREATE; };
  // Рисование 3D объектов
  void Draw3D();
  // Получить параметры
  bool GetParam();
  // Отрисовка объекта спецификации
  bool DrawSpcObj(ISpecificationBaseObjectPtr & spcObj);
  // AddRef - не делает
  IPartPtr & GetPart() { return m_part; }
  // AddRef - не делает
  IKompasDocument3DPtr GetDoc() { return m_doc3D; }

  bool AddMate(MateConstraintType constraintType, ksMateConstraintAlignmentEnum direction, ksMateFixedTypeEnum fixed, double val,
               IModelObjectPtr ent, const std::wstring & surfaceName, IPartPtr p, IMateConstraints3DPtr mCol);
  // Находит у детали поверхность с заданным именем
  IModelObjectPtr GetEntityByName(const std::wstring & name, IPartPtr & p);

  // Менеджер свойств ##################################################################
  // Обработка смены ГОСТа
  void OnChangeGost(const std::wstring & newGost);
  // Событие prButtonClick - Нажатие кнопки.
  virtual void OnButtonClick(int32_t buttonID) override;
  // Событие prChangeControlValue - Изменение значения контрола
  virtual void OnChangeControlValue(const IPropertyControlPtr & control) override;

  // Перерисовка фантома
  virtual void RedrawPhantom() override;
  // Виртуальная функция наполнения контролов элемента
  virtual void ShowControls() override;
  // Вывод параметров
  virtual void ShowParam() override;
  // Количество параметров выводимых в гриде
  virtual int ParamCount() override { return PARAM_COUNT; }
  // Идентификатор слайда
  virtual int32_t GetSlideID() override;

  // Заполняет список диаметров
  void FillDiametr(IPropertyListPtr & diamList);
  void FillDiametr(std::vector<double> & diamList, float & curDiam);

  // Заполняет список длин
  void FillLenght(IPropertyListPtr & lenghtList);
  void FillLenght(std::vector<double> & lenList, float & curDiam);
  // Заполняет список длин ввинчиваемого конца
  void FillScrewEnding(IPropertyListPtr & screwEndingList);
  void FillScrewEnding(std::vector<std::wstring> & screwEndingList);
  // Заполняет список класса точности
  void FillClass(IPropertyListPtr & classList); // Заполняет список класса точности
  void FillClass(std::vector<std::wstring> & classList);
  // Заполняет список ГОСТов
  void FillGost(IPropertyListPtr & gostList);
  void FillGost(std::vector<std::wstring> & gostList);
  // Заполняет список материалов
  void FillMaterial(IPropertyListPtr & materialList);
  void FillMaterial(std::vector<std::wstring> & gostList);

  // Признак изменения параметров
  bool Changed() { return m_changed; }
  void SetChanged(bool newChanged = true) { m_changed = newChanged; }
  // Менеджер свойств ##################################################################

  // Манипулятор ##################################################################
  // ksRotateManipulator - Поворот манипулятора
  virtual bool RotateManipulator(double x0, double y0, double z0, double axisX, double axisY, double axisZ, double angle,
                                 bool fromEdit) override;
  // ksMoveManipulator   - Передвижение манипулятора
  virtual bool MoveManipulator(int32_t manipulatorId, double vX, double vY, double vZ, double delta, bool fromEdit) override;
  // ksClickManipulatorPrimitive - клик по примитиву манипулятора
  virtual bool ClickManipulatorPrimitive(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType,
                                         bool doubleClick) override
  {
    return false;
  }
  // ksBeginDragManipulator - начало перетаскивания манипулятора
  virtual bool BeginDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType) override;
  // ksEndDragManipulator - окончание перетаскивания манипулятора
  virtual bool EndDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType) override;

  bool InitPoint3D(bool visible, bool fix);


protected:
  // ksFilterObjects - Фильтрация объектов
  virtual bool FilterObjects(const IModelObjectPtr & entity) override;
  // ksEndProcess - Окончание процесса.
  virtual bool EndProcess() override;
  virtual void UnadviseEvents() override;

  // ksEndProcess - Событие создания объекта в подчиненном режиме.
  virtual bool CreateTakeObject(const IModelObjectPtr & object) override;
  // ksPlacementChanged - Изменено положения объекта
  virtual bool PlacementChanged(const IModelObjectPtr & object) override;
  //Создать временное сопряжение в соответствии с указанной поверхностью
  bool MateProcess(const IModelObjectPtr & object);
};

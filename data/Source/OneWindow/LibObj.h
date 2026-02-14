////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "Resource.h"
#include "CPropMen.h"
#include "Events/OneWindowEvents.h"

#define NULL_EPSILON 1E-30 ///< погрешность для проверки на равенство нулю

struct Matrix3D;
class Process3DEvent;

////////////////////////////////////////////////////////////////////////////////
//
/// 3D точка
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct Point3D
{
public:
  double x; ///< Координата x
  double y; ///< Координата y
  double z; ///< Координата z

public:
  /// Конструктор
  Point3D();
  /// Конструктор
  Point3D(double _x, double _y, double _z);
  /// Конструктор копирования
  Point3D(const Point3D & other);
  /// Операция вычитания
  Point3D & operator-=(const Point3D & other);
  /// Трансформировать точку по матрице
  void Transform(const Matrix3D & mtr);
};


////////////////////////////////////////////////////////////////////////////////
//
/// 3D вектор
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct Vector3D
{
public:
  double x; ///< Координата x
  double y; ///< Координата y
  double z; ///< Координата z

public:
  /// Конструктор
  Vector3D()
    : x(0.0)
    , y(0.0)
    , z(0.0)
  {
  }

  /// Конструктор
  Vector3D(double _x, double _y, double _z)
    : x(_x)
    , y(_y)
    , z(_z)
  {
  }

  /// Конструктор копирования
  Vector3D(Vector3D & other)
    : x(other.x)
    , y(other.y)
    , z(other.z)
  {
  }
};


////////////////////////////////////////////////////////////////////////////////
//
/// Матрица 3D
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct Matrix3D
{
public:
  double el[4][4]; ///< Элементы матрицы

public:
  /// Конструктор для единичной матрицы
  Matrix3D() { Init(); }
  /**
    \brief Конструктор
    \param[in] original - Точка привязки
    \param[in] normal - Единичный вектор
    \param[in] axisX - Единичный вектор
    \param[in] axisY - Единичный вектор
  */
  Matrix3D(Point3D & original, Vector3D & normal, Vector3D & axisX, Vector3D & axisY);
  /**
    \brief Конструктор
    \param[in] surf - Математическая поверхность
    \param[in] isSameSense - Совпадает ли направление с нормалью поверхности или оно противоположное
    \param[in] u - параметр u
    \param[in] v - параметр v
  */
  Matrix3D(ksapi::IMathSurface3DPtr & surf, bool isSameSense, double u, double v);

  /// Для получения из SafeArray-я
  Matrix3D(double * el16);

  /// Получить единичный вектор X
  Vector3D & GetAxisX() { return reinterpret_cast<Vector3D &>(*el[0]); }
  /// Получить единичный вектор Y
  Vector3D & GetAxisY() { return reinterpret_cast<Vector3D &>(*el[1]); }
  /// Получить единичный вектор Z
  Vector3D & GetAxisZ() { return reinterpret_cast<Vector3D &>(*el[2]); }
  /// Получить точку привязки
  Point3D & GetOrigin() { return reinterpret_cast<Point3D &>(*el[3]); }
  /// Единичная матрица
  void Init();
  /// Инициализация по единичным векторам и точке привязки
  void Init(Point3D & original, Vector3D & normal, Vector3D & axisX, Vector3D & axisY);
  /**
    \brief Инициализация по параметрам математической поверхности
    \param[in] surf - Математическая поверхность
    \param[in] isSameSense - совпадает ли направление с нормалью поверхности или оно противоположное
    \param[in] u - параметр u
    \param[in] v - параметр v
  */
  void Init(ksapi::IMathSurface3D & surf, bool isSameSense, double u, double v);
  /// Получить оператор this = (B * A), Init( A, B ) != A * B !!!
  void Init(const Matrix3D & A, const Matrix3D & B);
  /// Умножение на матрицу
  void operator*=(const Matrix3D & m) { Multiply(m); }
  /// Чёткое сравнение (точность - Math3D::lengthEpsilon)
  bool operator==(const Matrix3D & m) const;
  /// Умножение на матрицу this = this * b;
  void Multiply(const Matrix3D & b);
  /// Обращение квадратной матрицы
  void Div(Matrix3D &) const;
  /// Получить список значений
  std::vector<double> GetValues();
};


////////////////////////////////////////////////////////////////////////////////
//
/// Параметры внешнего объекта с триангуляцией
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct ExternalTeselationParam
{
public:
  std::vector<double> m_points;        ///< Координаты вершин
  std::vector<double> m_normals;       ///< Координаты нормалей
  std::vector<std::int32_t> m_indices; ///< Индексы вершин

  ksapi::Color color;  ///< Цвет
  double ambient;      ///< Общий свет
  double diffuse;      ///< Диффузия
  double specularity;  ///< Зеркальность
  double shininess;    ///< Блеск
  double transparency; ///< Прозрачность
  double emission;     ///< Излучение
  bool edge;

public:
  /// Конструктор
  ExternalTeselationParam(ksapi::IModelObjectPtr & obj, ksapi::IPartPtr & topPart);
  /// Конструктор
  ExternalTeselationParam(std::vector<ExternalTeselationParam> & params);

  /// Проверка внешней триангуляции
  bool IsValid() { return transparency > 0.0; }
  /// Инициализировать по объекту внешней триангуляции
  void Init(ksapi::IExternalTessellationObjectPtr & obj);
  /// Инициализировать по грани
  void InitByFace(const ksapi::IFacePtr & faceDef);
  /// Инициализировать по ребру
  void InitByEdge(const ksapi::IEdgePtr & edgeDef);
  /// Упаковать параметры ребер в один элемент
  void PacEdges(std::vector<ExternalTeselationParam> & params);
};

////////////////////////////////////////////////////////////////////////////////
//
/// Параметры узла
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct TeselationNodeParam
{
public:
  std::vector<ksapi::IFacePtr> m_faces; ///< Стрелка
  std::vector<std::int32_t> m_iids;     ///< Идентификаторы внешних объектов для стрелок
  std::int32_t m_index;                 ///< Индекс
  std::wstring m_name;                  ///< Имя узла соответствующего стрелкам
  bool m_visible;                       ///< Видимость стрелок

public:
  /// Конструкций
  TeselationNodeParam()
    : m_index(0)
    , m_visible(false)
  {
  }
  /// Деструктор
  virtual ~TeselationNodeParam() = default;
  /// Получить индекс
  std::int32_t GetID() { return m_index; }
  /// Сохранение параметров в хранилище
  virtual void Save(const std::wstring & pasword, ksapi::IUserDataStoragePtr & s);
  /// Получение параметров из хранилища
  virtual bool Load(ksapi::IUserDataStoragePtr & s, std::int32_t version, size_t index);
};


////////////////////////////////////////////////////////////////////////////////
//
/// Параметры стрелки
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct ArrowParam : public TeselationNodeParam
{
  /// Конструктор
  ArrowParam()
    : TeselationNodeParam()
  {
  }

  /// Расчет параметров создания стрелок для одной грани
  void Make(const ksapi::IFacePtr & face, const ksapi::IDocumentFramePtr & frame,
            std::vector<ExternalTeselationParam *> & arrowParams);
  /// Расчет нагрузок
  void Make(ksapi::IDocumentFramePtr & frame, std::vector<ExternalTeselationParam *> & arrowParams);
};

using ArrowParamPtr = std::unique_ptr<ArrowParam>;

class PropertyTreeEventCallbacks
{
public:
  virtual void OnBeginTextEdit(const ksapi::IPropertyTreeNodePtr & node) = 0;
  virtual void OnEndTextEdit(const ksapi::IPropertyTreeNodePtr & node) = 0;
  virtual void OnSelectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes) = 0;
  virtual void OnUnselectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes) = 0;
  virtual void OnMenuCommand(std::int32_t id) {}
  virtual void OnContextMenuRequest(const ksapi::IPropertyTreeNodePtr & node) = 0;
};


class DocumentFrameEventCallback
{
public:
  /// Начало создания листа в контексте OpenGL
  virtual bool BeginPaintGL(const ksapi::IOpenGLObjectPtr & glObj, std::int32_t drawMode) { return true; }
  /// Определение габаритов документа
  virtual bool AddGabarit(ksapi::IGabaritObjectPtr & gabObj) { return true; }
  /// Закрытие окна
  virtual bool CloseFrame() { return true; }
};


////////////////////////////////////////////////////////////////////////////////
//
/// Базовый класс элемента
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class LibObj : public PropertyManagerObject,
               public DocumentFrameEventCallback,
               public PropertyTreeEventCallbacks
{
private:
  static LibObj * editObj; ///< Редактируемый объект

private:
  std::int32_t m_command;        ///< Команда
  std::int32_t m_arrowsIndex;    ///< Счетчик для стрелок
  std::int32_t m_currentCommand; ///< Текущая команда

  bool m_treeVisible; ///< Видимость дерева

  size_t m_documentFrameEvent; ///< События окна документа
  size_t m_treeViewEvent;      ///< События дерева
  size_t m_process3DEvent;     ///< События процесса
  size_t m_propMngEvent;       ///< События панели свойств

  ksapi::IPropertyTreePtr m_treeCtrl;                       ///< Дерево
  ksapi::IPropertyTabPtr m_tab;                             ///< Закладка панели свойств
  ksapi::IPropertyManagerPtr m_propMng;                     ///< Менеджер панели свойств
  ksapi::IPropertyTreeNodePtr m_arrrowsNode;                ///< Узел дерева
  std::vector<ksapi::IPropertyTreeNodePtr> m_selectedNodes; ///< Селектированные узлы

  ArrowParam * m_newArrow;                            ///< Новая стрелка
  std::vector<ExternalTeselationParam *> arrowParams; ///< Стрелка
  std::vector<ArrowParamPtr> m_arrows;                ///< Массив, содержащий нагрузки

public:
  /// Конструктор
  LibObj(const ksapi::IDocumentFramePtr & frame);
  /// Деструктор
  virtual ~LibObj();

  /// Завершение процесса
  virtual void EndProcess() override;

  /// Создание диформационной нагрузки
  void CreateForce();
  /// Показать скрыть дерево
  void OnOfTree();
  /// Пересчитать нагрузки
  void Refresh();
  /// Создание дерева нагрузок в окне документа
  void CreateTree();
  /// Удаление дерева нагрузок
  void CloseTree();
  /// Сохранить дерево в документ
  void SaveTree();
  /// Считать дерево из документа
  void LoadTree();
  /// Получить пользовательское хранилище по имени
  ksapi::IUserDataStoragePtr GetUserDataStorageByName(const std::wstring & treeName);

  /// Проверка существования окна документа
  bool IsMyFrame();
  /// Накачка дерева
  void FillTree(ksapi::IPropertyTreePtr & tree);
  /// Проверка узла нагрузки
  bool CheckIfArrowNode(ksapi::IPropertyTreeNodePtr node);
  /// Наполним узлы дерева
  void FillTreeNodes(std::vector<ArrowParamPtr> & nodeElements);

#pragma region PropertyManagerObject
  /// Нажатие кнопки.
  virtual bool OnButtonClick(std::int32_t buttonID) override;
  /// Нажатие кнопки контрола
  virtual bool OnControlCommand(std::int32_t ctrlID, std::int32_t buttonID) override;
  /// Установка состояния кнопки спецпанели
  virtual bool OnButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable) override;
  /// Начало создания листа в контексте OpenGL
  virtual bool BeginPaintGL(const ksapi::IOpenGLObjectPtr & glObj, std::int32_t drawMode) override;
  /// Определение габаритов документа
  virtual bool AddGabarit(ksapi::IGabaritObjectPtr & gabObj) override;
  /// Закрытие окна
  virtual bool CloseFrame() override;
#pragma endregion

#pragma region TreeEventCallBack
  virtual void OnSelectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes) override;
  virtual void OnUnselectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes) override;
  virtual void OnMenuCommand(std::int32_t Id) override;
  virtual void OnBeginTextEdit(const ksapi::IPropertyTreeNodePtr & node) override;
  virtual void OnEndTextEdit(const ksapi::IPropertyTreeNodePtr & node) override;
  virtual void OnContextMenuRequest(const ksapi::IPropertyTreeNodePtr & node) override;
#pragma endregion

  /// Функция обратной связи
  static bool SelectFace(const ksapi::IKompasAPIObjectPtr & object);
  /// Клик по узлу дерева
  void InitMenu(int menuType);
  /// Обработчик селектирования грани
  bool OnSelectFace(const ksapi::IFacePtr & face);
  /// Создание диформационной нагрузки
  void CreateForce(const ksapi::IFacePtr & face);
  /// Запуск процесса указания граней для создания нагрузок
  void SelectFacesForForce();
  /// Запуск процесса указания граней для создания нагузок
  void SelectFaces(std::int32_t procID, std::int32_t promtID);
  /// Включение\выключение и удаление отрисовки внешних объектов для заданного окна
  void OnOffTesselations(std::vector<std::int32_t> & iids, bool vis, bool oldVis, bool del);

  /// Добавление узла дерева
  bool AddNode(const std::wstring & nodeName, int index);
  /// Составить имя для узла
  std::wstring MakeNodeName(std::wstring name, int index);

  /// Проверка видимости стрелок
  bool IsArrowsVisible();
  /// Расчет триангуляций
  void MakeTessellations();
  /// Выключить внешнюю триангуляцию
  void OffAllTessellations();
  /// Загрузка триангуляции стрелки из файла
  void LoadArrowParams();

private:
  /// Очистка стрелок
  void ClearArrows();
};

/// Возвращает полное имя файла
std::wstring GetFullName(const std::wstring & inName);
/// Найти или создать объект библиотеки
LibObj * FindOrCreateLibObj(const ksapi::IDocumentFramePtr & docFrame, bool create);

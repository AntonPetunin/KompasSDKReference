#pragma once

////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include <stack>
#include <string>

using namespace ksapi;

struct PropertyParam;
class ParameterObject;


////////////////////////////////////////////////////////////////////////////////
//
/// Диспетчер активного документа
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Doc2DDispatcher
{
private:
  IKompasDocument2DPtr m_document;                    ///< Активный документ
  std::stack<IDrawingContainerPtr> m_containers;      ///< Контейнеры объектов
  std::stack<IDrawingGroupPtr> m_groups;              ///< Группы
  std::stack<IMacroObjectPtr> m_macroObjs;            ///< Макро-объекты
  IDrawingObjectPtr m_filling;                        ///< Штриховка или заливка
  std::vector<IDrawingObjectPtr> m_fillingBoundaries; ///< Границы штриховки или заливки

public:
  /// Конструктор
  Doc2DDispatcher();

  /// Установить активный документ
  void SetActiveDocument();
  /// Получить активный документ
  IKompasDocument2DPtr GetActiveDocument() const { return m_document; };
  /// Создать отрезок по координатам
  void CreateLineSegment(double x1, double y1, double x2, double y2, unsigned short style);
  /// Создать окружность по центру и радиусу
  void CreateCircle(double xc, double yc, double rad, unsigned short style);
  /// Создать дугу по трем точкам
  void CreateArcBy3Points(double x1, double y1, double x2, double y2, double x3, double y3, unsigned short style);
  /// Создать дугу по центру, радиусу и двум точкам
  void CreateArcByPoint(double xc, double yc, double rad, double x1, double y1, double x2, double y2, bool direction,
                        unsigned short style);
  /// Создать заливку
  void CreateColoring(std::int32_t color);
  /// Создать штриховку
  void CreateHatch(unsigned short style, double angle, double step, double width, double x0, double y0);
  /// Закрыть создание заливки или штриховки
  void CloseFilling();
  /// Создать группу
  IDrawingGroupPtr CreateDrawingGroup(bool tmpGroup);
  /// Добавить объект в группу
  void AddObjGroup(ksapi::IDrawingGroupPtr & group, ksapi::IMacroObjectPtr & macroObject);
  /// Открыть группу
  IDrawingGroupPtr OpenDrawingGroup(IDrawingGroupPtr & createdGroup);
  /// Очистить группу
  void ClearDrawingGroup(ksapi::IDrawingGroupPtr & group, bool deleteTmp = true);
  /// Закрыть редактирование группы
  IDrawingGroupPtr CloseGroup();
  /// Создать макро
  IMacroObjectPtr CreateMacroObject(bool multyLayer);
  /// Закрыть работу с макро
  IMacroObjectPtr CloseMacroObject();
  /// Снять выделение объекта
  void Unchoose(const IAPIObjectPtr & obj);
  /// Выделить объект
  void ChooseObj(const IAPIObjectPtr & obj);
  /// Подсветить объект
  void LightObj(ksapi::IPositionLeaderPtr & posLeader, bool choose);

private:
  /// Установить активный контейнер объектов
  void SetDocumentDrawingContainer();
  /// Получить активный контейнер объектов
  IDrawingContainerPtr GetContainer();
};


namespace math
{
/// Косинус
double CosD(double angle);
/// Синус
double SinD(double angle);
/// Тангенс
double TanD(double angle);
/// Пересечение двух отрезков
void IntersectLinSLinS(double x11, double y11, //первая точка первого отрезка
                       double x12, double y12, //вторая точка первого отрезка
                       double x21, double y21, //первая точка второго отрезка
                       double x22, double y22, //вторая точка второго отрезка
                       int & kp,               //количество точек пересечения
                       double & xp, double & yp);
/// Пересечение двух линий
void IntersectLinLin(double x1, double y1,      //точка на первой прямой
                     double angle1,             //угол первой прямой
                     double x2, double y2,      //точка на второй прямой
                     double angle2,             //угол второй прямой
                     int & kp,                  //количество точек пересечения
                     double & xp, double & yp); //точка пересечения

double Angle(double x1, double y1,  //первая точка
             double x2, double y2); //вторая точка
} // namespace math


namespace macro
{
/// Установить параметры макро-объекта
void SetMacroParam(IMacroObjectPtr macroObj,      // указатель на макроэлемент
                   void * userPars,               // указатель на буфер дополнительных параметров
                   unsigned int size,             // размер буфера дополнительных параметров
                   const std::wstring & fileName, // имя файла библиотеки, в которой находтся функция редактирования
                   const std::wstring & libName,  // имя библиотеки, в которой находтся функция редактирования
                   int command, bool hotPointEditable, bool propObjEditable);
/// Получить параметры макро-объекта
int GetMacroParams(IMacroObjectPtr macroObj, std::wstring * fileName = nullptr, std::wstring * libName = nullptr);
/// Обновить макро
void UpdateMacro(IMacroObjectPtr & macroObject, IDrawingGroupPtr & group);
/// Получить редактируемый макро
IMacroObjectPtr GetEditMacroObject();
/// Получить пользовательские параметры
bool GetMacroParam(ParameterObject & paramObj, const IMacroObjectPtr & macroObj);
/// Получить положение и угол макро
bool GetMacroPlacement(ksapi::IMacroObjectPtr & macroObj, double & x, double & y, double & angle, bool & mirrorSymetry);
} // namespace macro

/// Конвертировать строку из std::string в std::wstring
std::wstring GetWStringFromString(const std::string & str);
/// Конвертировать строку из std::wstring в std::string
std::string GetStringFromWString(const std::wstring & wstr);

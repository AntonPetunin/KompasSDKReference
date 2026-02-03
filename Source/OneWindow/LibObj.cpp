////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Базовый класс для элементов
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "OneWindow_pch.h"

#include "LibObj.h"
#include <math.h>
#include "OneWindow.h"

#include <algorithm>
#include <variant>
#include <format>
#include <ranges>
#include <filesystem>
#include "CPropMen.h"

#include "GL/glew.h"

#pragma comment(lib, "opengl32.lib")

#define LENGTH_EPSILON 1E-10 ///< Погрешность длины

extern ksapi::IApplication * application; ///< Интерфейс приложения Компас
std::wstring handlerOwner = L"OneWindow"; ///< Имя владельца событий
std::vector<LibObj *> m_libObjects;       ///< Список объектов библиотеки
LibObj * LibObj::editObj = nullptr;       ///< Редактируемый объект

#define SELECT      "SELECT"
#define ROOT        "ROOT"
#define FORCES      "FORCES"
#define FOLDER      "FOLDER"
#define EXPAND      "EXPAND"
#define STRAINS     "STRAINS"
#define FORCES_TREE L"FORCES TREE"

#define ARROW_M3D  L"arrow.m3d"
#define STRAIN_M3D L"part_def.m3d"

#define LIBVERSION 1l


//------------------------------------------------------------------------------
/**
  Очистить список объектов библиотеки
*/
//---
void ClearLibObjs()
{
  for (size_t i = 0, c = m_libObjects.size(); i < c; i++)
  {
    if (m_libObjects[i])
      delete m_libObjects[i];
  }

  m_libObjects.clear();
}


//------------------------------------------------------------------------------
/**
  Найти или создать объект библиотеки
*/
//---
LibObj * FindOrCreateLibObj(const ksapi::IDocumentFramePtr & docFrame, bool create)
{
  LibObj * res = nullptr;

  if (docFrame)
  {
    LibObj * next = nullptr;

    for (size_t i = 0, count = m_libObjects.size(); i < count; i++)
    {
      next = m_libObjects[i];

      if (next && next->IsMyFrame())
      {
        res = next;
        break;
      }
    }

    if (!res && create)
    {
      res = new LibObj(docFrame);
      m_libObjects.emplace_back(res);
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Извлечь координаты точки
*/
//---
void ExtractPoints(const std::vector<Point3D> & points, std::vector<double> & outValue)
{
  for (size_t i = 0, c = points.size(); i < c; i++)
  {
    outValue.emplace_back(points[i].x);
    outValue.emplace_back(points[i].y);
    outValue.emplace_back(points[i].z);
  }
}


//------------------------------------------------------------------------------
/**
  Возвращает полное имя файла
*/
//---
std::wstring GetFullName(const std::wstring & inName)
{
  std::wstring outName;

  if (ksapi::ILibraryManagerPtr libMng = application ? application->GetLibraryManager() : nullptr)
  {
    if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = procLibs->GetItemByName(L"OneWindow"))
        outName = lib->GetFullPath();
    }
  }

  if (!outName.empty())
  {
    auto fullName = std::filesystem::path(outName).parent_path();
    bool res = std::filesystem::exists(fullName.append(inName));

    if (!res)
      bool res = std::filesystem::exists(fullName.append(L"Resources").append(inName));

    outName = res ? fullName.wstring() : L"";
  }

  return outName;
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Point3D::Point3D()
  : x(0.0)
  , y(0.0)
  , z(0.0)
{
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Point3D::Point3D(double _x, double _y, double _z)
  : x(_x)
  , y(_y)
  , z(_z)
{
}


//------------------------------------------------------------------------------
/**
  Конструктор копирования
*/
//---
Point3D::Point3D(const Point3D & other)
  : x(other.x)
  , y(other.y)
  , z(other.z)
{
}


//------------------------------------------------------------------------------
/**
  Операция вычитания
*/
//---
Point3D & Point3D::operator-=(const Point3D & other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}


//------------------------------------------------------------------------------
/**
  Трансформировать точку по матрице
*/
//---
void Point3D::Transform(const Matrix3D & mtr)
{
  double xx = x * mtr.el[0][0] + y * mtr.el[1][0] + z * mtr.el[2][0] + mtr.el[3][0];
  double yy = x * mtr.el[0][1] + y * mtr.el[1][1] + z * mtr.el[2][1] + mtr.el[3][1];
  double zz = x * mtr.el[0][2] + y * mtr.el[1][2] + z * mtr.el[2][2] + mtr.el[3][2];

  x = xx;
  y = yy;
  z = zz;
}


//------------------------------------------------------------------------------
/**
  Единичная матрица
*/
//---
void Matrix3D::Init()
{
  el[0][1] = el[0][2] = el[0][3] = 0.0;
  el[1][0] = el[1][2] = el[1][3] = 0.0;
  el[2][0] = el[2][1] = el[2][3] = 0.0;
  el[3][0] = el[3][1] = el[3][2] = 0.0;
  el[0][0] = 1.0;
  el[1][1] = 1.0;
  el[2][2] = 1.0;
  el[3][3] = 1.0;
}


//------------------------------------------------------------------------------
/**
  Конструктор
  el16 указатель на массив double[16]
*/
//---
Matrix3D::Matrix3D(double * el16)
{
  el[0][0] = el16[0];
  el[0][1] = el16[1];
  el[0][2] = el16[2];
  el[0][3] = el16[3];
  el[1][0] = el16[4];
  el[1][1] = el16[5];
  el[1][2] = el16[6];
  el[1][3] = el16[7];
  el[2][0] = el16[8];
  el[2][1] = el16[9];
  el[2][2] = el16[10];
  el[2][3] = el16[11];
  el[3][0] = el16[12];
  el[3][1] = el16[13];
  el[3][2] = el16[14];
  el[3][3] = el16[15];
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Matrix3D::Matrix3D(ksapi::IMathSurface3DPtr & surf, bool isSameSense,
                   /* совпадает ли направление с нормалью поверхности или оно противоположное */ double u,
                   /* Единичный вектор */ double v) // Единичный вектор
{
  if (surf)
    Init(*surf, isSameSense, u, v);
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Matrix3D::Matrix3D(Point3D & original, Vector3D & normal, Vector3D & axisX, Vector3D & axisY)
{
  Init(original, normal, axisX, axisY);
}


//------------------------------------------------------------------------------
/**
  Инициализация по единичным векторам и точке привязки
*/
//---
void Matrix3D::Init(Point3D & original, Vector3D & normal, Vector3D & axisX, Vector3D & axisY)
{
  GetOrigin() = original;
  GetAxisX() = axisX;
  GetAxisY() = axisY;
  GetAxisZ() = normal;
}


//------------------------------------------------------------------------------
/**
  Инициализация по параметрам математической поверхности
  \param[in] surf - Математическая поверхность
  \param[in] isSameSense - совпадает ли направление с нормалью поверхности или оно противоположное
  \param[in] u - параметр u
  \param[in] v - параметр v
*/
//---
void Matrix3D::Init(ksapi::IMathSurface3D & surf, bool isSameSense, double u, double v)
{
  Init();
  Point3D & original = GetOrigin();
  surf.GetPoint(u, v, original.x, original.y, original.z);
  Vector3D & ax = GetAxisX();
  surf.GetTangentVectorU(u, v, ax.x, ax.y, ax.z); // Косательный вектор по U

  Vector3D & ay = GetAxisY();
  surf.GetTangentVectorV(u, v, ay.x, ay.y, ay.z); // Косательный вектор по V

  Vector3D & az = GetAxisZ();
  surf.GetNormal(u, v, az.x, az.y, az.z); // Нормаль в точке uv
  if (!isSameSense)                       // Нужно развернуть ось z и ось y вокруг оси x
  {
    // Развернем ось Z
    az.x = -az.x;
    az.y = -az.y;
    az.z = -az.z;
    // Развернем ось Y
    ay.x = -ay.x;
    ay.y = -ay.y;
    ay.z = -ay.z;
  }
}


//------------------------------------------------------------------------------
/**
  проверка на равенство матриц
*/
//---
bool Matrix3D::operator==(const Matrix3D & m) const
{
  bool bRes = true;
  for (int i = 0; (i < 4) && bRes; i++)
  {
    bRes = ((::fabs(el[i][0] - m.el[i][0]) <= LENGTH_EPSILON) && (::fabs(el[i][1] - m.el[i][1]) <= LENGTH_EPSILON) &&
            (::fabs(el[i][2] - m.el[i][2]) <= LENGTH_EPSILON) && (::fabs(el[i][3] - m.el[i][3]) <= LENGTH_EPSILON));
  }
  return bRes;
}


Matrix3D staticMatrix; ///< для ускорения вычислений

//------------------------------------------------------------------------------
/**
  Умножение на матрицу this = this * b;
*/
//---
void Matrix3D::Multiply(const Matrix3D & b)
{
  staticMatrix.Init(b, *this);
  ::memcpy(el, staticMatrix.el, sizeof(el));
}

//------------------------------------------------------------------------------
/**
  Получить оператор this = (B * A), Init( A, B ) != A * B !!!
*/
//---
void Matrix3D::Init(const Matrix3D & A, const Matrix3D & B)
{
  el[0][0] = (A.el[0][0] * B.el[0][0] + A.el[1][0] * B.el[0][1] + A.el[2][0] * B.el[0][2]);
  el[0][1] = (A.el[0][1] * B.el[0][0] + A.el[1][1] * B.el[0][1] + A.el[2][1] * B.el[0][2]);
  el[0][2] = (A.el[0][2] * B.el[0][0] + A.el[1][2] * B.el[0][1] + A.el[2][2] * B.el[0][2]);

  el[1][0] = (A.el[0][0] * B.el[1][0] + A.el[1][0] * B.el[1][1] + A.el[2][0] * B.el[1][2]);
  el[1][1] = (A.el[0][1] * B.el[1][0] + A.el[1][1] * B.el[1][1] + A.el[2][1] * B.el[1][2]);
  el[1][2] = (A.el[0][2] * B.el[1][0] + A.el[1][2] * B.el[1][1] + A.el[2][2] * B.el[1][2]);

  el[2][0] = (A.el[0][0] * B.el[2][0] + A.el[1][0] * B.el[2][1] + A.el[2][0] * B.el[2][2]);
  el[2][1] = (A.el[0][1] * B.el[2][0] + A.el[1][1] * B.el[2][1] + A.el[2][1] * B.el[2][2]);
  el[2][2] = (A.el[0][2] * B.el[2][0] + A.el[1][2] * B.el[2][1] + A.el[2][2] * B.el[2][2]);

  el[3][0] = (A.el[0][0] * B.el[3][0] + A.el[1][0] * B.el[3][1] + A.el[2][0] * B.el[3][2]);
  el[3][1] = (A.el[0][1] * B.el[3][0] + A.el[1][1] * B.el[3][1] + A.el[2][1] * B.el[3][2]);
  el[3][2] = (A.el[0][2] * B.el[3][0] + A.el[1][2] * B.el[3][1] + A.el[2][2] * B.el[3][2]);


  el[3][0] += (A.el[3][0] * B.el[3][3]);
  el[3][1] += (A.el[3][1] * B.el[3][3]);
  el[3][2] += (A.el[3][2] * B.el[3][3]);

  el[0][3] = 0.0;
  el[1][3] = 0.0;
  el[2][3] = 0.0;

  el[3][3] = (A.el[3][3] * B.el[3][3]);
}


//------------------------------------------------------------------------------
/**
  Обращение квадратной матрицы
*/
//---
void Matrix3D::Div(Matrix3D & m) const
{
  m.Init(); // делаем единичной матрицу

  double a11 = (el[1][1] * (el[2][3] * el[3][2] - el[2][2] * el[3][3]) + el[1][2] * (el[2][1] * el[3][3] - el[2][3] * el[3][1]) +
                el[1][3] * (el[2][2] * el[3][1] - el[2][1] * el[3][2]));
  double a12 = (el[1][0] * (el[2][2] * el[3][3] - el[2][3] * el[3][2]) + el[1][2] * (el[2][3] * el[3][0] - el[2][0] * el[3][3]) +
                el[1][3] * (el[2][0] * el[3][2] - el[2][2] * el[3][0]));
  double a13 = (el[1][0] * (el[2][3] * el[3][1] - el[2][1] * el[3][3]) + el[1][1] * (el[2][0] * el[3][3] - el[2][3] * el[3][0]) +
                el[1][3] * (el[2][1] * el[3][0] - el[2][0] * el[3][1]));
  double a14 = (el[1][0] * (el[2][1] * el[3][2] - el[2][2] * el[3][1]) + el[1][1] * (el[2][2] * el[3][0] - el[2][0] * el[3][2]) +
                el[1][2] * (el[2][0] * el[3][1] - el[2][1] * el[3][0]));

  double det = (el[0][0] * a11 + el[0][1] * a12 + el[0][2] * a13 + el[0][3] * a14);

  if (::fabs(det) > LENGTH_EPSILON)
  {
    det = (1.0 / det);

    m.el[0][0] = (a11 * det);
    m.el[1][0] = (a12 * det);
    m.el[2][0] = (a13 * det);
    m.el[3][0] = (a14 * det);

    m.el[0][1] =
      ((el[0][1] * (el[2][2] * el[3][3] - el[2][3] * el[3][2]) + el[0][2] * (el[2][3] * el[3][1] - el[2][1] * el[3][3]) +
        el[0][3] * (el[2][1] * el[3][2] - el[2][2] * el[3][1])) *
       det);
    m.el[1][1] =
      ((el[0][0] * (el[2][3] * el[3][2] - el[2][2] * el[3][3]) + el[0][2] * (el[2][0] * el[3][3] - el[2][3] * el[3][0]) +
        el[0][3] * (el[2][2] * el[3][0] - el[2][0] * el[3][2])) *
       det);
    m.el[2][1] =
      ((el[0][0] * (el[2][1] * el[3][3] - el[2][3] * el[3][1]) + el[0][1] * (el[2][3] * el[3][0] - el[2][0] * el[3][3]) +
        el[0][3] * (el[2][0] * el[3][1] - el[2][1] * el[3][0])) *
       det);
    m.el[3][1] =
      ((el[0][0] * (el[2][2] * el[3][1] - el[2][1] * el[3][2]) + el[0][1] * (el[2][0] * el[3][2] - el[2][2] * el[3][0]) +
        el[0][2] * (el[2][1] * el[3][0] - el[2][0] * el[3][1])) *
       det);

    m.el[0][2] =
      ((el[0][1] * (el[1][3] * el[3][2] - el[1][2] * el[3][3]) + el[0][2] * (el[1][1] * el[3][3] - el[1][3] * el[3][1]) +
        el[0][3] * (el[1][2] * el[3][1] - el[1][1] * el[3][2])) *
       det);
    m.el[1][2] =
      ((el[0][0] * (el[1][2] * el[3][3] - el[1][3] * el[3][2]) + el[0][2] * (el[1][3] * el[3][0] - el[1][0] * el[3][3]) +
        el[0][3] * (el[1][0] * el[3][2] - el[1][2] * el[3][0])) *
       det);
    m.el[2][2] =
      ((el[0][0] * (el[1][3] * el[3][1] - el[1][1] * el[3][3]) + el[0][1] * (el[1][0] * el[3][3] - el[1][3] * el[3][0]) +
        el[0][3] * (el[1][1] * el[3][0] - el[1][0] * el[3][1])) *
       det);
    m.el[3][2] =
      ((el[0][0] * (el[1][1] * el[3][2] - el[1][2] * el[3][1]) + el[0][1] * (el[1][2] * el[3][0] - el[1][0] * el[3][2]) +
        el[0][2] * (el[1][0] * el[3][1] - el[1][1] * el[3][0])) *
       det);

    m.el[0][3] =
      ((el[0][1] * (el[1][2] * el[2][3] - el[1][3] * el[2][2]) + el[0][2] * (el[1][3] * el[2][1] - el[1][1] * el[2][3]) +
        el[0][3] * (el[1][1] * el[2][2] - el[1][2] * el[2][1])) *
       det);
    m.el[1][3] =
      ((el[0][0] * (el[1][3] * el[2][2] - el[1][2] * el[2][3]) + el[0][2] * (el[1][0] * el[2][3] - el[1][3] * el[2][0]) +
        el[0][3] * (el[1][2] * el[2][0] - el[1][0] * el[2][2])) *
       det);
    m.el[2][3] =
      ((el[0][0] * (el[1][1] * el[2][3] - el[1][3] * el[2][1]) + el[0][1] * (el[1][3] * el[2][0] - el[1][0] * el[2][3]) +
        el[0][3] * (el[1][0] * el[2][1] - el[1][1] * el[2][0])) *
       det);
    m.el[3][3] =
      ((el[0][0] * (el[1][2] * el[2][1] - el[1][1] * el[2][2]) + el[0][1] * (el[1][0] * el[2][2] - el[1][2] * el[2][0]) +
        el[0][2] * (el[1][1] * el[2][0] - el[1][0] * el[2][1])) *
       det);
  }
}


//------------------------------------------------------------------------------
/**
  Получить список значений
*/
//---
std::vector<double> Matrix3D::GetValues()
{
  std::vector<double> values;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
      values.push_back(el[i][j]);
  }

  return values;
}


#pragma warning(disable : 4786)
//------------------------------------------------------------------------------
/**
  Конструктор для получения триангуляции грани и полигона для кривой
*/
//---
ExternalTeselationParam::ExternalTeselationParam(ksapi::IModelObjectPtr & obj, ksapi::IPartPtr & topPart)
  : color(0)        // Цвет
  , ambient(1)      // общий свет
  , diffuse(1)      // диффузия
  , specularity(1)  // зеркальность
  , shininess(1)    // блеск
  , transparency(1) // прозрачность
  , emission(1)     // излучение
  , edge(false)     // Ребро или грань
{
  if (obj)
  {
    if (obj->GetType() == ksObjectFace)
    {
      if (ksapi::IColorParamPtr colorParam = obj)
        colorParam->GetAdvancedColor(color, ambient, diffuse, specularity, shininess, transparency, emission);

      if (transparency > 0.001) // Если объект не абсолютно прозрачен
      {
        if (ksapi::IFacePtr face = obj)
          InitByFace(face);
      }
    }
    else if (obj->GetType() == o3d_edge)
    {
      if (ksapi::IEdgePtr edgeDef = obj)
      {
        if (ksapi::IFacePtr face = edgeDef->GetAdjacentFace(false))
        {
          InitByEdge(edgeDef);
          edge = true;
          color = 0;
        }
      }
    }

    ksapi::IPartPtr parentPart;

    if (topPart)
    {
      parentPart = obj->GetParent();

      if (topPart == parentPart)
        parentPart = nullptr;
    }

    if (parentPart)
    {
      if (!m_points.empty())
        topPart->TransformPoints(m_points, parentPart);

      if (!m_normals.empty())
      {
        double x0 = 0.0, y0 = 0.0, z0 = 0.0;
        topPart->TransformPoint(x0, y0, z0, parentPart);

        if (fabs(x0) > LENGTH_EPSILON || fabs(y0) > LENGTH_EPSILON || fabs(z0) > LENGTH_EPSILON)
        {
          topPart->TransformPoints(m_normals, parentPart);
          Point3D p0(x0, y0, z0);

          for (size_t i = 0, count = m_normals.size(); i < count; i += 3)
          {
            m_normals[i] -= p0.x;
            m_normals[i + 1] -= p0.y;
            m_normals[i + 2] -= p0.z;
          }
        }
      }
    }
  }
}
#pragma warning(once : 4786)

//------------------------------------------------------------------------------
/**
  Упаковать все ребра в один внешний объект
*/
//---
ExternalTeselationParam::ExternalTeselationParam(std::vector<ExternalTeselationParam> & params)
  : color(0)        // Цвет
  , ambient(1)      // общий свет
  , diffuse(1)      // диффузия
  , specularity(1)  // зеркальность
  , shininess(1)    // блеск
  , transparency(1) // прозрачность
  , emission(1)     // излучение
  , edge(true)      // Ребро или грань
{
  PacEdges(params);
}


//------------------------------------------------------------------------------
/**
  Инициализировать по объекту внешней триангуляции
*/
//---
void ExternalTeselationParam::Init(ksapi::IExternalTessellationObjectPtr & obj)
{
  if (obj)
  {
    std::vector<float> points;
    std::ranges::copy(std::views::transform(m_points, [](double value) { return static_cast<float>(value); }),
                      std::back_inserter(points));
    std::vector<float> normals;
    std::ranges::copy(std::views::transform(m_normals, [](double value) { return static_cast<float>(value); }),
                      std::back_inserter(normals));

    if (edge)
      obj->SetEdges(points, m_indices, {} /*colors*/);
    else
      obj->SetTessellation(points, m_indices, normals, {} /*colors*/);

    obj->SetAdvancedColor(color, ambient, diffuse, specularity, shininess, transparency, emission);
  }
}


//------------------------------------------------------------------------------
/**
  Получение параметров триангуляции
*/
//---
void ExternalTeselationParam::InitByFace(const ksapi::IFacePtr & faceDef)
{
  if (faceDef)
  {
    bool plane = faceDef->IsPlanar();

    if (ksapi::ITessellationPtr tess = faceDef->GetTessellation())
      tess->GetFacetPoints(m_points, m_indices, m_normals);
  }
}


//------------------------------------------------------------------------------
/**
  Инициализировать по ребру
*/
//---
void ExternalTeselationParam::InitByEdge(const ksapi::IEdgePtr & edgeDef)
{
  if (edgeDef)
  {
    if (ksapi::IMathCurve3DPtr curve = edgeDef->GetMathCurve())
    {
      // Если это отрезок то будем рисовать его по двум точкам
      if (curve->IsLineSeg())
      {
        std::vector<double> p;
        double x, y, z;
        curve->GetPoint(curve->GetParamMin(), x, y, z);
        std::ranges::copy(std::vector{x, y, z}, std::back_inserter(p));
        curve->GetPoint(curve->GetParamMax(), x, y, z);
        std::ranges::copy(std::vector{x, y, z}, std::back_inserter(p));
        m_points = p;
      }
      else
      {
        // Расчитываем полигон для кривой
        std::vector<float> points = curve->CalculatePolygon(0 /*step*/);
        m_points = {points.begin(), points.end()};
      }

      if (!m_points.empty())
      {
        std::vector<std::int32_t> ind;
        std::int32_t count = static_cast<std::int32_t>(m_points.size()) / 3;
        ind.push_back(count);

        for (std::int32_t i = 0; i < count; i++)
          ind.push_back(i);

        m_indices = ind;
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Упаковать параметры ребер в один элемент
*/
//---
void ExternalTeselationParam::PacEdges(std::vector<ExternalTeselationParam> & params)
{
  std::vector<double> p;
  std::vector<std::int32_t> edges;

  for (size_t i = 0, count = params.size(); i < count; i++)
  {
    if (params[i].edge)
    {
      size_t oldCount = p.size();
      p = params[i].m_points;
      size_t newCount = p.size();
      edges.push_back(static_cast<std::int32_t>(newCount - oldCount));

      for (size_t j = oldCount; j < newCount; j++)
        edges.push_back(static_cast<std::int32_t>(j)); // новый индекс
    }
  }

  m_points = p;
  m_indices = edges;
}


//------------------------------------------------------------------------------
/**
  Получение триангуляции из компонента
*/
//---
void LoadTessellations(std::vector<ExternalTeselationParam *> & params, ksapi::IPartPtr & topPart,
                       const ksapi::IPartPtr & nextPart)
{
  if (nextPart)
  {
    ksapi::IPoint3DPtr pointCS(nextPart->GetDefaultObject(o3d_pointCS));

    if (pointCS)
    {
      pointCS->SetHidden(true);
      pointCS->Update();
    }

    std::vector<ksapi::IModelObjectPtr> objs;

    if (ksapi::IModelContainerPtr cont = nextPart)
      objs = cont->GetObjects(o3d_face);

    if (!objs.empty())
    {
      for (size_t i = 0, count = objs.size(); i < count; i++)
      {
        ksapi::IModelObjectPtr obj(objs[i]);
        if (obj)
        {
          ExternalTeselationParam * param = new ExternalTeselationParam(obj, topPart);

          if (param->IsValid())
            params.emplace_back(param);
          else
            delete param;
        }
      }
    }

    if (ksapi::IModelContainerPtr cont = nextPart)
    {
      objs = cont->GetObjects(o3d_edge);

      if (objs.empty())
      {
        for (size_t i = 0, count = objs.size(); i < count; i++)
        {
          if (objs[i])
          {
            ExternalTeselationParam * param = new ExternalTeselationParam(objs[i], topPart);
            if (param->IsValid())
              params.emplace_back(param);
            else
              delete param;
          }
        }
      }
    }

    if (!nextPart->IsDetail())
    {
      // Получаем все вставки в том числе и из массивов и макро
      std::vector<ksapi::IPartPtr> partsArr = nextPart->GetPartsArray(ksAllParts);

      for (size_t i = 0, count = partsArr.size(); i < count; i++)
        LoadTessellations(params, topPart, partsArr[i]);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Загрузка триангуляции из файла
*/
//---
bool LoadTessellations(std::vector<ExternalTeselationParam *> & params, const std::wstring & fileName)
{
  // Для примера загружаем триангуляцию с другой модели
  if (fileName.length() > 0 && application)
  {
    if (ksapi::IDocumentsPtr docs = application->GetDocuments())
    {
      if (ksapi::IKompasDocument3DPtr doc3D = docs->Open(fileName, false, false))
      {
        { // Освободим интерфейсы перед закрытием документа
          if (ksapi::IPartPtr topPart = doc3D->GetTopPart())
            LoadTessellations(params, topPart, topPart);
        }

        doc3D->Close(kdDoNotSaveChanges);
      }
    }
  }

  return !params.empty();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
LibObj::LibObj(const ksapi::IDocumentFramePtr & frame)
  : PropertyManagerObject()
  , m_command(0)
  , m_documentFrameEvent(0)
  , m_treeViewEvent(0)
  , m_process3DEvent(0)
  , m_propMngEvent(0)
  , m_arrowsIndex(1)
  , m_newArrow(nullptr)
  , m_treeVisible(true)
  , m_currentCommand(-1)

{
  if (frame)
  {
    m_documentFrameEvent = m_events.Add(new DocumentFrameEvent(handlerOwner, *frame, *this));
    LoadTree();
  }
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
LibObj::~LibObj()
{
  CloseTree();
  OffAllTessellations();
  m_events.Clear();
  auto eraseIt = std::ranges::remove(m_libObjects, this);
  m_libObjects.erase(eraseIt.begin(), eraseIt.end());
  ClearArrows();
}


//------------------------------------------------------------------------------
/**
  Завершение процесса
*/
//---
void LibObj::EndProcess()
{
  m_events.RemoveWithUnadvise(m_process3DEvent);
  m_process3DEvent = 0;

  PropertyManagerObject::EndProcess();
}

//------------------------------------------------------------------------------
/**
  Выключить внешнюю триангуляцию
*/
//---
void LibObj::OffAllTessellations()
{
  for (size_t i = 0, count = m_arrows.size(); i < count; i++)
    OnOffTesselations(m_arrows[i]->m_iids, false, m_arrows[i]->m_visible, true);
}


//------------------------------------------------------------------------------
/**
  Выполнить команду библиотеки
*/
//---
void ExecuteKompasLibraryCommand(std::int32_t commandId)
{
  if (application)
  {
    ksapi::ILibraryManagerPtr mng = application->GetLibraryManager();
    ksapi::IProceduresLibrariesPtr libs = mng ? mng->GetProceduresLibraries() : nullptr;

    if (auto item = libs->GetItemByName(LoadStr(IDR_LIBID)))
      item->Execute(commandId, true, nullptr);
  }
}


//------------------------------------------------------------------------------
/**
  Обработка нажатия кнопок
*/
//---
bool LibObj::OnButtonClick(std::int32_t buttonID)
{
  bool res = true;

  switch (buttonID)
  {
    case pbEnter:
    {
      if (m_newArrow && m_command)
      {
        m_newArrow->m_index = m_arrowsIndex;
        AddNode(MakeNodeName(LoadStr(IDP_FORCE), m_arrowsIndex), m_arrowsIndex);
        m_arrowsIndex++;
        m_newArrow = nullptr;
      }

      // Процесс не завершаем
      res = false;
      break;
    }

    case pbEsc:
    {
      if (editObj)
      {
        if (m_newArrow)
        {
          OnOffTesselations(m_newArrow->m_iids, false, true, true); // Удалить отрисовку элементов, которых нет в дереве

          m_arrows.erase(std::remove_if(m_arrows.begin(), m_arrows.end(),
                                        [this](const ArrowParamPtr & param) { return param.get() == m_newArrow; }),
                         m_arrows.end());
          m_newArrow = nullptr;
        }

        ksapi::IKompasDocument3DPtr doc3D = application ? application->GetActiveDocument() : nullptr;

        if (ksapi::IDocumentFramePtr frame = doc3D->GetDocumentFrame())
          m_process->Stop();
      }
      else
      {
        SaveTree();
        CloseTree();
        OffAllTessellations();

        m_events.RemoveWithUnadvise(m_documentFrameEvent);
        m_documentFrameEvent = 0;
      }

      break;
    }

    case IDM_BUTTON1_ID:
    {
      CreateForce();
      break;
    }

    case IDM_BUTTON2_ID:
    {
      ExecuteKompasLibraryCommand(4);
      break;
    }

    case IDM_BUTTON3_ID:
    {
      ExecuteKompasLibraryCommand(5);
      break;
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки контрола
*/
//---
bool LibObj::OnControlCommand(std::int32_t ctrlID, std::int32_t buttonID)
{
  OnMenuCommand(buttonID);
  return true;
}


//------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели
*/
//---
bool LibObj::OnButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable)
{
  switch (buttonID)
  {
    case IDM_BUTTON1_ID:
      enable = !m_newArrow;
      break;
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Создание диформационной нагрузки
*/
//---
void LibObj::CreateForce()
{
  m_command = 1;
  if (!m_propMng)
    CreateTree();

  SelectFacesForForce();
  SaveTree();
}


//------------------------------------------------------------------------------
/**
  Получить файл шрифта
*/
//---
std::wstring GetLibraryIconFont()
{
  static std::wstring iconfont;

  if (iconfont.empty() && application)
  {
    if (ksapi::ILibraryManagerPtr mng = application->GetLibraryManager())
    {
      ksapi::IProceduresLibrariesPtr libs = mng->GetProceduresLibraries();
      ksapi::IProceduresLibraryPtr lib = libs ? libs->GetItemByName(LoadStr(IDR_LIBID)) : nullptr;

      if (lib)
        iconfont = lib->GetIconsFont();
    }
  }

  return iconfont;
}


//------------------------------------------------------------------------------
/**
  Создание дерева нагрузок в окне документа
*/
//---
void LibObj::CreateTree()
{
  m_propMng = application->CreatePropertyManager(false);

  if (m_propMng)
  {
    m_propMng->SetLayout(pmAlignRight);
    m_propMng->SetCaption(LoadStr(IDS_TREE_PANEL));
    m_propMng->SetSpecToolbar(pnEscHelp);

    std::wstring font = GetLibraryIconFont();

    m_propMng->AddSpecToolbarButton(IDM_BUTTON1_ID, IDM_BUTTON1_ICON, LoadStr(IDS_BUTTON1_TIPS), font);
    m_propMng->AddSpecToolbarButton(IDM_BUTTON2_ID, IDM_BUTTON2_ICON, LoadStr(IDS_BUTTON2_TIPS), L"");
    m_propMng->AddSpecToolbarButton(IDM_BUTTON3_ID, IDM_BUTTON3_ICON, LoadStr(IDS_BUTTON3_TIPS), font);

    m_propMngEvent = m_events.Add(new PropertyManagerEvent(handlerOwner, *m_propMng, *this));
    m_tab = m_propMng->GetPropertyTabs()->Add(LoadStr(IDS_TREE_TAB));
    // Получаем коллекцию контролов
    ksapi::IPropertyControlsPtr collection = m_tab->GetPropertyControls();

    if (!m_treeCtrl && m_treeVisible)
    {
      m_treeCtrl = collection->Add(ksControlTree);
      if (m_treeCtrl)
      {
        FillTree(m_treeCtrl); // Наполним дерево

        m_treeCtrl->SetName(LoadStr(IDS_TREE));
        m_treeCtrl->SetMinHeight(100);
        m_treeCtrl->SetResizable(true);
        m_treeCtrl->SetId(1000);
        m_treeCtrl->SetAutoSize(true);
        m_treeCtrl->SetMultiSelect(true);
        m_treeViewEvent = m_events.Add(new PropertyTreeEvent(handlerOwner, *m_treeCtrl, *this));
      }

      m_propMng->ShowTabs();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Удаление дерева нагрузок
*/
//---
void LibObj::CloseTree()
{
  m_events.RemoveWithUnadvise(m_treeViewEvent);
  m_treeViewEvent = 0;

  if (m_propMng)
  {
    m_events.RemoveWithUnadvise(m_propMngEvent);
    m_propMngEvent = 0;
    m_propMng->HideTabs();
    m_treeCtrl = nullptr;
    m_propMng = nullptr;
  }
}


//------------------------------------------------------------------------------
/**
  Показать\скрыть дерево
*/
//---
void LibObj::OnOfTree()
{
  if (m_treeVisible = m_treeCtrl && m_treeVisible ? false : true)
  {
    CreateTree();
    if (m_treeCtrl)
      m_propMng->SetVisible(m_treeVisible);
  }
  else
  {
    CloseTree();
  }
}


//------------------------------------------------------------------------------
/**
  Проверка существования окна документа
*/
//---
bool LibObj::IsMyFrame()
{
  ksapi::IKompasDocumentPtr doc = application ? application->GetActiveDocument() : nullptr;
  ksapi::IDocumentFramePtr frame = doc ? doc->GetDocumentFrame() : nullptr;
  IOneWindowEvent * frameEvent = !!m_documentFrameEvent ? m_events.GetEvent(m_documentFrameEvent) : nullptr;

  return frame && frameEvent && frame == frameEvent->GetEventObject();
}


//------------------------------------------------------------------------------
/**
  Накачка дерева
*/
//---
void LibObj::FillTree(ksapi::IPropertyTreePtr & tree)
{
  if (tree)
  {
    auto rootNode = tree->GetRootNode();

    if (rootNode)
      rootNode->Delete();

    std::wstring iconFont = GetLibraryIconFont();

    if (rootNode = tree->CreateNode(LoadStr(IDS_TREE)))
    {
      rootNode->SetItemsHolder(true);
      rootNode->SetNodeIcon(IDB_ROOT, iconFont);

      if (tree->AddNode(rootNode))
      {
        m_arrrowsNode = tree->CreateNode(LoadStr(IDS_FORCES));

        if (m_arrrowsNode)
        {
          m_arrrowsNode->SetItemsHolder(true);
          m_arrrowsNode->SetNodeIcon(IDB_ARROW, iconFont);

          if (rootNode->AddNode(m_arrrowsNode, nullptr))
            FillTreeNodes(m_arrows);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Проверка узла нагрузки
*/
//---
bool LibObj::CheckIfArrowNode(ksapi::IPropertyTreeNodePtr node)
{
  if (node)
  {
    std::wstring label = node->GetText();
    return !(label == LoadStr(IDS_TREE) || label == LoadStr(IDS_FORCES));
  }

  return false;
}


//------------------------------------------------------------------------------
/**
  Начало создания листа в контексте OpenGL
*/
//---
bool LibObj::BeginPaintGL(const ksapi::IOpenGLObjectPtr & glObj, std::int32_t drawMode)
{
  bool visible = false;

  if (visible)
  {
    // Стераем старое изображение
    ::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  return !visible;
}


//------------------------------------------------------------------------------
/**
  Определение габаритов документа
*/
//---
bool LibObj::AddGabarit(ksapi::IGabaritObjectPtr & gabObj)
{
  bool arrowsVisible = IsArrowsVisible();

  if (gabObj && arrowsVisible)
  {
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;

    if (bool res = gabObj->GetCurrentGabarit(0, x1, y1, z1, x2, y2, z2))
    {
      double delta = 100;
      x1 -= delta;
      y1 -= delta;
      z1 -= delta;
      x2 += delta;
      y2 += delta;
      z2 += delta;
      res = gabObj->AddGabarit(x1, y1, z1, x2, y2, z2);
    }
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Закрытие окна
*/
//---
bool LibObj::CloseFrame()
{
  // Окно закрылось удалим себя
  delete this;
  return true;
}


//------------------------------------------------------------------------------
/**
  Функция обратной связи
*/
//---
bool LibObj::SelectFace(const ksapi::IKompasAPIObjectPtr & entity)
{
  bool res = false;

  if (editObj && entity && entity->GetType() == ksObjectFace) // Фильтруем только грани
    res = editObj->OnSelectFace(entity);

  return res;
}


//------------------------------------------------------------------------------
/**
  Обработчик селектирования грани
*/
//---
bool LibObj::OnSelectFace(const ksapi::IFacePtr & face)
{
  switch (m_command)
  {
    case 1:
      CreateForce(face);
      break; // Создание диформационной нагрузки
  }
  return true;
}


#pragma warning(disable : 4786)

//------------------------------------------------------------------------------
/**
  Получить суммарную матрицу пересчета координат из компонента в сборку
*/
//---
bool GetSummMtr(ksapi::IKompasDocument3DPtr & doc3D, Matrix3D & summMtr, const ksapi::IFacePtr & face)
{
  if (doc3D && face)
  {
    ksapi::IPartPtr topPart = doc3D->GetTopPart();
    ksapi::IPartPtr parent(face->GetPart());

    if (topPart && parent)
    {
      ksapi::IPartPtr part(parent->GetParent());

      if (topPart != part)
      {
        // получение суммарной матрицы для пересчета координат
        std::vector<double> mtr = topPart->GetSummMatrix(part);

        if (mtr.size() / 16 > 0)
          summMtr = &mtr[0];
      }
    }
  }
  return !(summMtr == Matrix3D());
}
#pragma warning(once : 4786)


//------------------------------------------------------------------------------
/**
  Загрузка триангуляции стрелки из файла
*/
//---
void LibObj::LoadArrowParams()
{
  if (arrowParams.empty()) // Треангуляцию для стрелки не делали
  {
    std::wstring arrowFile = GetFullName(ARROW_M3D);

    if (!arrowFile.empty())
    {
      // По первому требованию загружаем триангуляцию
      LoadTessellations(arrowParams, arrowFile);
    }
    else
    {
      LibMessage(std::format(L"{} : {}", LoadStr(IDS_FILE_NOT_FOUND), ARROW_M3D));
    }
  }
}


//------------------------------------------------------------------------------
/**
  Очистка стрелок
*/
//---
void LibObj::ClearArrows()
{
  m_arrows.clear();
  delete m_newArrow;
  m_newArrow = nullptr;
}

//------------------------------------------------------------------------------
/**
  Наполним узлы дерева
*/
//---
void LibObj::FillTreeNodes(std::vector<ArrowParamPtr> & nodeElements)
{
  for (size_t i = 0, count = nodeElements.size(); i < count; ++i)
  {
    if (auto && param = nodeElements[i])
    {
      std::int32_t id = param->GetID();
      std::wstring & name = param->m_name;
      AddNode(name.length() > 0 ? name : MakeNodeName(LoadStr(IDP_FORCE), id), id);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание диформационной нагрузки
*/
//---
void LibObj::CreateForce(const ksapi::IFacePtr & face)
{
  LoadArrowParams();

  if (ksapi::IKompasDocument3DPtr doc3D = application ? application->GetActiveDocument() : nullptr)
  {
    if (ksapi::IDocumentFramePtr docFrame = doc3D->GetDocumentFrame())
    {
      if (!m_newArrow)
      {
        m_newArrow = new ArrowParam;
        m_arrows.emplace_back(m_newArrow);
      }

      m_newArrow->m_faces.emplace_back(face);
      m_newArrow->m_visible = true;
      m_newArrow->Make(face, docFrame, arrowParams);

      docFrame->SetGabaritModifying();
      docFrame->RefreshWindow();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Расчет матриц для отрисовки стрелок
  № пп  Параметр U, % Параметр V, %
  1     0             0
  2     0             50
  3     0             100
  4     50            0
  5     50            50
  6     50            100
  7     100           0
  8     100           50
  9     100           100
*/
//---
void MakeMatrices(ksapi::IMathSurface3DPtr & surf, bool isSameSense, std::vector<Matrix3D> & matrix)
{
  if (surf)
  {
    double uMin = surf->GetParamUMin(), uMax = surf->GetParamUMax(), vMin = surf->GetParamVMin(), vMax = surf->GetParamVMax();
    double du = (uMax - uMin) * 0.5;
    double dv = (vMax - vMin) * 0.5;
    Matrix3D m;
    double u = uMin, v;

    for (int i = 0; i < 3; i++, u += du)
    {
      v = vMin;
      for (int j = 0; j < 3; j++, v += dv)
      {
        m.Init(*surf, isSameSense, u, v);
        matrix.emplace_back(m);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Запуск процесса указания граней для создания нагрузок
*/
//---
void LibObj::SelectFacesForForce()
{
  SelectFaces(IDP_FORCE, STR_SELECT_FACES);
}


//------------------------------------------------------------------------------
/**
  Запуск процесса указания граней для создания нагузок
*/
//---
void LibObj::SelectFaces(std::int32_t procID, std::int32_t promtID)
{
  m_propMng->SetVisible(false);
  m_propMng->UpdateTabs();

  ksapi::IKompasDocument3DPtr komDoc3D = application ? application->GetActiveDocument() : nullptr;

  if (komDoc3D)
  {
    m_process = komDoc3D->GetLibProcess(ksProcess3DPlacementAndEntity);

    if (m_process && InitProcessParam(procID, pnEnterEscHelp, procID))
    {
      if (ksapi::IProcess3DPtr process = m_process)
        m_process3DEvent = m_events.Add(new Process3DEvent(handlerOwner, *this, *process));

      m_procParam->SetAutoReduce(false);
      m_process->SetPrompt(LoadStr(promtID));
      m_process->SetProcessParam(m_procParam);
      editObj = this;
      m_process->Run(true, true);

      editObj = nullptr;
      EndProcess();
    }

    m_propMng->SetVisible(true);
    m_propMng->UpdateTabs();
  }
}


//------------------------------------------------------------------------------
/**
  Включение\выключение и удаление отрисовки внешних объектов для заданного окна
*/
//---
void LibObj::OnOffTesselations(std::vector<std::int32_t> & iids, bool vis, bool oldVis, bool del)
{
  if (!iids.empty())
  {
    ksapi::IKompasDocument3DPtr komDoc3D(application ? application->GetActiveDocument() : nullptr);

    if (ksapi::IDocumentFramePtr frame = komDoc3D ? komDoc3D->GetDocumentFrame() : nullptr)
    {
      if (ksapi::IExternalTessellationManagerPtr mng = komDoc3D)
      {
        // Скрыть\показать
        if (vis != oldVis)
          mng->SetObjectsVisible(iids, frame, vis);

        if (del)
        {
          mng->DeleteObjects(iids); // Удалить
          iids.clear();
        }

        frame->SetGabaritModifying();
        frame->RefreshWindow();
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Добавление узла дерева
*/
//---
bool LibObj::AddNode(const std::wstring & nodeName, int index)
{
  bool res = false;

  if (m_arrrowsNode && m_treeCtrl)
  {
    auto node = m_treeCtrl->CreateNode(nodeName);
    if (node)
    {
      res = m_arrrowsNode->AddNode(node, nullptr);
      node->SetNodeIcon(IDS_FORCE, GetLibraryIconFont());
      node->SetNodeData(index);
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Составить имя для узла
*/
//---
std::wstring LibObj::MakeNodeName(std::wstring name, int index)
{
  static std::wstring nodeName;

  if (index != 0)
    nodeName = std::format(L"{}:{}", name, index);
  else
    nodeName = name;

  return nodeName;
}


//------------------------------------------------------------------------------
/**
  Проверка видимости стрелок
*/
//---
bool LibObj::IsArrowsVisible()
{
  bool res = !!m_newArrow;

  for (size_t i = 0, count = m_arrows.size(); i < count && !res; i++)
    res = m_arrows[i]->m_visible;

  return res;
}


//------------------------------------------------------------------------------
/**
  Обработка команд меню
*/
//---
void LibObj::OnMenuCommand(std::int32_t Id)
{
  switch (Id)
  {
    case 502: // Удалить стрелки
    {
      std::vector<ksapi::IPropertyTreeNodePtr> nodeVector = m_treeCtrl->GetSelectedNodes();
      bool deleted = false;

      for (auto nodeIt = nodeVector.begin(); nodeIt != nodeVector.end();)
      {
        auto node = *nodeIt;

        for (auto arrowsIt = m_arrows.begin(); arrowsIt != m_arrows.end();)
        {
          auto && param = *arrowsIt;

          if (param && node && node->GetNodeData() == param->m_index)
          {
            OnOffTesselations(param->m_iids, false, param->m_visible, true); // Удаляем стрелки
            arrowsIt = m_arrows.erase(arrowsIt);
            m_arrrowsNode->DeleteNode(node);
            deleted = true;
            m_arrowsIndex--;

            break;
          }
          else
          {
            ++arrowsIt;
          }
        }

        nodeIt = nodeVector.erase(nodeIt);
      }

      if (deleted && application)
      {
        ksapi::IKompasDocument3DPtr doc3D = application->GetActiveDocument();
        ksapi::IDocumentFramePtr docFrame(doc3D ? doc3D->GetDocumentFrame() : nullptr);

        for (size_t i = 0, count = m_arrows.size(); i < count; ++i)
          m_arrows[i]->m_index = static_cast<std::int32_t>(i + 1);

        for (std::int32_t i = 0, count = m_arrrowsNode->GetChildsCount(); i < count; ++i)
          m_arrrowsNode->GetChild(i)->SetNodeData(i + 1);

        if (docFrame)
          docFrame->RefreshWindow();
      }

      SaveTree();
      break;
    }

    case 503: // Показать/Скрыть
    {
      std::vector<ksapi::IPropertyTreeNodePtr> nodeVector = m_treeCtrl->GetSelectedNodes();

      for (auto nodeIt = nodeVector.begin(); nodeIt != nodeVector.end();)
      {
        auto node = *nodeIt;

        for (auto arrowsIt = m_arrows.begin(); arrowsIt != m_arrows.end();)
        {
          auto && param = *arrowsIt;

          if (param && node && node->GetNodeData() == param->m_index)
          {
            OnOffTesselations(param->m_iids, !param->m_visible, param->m_visible, false); // Показаем/Скрываем
            param->m_visible = !param->m_visible;
            break;
          }
          else
          {
            ++arrowsIt;
          }
        }

        ++nodeIt;
      }

      break;
    }
  }
}

void LibObj::OnBeginTextEdit(const ksapi::IPropertyTreeNodePtr & node)
{
  if (node && YesNoMessage(LoadStr(IDS_TEXT_EDIT_INFO)) == 1)
    node->EditText();
}

void LibObj::OnEndTextEdit(const ksapi::IPropertyTreeNodePtr & node)
{
  if (node)
  {
    std::int64_t index = node->GetNodeData();

    if (index > 0)
    {
      if (auto && param = m_arrows[index - 1])
        param->m_name = node->GetText();
    }
  }
}

void LibObj::OnContextMenuRequest(const ksapi::IPropertyTreeNodePtr & node)
{
  if (CheckIfArrowNode(node))
  {
    if (std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node) == m_selectedNodes.end())
    {
      for (auto node : m_selectedNodes)
        node->SetSelected(false);

      m_selectedNodes.clear();
      m_selectedNodes.push_back(node);
    }

    InitMenu(1);
  }
  else
  {
    InitMenu(0);
  }
}

extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Клик по узлу дерева
*/
//---
void LibObj::InitMenu(int menuType)
{
  ksapi::IPropertyControlPtr control1(m_treeCtrl);

  if (control1)
  {
    // Если элемент это папка нагрузок
    ksapi::IMenuPtr menu;
    if (menuType == 1)
    {
      menu = application->CreateMenu();
      menu->Load(IDM_TREE_MENU, GetModule());
    }
    control1->SetMenu(menu);
  }
}

void LibObj::OnSelectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes)
{
  m_selectedNodes.clear();
  m_selectedNodes = m_treeCtrl->GetSelectedNodes();
}

void LibObj::OnUnselectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes)
{
  for (auto node : m_treeCtrl->GetSelectedNodes())
  {
    auto it = std::ranges::find(m_selectedNodes, node);

    if (it != m_selectedNodes.end())
    {
      m_selectedNodes.erase(it);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Сохранение параметров дерева в хранилище
*/
//---
void LibObj::SaveTree()
{
  ksapi::IKompasDocument3DPtr doc3D = application ? application->GetActiveDocument() : nullptr;
  ksapi::IUserDataStoragesMngPtr storageMng = doc3D ? doc3D->GetUserDataStoragesMng() : nullptr;

  if (ksapi::IUserDataStoragesPtr storages = storageMng ? storageMng->Add(nullptr) : nullptr)
  {
    std::wstring treeName(FORCES_TREE);
    ksapi::IUserDataStoragePtr treePars = storages->GetItemByName(treeName);

    if (treePars)
    {
      treePars->Clear(treeName);
    }
    else
    {
      treePars = storages->Add(treeName);

      if (treePars)
        treePars->SetPassword(L"", treeName);
    }

    if (treePars)
    {
      treePars->SetVersion(treeName, LIBVERSION);                    // Версия параметров
      treePars->AddValue(treeName, m_arrowsIndex, L"arrowsIndex");   // Индекс следующей нагрузки
      treePars->AddValue(treeName, m_arrows.size(), L"arrowsCount"); // Количество нагрузок

      for (size_t i = 0, count = m_arrows.size(); i < count; i++)
        m_arrows[i]->Save(treeName, treePars);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Получить пользовательское хранилище по имени
*/
//---
ksapi::IUserDataStoragePtr LibObj::GetUserDataStorageByName(const std::wstring & treeName)
{
  ksapi::IUserDataStoragePtr treePars;

  if (ksapi::IKompasDocument3DPtr doc3D = application ? application->GetActiveDocument() : nullptr)
  {
    if (ksapi::IDocumentFramePtr docFrame = doc3D->GetDocumentFrame())
    {
      if (ksapi::IUserDataStoragesMngPtr storageMng = doc3D->GetUserDataStoragesMng())
      {
        if (ksapi::IUserDataStoragesPtr storages = storageMng->Add(nullptr))
          treePars = storages->GetItemByName(treeName);
      }
    }
  }

  return treePars;
}


//------------------------------------------------------------------------------
/**
  Получение значения по индексу
*/
//---
ksapi::StorageValue GetStorageValue(ksapi::IUserDataStoragePtr & treePars, const std::wstring & comment)
{
  ksapi::StorageValue obj;
  treePars->GetValueByComment(comment, obj);
  return obj;
}

std::vector<ksapi::IKompasAPIObjectPtr> GetObjs(ksapi::IUserDataStoragePtr & treePars, const std::wstring & comment)
{
  std::vector<ksapi::IKompasAPIObjectPtr> objs;
  treePars->GetObjectsByComment(comment, objs);
  return objs;
}


//------------------------------------------------------------------------------
/**
  Загрузка параметров дерева из хранилища
*/
//---
void LibObj::LoadTree()
{
  std::wstring treeName(FORCES_TREE);
  ksapi::IUserDataStoragePtr treePars = GetUserDataStorageByName(treeName);

  if (treePars)
  {
    std::int32_t version = treePars->GetVersion(); // Версия параметров

    if (version <= LIBVERSION)
    {
      // Индекс следующей нагрузки
      m_arrowsIndex = GetStorageValue(treePars, L"arrowsIndex").GetInt32();
      size_t count = GetStorageValue(treePars, L"arrowsCount").GetUint64();

      // Количество нагрузок
      for (size_t i = 0; i < count; i++)
      {
        ArrowParamPtr param = std::make_unique<ArrowParam>();

        if (param->Load(treePars, version, i))
          m_arrows.push_back(std::move(param));
      }

      /// Отсортируем нагрузки по возрастанию их индексов
      auto comp{[](const ArrowParamPtr & first, const ArrowParamPtr & second)
                {
                  return first->m_index < second->m_index;
                }};

      std::sort(m_arrows.begin(), m_arrows.end(), comp);
      MakeTessellations();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Расчет триангуляций
*/
//---
void LibObj::MakeTessellations()
{
  if (ksapi::IKompasDocument3DPtr doc3D = application ? application->GetActiveDocument() : nullptr)
  {
    if (ksapi::IDocumentFramePtr docFrame = doc3D->GetDocumentFrame())
    {
      LoadArrowParams();

      for (size_t i = 0, count = m_arrows.size(); i < count; i++)
        m_arrows[i]->Make(docFrame, arrowParams);

      docFrame->SetGabaritModifying();
      docFrame->RefreshWindow();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Пересчитать нагрузки
*/
//---
void LibObj::Refresh()
{
  OffAllTessellations();
  MakeTessellations();
}

//------------------------------------------------------------------------------
/**
  Сохранение параметров в хранилище
*/
//---
void TeselationNodeParam::Save(const std::wstring & pasword, ksapi::IUserDataStoragePtr & s)
{
  if (s)
  {
    std::wstring indexStr = std::to_wstring(m_index);
    s->AddValue(pasword, m_index, L"index" + indexStr);     // Индекс
    s->AddValue(pasword, m_visible, L"visible" + indexStr); // Видимость
    size_t count = m_faces.size();
    s->AddValue(pasword, count, L"facesCount" + indexStr); // Количество граней

    if (count)
    {
      std::vector<ksapi::IKompasAPIObjectPtr> objs = {m_faces.begin(), m_faces.end()};
      s->AddObjects(pasword, objs, L"faces" + indexStr); // Видимость
    }

    if (m_name.length() > 0)
      s->AddValue(pasword, m_name, L"name" + indexStr);
  }
}


//------------------------------------------------------------------------------
/**
  Получение параметров из хранилища
*/
//---
bool TeselationNodeParam::Load(ksapi::IUserDataStoragePtr & s, std::int32_t version, size_t index)
{
  bool res = false;
  if (s)
  {
    std::wstring indexStr = std::to_wstring(index + 1);

    m_index = GetStorageValue(s, L"index" + indexStr).GetInt32();            // Индекс
    m_visible = GetStorageValue(s, L"visible" + indexStr).GetBool();         // Видимость
    size_t count = GetStorageValue(s, L"facesCount" + indexStr).GetUint64(); // Количество граней

    if (count)
    {
      std::vector<ksapi::IFacePtr> faces;
      m_faces.clear();
      std::ranges::copy(GetObjs(s, L"faces" + indexStr), std::back_inserter(faces));
      m_faces = faces;
    }

    m_name = GetStorageValue(s, L"name" + indexStr).GetString(); // Имя соответствующего узла
    res = true;
  }

  return res;
}


#pragma warning(disable : 4786)

//------------------------------------------------------------------------------
/**
  Расчет параметров создания стрелок для одной грани
*/
//---
void ArrowParam::Make(const ksapi::IFacePtr & face, const ksapi::IDocumentFramePtr & frame,
                      std::vector<ExternalTeselationParam *> & arrowParams)
{
  if (face && application)
  {
    ksapi::IKompasDocument3DPtr doc3D(application->GetActiveDocument());

    if (ksapi::IExternalTessellationManagerPtr mng = doc3D)
    {
      std::vector<Matrix3D> matrices;
      ksapi::IMathSurface3DPtr surf(face->GetMathSurface());

      // Расчет матриц для отрисовки стрелок
      MakeMatrices(surf, face->GetNormalOrientation(), matrices);

      if (doc3D->GetDocumentType() == ksDocumentAssembly)
      {
        // если грань принадлежит детали вставленной в сторку то нужно домножить на суммарную матрицу пересчета координат
        // из детали в сборку
        Matrix3D summMtr;
        if (GetSummMtr(doc3D, summMtr, face))
        {
          for (size_t i = 0, count = matrices.size(); i < count; i++)
            matrices[i] *= summMtr;
        }
      }

      std::vector<std::int32_t> objects;
      std::vector<double> matricesValues;

      for (auto matrix : matrices)
      {
        for (auto value : matrix.GetValues())
          matricesValues.emplace_back(value);
      }

      for (size_t i = 0, count = arrowParams.size(); i < count; i++)
      {
        if (ksapi::IExternalTessellationObjectPtr obj = mng->Add())
        {
          std::int32_t id = obj->GetObjectId();
          objects.push_back(id);
          arrowParams[i]->Init(obj);
          obj->SetPlaces(matricesValues);
        }
      }

      if (m_visible)
        mng->SetObjectsVisible(objects, frame, true);

      m_iids.insert(m_iids.end(), objects.begin(), objects.end());
    }
  }
}

#pragma warning(once : 4786)

//------------------------------------------------------------------------------
/**
  Расчет нагрузок
*/
//---
void ArrowParam::Make(ksapi::IDocumentFramePtr & frame, std::vector<ExternalTeselationParam *> & arrowParams)
{
  if (frame && !arrowParams.empty())
  {
    for (size_t i = 0, count = m_faces.size(); i < count; i++)
      Make(m_faces[i], frame, arrowParams);
  }
}

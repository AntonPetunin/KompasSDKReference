#include "DocumentFrameEvents.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <GL\gl.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

int DocumentFrameEvents::redrawType = 0;

void RecursiveDraw(ksapi::IPartPtr & pTopPart, const ksapi::IFeaturePtr & nextFeature);
void RecursiveDraw(ksapi::IPartPtr & topPart, const std::vector<ksapi::IFeaturePtr> & features);
void RedrawModelFaces(ksapi::IPartPtr & topPart, ksapi::IPartPtr & nextPart);
void RedrawObjects(ksapi::IPartPtr & topPart, ksapi::IPartPtr & nextPart, const std::vector<ksapi::IModelObjectPtr> & objects);
void RedrawFace(ksapi::IPartPtr & topPart, ksapi::IPartPtr & nextPart, ksapi::IFacePtr & face);

//-------------------------------------------------------------------------------
//
// ---
DocumentFrameEvents::DocumentFrameEvents(ksapi::IApplication * kompasApp, ksapi::IDocumentFramePtr & documentFrame,
                                         int _redrawType /*= 0*/)
  : m_kompasApp(kompasApp)
  , m_events(documentFrame->Events())
{
  AdviseFrameEvent();
  redrawType = _redrawType;
}

//-------------------------------------------------------------------------------
//
// ---
DocumentFrameEvents::~DocumentFrameEvents()
{
  UnAdviseFrameEvent();
}


#pragma region Обработчики событий окна документа
//-------------------------------------------------------------------------------
//
// ---
bool DocumentFrameEvents::OnBeginPaintGl(unsigned int drawMode, const ksapi::IOpenGLObjectPtr & glObject)
{
  bool res = true;

  if (m_kompasApp && drawMode == vm_Shaded)
  {
    if (ksapi::IKompasDocument3DPtr doc3D = m_kompasApp->GetActiveDocument())
    {
      if (ksapi::IPartPtr topPart = doc3D->GetTopPart())
      {
        // Стираем старое изображение
        ::glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        ::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ::glDisable(GL_LIGHTING);
        ::glEnable(GL_DEPTH_TEST);

        if (ksapi::IFeaturePtr partFeature = topPart)
          RecursiveDraw(topPart, partFeature); // Рекурсивный обход объектов сборки


        ksapi::IDocumentFramePtr documentFrame(doc3D->GetDocumentFrame());
        int32_t Left, Right, Bottom, Top;
        documentFrame->GetWorkAreaRect(Left, Top, Right, Bottom);


        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(Left, Right, Top, Bottom, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();


        glLoadIdentity();
        glDisable(GL_LIGHTING);

        glBegin(GL_QUADS);
        glColor3d(1.0, 0, 1.0);
        glVertex2d(615, 680);
        glVertex2d(815, 680);
        glVertex2d(815, 880);
        glVertex2d(615, 880);
        glEnd();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);

        ::glEnable(GL_LIGHTING);

        ::glPopAttrib();
        res = false;
      }
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void DocumentFrameEvents::OnClose()
{
  UnAdviseFrameEvent();
}
#pragma endregion

//-------------------------------------------------------------------------------
// Подписка на события окна документа
// ---
void DocumentFrameEvents::AdviseFrameEvent()
{
  using namespace std::placeholders;

  if (!m_events)
    return;

  m_events->AddBeginPaintGlHandler(libname, [this](unsigned int drawMode, const ksapi::IOpenGLObjectPtr & glObject)
                                   { return OnBeginPaintGl(drawMode, glObject); });
  m_events->AddCloseHandler(libname, [this]() { OnClose(); });

  m_is_advised = true;
}

//-------------------------------------------------------------------------------
// Отписка от событий окна документа
// ---
void DocumentFrameEvents::UnAdviseFrameEvent()
{
  if (!m_is_advised)
    return;

  if (!m_events)
    return;

  m_events->RemoveAllHandlers(libname);
  m_is_advised = false;
}

//-------------------------------------------------------------------------------
// Получение коллекции граней детали и их перерисовка
// ---
void RedrawModelFaces(ksapi::IPartPtr & topPart, ksapi::IPartPtr & nextPart)
{
  ksapi::IFeaturePtr nextFeature = nextPart;

  if (nextPart && !nextPart->IsHidden() && (nextFeature && !nextFeature->IsExcluded()))
  {
    switch (DocumentFrameEvents::redrawType)
    {
      case 0: // Получаем грани с детали
      {
        // Получить массив объектов заданного типа (вершины)
        std::vector<ksapi::IModelObjectPtr> faces =
          nextFeature ? nextFeature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_face})
                      : std::vector<ksapi::IModelObjectPtr>();
        RedrawObjects(topPart, nextPart, faces);
        break;
      }
      case 1: // Получаем грани с операций
      {
        for (auto & feature : nextFeature->GetSubFeatures(ksTreeTypeEnum::ksOperTree, false, true))
        {
          std::vector<ksapi::IModelObjectPtr> faces = feature
                                                      ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_face})
                                                      : std::vector<ksapi::IModelObjectPtr>();
          RedrawObjects(topPart, nextPart, faces);
        }
        break;
      }
      case 2: // Получение коллекции граней с коллекции тел
      {
        std::vector<ksapi::IBodyPtr> resulBodies = nextFeature->GetResultBodies();
        for (int i = 0, count = static_cast<int>(resulBodies.size()); i < count; i++)
        {
          if (ksapi::IBodyPtr body = resulBodies[i])
          {
            ksapi::IFeaturePtr feature = body;
            std::vector<ksapi::IModelObjectPtr> faces =
              feature ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_face})
                      : std::vector<ksapi::IModelObjectPtr>();

            RedrawObjects(topPart, nextPart, faces);
          }
        }

        break;
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Рекурсивный обход объектов сборки
// ---
void RecursiveDraw(ksapi::IPartPtr & topPart, const ksapi::IFeaturePtr & nextFeature)
{
  if (topPart && nextFeature)
  {
    switch (nextFeature->GetFeatureType())
    {
      case o3d_part:
      {
        ksapi::IPartPtr part(nextFeature);
        // Не рисуем грани детали, если используется обход по операциям
        if (part && DocumentFrameEvents::redrawType != 1)
          RedrawModelFaces(topPart, part); // Отрисовка граней модели

        break;
      }
      case o3d_entity:
      {
        if (ksapi::IModelObjectPtr object = nextFeature)
        {
          if (nextFeature->IsExcluded() || object->IsHidden())
            break;

          if (DocumentFrameEvents::redrawType == 1) // По граням операций
          {
            // Получение колекций граней для сборочных операций
            ksapi::IPartPtr parent(object->GetParent());
            if (parent)
            {
              std::vector<ksapi::IModelObjectPtr> faces =
                nextFeature ? nextFeature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_face})
                            : std::vector<ksapi::IModelObjectPtr>();

              for (auto & face : faces)
              {
                // грани сборочной операции могут принадлежать разным деталям
                if (ksapi::IPartPtr parent = face->GetParent())
                {
                  ksapi::IFacePtr faceFeature = face;
                  RedrawFace(topPart, parent, faceFeature);
                }
              }
              break;
            }
          }
        }

        break;
      }
    }

    // Рекурсивный обход дерева модели
    std::vector<ksapi::IFeaturePtr> features = nextFeature->GetSubFeatures(ksTreeTypeEnum::ksOperTree, false, true);
    RecursiveDraw(topPart, features);
  }
}

//-------------------------------------------------------------------------------
// Обход объектов коллекции
// ---
void RecursiveDraw(ksapi::IPartPtr & topPart, const std::vector<ksapi::IFeaturePtr> & features)
{
  for (auto & feature : features)
  {
    if (!feature->IsExcluded())
      RecursiveDraw(topPart, feature);
  }
}

//-------------------------------------------------------------------------------
// Разукрасим объекты
// ---
void RedrawObjects(ksapi::IPartPtr & topPart, ksapi::IPartPtr & nextPart, const std::vector<ksapi::IModelObjectPtr> & objects)
{
  for (const auto & object : objects)
  {
    if (ksapi::IFacePtr face = object)
    {
      RedrawFace(topPart, nextPart, face);
    }
  }
}

//-------------------------------------------------------------------------------
// Разукрасим грани
// ---
void RedrawFace(ksapi::IPartPtr & topPart, ksapi::IPartPtr & nextPart, ksapi::IFacePtr & face)
{
  bool transform = topPart && topPart != nextPart;
  if (face)
  {
    // Получаем триангуляцию
    if (ksapi::ITessellationPtr tess = face->GetTessellation())
    {
      std::vector<double> points;
      std::vector<int32_t> indexes;
      std::vector<double> normals;
      // Получаем параметры триангуляции
      tess->GetFacetPoints(points, indexes, normals);

      // массив должен быть одномерный и количество точек должно быть кратно трем
      if ((!points.empty() && !(points.size() % 3)) && (!indexes.empty() && !(indexes.size() % 3)) &&
          (!normals.empty() && !(normals.size() % 3)))
      {
        int trianglesCount = static_cast<int>(indexes.size()) / 3;

        for (int i = 0, j = 0; i < trianglesCount; i++, j++)
        {
          glBegin(GL_TRIANGLES);
          double x, y, z;
          int index = indexes[j] * 3; // Индекс первой вершины
                                      // Получение координат вершин треугольника
          x = points[index++];        // Координаты первой вершины
          y = points[index++];
          z = points[index];

          if (transform)
            // Перевод координат из системы координат детали в систему координат сборки
            topPart->TransformPoint(x, y, z, nextPart);

          glColor3d(1.0f, 0.0f, 0.0f);
          glVertex3d(x, y, z);
          index = indexes[j++] * 3; // Индекс нормали первой вершины
          x = normals[index++];     // Координаты нормали первой вершины
          y = normals[index++];
          z = normals[index];
          glNormal3d(x, y, z);

          index = indexes[j] * 3; // Индекс второй вершины
          x = points[index++];    // Координаты второй вершины
          y = points[index++];
          z = points[index];

          if (transform)
            // Перевод координат из системы координат детали в систему координат сборки
            topPart->TransformPoint(x, y, z, nextPart);

          glColor3d(0.0f, 1.0f, 0);
          glVertex3d(x, y, z);
          index = indexes[j++] * 3; // Индекс нормали второй вершины
          x = normals[index++];     // Координаты нормали второй вершины
          y = normals[index++];
          z = normals[index];
          glNormal3d(x, y, z);

          index = indexes[j] * 3; // Индекс третьей вершины
          x = points[index++];    // Координаты третьей вершины
          y = points[index++];
          z = points[index];

          if (transform)
            // Перевод координат из системы координат детали в систему координат сборки
            topPart->TransformPoint(x, y, z, nextPart);

          glColor3d(0.0f, 0.0f, 1.0f);
          glVertex3d(x, y, z);
          index = indexes[j] * 3; // Индекс нормали третьей вершины
          x = normals[index++];   // Координаты нормали третьей вершины
          y = normals[index++];
          z = normals[index];
          glNormal3d(x, y, z);
          glEnd();
        }
      }
    }
  }
}

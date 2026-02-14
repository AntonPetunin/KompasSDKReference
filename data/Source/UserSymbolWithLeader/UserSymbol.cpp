////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Класс создания линии-выноски с пользовательским символом.
 
  Также содержит реализации вспомогательных функций.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <UserSymbol.h>

extern ksapi::IApplication * kompasApp;
extern std::wstring LoadResourceString(int32_t strId);

namespace
{

/// Ключ для обработчиков 2D-событий библиотеки.
const std::wstring kHandlerOwnerName = klibName + L"2D";
/// Минимальная  длина линии-выноски, для предотвращения создания вырожденных объектов.
constexpr double kMinLength = 1e-4;

//-------------------------------------------------------------------------------
/**
  Получить контейнер объектов 2D
*/
// ---
ksapi::IDrawingContainerPtr GetDrawingContainer(IKompasDocument2DPtr doc)
{
  if (doc)
    // Получим менеджер для работы с видами и слоями.
    if (ksapi::IViewsAndLayersManagerPtr viewsMng = doc->GetViewsAndLayersManager())
      // Получим коллекцию видов.
      if (ksapi::IViewsPtr views = viewsMng->GetViews())
        //  Получаем контейнер, который является доп. интерфейсом для вида.
        if (views)
          return views->GetActiveView();
  return nullptr;
}

//-----------------------------------------------------------------------------
/**
  Создать аннотационный отрезок по начальной точке, длине и углу.
  \param[in] lineSegCollection - коллекция отрезков.
  \param[in] annContainer - контейнер аннотационных объектов.
  \param[in] x, y - координаты начала отрезка.
  \param[in] length - длина отрезка.
  \param[in] angle - угол наклона отрезка.
  \param[in] style - стиль линии отрезка.
  \return Координата Х конца отрезка, std::nullopt в случае, если отрезок не создан.
*/
//---
double CreateLineSeg(ksapi::ILineSegmentsPtr & lineSegCollection, IAnnotativeContainerPtr & annContainer, double x, double y,
                     double length, double angle, int32_t style)
{
  if (!lineSegCollection)
    return 0;

  // Добавление отрезка в коллекцию.
  ksapi::ILineSegmentPtr lineSeg = lineSegCollection->Add();
  if (!lineSeg)
    return 0;

  // Наполнение объекта параметрами.
  lineSeg->SetX1(x);
  lineSeg->SetY1(y);
  lineSeg->SetLength(length);
  lineSeg->SetAngle(angle);
  lineSeg->SetStyle(style);

  // Создание объекта.
  lineSeg->Update();

  // Преобразуем отрезок в аннотационный и удаляем исходный
  if (annContainer)
  {
    annContainer->CreateByGeomObject(lineSeg);
    lineSeg->Update();
  }

  return lineSeg->GetX2(); // Для вычисления точки привязки текста
}

//-----------------------------------------------------------------------------
/**
  Создать отрезок по двум точкам.
  \param[in] lineSegCollection - коллекция отрезков.
  \param[in] x1, y1 - координаты начала отрезка.
  \param[in] x2, y2 - координаты конца отрезка.
  \return Указатель на отрезок, либо nullptr в случае неудачи.
*/
//---
ksapi::ILineSegmentPtr CreateLineSegmentByPoints(ksapi::ILineSegmentsPtr & lineSegCollection, double x1, double y1, double x2,
                                                 double y2)
{
  if (!lineSegCollection)
    return nullptr;

  // Добавление отрезка в коллекцию.
  ksapi::ILineSegmentPtr lineSeg = lineSegCollection->Add();
  if (!lineSeg)
    return nullptr;

  // Наполнение объекта параметрами.
  lineSeg->SetX1(x1);
  lineSeg->SetY1(y1);
  lineSeg->SetX2(x2);
  lineSeg->SetY2(y2);
  lineSeg->SetStyle(ksCSNormal);

  // Создание и отрисовка объекта.
  lineSeg->Update();

  return lineSeg;
}

//-----------------------------------------------------------------------------
/**
  Создать текст
  \param[in] textsCollection - коллекция текстов.
  \param[in] annContainer - контейнер аннотационных объектов.
  \param[in] x, y - координаты точки привязки текста.
  \param[in] str - строка текста.
*/
//---
void CreateText(ksapi::IDrawingTextsPtr & textsCollection, ksapi::IAnnotativeContainerPtr & annContainer, double x, double y,
                const std::wstring & str)
{
  if (!textsCollection)
    return;

  // Добавляем текст в коллекцию.
  ksapi::IDrawingTextPtr drawText = textsCollection->Add();
  if (!drawText)
    return;

  // Задаем точку привязки.
  drawText->SetX(x);
  drawText->SetY(y);

  // Интерфейс IText для установки текста.
  if (ksapi::ITextPtr text = drawText)
    text->SetStr(str);

  // Создание и отрисовка объекта.
  drawText->Update();

  // Преобразуем текст в аннотационный и удаляем исходный
  if (annContainer)
  {
    annContainer->CreateByGeomObject(drawText);
    drawText->Update(); ///////////////////////////////////////////
  }
}

//-----------------------------------------------------------------------------
/**
  Создать выносную линию.
  \param[in] macroObj - макроэлемент.
  \param[in] beginX, beginY - координаты начала полки.
  \param[in] endX, endY - координаты конечной точки ответвления.
  \return Выносная линия, либо nullptr в случае неудачи.
*/
//---
ksapi::IBaseLeaderPtr CreateLeader(ksapi::IMacroObjectPtr macroObj, double beginX, double beginY, double endX, double endY)
{
  if (!macroObj)
    return nullptr;

  ksapi::IBaseLeaderPtr baseLeader;

  // Получаем коллекцию присоединенных линий-выносок
  if (ksapi::ILeadersPtr leadersCollection = macroObj->GetAttachedLeaders())
  {
    // Добавляем линию-выноску
    if (ksapi::ILeaderPtr leader = leadersCollection->Add(ksDrLeader))
    {
      // Добавляем ответвление
      if (ksapi::IBranchsPtr branches = leader)
      {
        branches->SetX0(beginX);
        branches->SetY0(beginY);
        branches->AddBranchByPoint(-1, endX, endY);
      }

      if (baseLeader = leader)
        baseLeader->Update();
    }
  }

  return baseLeader;
}


//-----------------------------------------------------------------------------
/**
  Создать заливку.
  \param drawContainer - контейнер графических объектов.
  \param annContainer - контейнер аннотационных объектов.
  \param x, y - координаты базовой точки для отрисовки заливки.
*/
//---
void CreateColouring(ksapi::IDrawingContainerPtr & drawContainer, ksapi::IAnnotativeContainerPtr & annContainer, double x,
                     double y)
{
  if (!drawContainer)
    return;

  ksapi::IColouringPtr colouring;
  ksapi::IBoundariesObjectPtr boundsObj;

  // Получаем коллекцию заливок.
  if (ksapi::IColouringsPtr colouringsCollection = drawContainer->GetColourings())
    // Добавляем границу.
    if (colouring = colouringsCollection->Add())
      boundsObj = colouring;

  // Создадим отрезки границы.
  ksapi::ILineSegmentsPtr lineSegsCollection;
  ksapi::ILineSegmentPtr lineSeg1 = CreateLineSegmentByPoints(lineSegsCollection, x, y + 5, x, y + 10);
  ksapi::ILineSegmentPtr lineSeg2 = CreateLineSegmentByPoints(lineSegsCollection, x, y + 10, x - 10, y + 7);
  ksapi::ILineSegmentPtr lineSeg3 = CreateLineSegmentByPoints(lineSegsCollection, x - 10, y + 7, x, y + 5);

  if (!boundsObj || !lineSeg1 || !lineSeg2 || !lineSeg3 || !annContainer)
    return;

  // Создаём аннотационные объекты из отрезков.
  ksapi::IAnnotativeObjectPtr annLineSeg1 = annContainer->CreateByGeomObject(lineSeg1);
  ksapi::IAnnotativeObjectPtr annLineSeg2 = annContainer->CreateByGeomObject(lineSeg2);
  ksapi::IAnnotativeObjectPtr annLineSeg3 = annContainer->CreateByGeomObject(lineSeg3);

  // Добавление отрезков к границе заливки (с помощью доп. интерфейса, реализующего границы).
  boundsObj->AddBoundaries({annLineSeg1, annLineSeg2, annLineSeg3} /*boundaries*/, true /*deleteSource*/);

  // Цвет в формате RGB - чёрный.
  colouring->SetColor1(Color{0x0});
  colouring->Update();
}

//-----------------------------------------------------------------------------
/**
  Создать аннотационный отрезок.
*/
//---
void CreateAnnLineSeg(ksapi::IAnnotativeContainerPtr annContainer, double x1, double y1, double x2, double y2,
                      ksAnnotativeTerminatorSignEnum sign1, ksAnnotativeTerminatorSignEnum sign2, ksCurveStyleEnum style)
{
  ksapi::IAnnotativeObjectPtr annObj;
  ksapi::ILineSegmentPtr lineSeg;
  if (annContainer)
    lineSeg = annObj = annContainer->Add(ksDrAnnLineSeg);
  if (!lineSeg)
    return;

  annObj->SetSign1(sign1);
  annObj->SetSign2(sign2);

  lineSeg->SetX1(x1);
  lineSeg->SetY1(y1);
  lineSeg->SetX2(x2);
  lineSeg->SetY2(y2);
  lineSeg->SetStyle(style);
  lineSeg->Update();
}

} // namespace


//-------------------------------------------------------------------------------------
/**
  Создать макроэлеменет - обозначение.
*/
//---
void UserSymbol::CreateMacro()
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(m_doc);
  ksapi::IViewPtr currentView = drawingContainer;
  if (!currentView)
    return;

  // Пересчет координат для масштабированных видов
  m_doc->CreateMatrix(0 /*x*/, 0 /*y*/, 0 /*angle*/, currentView->GetScale() /*scaleX*/, currentView->GetScale() /*scaleY*/);
  double x = m_x;
  double y = m_y;
  double length1 = 52;
  double length2 = 2;
  double length3 = 8;
  m_doc->TransformPointFromMatrix(m_x /*x*/, m_y /*y*/, x /*xn*/, y /*yn*/);
  m_doc->TransformLengthIntoMatrix(length1);
  m_doc->TransformLengthIntoMatrix(length2);
  m_doc->TransformLengthIntoMatrix(length3);
  m_doc->DeleteMatrix();

  // Добавляем новый макрообъект и кастуем ко всем требуемым интерфейсам.
  ksapi::IMacroObjectPtr macroObj;
  ksapi::IUserParametersPtr macroParams;
  ksapi::IDrawingContainerPtr macroDrawingContainer;
  ksapi::IAnnotativeContainerPtr macroAnnContainer;
  if (ksapi::IMacroObjectsPtr macroObjs = drawingContainer->GetMacroObjects())
    macroAnnContainer = macroDrawingContainer = macroParams = macroObj = macroObjs->Add(false /*multiLayer*/);
  if (!macroAnnContainer)
    return;

  macroParams->SetCommand(1);              // Через интерфейс пользовательских параметров установим команду для редактирования.
  macroObj->SetBreakObjectsEnabled(false); // Не учитывать разрыв вида.

  ksapi::ILineSegmentsPtr lineSegments = macroDrawingContainer->GetLineSegments();
  CreateLineSeg(lineSegments, macroAnnContainer, x, y, 90 /*length*/, 0 /*angle*/, ksCSThin /*style*/);
  CreateLineSeg(lineSegments, macroAnnContainer, x, y, 10 /*length*/, 90 /*angle*/, ksCSThin /*style*/);
  CreateLineSeg(lineSegments, macroAnnContainer, x + 2, y - 1, 88 /*length*/, 0 /*angle*/, ksCSDashed /*style*/);
  CreateLineSeg(lineSegments, macroAnnContainer, x + 40, y - 10, 5 /*length*/, 0 /*angle*/, ksCSNormal /*style*/);
  CreateLineSeg(lineSegments, macroAnnContainer, x + 50, y + 10, 5 /*length*/, 180 /*angle*/, ksCSNormal /*style*/);
  CreateLineSeg(lineSegments, macroAnnContainer, x + 90, y, 5 /*length*/, 45 /*angle*/, ksCSThin /*style*/);

  // Вычисляем точку привязки текста
  double textX = CreateLineSeg(lineSegments, macroAnnContainer, x + 90, y, 5 /*length*/, 315 /*angle*/, ksCSThin /*style*/);
  textX += 2;

  if (ksapi::ILineSegmentPtr lineSeg =
        CreateLineSegmentByPoints(lineSegments, x + 40 /*x1*/, y - 10 /*y1*/, x + 50 /*x2*/, y + 10 /*y2*/))
  {
    macroAnnContainer->CreateByGeomObject(lineSeg);
    lineSeg->Update();
  }

  // Пересчёт координат для масштабированных видов.
  m_doc->CreateMatrix(0 /*x*/, 0 /*y*/, 0 /*angle*/, currentView->GetScale() /*scaleX*/, currentView->GetScale() /*scaleY*/);
  double textDx = textX;
  double textDy = 0;
  m_doc->TransformPointIntoMatrix(textX /*x*/, m_x + 2 /*y*/, textDx /*xn*/, textDy /*yn*/);
  m_doc->DeleteMatrix();

  // Создаём объекты текстов.
  ksapi::IDrawingTextsPtr macroTexts = macroDrawingContainer->GetDrawingTexts();
  CreateText(macroTexts, macroAnnContainer, m_x + length1 /*x*/, m_y + length2 /*y*/, L"Текст1");
  CreateText(macroTexts, macroAnnContainer, m_x + length1 /*x*/, m_y - length3 /*y*/, L"Текст2");
  CreateText(macroTexts, macroAnnContainer, textDx /*x*/, m_y + length2 /*y*/, L"Текст3");
  CreateText(macroTexts, macroAnnContainer, textDx /*x*/, m_y - length3 /*y*/, L"Текст4");

  // Заливка и линия-выноска
  CreateColouring(macroDrawingContainer, macroAnnContainer, x, y);
  m_leader = CreateLeader(macroObj, m_x /*beginX*/, m_y /*beginY*/, m_endX /*endX*/, m_endY /*endY*/);

  macroObj->Update();
}

//-------------------------------------------------------------------------------------
/**
  Создать или отредактировать обозначение. Метод вызывается при вызове команды.
*/
//---
void UserSymbol::Draw()
{
  // Инициализируются параметры процесса - наполняется спецпанель.
  if (!m_doc || !InitProcessParam())
    return;

  // При вызове команды создаём объект процесса
  m_process = m_doc->GetLibProcess(ksProcess2DCursor);
  if (!m_process)
    return;

  // Установка типа фантома для процесса.
  if (ksapi::IPhantom2DPtr phantom = m_process->GetPhantom2D())
    phantom->SetPhantomType(ksUserPhantom);

  // Установка обработчика события клика (или изменения положения курсора) - событие 2D процесса.
  m_process->Events()->AddPlacementChangedHandler(kHandlerOwnerName, [this](double x, double y, double angle, bool dynamic)
                                                  { return PointCallBack(x, y, angle, dynamic); });
  // Цикл для того что бы оставатся внутри команды при включении/выключении редактирования линии-выноски
  m_repeat = true;
  while (m_repeat)
  {
    m_repeat = false;

    // Запуск процесса указания точки пользователем.
    if (ksapi::IProcessPtr process = m_process)
    {
      process->SetDynamic(true); // Вызывать обработчик не только для клика, но и для изменения положения курсора
      process->SetProcessParam(m_procParam);
      process->Run(true /*modal*/, false /*postMessage*/); // Вызов заблокирует поток до завершения процесса.
    }

    // Если нажата кнопка редактирования линии-выноски в процессе, будет установлен флаг m_editLeader.
    if (m_editLeader)
    {
      m_doc->VisualEditObject(m_leader); // Запуск процесса Компаса - редактирование линии-выноски.
      m_editLeader = false;
    }
  }

  // Перед завершением команды библиотеки нужно очистить фантомную группу.
  if (m_phantomGroup)
  {
    m_phantomGroup->Delete();
    m_phantomGroup = nullptr;
  }

  // Перед завершением команды удаляем обработчик PlacementChange.
  if (m_process)
    m_process->Events()->RemoveAllHandlers(kHandlerOwnerName);
}

//-------------------------------------------------------------------------------------
/**
  Обработчик события изменения положения курсора (dynamic == true), или выбора точки пользователем (dynamic == false).
*/
//---
bool UserSymbol::PointCallBack(double x, double y, double angle, bool dynamic)
{
  // Состояние - пользователь ещё не выбрал первую точку.
  if (m_firstPoint && dynamic)
    return true;

  // Состояние - пользователь выбрал первую точку.
  if (m_firstPoint && !dynamic)
  {
    m_endX = x;
    m_endY = y;
    m_firstPoint = false;
    return true;
  }

  // Получение фантома процесса
  ksapi::IPhantom2DPtr phantom;
  if (m_process)
    phantom = m_process->GetPhantom2D();
  if (!phantom)
    return false;

  // Состояние - Пользователь выбрал вторую точку.
  if (!dynamic)
  {
    CreateMacro();                     // Создать объект в документе.
    m_firstPoint = true;               // Сбросить состояние.
    phantom->SetPhantomGroup(nullptr); // Отвязать группу от фантома
    phantom->Update();                 // Синхронизировать фантом с модельной частью
    m_phantomGroup->Delete();          // Удалить группу для фантома
    m_phantomGroup = nullptr;
    return true; // false вернёт управление из Run процесса, но мы предлагаем создать ещё объект.
  }

  // Состояние - пользователь выбрал первую точку, но ещё не выбрал вторую (отрисовка фантома).
  // Запоминаем координаты курсора.
  m_x = x;
  m_y = y;

  // Пересчёт координат для масшабированных видов.
  ksapi::IViewPtr currentView = GetDrawingContainer(m_doc); // Получение активного вида
  if (!currentView)
    return false;
  m_doc->CreateMatrix(0 /*x*/, 0 /*y*/, 0 /*angle*/, currentView->GetScale() /*scaleX*/, currentView->GetScale() /*scaleY*/);
  double beginX = x;
  double beginY = y;
  double endX = m_endX;
  double endY = m_endY;
  m_doc->TransformPointFromMatrix(x /*x*/, y /*y*/, beginX /*xn*/, beginY /*yn*/);
  m_doc->TransformPointFromMatrix(m_endX /*x*/, m_endY /*y*/, endX /*xn*/, endY /*yn*/);
  m_doc->DeleteMatrix();

  // Проверка, что бы не создавать вырожденную линию.
  if (kompasApp)
    if (ksapi::IMath2DPtr math = kompasApp->GetMath2D())
      if (math->GetPointsDistance(beginX, beginY, endX, endY) < kMinLength)
        return true;

  // Перерисовка фантома.
  // Если это первое попадание сюда - нужно создать группу для фантома.
  bool newPhantom = !m_phantomGroup;
  if (newPhantom)
  {
    if (ksapi::IDrawingGroupsPtr groups = m_doc->GetDrawingGroups())
      m_phantomGroup = groups->Add(true /*temp*/, L"phantom" /*name*/);
    if (!m_phantomGroup)
      return false;
    phantom->SetPhantomGroup(m_phantomGroup);
  }
  else
  {
    phantom->Hide();
    m_phantomGroup->Clear(true /*deleteTmp*/); // Тут удалятся временные объекты прошлого фантома
  }

  // Создание нового макрообъекта. Предыдущий удалился при очистке группы.
  ksapi::IAnnotativeContainerPtr macroContainer;
  if (ksapi::IDrawingContainerPtr viewDrawingContainer = currentView)
    if (ksapi::IMacroObjectsPtr macroObjs = viewDrawingContainer->GetMacroObjects())
      macroContainer = macroObjs->Add(false /*multilayer*/);
  if (!macroContainer)
    return false;

  // Наполнение фантомной группы временными объектами
  m_phantomGroup->Open(); // Создаваемые объекты будут попадать в группу
  // Отрисовка стрелки.
  CreateAnnLineSeg(macroContainer, beginX, beginY, endX, endY, ksASUnknown /*sign1*/, ksASArrowInside /*sign2*/,
                   ksCSNormal /*style*/);
  // Габаритный прямоугольник.
  CreateAnnLineSeg(macroContainer, beginX, beginY, beginX, beginY + 20, ksASUnknown, ksASUnknown, ksCSNormal);
  CreateAnnLineSeg(macroContainer, beginX, beginY + 20, beginX + 125, beginY + 20, ksASUnknown, ksASUnknown, ksCSNormal);
  CreateAnnLineSeg(macroContainer, beginX + 125, beginY + 20, beginX + 125, beginY, ksASUnknown, ksASUnknown, ksCSNormal);
  CreateAnnLineSeg(macroContainer, beginX + 125, beginY, beginX, beginY, ksASUnknown, ksASUnknown, ksCSNormal);

  if (ksapi::IMacroObjectPtr macroObj = macroContainer)
    macroObj->Update();
  m_phantomGroup->Close(); // Завершаем наполнение группы

  // Отобразить фантом.
  phantom->Update();
  phantom->Show();

  return true;
}

//-------------------------------------------------------------------------------------
/**
  Обработчик нажатия кнопки спецпанели.
*/
//---
bool UserSymbol::OnButtonClick(int32_t buttonID)
{
  switch (buttonID)
  {
    // Кнопка запуска редактирования линии-выноски, добавлена в параметры процесса в PropertyManagerObject::InitProcessParam
    case IDS_EDIT:
      m_repeat = true;     // Флаг, что нужна будет ещё итерация в методе Draw
      m_editLeader = true; // Флаг, что нужно переключится в режим редактирования линии-выноски.

      // Остановка процесса выбора точки. Требуется запустить редактирование линии-выноски (метод Draw),
      // после которого снова будет запущен наш процесс (на следующей итерации в методе Draw)
      if (kompasApp)
        kompasApp->StopCurrentProcess(true /*postMessage*/, m_doc);
      break;
  }
  return true;
}

//-------------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели.
*/
//---
bool UserSymbol::OnButtonUpdate(int32_t buttonID, bool & enable, int & checked)
{
  switch (buttonID)
  {
    case IDS_EDIT: // Кнопка "Редактировать линию-выноску"
      // Признак активной кнопки - созданная линия-выноска и ожидание следующей точки от пользователя.
      enable = static_cast<bool>(m_leader) && m_firstPoint;
      break;
  }
  return true;
}

//-------------------------------------------------------------------------------------
/**
  Инициализировать параметры процесса получения точки от пользователя..
*/
//---
bool UserSymbol::InitProcessParam()
{
  if (!PropertyManagerObject::InitProcessParam())
    return false;

  m_procParam->SetSpecToolbar(pnEscHelp); // Набор стандартных кнопок спецпанели
  m_procParam->SetCaption(L"Панель");     // Устанавливаем заголовок панели свойств.
  m_procParam->SetAutoReduce(true);       // Процесс завершится после установки всех параметров.

  // Создать кнопку на спецпанели. ID иконки из Компаса - файл шрифта указывать не требуется.
  m_procParam->AddSpecToolbarButton(IDS_EDIT /*buttonId*/, IDC_EDIT_ICO /*iconId*/, LoadResourceString(IDS_EDIT_TIP) /*tips*/,
                                    L"" /*iconFont*/);

  // Включение системной закладки - с координатами выбираемой точки
  if (ksapi::IPropertyTabsPtr propTabs = m_procParam->GetPropertyTabs())
    if (ksapi::IPropertyTabPtr systemTab = propTabs->GetSystemTab())
      systemTab->SetVisible(true);

  return true;
}

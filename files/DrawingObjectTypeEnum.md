<!-- # **Документация перечисления КОМПАС API** -->

# **Перечисление DrawingObjectTypeEnum**

## Общее описание

Перечисление `DrawingObjectTypeEnum` определяет типы графических объектов, которые могут быть размещены на чертеже КОМПАС. Используется для идентификации типа графического объекта при работе с коллекциями, для фильтрации объектов по типу, при создании новых объектов и для определения типа существующего объекта.

## Таблица значений

| Идентификатор | Значение | Название объекта | Интерфейс |
|---------------|----------|-----------------|------------|
| ksUnknown | -1 | Неизвестный объект | - |
| ksAllObj | 0 | Все объекты | - |
| ksDrLineSeg | 1 | Отрезок | ILineSegment |
| ksDrCircle | 2 | Окружность | ICircle |
| ksDrArc | 3 | Дуга | IArc |
| ksDrDrawText | 4 | Текст на чертеже | IDrawingText |
| ksDrPoint | 5 | Точка | IPoint |
| ksDrHatch | 7 | Штриховка | IHatch |
| ksDrBezier | 8 | Bezier сплайн | IBezier |
| ksDrLDimension | 9 | Линейный размер | ILineDimension |
| ksDrADimension | 10 | Угловой размер | IAngleDimension |
| ksDrDDimension | 13 | Диаметральный размер | IDiametralDimension |
| ksDrRDimension | 14 | Радиальный размер | IRadialDimension |
| ksDrRBreakDimension | 15 | Радиальный размер с изломом | IBreakRadialDimension |
| ksDrRough | 16 | Шероховатость | IRough |
| ksDrBase | 17 | База | IBase |
| ksDrWPointer | 18 | Стрелка вида | IViewPointer |
| ksDrCut | 19 | Линия разреза | ICutLine |
| ksDrLeader | 20 | Простая линия выноски | ILeader |
| ksDrPosLeader | 21 | Линия выноски для обозначения позиции | IPositionLeader |
| ksDrBrandLeader | 22 | Линия выноски для обозначения клеймения | IBrandLeader |
| ksDrMarkerLeader | 23 | Линия выноски для обозначения маркирования | IMarkLeader |
| ksDrTolerance | 24 | Допуск формы | ITolerance |
| ksDrTable | 25 | Таблица | IDrawingTable |
| ksDrContour | 26 | Контур | IDrawingContour |
| ksDrMacro | 27 | Нетипизированный макроэлемент | IMacroObject |
| ksDrLine | 28 | Линия | ILine |
| ksLayer | 29 | Слой | ILayer |
| ksDrFragment | 30 | Вставной фрагмент | IFragment |
| ksDrPolyline | 31 | Полилиния | IPolyLine2D |
| ksDrEllipse | 32 | Эллипс | IEllipse |
| ksDrNurbs | 33 | NURBS-кривая по полюсам | INurbs |
| ksDrEllipseArc | 34 | Дуга эллипса | IEllipseArc |
| ksDrRectangle | 35 | Прямоугольник | IRectangle |
| ksDrRegularPolygon | 36 | Многоугольник | IRegularPolygon |
| ksDrEquid | 37 | Эквидистанта | IEquidistant |
| ksDrLBreakDimension | 38 | Линейный размер с обрывом | IBreakLineDimension |
| ksDrABreakDimension | 39 | Угловой размер с обрывом | IBreakLineDimension |
| ksDrOrdinateDimension | 40 | Размер высоты | IHeightDimension |
| ksDrColorFill | 41 | Фоновая заливка цветом | IColouring |
| ksDrCentreMarker | 42 | Обозначение центра | ICentreMarker |
| ksDrArcDimension | 43 | Размер длины дуги | IArcDimension |
| ksDrRaster | 45 | Растровый объект | IRaster |
| ksDrChangeLeader | 46 | Обозначение изменения | IChangeLeader |
| ksDrRemoteElement | 47 | Выносной элемент | IRemoteElement |
| ksDrAxisLine | 48 | Осевая линия | IAxisLine |
| ksDrOLEObject | 49 | Вставка OLE объекта | IOleDrawingObject |
| ksDrUnitNumber | 50 | Номер узла | IUnitNumber |
| ksDrBrace | 51 | Фигурная скобка | IBrace |
| ksDrMarkOnLeader | 52 | Марка/позиционное обозначение с линией-выноской | IMark |
| ksDrMarkOnLine | 53 | Марка/позиционное обозначение на линии | IMark |
| ksDrMarkInsideForm | 54 | Марка/позиционное обозначение без линии-выноски | IMark |
| ksDrWaveLine | 55 | Волнистая линия | IWaveLine |
| ksDrStraightAxis | 56 | Прямая ось | IAxisLine |
| ksDrBrokenLine | 57 | Линия обрыва с изломами | IBrokenLine |
| ksDrCircleAxis | 58 | Круговая ось | ICircularsCentres |
| ksDrArcAxis | 59 | Дуговая ось | ILinearsCentres |
| ksDrCutUnitMarking | 60 | Обозначение узла в сечении | ICutUnitMarkings |
| ksDrUnitMarking | 61 | Обозначение узла | IUnitMarkings |
| ksDrMultiTextLeader | 62 | Выносная надпись к многослойным конструкциям | IMultiTextLeaders |
| ksDrExternalView | 63 | Вставка внешнего вида | IInsertionView |
| ksDrAnnLineSeg | 64 | Аннотационный отрезок | IAnnotativeObject |
| ksDrAnnCircle | 65 | Аннотационная окружность | IAnnotativeObject |
| ksDrAnnEllipse | 66 | Аннотационный эллипс | IAnnotativeObject |
| ksDrAnnArc | 67 | Аннотационная дуга | IAnnotativeObject |
| ksDrAnnEllipseArc | 68 | Аннотационная дуга эллипса | IAnnotativeObject |
| ksDrAnnPolyline | 69 | Аннотационная полилиния | IAnnotativeObject |
| ksDrAnnPoint | 70 | Аннотационная точка | IAnnotativeObject |
| ksDrAnnText | 71 | Текст с аннотационной точкой привязки | IAnnotativeObject |
| ksDrMultiLine | 72 | Мультилиния | IMultiline |
| ksDrBuildingCutLine | 73 | Линия разреза/сечения для СПДС | ICutLine |
| ksDrAttachedLeader | 74 | Присоединённая линия выноски | ILeader |
| ksDrConditionCrossing | 75 | Условное пересечение | - |
| ksReportTable | 76 | Ассоциативная таблица отчёта | IAssociationTable |
| ksEmbodimentsTable | 77 | Таблица исполнений | IAssociationTable |
| ksDrSpecialCurve | 78 | Кривая общего вида | - |
| ksArrayParamTable | 79 | Таблица параметров массива | IAssociationTable |
| ksDrNurbsByPoints | 80 | NURBS-кривая по точкам | - |
| ksDrConicCurve | 81 | Коническая кривая | - |
| ksDrCircularCentres | 84 | Круговая сетка центров | ICircularsCentres |
| ksDrLinearCentres | 85 | Линейная сетка центров | ILinearsCentres |
| ksDrEllipseArcAxis | 86 | Дуговая осевая линия | - |
| ksDrStraightSlot | 87 | Паз | IStraightSlots |
| ksDrArcSlot | 88 | Паз дуговой | IArcSlots |
| ksDrCsPoint | 89 | Точка начала СК | - |
| ksView | 123 | Вид | IView |

---

## Примеры использования

### Пример 1: Определение типа объекта

```cpp
// Определение типа графического объекта
ksapi::IDrawingObjectPtr obj = /* получение объекта */;
if (!obj)
    return;

switch (obj->GetDrawingObjectType())
{
    case DrawingObjectTypeEnum::ksDrCircle:
        // Это окружность
        break;
    case DrawingObjectTypeEnum::ksDrLineSeg:
        // Это отрезок
        break;
    case DrawingObjectTypeEnum::ksDrArc:
        // Это дуга
        break;
    case DrawingObjectTypeEnum::ksDrText:
        // Это текст
        break;
    default:
        // Другой тип объекта
        break;
}
```

### Пример 2: Создание объекта с указанием типа

```cpp
// Создание линии выноски
ksapi::ILeaderPtr leader = leaders->Add(DrawingObjectTypeEnum::ksDrLeader);
if (!leader)
    return;

// Создание позиционной линии выноски
ksapi::IPositionLeaderPtr posLeader = leaders->Add(DrawingObjectTypeEnum::ksDrPosLeader);

// Создание углового размера
ksapi::IAngleDimensionPtr angleDimension = angleDimensions->Add(DrawingObjectTypeEnum::ksDrADimension);
```

### Пример 3: Фильтрация объектов по типу

```cpp
// Получение только макрообъектов вида
std::vector<ksapi::IDrawingObjectPtr> objects = drawingContainer->GetObjects({DrawingObjectTypeEnum::ksDrMacro});

// Получение всех графических объектов
std::vector<ksapi::IDrawingObjectPtr> allObjects = drawingContainer->GetObjects({DrawingObjectTypeEnum::ksAllObj});

// Получение отрезков и полилиний
std::vector<ksapi::IDrawingObjectPtr> lineObjects = drawingContainer->GetObjects({
    DrawingObjectTypeEnum::ksDrLineSeg, 
    DrawingObjectTypeEnum::ksDrPolyline,
    DrawingObjectTypeEnum::ksDrRectangle
});
```

### Пример 4: Проверка типа объекта

```cpp
// Проверка, является ли объект окружностью
bool IsCircle(ksapi::IDrawingObjectPtr obj)
{
    if (!obj)
        return false;
    return obj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle;
}

// Проверка, является ли объект размером
bool IsDimension(ksapi::IDrawingObjectPtr obj)
{
    if (!obj)
        return false;
    auto type = obj->GetDrawingObjectType();
    return type == DrawingObjectTypeEnum::ksDrLDimension ||
           type == DrawingObjectTypeEnum::ksDrADimension ||
           type == DrawingObjectTypeEnum::ksDrDDimension ||
           type == DrawingObjectTypeEnum::ksDrRDimension;
}
```

---

## Частые ошибки

### 1. Неправильное сравнение типов

```cpp
// НЕПРАВИЛЬНО
if (obj->GetDrawingObjectType() == "ksDrCircle")  // Сравнение со строкой!

// ПРАВИЛЬНО
if (obj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
```

### 2. Использование неполного списка типов

```cpp
// НЕПРАВИЛЬНО - забыли про прямоугольник
if (type == DrawingObjectTypeEnum::ksDrLineSeg || 
    type == DrawingObjectTypeEnum::ksDrPolyline)

// ПРАВИЛЬНО
if (type == DrawingObjectTypeEnum::ksDrLineSeg || 
    type == DrawingObjectTypeEnum::ksDrPolyline ||
    type == DrawingObjectTypeEnum::ksDrRectangle)
```
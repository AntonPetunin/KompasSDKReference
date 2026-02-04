<!-- # **Документация интерфейса КОМПАС API** -->

# **Интерфейс ICircle**

## Иерархия наследования

<style>
.top-parent{
    color: #ffe3d8;
    background: green;
    border: 3px solid #046709;
    border-radius: 4px;
    padding: 5px;
    text-align: center;
    margin-bottom: 5px;
    font-weight: bold;
    width: 40%;
    max-width: 220px;
}

.other-parents{
    color: #ffe3d8;
    background: #4a6fa5;
    border: 3px solid #3d5b87;
    border-radius: 4px;
    padding: 5px;
    text-align: center;
    margin: 5px 10%;
    font-weight: bold;
    width: 40%;
    max-width: 220px;
}

.interface{
    color: #ffe3d8;
    background: #103874;
    border: 3px solid #002152;
    border-radius: 4px;
    padding: 5px;
    text-align: center;
    margin: 5px 10%;
    font-weight: bold;
    width: 40%;
    max-width: 220px;
}
</style>

<div style="padding: 10px; background: #f5f5f5; border-radius: 5px; max-width: auto; margin-bottom: 20px">
<div class="top-parent">IAPIObject</div>

<div style="text-align: left; color:black; margin: 5px 15%;">▼</div>
<div class="other-parents">IKompasAPIObject</div>

<div style="text-align: left; color:black; margin: 5px 25%;">▼</div>
<div class="other-parents" style="margin: 5px 20%;">IDrawingObject</div>

<div style="text-align: left; color:black; margin: 5px 35%;">▼</div>
<div class="interface" style="margin: 5px 30%;">ICircle</div>
</div>

## Общее описание

Интерфейс `ICircle` представляет окружность в 2D документах КОМПАС (фрагменты, чертежи). Является графическим объектом, наследующим от `IDrawingObject`, и предоставляет методы для управления параметрами окружности: координатами центра, точки на окружности и радиусом.

Окружность является базовым геометрическим примитивом, широко используемым для создания конструктивных элементов, обозначений отверстий, болтов, гаек и других круглых элементов в технической документации.

## Важные примечания

1. **Вызов Update() обязателен** - После установки всех параметров окружности необходимо вызвать метод `Update()` для применения изменений и отображения объекта на чертеже.

2. **Проверка валидности** - Перед использованием окружности рекомендуется проверять её валидность методом `IsValid()`, особенно при получении из коллекции или при работе с параметрическими документами.

3. **Единицы измерения** - Все координаты и радиус задаются в миллиметрах (мм).

4. **Стиль линии** - По умолчанию окружность создаётся со стилем `ksCSNormal` (основная линия). Стиль можно изменить через унаследованный метод `SetStyle()`.

## Получение интерфейса

### Основные способы получения:

1. **Из коллекции окружностей:**
   - [`IDrawingContainer::GetCircles()`](KsAPI/KsAPI.h:27929) - получение коллекции окружностей из контейнера чертежа
   - [`ICircles::Add()`](KsAPI/KsAPI.h:20183) - создание новой окружности
   - [`ICircles::GetCircle(index)`](KsAPI/KsAPI.h:20181) - получение окружности по индексу

2. **Из документа или контекста:**
   - При работе с эскизом 3D модели через `IFragmentDocument`

3. **Приведение типов:**
   - При получении объекта типа `IDrawingObject` можно выполнить приведение к `ICircle`, если `GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle`

### Примеры получения:

```cpp
// Пример 1: Создание новой окружности через IDrawingContainer
ksapi::IDrawingContainerPtr drawingContainer = /* получение контейнера */;
ksapi::ICirclesPtr circles = drawingContainer->GetCircles();
ksapi::ICirclePtr circle = circles->Add();

// Пример 2: Получение существующей окружности из коллекции
ksapi::ICirclePtr existingCircle = circles->GetCircle(0);

// Пример 3: Приведение типа при работе с выбранным объектом
ksapi::IDrawingObjectPtr baseObj = /* получение объекта */;
if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
{
    ksapi::ICirclePtr circle = baseObj;  // автоматическое приведение
}
```

## Дополнительные интерфейсы

Интерфейс `ICircle` наследует функциональность от `IDrawingObject`, через которую доступны дополнительные интерфейсы:

- **`IUserDataStorage`** - интерфейс хранения пользовательских данных
- **`IPropertyKeeper`** - интерфейс работы со свойствами
- **`ICurve2D`** - интерфейс математической кривой (доступен через `GetCurve2D()`)
- **`IParametricConstraint`** - интерфейс параметрических ограничений

**Связанные интерфейсы для работы с окружностями:**

- **`ICircles`** - коллекция окружностей


## Методы интерфейса

### Группа 1: Координаты центра

- [`SetXc()`](#setxc) - установить координату X центра
- [`GetXc()`](#getxc) - получить координату X центра
- [`SetYc()`](#setyc) - установить координату Y центра
- [`GetYc()`](#getyc) - получить координату Y центра

### Группа 2: Координаты точки на окружности

- [`SetX()`](#setx) - установить координату X точки на окружности
- [`GetX()`](#getx) - получить координату X точки на окружности
- [`SetY()`](#sety) - установить координату Y точки на окружности
- [`GetY()`](#gety) - получить координату Y точки на окружности

### Группа 3: Радиус

- [`SetRadius()`](#setradius) - установить радиус
- [`GetRadius()`](#getradius) - получить радиус

---

### SetXc

[Группа 1: Координаты центра](#группа-1-координаты-центра) | [К оглавлению](#методы-интерфейса)

**Кратко:** Устанавливает X-координату центра окружности.

**Полное описание:**
Метод `SetXc()` задаёт горизонтальную координату центра окружности. Координата измеряется в миллиметрах относительно начала системы координат текущего вида или фрагмента. Изменение координаты центра не требует пересчёта радиуса или точки на окружности.

**Синтаксис:**

```cpp
virtual void SetXc(double xc) = 0;
```

**Параметры:**

- `xc` (in) - координата X центра окружности в миллиметрах (мм)

**Возвращаемое значение:** Метод не возвращает значение. При ошибке будет сгенерировано исключение.

#### **Пример использования**

**Минимальный пример:**

```cpp
circle->SetXc(50.0);
```

**Расширенный пример:**

```cpp
// Создание окружности с заданным центром
ksapi::ICirclePtr circle = circles->Add();
circle->SetXc(100.0);  // Центр по оси X
circle->SetYc(75.0);   // Центр по оси Y
circle->SetRadius(25.0);
circle->Update();
```

**Примечания:**

- Значение может быть любым вещественным числом (положительным, отрицательным или нулём)
- При работе с параметрическими документами изменение координаты может нарушить связи с другими объектами

---

### GetXc

[Группа 1: Координаты центра](#группа-1-координаты-центра) | [К оглавлению](#методы-интерфейса)

**Кратко:** Возвращает X-координату центра окружности.

**Полное описание:**
Метод `GetXc()` возвращает текущее значение горизонтальной координаты центра окружности в миллиметрах. Это значение было установлено явно через `SetXc()` или вычислено на основе других параметров (например, при задании точки на окружности и радиуса).

**Синтаксис:**

```cpp
virtual double GetXc() = 0;
```

**Параметры:** Отсутствуют.

**Возвращаемое значение:** Координата X центра окружности в миллиметрах (double).

#### **Пример использования**

**Минимальный пример:**

```cpp
double xCenter = circle->GetXc();
```

**Расширенный пример:**

```cpp
// Получение координат центра окружности
double xCenter = circle->GetXc();
double yCenter = circle->GetYc();
double radius = circle->GetRadius();

// Вывод информации о окружности
std::cout << "Окружность: центр(" << xCenter << ", " << yCenter << "), радиус " << radius << " мм";
```

---

### SetYc

[Группа 1: Координаты центра](#группа-1-координаты-центра) | [К оглавлению](#методы-интерфейса)

**Кратко:** Устанавливает Y-координату центра окружности.

**Полное описание:**
Метод `SetYc()` задаёт вертикальную координату центра окружности. Координата измеряется в миллиметрах относительно начала системы координат текущего вида или фрагмента. Ось Y направлена снизу вверх.

**Синтаксис:**

```cpp
virtual void SetYc(double yc) = 0;
```

**Параметры:**

- `yc` (in) - координата Y центра окружности в миллиметрах (мм)

**Возвращаемое значение:** Метод не возвращает значение.

#### **Пример использования**

```cpp
circle->SetYc(100.0);
```

---

### GetYc

[Группа 1: Координаты центра](#группа-1-координаты-центра) | [К оглавлению](#методы-интерфейса)

**Кратко:** Возвращает Y-координату центра окружности.

**Полное описание:**
Метод `GetYc()` возвращает текущее значение вертикальной координаты центра окружности в миллиметрах.

**Синтаксис:**

```cpp
virtual double GetYc() = 0;
```

**Параметры:** Отсутствуют.

**Возвращаемое значение:** Координата Y центра окружности в миллиметрах (double).

---

### SetX

[Группа 2: Координаты точки на окружности](#группа-2-координаты-точки-на-окружности) | [К оглавлению](#методы-интерфейса)

**Кратко:** Устанавливает X-координату точки на окружности.

**Полное описание:**
Метод `SetX()` задаёт горизонтальную координату точки, через которую проходит окружность. Эта точка используется совместно с радиусом для определения положения окружности. Расстояние от центра до этой точки должно соответствовать радиусу.

**Синтаксис:**

```cpp
virtual void SetX(double x) = 0;
```

**Параметры:**

- `x` (in) - координата X точки на окружности в миллиметрах (мм)

**Возвращаемое значение:** Метод не возвращает значение.

#### **Пример использования**

```cpp
// Создание окружности через центр и точку на окружности
circle->SetXc(50.0);
circle->SetYc(50.0);
circle->SetX(70.0);  // Точка на окружности (с учётом радиуса)
circle->SetY(50.0);
circle->SetRadius(20.0);
circle->Update();
```

---

### GetX

[Группа 2: Координаты точки на окружности](#группа-2-координаты-точки-на-окружности) | [К оглавлению](#методы-интерфейса)

**Кратко:** Возвращает X-координату точки на окружности.

**Полное описание:**
Метод `GetX()` возвращает текущее значение горизонтальной координаты точки, через которую проходит окружность.

**Синтаксис:**

```cpp
virtual double GetX() = 0;
```

**Параметры:** Отсутствуют.

**Возвращаемое значение:** Координата X точки на окружности в миллиметрах (double).

---

### SetY

[Группа 2: Координаты точки на окружности](#группа-2-координаты-точки-на-окружности) | [К оглавлению](#методы-интерфейса)

**Кратко:** Устанавливает Y-координату точки на окружности.

**Полное описание:**
Метод `SetY()` задаёт вертикальную координату точки, через которую проходит окружность.

**Синтаксис:**

```cpp
virtual void SetY(double y) = 0;
```

**Параметры:**

- `y` (in) - координата Y точки на окружности в миллиметрах (мм)

**Возвращаемое значение:** Метод не возвращает значение.

---

### GetY

[Группа 2: Координаты точки на окружности](#группа-2-координаты-точки-на-окружности) | [К оглавлению](#методы-интерфейса)

**Кратко:** Возвращает Y-координату точки на окружности.

**Полное описание:**
Метод `GetY()` возвращает текущее значение вертикальной координаты точки, через которую проходит окружность.

**Синтаксис:**

```cpp
virtual double GetY() = 0;
```

**Параметры:** Отсутствуют.

**Возвращаемое значение:** Координата Y точки на окружности в миллиметрах (double).

---

### SetRadius

[Группа 3: Радиус](#группа-3-радиус) | [К оглавлению](#методы-интерфейса)

**Кратко:** Устанавливает радиус окружности.

**Полное описание:**
Метод `SetRadius()` задаёт радиус окружности в миллиметрах. Радиус должен быть положительным значением. При нулевом или отрицательном радиусе окружность будет считаться вырожденной (`IsValid()` вернёт `false`).

**Синтаксис:**

```cpp
virtual void SetRadius(double radius) = 0;
```

**Параметры:**

- `radius` (in) - радиус окружности в миллиметрах (мм). Должен быть > 0.

**Возвращаемое значение:** Метод не возвращает значение.

#### **Пример использования**

```cpp
circle->SetRadius(25.0);
```

**Примечания:**

- Радиус должен быть положительным числом
- При создании окружности через центр и точку на окружности убедитесь, что расстояние от центра до точки соответствует радиусу

---

### GetRadius

[Группа 3: Радиус](#группа-3-радиус) | [К оглавлению](#методы-интерфейса)

**Кратко:** Возвращает радиус окружности.

**Полное описание:**
Метод `GetRadius()` возвращает текущее значение радиуса окружности в миллиметрах.

**Синтаксис:**

```cpp
virtual double GetRadius() = 0;
```

**Параметры:** Отсутствуют.

**Возвращаемое значение:** Радиус окружности в миллиметрах (double).

#### **Пример использования**

```cpp
double radius = circle->GetRadius();
if (radius > 0)
{
    // Радиус корректный
}
```

---

## Частые ошибки

### 1. **Забыли вызвать Update()**

```cpp
// НЕПРАВИЛЬНО
ksapi::ICirclePtr circle = circles->Add();
circle->SetXc(50);
circle->SetYc(50);
circle->SetRadius(25);
// Окружность не отображается - забыли Update()

// ПРАВИЛЬНО
ksapi::ICirclePtr circle = circles->Add();
circle->SetXc(50);
circle->SetYc(50);
circle->SetRadius(25);
circle->Update();  // Обязательный вызов для применения изменений
```

### 2. **Проверка на nullptr перед использованием**

```cpp
// НЕПРАВИЛЬНО
ksapi::ICirclesPtr circles = drawingContainer->GetCircles();
ksapi::ICirclePtr circle = circles->Add();
circle->SetXc(50);  // Если circles == nullptr, будет crash

// ПРАВИЛЬНО
ksapi::ICirclesPtr circles = drawingContainer->GetCircles();
if (!circles)
    return nullptr;
ksapi::ICirclePtr circle = circles->Add();
if (!circle)
    return nullptr;
circle->SetXc(50);
circle->Update();
```

### 3. **Отрицательный или нулевой радиус**

```cpp
// НЕПРАВИЛЬНО
circle->SetRadius(-10);  // Вырожденная окружность
circle->Update();
if (!circle->IsValid())  // Будет false
{
    // Ошибка - нужно обработать
}

// ПРАВИЛЬНО
double radius = 10.0;
if (radius > 0)
{
    circle->SetRadius(radius);
    circle->Update();
}
```

### 4. **Неправильное определение типа объекта перед приведением**

```cpp
// НЕПРАВИЛЬНО
ksapi::IDrawingObjectPtr baseObj = /* получение объекта */;
ksapi::ICirclePtr circle = baseObj;  // Может быть не окружностью!

// ПРАВИЛЬНО
ksapi::IDrawingObjectPtr baseObj = /* получение объекта */;
if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
{
    ksapi::ICirclePtr circle = baseObj;  // Теперь безопасно
    // Работа с окружностью
}
```

### 5. **Радиус не соответствует точке на окружности**

```cpp
// НЕПРАВИЛЬНО
circle->SetXc(50);
circle->SetYc(50);
circle->SetX(70);  // Точка на окружности (x = 70)
circle->SetY(50);  // Точка на окружности (y = 50)
circle->SetRadius(10);  // Расстояние от центра до точки = 20, а не 10!

// ПРАВИЛЬНО - вариант 1: задать правильный радиус
circle->SetXc(50);
circle->SetYc(50);
circle->SetX(70);
circle->SetY(50);
circle->SetRadius(20);  // Расстояние от (50,50) до (70,50) = 20

// ПРАВИЛЬНО - вариант 2: использовать только центр и радиус
circle->SetXc(50);
circle->SetYc(50);
circle->SetRadius(10);
```

---

## Практические примеры из исходников

### Пример 1: Создание окружности в эскизе 3D модели

```cpp
// Source/Steps/Step3D3/Step3D3.cpp
void Circle(ksapi::IFragmentDocumentPtr fragmentDocument, double xc, double yc,
            double radius, ksCurveStyleEnum style)
{
    if (!fragmentDocument)
        return;

    ksapi::IDrawingContainerPtr drawingContainer = fragmentDocument->GetDrawingContainer();
    if (!drawingContainer)
        return;

    if (ksapi::ICirclesPtr circles = drawingContainer->GetCircles())
    {
        if (ksapi::ICirclePtr circle = circles->Add())
        {
            circle->SetXc(xc);
            circle->SetYc(yc);
            circle->SetRadius(radius);
            circle->SetStyle(static_cast<int32_t>(style));
            circle->Update();
        }
    }
}
```

### Пример 2: Создание окружности с проверкой валидности

```cpp
// Source/Steps/Step7/Step7.cpp
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r)
{
    if (!drawingContainer)
        return nullptr;

    ksapi::ICirclesPtr circles = drawingContainer->GetCircles();
    if (!circles)
        return nullptr;

    ksapi::ICirclePtr circle = circles->Add();
    if (!circle)
        return nullptr;

    circle->SetXc(xc);
    circle->SetYc(yc);
    circle->SetRadius(r);
    circle->Update();

    return circle;
}
```

### Пример 3: Создание штриховки между двумя концентрическими окружностями

```cpp
// Source/Steps/Step7/Step7.cpp
void CreateHatchBetweenCircles(ksapi::IDrawingContainerPtr drawingContainer)
{
    // Создаём две концентрические окружности
    ksapi::ICirclePtr circle1 = CreateCircle(drawingContainer, 30, 30, 20);
    ksapi::ICirclePtr circle2 = CreateCircle(drawingContainer, 30, 30, 10);

    // Создаём штриховку между ними
    std::vector<ksapi::IDrawingObjectPtr> contours = {circle1, circle2};
    ksapi::IHatchPtr hatch = CreateHatch(drawingContainer, contours);
}
```

### Пример 4: Получение окружности из коллекции по индексу

```cpp
// Source/Steps/Step2_KsAPI_2D/Step2_KsAPI_2D.cpp
// Является ли объект окружностью
if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
{
    // Получить интерфейс окружности
    if (ksapi::ICirclePtr circle = baseObj)
    {
        double xc = circle->GetXc();
        double yc = circle->GetYc();
        double radius = circle->GetRadius();
        // Обработка параметров окружности
    }
}
```

---

## Шаблоны использования

### Шаблон 1: Создание окружности с нуля

```cpp
// Описание: Базовый шаблон для создания окружности в чертеже
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainerPtr container, double xc, double yc, double radius)
{
    if (!container)
        return nullptr;

    ksapi::ICirclesPtr circles = container->GetCircles();
    if (!circles)
        return nullptr;

    ksapi::ICirclePtr circle = circles->Add();
    if (!circle)
        return nullptr;

    circle->SetXc(xc);
    circle->SetYc(yc);
    circle->SetRadius(radius);
    circle->Update();

    return circle;
}
```

### Шаблон 2: Проверка типа объекта перед приведением

```cpp
// Описание: Безопасное приведение IDrawingObject к ICircle
ksapi::ICirclePtr SafeCastToCircle(ksapi::IDrawingObjectPtr obj)
{
    if (!obj)
        return nullptr;

    if (obj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
        return obj;  // автоматическое приведение

    return nullptr;  // объект не является окружностью
}
```

### Шаблон 3: Создание массива окружностей

```cpp
// Описание: Создание нескольких окружностей в цикле
void CreateCirclesPattern(ksapi::IDrawingContainerPtr container, double centerX, double centerY,
                          double startRadius, double radiusStep, int count)
{
    for (int i = 0; i < count; ++i)
    {
        double radius = startRadius + i * radiusStep;
        CreateCircle(container, centerX, centerY, radius);
    }
}
```

---

## Связанные интерфейсы

### Работа в паре с:

- **`IArcs`** - для создания дуг окружностей (части окружностей)
- **`IEllipses`** - для создания эллипсов
- **`IHatch`** - для штриховки областей, ограниченных окружностями
- **`IArcDimension`** - для простановки радиальных и диаметральных размеров

### Часто используется вместе с:

- **`ILineSegments`** - для создания отрезков и ломаных
- **`ISketch`** - для работы с эскизами 3D моделей
- **`IView`** - для работы с видами чертежа
- **`ILayer`** - для размещения объектов на слоях
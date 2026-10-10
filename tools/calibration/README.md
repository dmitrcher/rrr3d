# Физическая калибровка Windows → Mac

Windows / PhysX 2.8.4 считается эталоном. Mac использует **тот же Jolt vehicle
adapter, который работает в игре**. Windows-стенд — отдельная консольная программа
на исходном PhysX SDK; игровые контроллеры, ИИ, RNG, оружие и рендеринг здесь
обходятся. Это позволяет отделить расхождения физики от расхождений игрового ввода.

13 одинаково заданных сценариев: `force`, `force-heavy`, `impulse`, `gravity`,
`spin`, `settle`, `wheel-drive`, `coast`, `brake`, `steer-left`, `steer-right`,
`lateral`, `suspension-drop`. Одинаковые массы, геометрия, материалы, настройки
колёс, стартовые состояния и команды записываются в метаданные. Синтетическая
машина имеет четыре колеса с `NX_WF_CLAMPED_FRICTION`, как исходный Marauder.
Координаты Z-up, +X вперёд; единицы мира исходной игры, секунды, кг и радианы.

**Сила** вводится как ускорение: `F = m*a`; сценарий `force` задаёт `a=(3,0,0)`
в течение 2 секунд. `impulse` задаёт однократное **изменение скорости** `(5,0,0)`.
Момент привода задаётся **на каждое ведущее колесо**, а не на весь двигатель.
В стенде нет коробки передач, assist torque, дополнительной воздушной гравитации
и других игровых модификаторов. Они требуют отдельных сценариев следующего уровня.

## Запуск

На подготовленной Windows VM:

```powershell
C:\MotorRock\calibration\Launch-Calibration.ps1 -Case all
```

Ярлык **MotorRock Physics Calibration** запускает этот сценарий. JSONL сохраняется
в `%LOCALAPPDATA%\MotorRock\calibration`. Из Mac можно вызвать Windows через
Parallels Tools, без автоматизации меню:

```sh
python3 tools/calibration/run-parallels.py --out windows.jsonl
```

Mac-стенд из готового набора инструментов:

```sh
./MotorRock-PhysicsCalibration-macOS mac.jsonl all
```

Или из новой сборки приложения, без открытия окна:

```sh
MotorRock-Calibration.app/Contents/MacOS/RRR3d --physics-calibration mac.jsonl all
```

Вместо `all` допустимо имя одного сценария. Частота записи — **каждый завершённый
шаг 1/60 секунды**. Состояние снимается после solve, без интерполяции рендера.
Есть UTC, монотонное время, номер tick, фактическая команда, позиция, quaternion,
скорость, угловой момент, скорости колёс, высота точек/наличие контактов.

Для другого ускорения можно передать одну и ту же величину обеим сторонам:

```sh
python3 tools/calibration/run-parallels.py --case force --acceleration 6 --out windows-a6.jsonl
RRR3D_CALIBRATION_ACCELERATION=6 ./MotorRock-PhysicsCalibration-macOS mac-a6.jsonl force
```

Также доступны `RRR3D_CALIBRATION_WHEEL_TORQUE`, `RRR3D_CALIBRATION_BRAKE_TORQUE`,
`RRR3D_CALIBRATION_STEER`, `RRR3D_CALIBRATION_DELTA_V`. Это входы эксперимента;
они должны быть одинаковы на Windows и Mac. Для текущего профиля подбора
использовались исходные значения 3 / 600 / 4000 / 0.3 / 5.

## Сравнение и подбор

```sh
python3 tools/calibration/compare.py windows.jsonl mac.jsonl --out report
python3 tools/calibration/fit.py windows.jsonl ./MotorRock-PhysicsCalibration-macOS --out fit
```

Сравнение отклоняет неполные записи, различные параметры/команды, пропуски ticks,
неверную фазу измерения, NaN и неверный порядок платформ. Результат: `report.json`,
`report.md`, `samples.csv`. Quaternion `q` и `-q` считаются одинаковым вращением.

Подбор использует пять сценариев; остальные восемь проверяют результат независимо.
Целевая функция — среднее нормированных RMS координат, скоростей, вращения и
несовпадения контактов. Она не включает времена загрузки и FPS.
Отдельный предохранитель запрещает ухудшать уже правильные ответы на силу и
гравитацию. Профиль не принимается при ухудшении сводной ошибки независимых
сценариев больше чем на 2%. Это компактный поиск коэффициентов, а не доказательство
эквивалентности движков или глобальный оптимум.

Для графиков дополнительно установите matplotlib:

```sh
python3 tools/calibration/plot.py windows.jsonl fit/baseline.jsonl fit/candidate.jsonl --out comparison.png
```

## Коррекция Mac

Профиль — UTF-8 текст `ключ число`, комментарии начинаются с `#`:

| Ключ | Что меняется |
|---|---|
| `solver_substeps` | 1, 2 или 4 внутренних solve на исходный шаг 1/60 |
| `gravity_scale` | гравитация |
| `torque_scale` | момент на ведущих колёсах |
| `brake_scale` | тормозной момент и его предел |
| `spring_scale`, `damper_scale` | пружины и демпферы |
| `longitudinal_grip_scale`, `lateral_grip_scale` | коэффициенты кривых шин |
| `body_friction_scale` | трение корпуса |

Масштабы допустимы в диапазоне 0.1…10. Неизвестный, повторный или некорректный ключ
вызывает явную ошибку создания мира. При отсутствии профиля штатные настройки
сохраняются. Активный профиль записывается событием `calibration.profile`.

```sh
RRR3D_CALIBRATION_PROFILE=profiles/windows-fixture-v3.cfg ./MotorRock-PhysicsCalibration-macOS corrected.jsonl all
```

Для интерактивной игры — `launch-calibrated-game.sh`; локально установлен
`~/Applications/MotorRock-Calibration.app` и лаунчер **MotorRock Windows Calibration.command**.
При обычном запуске приложения профиль не включён. Через лаунчер он включается
только для этого процесса и сохраняет отдельные логи/скриншоты.

## Первая измеренная коррекция — 10 октября 2026

Профиль `profiles/windows-fixture-v3.cfg` экспериментальный. На стенде:

- `a=3`, 2 секунды: Windows `x=6.04999971`, Mac до `6.02500868`, после `6.04999971`;
- падение с `z=100`, `g=-20`, 2 секунды: Windows `z=59.6666489`, Mac до `59.8333588`,
  после `59.6666489`;
- это расхождение устраняется выбором одного solve 1/60 вместо двух solve 1/120;
- сводная нормированная ошибка подбора: `0.412858 → 0.0392239` (примерно −90.5%);
- ошибка восьми независимых сценариев: `0.605355 → 0.0880410` (примерно −85.5%).

Проверены 34 исходных теста Mac, тест профиля и 11 тестов анализатора.
Полный Mac render-smoke с этим профилем прошёл 600 кадров: шесть машин, ИИ,
собственные меню/HUD/камеры/рендер. Это проверка работоспособности, **не сравнение
того заезда с Windows**. Сохраняются расхождения сцепления, подвески и бокового
скольжения. Профиль не записан в штатные игровые параметры. Перед такой заменой
нужна матрица настоящих карт, машин, столкновений и повторов Windows/Mac.

## Сборка стенда

Mac: тот же CMake preset/флаги, что для оригинального меню с Jolt:

```sh
cmake --build build/macos-arm64-m10 --target rrr3d_physics_calibration RRR3d
```

Windows: исходный SDK с `prog/Third Party`, Visual C++ 2008 SP1:

```bat
tools\calibration\build-windows.cmd C:\MotorRock\source\prog C:\MotorRock\calibration
```

Положите рядом `PhysXLoader.dll`, `PhysXCore.dll`, `PhysXCooking.dll`,
`PhysXDevice.dll`, `cudart32_30_9.dll` исходной игры и обычный VC90 CRT.
Для запуска не нужны Debug_NonRedist DLL. Сборка SDK не включена в репозиторий.

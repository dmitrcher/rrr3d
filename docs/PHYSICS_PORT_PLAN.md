Текущий исследовательский checkpoint v64 (12.10.2026): 65/608 точных сценариев, все прежние 65 сохранены; 291 независимый тест совпадает с Windows, 53 проверки сборки прошли. Порядок первого создания ограничений подвески сохраняет улучшение препятствий и устраняет найденную регрессию приземления xCar. По полной матрице максимум ошибки положения улучшен в 265 сценариях, ухудшен в 35. Вариант выключен по умолчанию; полного совпадения и сертификации обычной игры нет. Приоритет — третий шаг столкновения с границей и причины оставшихся ухудшений. Подробности: PHYSICS_SUSPENSION_ORDER_V64_2026-10-12.md.

# Действующая цель сравнения физики — уточнение 12 октября 2026

Довести физическое поведение Mac до эталонной оригинальной Windows/PhysX
версии. Цель остаётся незавершённой, пока есть воспроизводимые расхождения.
Ниже приведён актуальный договор проверки; историческое описание M9.1
сохранено отдельно и не является свидетельством достижения этой цели.

- Сначала устранять крупные ошибки за весь тест: положение, вектор скорости,
  ориентация, угловое движение, контакты колёс, ход и силы подвески. Проверять
  максимальную, среднеквадратичную и конечную ошибки, а не только первый
  несовпадающий кадр. Побитовое сравнение сохраняется для поиска причины.
- Для выбранной ошибки отключать взаимодействующие силы в обоих пробниках,
  проверять одну силу, затем добавлять следующую. Не подгонять мощность,
  массу, коэффициенты шин или другие исходные параметры вместо исправления
  реализации. Windows остаётся неизменным эталоном.
- Базовая проверка: одна машина, без ИИ и без акторов других автомобилей,
  одинаковые данные, начальное состояние и записанные команды по номеру
  физического шага. Повторять каждую проверку для выявления случайности.
  Столкновения двух машин проверять отдельно заданными сценариями без ИИ.
- Проверять полную игровую цепочку: газ/тормоз/руль → передача → момент
  двигателя → фактически приложенные команды колёс. Стенд, напрямую
  задающий момент, не подтверждает совпадение этой цепочки.
- После изолированных проверок запускать одинаковую трассу в обеих играх
  с одной машиной и фиксированным вводом. Подтверждать совпадение идентичности
  карты и автомобиля, шага времени, загрузки и порядка применения команд.
- Каждый принятый результат сопровождать измерениями до/после за весь
  сценарий, проверкой регрессий и проверяемой игровой сборкой. Сдвиг первого
  несовпадения с кадра 22 на 26 сам по себе не является улучшением поведения.
- Экспериментальные исправления не объявлять исправлениями обычной игры,
  пока они не включены в проверенную сборку. Произвольный допуск не называть
  точным совпадением с Windows. Сохранять список непокрытых параметров и
  нерешённых расхождений, синхронизировать принятый код и сборки с GitHub.

Исследование v58/v59 подтвердило четыре дефекта реализации: тяга
прикладывала весь предел сцепления вместо ограничения целевой скоростью;
подвеска пересчитывала потерянный контакт из старой мировой точки;
Mac отключал сохранённые на один шаг строки шин; луч подвески использовал
уменьшенный коробок контактов кузова вместо исходного коробка Windows.

По полным 600 шагам максимум ошибки Monstertruck/wheel2 снизился с
0.734993 (v57) до 0.000680750 (v59), ориентации — с 13.9231° до 0.00111063°.
На боковом бордюре ошибка положения v58 → v59: 0.0983446 → 0.00790117;
ориентации: 3.22368° → 0.0822177°. Изменения подтверждены прямыми
измерениями оригинального SDK, раздельными проверками и двумя повторами
всей матрицы. Исходные коэффициенты машины сохраняются.

Полное совпадение не достигнуто. Контакт с границей остаётся приоритетом:
`border-grounded`, максимум положения 4.24393, ориентации 21.2123°.
До шага 60 положение и скорость совпадают; на шаге 61 отличается реакция кузова.
Следующая проверка — подготовка и решение контактов кузова на шагах 61–63
и их взаимодействие с подвеской. В `bump-crossing` исправленная поверхность
улучшила скорость и подвеску, но ухудшила положение 0.0177955 → 0.0419384;
это открытая регрессия. Подробные текущие результаты:
`PHYSICS_PRACTICAL_V59_2026-10-12.md`.

Не включать всю исследовательскую цепочку в обычную сборку до устранения
регрессий и сравнения полного GameCar с Windows. Повторяемость GameCar
на Mac сама по себе не подтверждает совпадение с Windows. Остаются парные
контакты, динамические поверхности и восемь слов точки косого луча.

# Исторический Milestone 9.1: перенос оригинальной гонки

> Продолжение renderer/particle backend выполнено в
> `docs/MILESTONE_9_2.md`. Ограничения ниже фиксируют границу именно M9.1.

## Результат

Milestone 9.1 продолжает исправленный путь M5–M9: это перенос исходной
логики и данных Motor Rock на portable C++/bgfx/Metal/Jolt, а не новая игра
по мотивам оригинала. Старые `PortableRace` и procedural-трасса не входят в
preset `macos-arm64-m9`.

`Single Player` загружает турнир, профиль, трассу, автомобили, workshop,
окружение, HUD, звук и эффекты из оригинальных XML, `.r3d`, DDS/PNG и Ogg.
Параметрами запуска можно выбрать любую из 88 турнирных трасс, 17 гаражных
машин и штатные варианты погоды.

## Источники и профиль

Portable loader использует оригинальные:

- `tournamet.xml`, `garage.xml`, `workshop.xml`, `db.xml` и карты
  `Data/Map/*/*.r3dMap`;
- visual/collision records, составные и разрушаемые decorations, material
  records, источники света и вложенные `includeList`;
- garage/workshop характеристики, четыре weapon slots, projectile variants,
  mines, hyper, droid и reflector;
- `UserInfo.xml`, `config.xml` и `achievment.xml`.

Профиль сохраняет выбранные машину, персонажа, планету и pass, деньги,
garage/workshop upgrades, музыку, язык, управление, сетевой профиль и полное
состояние achievements. При первом запуске определения achievements берутся
из штатного XML, а пользовательский файл накладывает накопленные значения.
Запись выполняется в Application Support, ресурсы игры не изменяются.

Windows-пути с несовпадающим регистром разрешаются только при единственном
однозначном case-insensitive совпадении. `ResourceFileSystem` по-прежнему
запрещает абсолютные пути, выход из game-data, неоднозначные совпадения и
слишком большие файлы.

## Физика и состояние гонки

Windows остаётся на PhysX 2.8.4. Apple Silicon использует Jolt Physics 5.5.0
через SDL/renderer-независимый `OriginalVehiclePhysics`:

- Z-up игровая система преобразуется в Y-up Jolt;
- static triangle collision строится из штатных `.r3d`;
- параметры body/material, center of mass, колёс, suspension, brakes,
  RPM, transmission, angular stabilization и torque берутся из `db.xml`;
- `CarMotorDesc` остаётся владельцем правил двигателя: полный момент выдаётся
  каждому ведущему колесу, а передачи используют исходные состояния
  neutral/reverse/1..5 без Jolt clutch/inertia;
- workshop `tireSpring` применяется как исходный cutoff реакции шины, а не
  как добавка к жёсткости подвески;
- игрок и все соперники имеют отдельные Jolt vehicles;
- simulation выполняется фиксированными шагами 1/120 s;
- стартовая решётка и reset/respawn повторяют исходные trace и формулы
  `Race::ResetCarPos`.

`OriginalRaceSession` переносит countdown, pause/resume, ordered checkpoints,
laps, place, wrong-way, finish, reset и respawn. В нём работают исходные
roster/loadout соперников, trace-based AI, collision/contact damage,
разрушение decorations, bonuses, shield, death/kill flow и achievements.

Это поведенческий перенос, но не численная эмуляция PhysX. Contact solver,
tire model и порядок разрешения ограничений Jolt отличаются, поэтому
побитовая идентичность траекторий Windows/PhysX не заявляется.

## Workshop, оружие и поддержка

Загружается полный workshop catalog и четыре оригинальных слота. Portable
session учитывает ammunition, cooldown, цены, upgrades, damage/radius,
скорость и lifetime projectile, multi-projectile, ray/attached weapons,
homing torpedo/impulse, frost, laser, fire и дробилку.

Перенесены:

- projectile models и их primary/secondary/tertiary visuals;
- oil, MineRip и остальные mine descriptors, включая grow/owner-lock;
- hyper/acceleration, spring velocity, droid repair и reflector;
- shield, medpack, ammunition, money и speed bonuses;
- AI fire, damage, kills/deaths и source-derived loadouts.

`enableMineBug` из оригинальной конфигурации сохраняет legacy owner-lock для
соответствующих типов мин. `springBorders` в исходнике только передаётся
внутрь старого PhysX wrapper; отдельная выдуманная механика для него не
добавлялась.

## Камера, HUD и mini-map

Камера перенесена по исходным режимам и константам `CameraManager`:
`pcThirdPerson` (названный `Rear view` в UI) и `pcIsometric`, target offset,
speed pull-back, FOV, near/far planes и переключение действий ввода.

HUD использует оригинальные GUI textures, bitmap font и layout-данные. Он
отображает:

- place, lap, speed, RPM, life, ammunition, money и четыре weapon slots;
- mines/hyper/support state, countdown, wrong-way, pause и finish;
- checkpoint/bonus/damage/kill/achievement notifications;
- mini-map по полной trace карты с позициями игрока и соперников.

Координаты mini-map и динамических индикаторов вычисляются из исходной trace,
а не из придуманной овальной схемы.

## Materials, окружение и effect graph

Renderer загружает все track/decor/bonus/car/wheel visual nodes, исходные
material groups и texture atlases. Перенесены ambient/sun/fog, alpha test с
исходным reference, blend/cull/depth flags, specular/shininess, emissive,
`ignoreFog`, UV atlas animation и map/world material mapping.

Environment loader охватывает World1–World5, Crush/Bonus exceptions, sky,
weather, grass, water, ground fog, magma, rain и исходные высоты/скорости
прокрутки. Разрушаемые decorations исчезают и создают исходные эффекты.

Generic parser переносит source `ntSprite`, `ntPlane`, nested `includeList`
и particle emitters: limits, lifetime/start ranges, density, position/scale
ranges, velocity, acceleration/gravity, world coordinate flag, material и
time/distance triggers. Эти данные используются для weapon impacts, trails,
frost/laser endpoints, mines, hyper и environment effects. Portable emitter
детерминированно ограничивает число одновременно рисуемых частиц; точный
D3D9 sorting/billboarding и внутренний scheduler legacy particle manager
не воспроизводятся побитово.

Metal backend пока не имеет отдельных legacy HDR, planar-reflection и
shadow-map passes. Их source-параметры и флаги загружаются, но текущие
отражения/тени являются portable renderer approximations, а не заявленной
графической parity.

## Звук и управление

SDL action layer поддерживает keyboard, mouse и gamepad: непрерывные throttle,
brake/steering, reset, pause, переключение camera modes, выбор каждого weapon
slot, next slot, fire, mine и hyper. Droid/reflector активируются через
штатное действие выбранного workshop slot.

MusicCat сохраняет shuffle, текущий track/cursor и pause, воспроизводит все
три оригинальных menu-трека и декодирует/preload их фоновым worker.

В гонке engine idle/RPM loops следуют физическим оборотам всех машин.
Перенесены исходные weapon, impact, damage, collision, bonus, lap, finish и
commentator Ogg. Для движущихся источников применяются distance attenuation,
stereo pan и pitch/doppler approximation. Legacy X3DAudio cones, obstruction
и его точная DSP-матрица не входят в portable backend.

## Финальная проверка

После завершения всего переноса M9.1 выполнен один финальный цикл:

```bash
cmake --preset macos-arm64-m9
cmake --build --preset macos-arm64-m9 --target RRR3d -j 8

build/macos-arm64-m9/Debug/RRR3d --physics-smoke-test
SDL_AUDIODRIVER=dummy \
  build/macos-arm64-m9/Debug/RRR3d --audio-smoke-test
build/macos-arm64-m9/Debug/RRR3d --race-render-smoke-test
build/macos-arm64-m9/Debug/RRR3d --verify-resources
```

Configure и arm64 Debug build завершились успешно. Physics/session smoke
проверил 1175 исходных collision triangles, source drivetrain/reverse/gears,
Jolt braking/steering/suspension, airborne dynamics, workshop mobility,
countdown, checkpoints/laps/finish, weapon/damage, bonus и respawn.
Audio/MusicCat smoke проверил 182 Ogg,
background decode, все три menu tracks, race sounds и lifecycle mixer.

Resource sweep разрешил все 88 tournament tracks и все 17 garage cars.
Оконный smoke прошёл реальный `MainMenu2 -> Single Player`, 240 кадров
bgfx/Metal race, движение Marauder и контакты всех четырёх колёс.

По прямому требованию для M9.1 Windows/Parallels A/B не выполнялся. Весь
финальный цикл запущен только после завершения кода и документации.

## Честная граница M9.1

Перенесены данные и независимая от Windows логика, найденные в исходной
гонке, профиле, workshop, HUD, camera, material/effect и audio слоях. В
portable target не включаются Windows-only D3D9/PhysX/XAudio2/X3DAudio,
Steam, network и video runtime.

Оставшиеся различия относятся не к подмене ресурсов или выдуманной игровой
механике, а к backend-эквивалентности:

- Jolt не даёт численно идентичный PhysX 2.8 solver/tire model;
- bgfx/Metal ещё требует отдельных HDR/reflection/shadow-map render passes;
- portable particle renderer использует исходные параметры, но не точный
  legacy D3D9 particle scheduler/sorting;
- SDL audio не воспроизводит точную X3DAudio DSP-матрицу.

Эти ограничения не скрываются и не замещаются процедурными аналогами.

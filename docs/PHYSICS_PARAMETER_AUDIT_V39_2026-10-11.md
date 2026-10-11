# Проверка прежних подстроек и игровой цепочки параметров

Проверена история CalibrationTuning, старые профили, прежняя компенсация
торможения, действующий путь подачи момента, исходный GameCar и загрузка
коэффициентов автомобиля. Это первый проход аудита, а не доказательство
корректности всех прежних правок подвески, AI, руления и столкновений.

## Что найдено

Старый `windows-fixture-v3.cfg` действительно содержит fitted-множители:
момент 0.8, тормоза 1.2, пружины 1.2, продольное сцепление 0.2.
Этот профиль явно выбирает `physx_wheel_compat 0`. Его нельзя считать
Windows-эталоном или смешивать с новым source-совместимым режимом.
Но он не загружается по умолчанию: загрузчик читает только явно переданный
`RRR3D_CALIBRATION_PROFILE`, а запускатель выбирает v14-default.
В текущих v39 прогонах все восемь actuator scales равны 1. В существующем
игровом логе найдены 11 различных профилей; все их scales также равны 1,
включая ранее выбранный legacy-вариант. Это проверка записанного лога,
не новая проверка запущенного окна или всех окружений пользователя.

Июльская компенсация отключала powered rest brake, чтобы Jolt не запирал
неведущие колёса. Сейчас она выполняется только при
`!tuning_.physxWheelCompat`; source-совместимый режим сохраняет restTorque.
Её наличие в старой ветке не означает применения поверх новой физики.

В исходном Windows `CarMotorDesc::CalcTorque` уже есть множитель 1.15:
`maxTorque * gearRatio * gearDiff * SEM * 1.15`. Значение эффективности
0.805 соответствует SEM 0.7 и этому игровому множителю; удалять 1.15 как
якобы постороннюю подстройку было бы ошибкой.

## Выявленный пробел проверки двигателя

Mac заранее сворачивает `SEM * 1.15` в `torqueEfficiency`, а потом
умножает четыре фактора. Windows исходник умножает пять последовательно.
Кроме перестановки округлений различается арифметика VC2008 x86 и arm64.

На реальной Windows VM с VC2008 и на Mac выполнен отдельный тест этих
выражений: 4 момента × 3 SEM × 6 передач = 72 строки, два повтора.
Windows source-expression и Mac collapsed-expression отличаются в 27
строках; максимум абсолютной разницы 0.00390625, относительной около
1.18e-7. Даже последовательное исходное выражение даёт межплатформенную
разницу в 20 строках. Простая отмена объединения коэффициентов поэтому
сама по себе не доказывает полного исправления.

Это standalone formula audit; он не вызывает оригинальный GameCar в
полной игре. Дальше необходимо подтвердить момент на границе
GameCar → NxWheelShape и GameCar → Mac adapter в одинаковых игровых
состояниях, затем исправлять вычисления с отдельным численным эталоном.

Текущие физические fixtures передают заданный момент напрямую колесу.
Число 600 — тестовый wheel torque / synthetic engine description, а не
изменение мощности реальных автомобилей. В 189 изоляционных сценариях
двигатель вообще выключен. Их совпадение не подтверждает всю цепочку
SEM, workshop upgrades, передач, ограничения скорости и hyperdrive.

В продолжении аудита нужны: сырой record автомобиля → гаражные upgrades
→ текущие GameCar параметры → рассчитанные команды → применённые силы.
Каждый уровень сравнивается отдельно с Windows; ранее неверные
компенсации отменяются после подтверждения их фактического применения.
Числа, исходник helper и профили: `tools/calibration/experimental/motor-formula-v39`.

Продолжение v40: вызвана оригинальная функция из Windows Debug DLL,
проверены отдельно PC53 и PC24, выявлено отличие literal 0.805F от
свёртки SEM*1.15 на 1 ULP. Production-расчёт двигателя не менялся,
точность реального игрового потока ещё не подтверждена. Подробности и
исправление отдельной ошибки contact cache:
[PHYSICS_CONTACT_V40_2026-10-11.md](PHYSICS_CONTACT_V40_2026-10-11.md).

## v41 follow-up

The solver inertia path was misidentified in v37: the public accessor matched,
but the original internal solver uses float SSE operations with different term
ordering. The corrected experiment closes nine more complete contact fixtures
without changing coefficients. See [v41 evidence](PHYSICS_CONTACT_V41_2026-10-12.md).
The complete goal remains active, with 45/72 tilted fixtures unresolved.

## v42 follow-up

Original internal geometry and cache poses differ from the saved API callback
pose on frozen actors. Separating them closes nine more long contacts (36/72
exact) and the p7 cached normal defect. New offset-COM fixtures also expose a
large restoration bug: preserving integrated COM reduces maximum position drift
from 6.44 to about 0.0000021. Four of six expanded cases still differ at float
rounding scale. No power, grip or suspension coefficients were tuned. See
[v42 evidence](PHYSICS_CONTACT_V42_2026-10-12.md). The full goal remains active.

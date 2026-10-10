# macOS Port Source-Parity Findings

Purpose: record confirmed or high-confidence discrepancies between the original Windows Motor Rock source and the current macOS/portable implementation.

Rules for this file:
- Do **not** fix the code from this audit.
- Record only discrepancies confirmed by direct source comparison.
- Keep rejected hypotheses out of the confirmed list.
- Preserve source quirks/bugs when the portable implementation intentionally matches them.

## Confirmed findings

### F-004 — Finished AI car is not released after finishing

**Severity:** P0 — gameplay

**Windows source behavior**
- `Race::CompleteRace(Player*)` calls `AIPlayer::FreeCar()` when a computer player finishes.
- The finished AI car therefore stops participating in the race world.

**Portable behavior**
- `OriginalRaceSession` marks the racer as `finished` and zeros its input.
- The ordinary `Finish` path does not disable/remove the vehicle from physics/rendering.
- `setVehicleEnabled(false)` exists for other lifecycle paths, so this is not simply an unavailable operation.

**Impact**
A finished AI car can remain visible and physically collide with racers that have not finished yet.

---

### F-005 — AI reward is calculated but not accumulated into AI runtime state

**Severity:** P1 — state parity

**Windows source behavior**
- Final race result processing applies `AddMoney(money + pickMoney)` and `AddPoints(points)` to every `Player`.

**Portable behavior**
- `rewardMoney` / `rewardPoints` are calculated for all racers.
- Accumulation into `runtime.money` and `runtime.points` is performed only for `racer == 0`.
- `completeRemainingRacers()` calculates the reward for AI racers but does not add it to their cumulative state.

**Impact**
Computer-player cumulative money/points diverge from Windows. This does not by itself break the normal single-player championship pass calculation, because the source championship total uses Human/Opponent rather than Computer1..5, but any logic reading AI cumulative state can differ.

---

### F-006 — Human finish loses the original 0.3 s control block and subsequent full brake

**Severity:** P0 — gameplay/control parity

**Windows source behavior**
- Finishing the human player calls `SetBlockTime(0.3f)`.
- While blocked, `HumanPlayer::Control` exits through `Player::IsBlock()`, preventing drive input, shooting, mines and reset.
- After the block expires, the car is put into `GameCar::mcBrake`.

**Portable behavior**
- A `finished` racer is given an empty `VehicleInput{}`, which means `brake = 0`, not source full-brake behavior.
- Weapon/mine/hyper/reset handling does not use an equivalent finished/block guard for the first 0.3 s.

**Impact**
The car behaves differently during the finish-to-menu interval, and combat/reset actions are not blocked exactly as in Windows.

---

### F-007 — Oil (`Maslo`) adds angular velocity instead of replacing angular momentum as in Windows

**Severity:** P0 — vehicle physics parity

**Windows source behavior**
- `Proj::MasloContact` calls `GameCar::LockClutch(strength)`.
- On the first clutch frame, `GameCar::StabilizeForce` transforms angular momentum to local space and **replaces** local Z momentum with `strength * mass`.
- The result is applied with `setAngularMomentum`.
- During the remaining 0.38 s clutch interval, normal Z damping is suppressed.

**Portable behavior**
- The 0.38 s clutch timer, sign/side handling, immunity, mine lock and wheel-traction disable are present.
- The contact path emits `AngularVelocityRequest{..., {0,0,strength}}`.
- Jolt consumes this using additive angular-velocity semantics (`addAngularVelocity` / `AddLinearAndAngularVelocity`).

**Impact**
The portable operation is not equivalent to the source operation. It adds omega directly instead of replacing angular momentum, so mass/inertia/current rotation state affect the result differently.

---

### F-008 — Border damage is incorrectly skipped when vehicle speed is <= 16

**Severity:** P0 — gameplay/damage parity

**Windows source behavior**
- In `GameCar::OnContact`, the `speed > 16` condition gates the spring-border velocity correction only.
- Touch damage is evaluated later from contact force regardless of that speed gate.
- Current data uses border touch damage 5 and force threshold 3,800,000.

**Portable behavior**
- The border-contact path performs a common `continue` when `vehicles[racer].speed <= 16`.
- Because the damage calculation is after this guard, sufficiently strong low-speed border impacts do no damage.

**Impact**
Border collision damage is systematically lower than Windows for strong contacts at speeds <= 16.

---

### F-009 — Car-to-car touch damage is artificially throttled by a 0.25 s cooldown

**Severity:** P0/P1 — gameplay damage parity

**Windows source behavior**
- Car actors subscribe with `NX_NOTIFY_ALL | NX_NOTIFY_CONTACT_MODIFICATION`.
- `ContactReport::onContactNotify` forwards contact notifications directly to `GameCar::OnContact` without a gameplay-level 0.25 s damage throttle.
- `dtTouch` is used for three-second attacker attribution, not as a damage cooldown.

**Portable behavior**
- Jolt records both `OnContactAdded` and `OnContactPersisted`.
- `OriginalRaceSession` applies `touchCooldown_ = 0.25F` around car-to-car touch damage.
- No equivalent fixed 0.25 s throttle exists in the Windows gameplay logic.

**Impact**
Persistent car-to-car contacts can receive touch damage at roughly four evaluations per second instead of following the original contact callback cadence, reducing collision damage.

**Note**
This finding is based on the source subscription/callback path plus the portable persisted-contact path. The old PhysX SDK headers are not present in the checkout, so the exact numeric bit expansion of `NX_NOTIFY_ALL` was not re-derived locally.

---

### F-010 — Purchased `armor4` achievement has no gameplay effect in the portable race builder

**Severity:** P0/P1 — upgrade/progression parity

**Windows source behavior**
- `achievment.xml` defines `armor4` as a purchasable achievement item (`asUnlocked`, price 20000) with no generic `records`.
- `ArmorItem::CheckArmor4()` calls `CheckAchievment("armor4")`.
- `ArmorItem::CalcLife()` grants an additional **+10 HP** when armor4 is opened and the relevant armor state is installed.
- That +10 is applied before the human difficulty multiplier.

**Portable behavior**
- The achievement menu allows `armor4` to be purchased, changes its state to `asOpened`, subtracts 20,000 achievement points and saves the profile.
- Generic achievement unlocks with `records` are implemented in `OriginalGarage`, but `armor4` has no records and relies on its special Windows code path.
- The portable race/loadout/life construction path does not consume `armor4` state and does not add the +10 base HP.

**Impact**
The player can spend 20,000 achievement points on armor4 and receive no corresponding race durability benefit.
For the human car the missing +10 base HP becomes approximately:
- Easy: +20 HP missing
- Normal: +17.5 HP missing
- Hard: +15 HP missing

The portable network/profile model also contains an `armor4` boolean, but the race-physics/life path does not currently apply the special effect.

## Checked hypotheses that were rejected

These were inspected and currently should **not** be treated as porting bugs:

- Border contact correctly resets the oil/clutch timer in portable code.
- Shield/immortality pickup replaces duration rather than stacking it, matching Windows.
- Repair droid intentionally heals +5 HP; portable preserves the Windows quirk instead of using `repairValue`.
- Death-plane timing is based on the vehicle body box touching Z=0 and is materially consistent with the Windows plane contact.
- Three-second touch-attacker attribution for a later death-plane death is present.
- Reflector behavior and human armor difficulty multipliers are implemented.
- Thunder reflection uses the portable `TrackBorder` classification corresponding to the Windows `cdgShotTransparency` track mesh and preserves the 0.1 s reflection cooldown.
- Impulse multi-target hit count/damage progression is implemented.
- Resonanse rotation is implemented.
- Spring-border bounce math was compared and is materially equivalent apart from F-008's misplaced speed gate.

## Unconfirmed candidates

### U-001 — PhysX `computeKineticEnergy()` vs portable `0.5 * m * v^2`

Windows selects the lower-energy car in a car-to-car collision using `NxActor::computeKineticEnergy()`, while portable code uses translational `0.5 * mass * speed^2`.

This may differ if the PhysX 2.x function includes rotational kinetic energy, but the exact PhysX 2.x SDK definition/documentation was not available in the checkout. Do not classify this as a confirmed bug until that semantic is verified.

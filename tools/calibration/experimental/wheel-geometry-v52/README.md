# Wheel geometry investigation v52 (preliminary)

Only probe logging has changed. The exact before/after source and patch require the existing v51 experimental chain and the hashes in base.json. They are not a production physics patch. RRR3D_WHEEL_GEOMETRY_TRACE=1 emits cached contact point, normal, suspension length and the actor pose before the step. All other values disable it.

Four actual scenarios contain 1980 physical states per run. Two independent logging-on and two logging-off runs match the published v51 Mac baseline; additional original Windows telemetry repeats exactly and preserves its unchanged broad reference. On first contact the preceding actor poses match Windows float32 words, yet wheel contact points and suspension lengths differ. The evidence records those values; it does not prove which internal raycast operation caused them or claim a correction.

See the companion repository report docs/PHYSICS_WHEEL_GEOMETRY_V52_2026-10-12.md. The broad matrix remains 65/608 exact and 543 unresolved; the overall goal remains active.

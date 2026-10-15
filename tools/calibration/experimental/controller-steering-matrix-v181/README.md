# V181: original body steering matrix contract

Six owned Source files and four Jolt MIT SDK files over V179 Source/V154 SDK. `RRR3D_EXPERIMENT_GAMECAR_STEER_MATRIX_V181=1`, default OFF. Original WFP angle arithmetic uses explicit measured float/double boundaries. A command marks whether original steering's contact/clutch/angle condition is satisfied, even for computed yaw0. OriginalRaceSession forwards it.

Apply the original rear-offset, yaw-matrix and inverse-offset products rather than a quaternion shortcut. Retain the cancellation of pivot translations at yaw0. Convert the resulting matrix back to a normalized quaternion. The initial public-vs-physical matrix decoder, normalizer and exact-pose case are refined by V182/V183; V181 alone is not final tilted steering acceptance.

The SDK addition accepts an already computed center-of-mass pose and performs the usual bounds/sleep/broadphase/activation update. Existing API functions are unchanged. It avoids a second Jolt origin-to-center conversion after the adapter has reproduced original arithmetic. SDK source is MIT; its license is included. SourceGameCar/bridge additions default neutral.

V181's frozen calibration binary passes the full608-case regression twice,255864 states per repetition. Four raw maps remain7200 exact, the rebuilt motor matrix remains27024 exact,53 CTests pass. Later final controller/tilt/inertia acceptance belongs to V184, whose fresh full regression is separately recorded. Full ordinary race remains open.

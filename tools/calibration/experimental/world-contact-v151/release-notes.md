The experimental adapter now uses the original converter quaternion association and returns body corner contact points through the original world body matrix. Other quaternion products and relative clipping retain their distinct arithmetic. No vehicle coefficients or Windows reference binaries change.

All four geometry/solver rows and the complete moving state at map0 sample358 now match Windows. Repeated moving maps0/16/48/64 first differ at359/249/358/238, versus358/244/358/238 previously. Independent flag ablations confirm the two causes. This is not complete moving/game parity.

Both full608-scenario runs match255864 original float32 physical states. All53 CTest checks pass; default-off900 states preserve V150. The archive includes the verified arm64 Mac calibration probe, unchanged WindowsWorldProbeV133, fixtures, profile and guarded three-file Source overlay with pinned unchanged V150 SDK prerequisites. Rebuild all dependent targets after applying. Ordinary installed game remains unchanged and proprietary dependencies are excluded.

Next359 is isolated to mixed contact normals/part order and a missed triangle separation test. Offline geometry is exact after explicit diagnostic reordering; general runtime order, material/cache frames and pair handling are still pending. See NEXT.md and manifold359-proof.json.

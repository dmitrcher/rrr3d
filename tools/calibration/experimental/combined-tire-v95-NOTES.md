# V95 research-note correction

The published V95 `next-work.md` misidentifies `native_body0.words[3]` as signed inverse mass. The actual inverse mass is `words[11]`: word979006478, value0.0008333333535119891, matching Mac for body-tilt-roll physical10 in both repeats. The static body inverse mass at word11 is0. Word3 has a different service value and is not inverse mass.

This corrects an investigation note, without changing physics, captured observations or the V95 source/ZIP manifests. The first prepared contact mass still differs1138.2818603515625 versus1138.28198 with equal incoming body, inverse inertia, lever, raw bias and now explicitly verified inverse mass. Contact normal/frame and initializer arithmetic remain unbound. No V96 physics fix is implied.

The verified V95 checkpoint remains396/608 exact,212 open. Original source commitb8907a6d1e3cea16b4d950f97a5011ec8a03811f and archive SHA25656f71d646998887483f3bcf60b49006c56714ab64ff198f7d4dc6e17edb3338d remain unchanged.

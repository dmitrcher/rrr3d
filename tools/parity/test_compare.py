import json
from pathlib import Path
import tempfile
import unittest
from compare import compare, image_difference

class CompareTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.addCleanup(self.temp.cleanup)
    def trace(self, name, map='Data/Map/test.r3dMap', seed='42', position=None, duplicate=False):
        rows = [{'event':'rng.seed','detail':seed}, {'event':'race.begin','detail':map}, {'event':'physics.sample','detail':'','sample': {'tick':9,'racer':0,'dt':1/60,'position':position or [0,0,0],'velocity':[0,0,0],'rotation':[0,0,0,1],'speed':0,'lap':0}}]
        if duplicate: rows.append(rows[-1].copy())
        for i, r in enumerate(rows): r.update(utc='2026-10-10T00:00:00Z',monotonic_ms=i*100,platform='test')
        p = self.root/name; p.write_text('\n'.join(json.dumps(r) for r in rows)); return p
    def test_equal_samples(self):
        r=compare(self.trace('a'),self.trace('b',map='data\\map\\test.r3dMap'),self.root/'out')
        self.assertTrue(r['comparable_scenario']); self.assertTrue(r['identical_sample_values'])
        self.assertEqual(r['maximum_errors']['rotation_error_radians'],0)
    def test_displacement(self):
        r=compare(self.trace('a'),self.trace('b',position=[3,4,0]),self.root/'out')
        self.assertEqual(r['maximum_errors']['position_error'],5)
    def test_mismatch_rejected(self):
        r=compare(self.trace('a'),self.trace('b',map='other',seed='99'),self.root/'out')
        self.assertFalse(r['comparable_scenario'])
    def test_seed_experiment_allowed(self):
        r=compare(self.trace('a'),self.trace('b',seed='99'),self.root/'out',True)
        self.assertTrue(r['comparable_scenario'])
    def test_multiple_races_rejected(self):
        with self.assertRaisesRegex(ValueError,'Multiple'): compare(self.trace('a',duplicate=True),self.trace('b'),self.root/'out')
    def test_malformed_json_rejected(self):
        p=self.root/'bad'; p.write_text('{')
        with self.assertRaisesRegex(ValueError,'bad:1'): compare(p,self.trace('b'),self.root/'out')

if __name__ == '__main__': unittest.main()

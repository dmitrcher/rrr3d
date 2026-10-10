import copy
import json
from pathlib import Path
import tempfile
import unittest
from compare import compare


def fixture(platform):
    meta={'schema':1,'fixture':'unit-fixture','dt':1/60,'steps':2,'sample_phase':'after_solve','mass':1200,'gravity':0,'height':100,'ground':False,'profile':'none','units':'source'}
    def row(event,s=None):return dict(platform=platform,event=event,detail='force',sample=s)
    rows=[row('calibration.begin',meta)]
    for tick in range(2):rows.append(row('calibration.sample',{'tick':tick,'time':(tick+1)/60,'command':{'force':[3,0,0]},'position':[.01*(tick+1),0,100],'velocity':[.05*(tick+1),0,0],'angular_momentum':[0,0,0],'rotation':[0,0,0,1],'wheels':[{'height':0,'angular_speed':0,'contact':False} for _ in range(4)]}))
    rows.append(row('calibration.end'))
    return rows

class CompareTest(unittest.TestCase):
    def setUp(self):self.tmp=tempfile.TemporaryDirectory();self.root=Path(self.tmp.name);self.a=fixture('windows-physx-d3d9');self.b=fixture('mac-jolt-metal')
    def tearDown(self):self.tmp.cleanup()
    def run_compare(self):
        for n,rows in [('a',self.a),('b',self.b)]: (self.root/n).write_text('\n'.join(json.dumps(r) for r in rows))
        return compare(self.root/'a',self.root/'b',self.root/'report')
    def test_equal(self):self.assertEqual(self.run_compare()['cases']['force']['rms']['position_error'],0)
    def test_exact_parity_gate_equal(self):
        self.assertTrue(self.run_compare()['exact_parity']['complete'])
    def test_exact_parity_gate_detects_wheel_height(self):
        for rows in [self.a,self.b]:
            for row in rows[1:3]:
                for wheel in row['sample']['wheels']:wheel['contact']=True
        self.b[1]['sample']['wheels'][0]['height']=1
        report=self.run_compare()
        self.assertFalse(report['exact_parity']['complete'])
        self.assertEqual(report['exact_parity']['cases_remaining'],['force'])
        self.assertEqual(report['cases']['force']['maximum']['wheel_height_error'],.5)
    def test_quaternion_sign(self):
        self.b[1]['sample']['rotation']=[0,0,0,-1];self.assertEqual(self.run_compare()['cases']['force']['rms']['rotation_error_radians'],0)
    def test_real_divergence(self):
        self.b[1]['sample']['velocity'][0]+=1
        self.assertGreater(self.run_compare()['cases']['force']['rms']['velocity_error'],.7)
    def test_suspension_divergence(self):
        for rows in [self.a,self.b]:
            for row in rows[1:3]:
                for wheel in row['sample']['wheels']:
                    wheel['contact']=True;wheel['suspension_length']=.2
        self.b[1]['sample']['wheels'][0]['suspension_length']=.6
        self.assertGreater(self.run_compare()['cases']['force']['rms']['suspension_length_error'],.14)
    def test_truncated(self):
        self.b.pop()
        with self.assertRaises(ValueError):self.run_compare()
    def test_changed_command(self):
        self.b[1]['sample']['command']['force'][0]=4
        with self.assertRaises(ValueError):self.run_compare()
    def test_nan(self):
        self.b[1]['sample']['velocity'][0]=float('nan')
        with self.assertRaises(ValueError):self.run_compare()
    def test_duplicate_tick(self):
        self.b[2]['sample']['tick']=0
        with self.assertRaises(ValueError):self.run_compare()
    def test_changed_mass(self):
        self.b[0]['sample']['mass']=2400
        with self.assertRaises(ValueError):self.run_compare()
    def test_changed_dt(self):
        self.b[0]['sample']['dt']=1/120
        with self.assertRaises(ValueError):self.run_compare()
    def test_wrong_reference(self):
        self.a[0]['platform']='mac-jolt-metal'
        with self.assertRaises(ValueError):self.run_compare()
    def test_duplicate_case(self):
        self.b+=copy.deepcopy(self.b)
        with self.assertRaises(ValueError):self.run_compare()

if __name__=='__main__':unittest.main()

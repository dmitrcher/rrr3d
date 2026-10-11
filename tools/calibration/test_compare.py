import copy
import json
import math
from pathlib import Path
import tempfile
import unittest
from compare import compare, rotation_error


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
    def test_tiny_rotation_is_not_rounded_to_zero(self):
        angle=1e-9
        self.b[1]['sample']['rotation']=[0,math.sin(angle/2),0,math.cos(angle/2)]
        report=self.run_compare()
        self.assertAlmostEqual(report['cases']['force']['maximum']['rotation_error_radians'],angle,places=16)
        self.assertFalse(report['exact_parity']['complete'])
    def test_rotation_metric_nonunit_and_large_angle(self):
        self.assertEqual(rotation_error([.25,.5,.25,.5],[.25,.5,.25,.5]),0)
        self.assertEqual(rotation_error([.25,.5,.25,.5],[-.25,-.5,-.25,-.5]),0)
        self.assertAlmostEqual(rotation_error([0,0,0,1],[1,0,0,0]),math.pi)
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
    def test_opposite_axis_errors_do_not_cancel(self):
        self.b[1]['sample']['position'][0]+=1
        self.b[2]['sample']['position'][0]-=1
        self.b[2]['sample']['velocity'][2]+=2
        report=self.run_compare()['cases']['force']
        self.assertEqual(report['rms']['position_x_error'],1)
        self.assertEqual(report['rms']['position_y_error'],0)
        self.assertAlmostEqual(report['rms']['velocity_z_error'],2**.5)
        self.assertEqual(report['maximum']['velocity_z_error'],2)
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
    def sdk_pair(self):
        for rows in (self.a, self.b):
            rows[0]['sample'].update(fixture='source-vehicle-matrix-v7-sdk',
                                    pair={'peer_mass':2400},
                                    fixture_parameters={'center_of_mass':[0,0,0]})
        for event,mass,inertia in [('calibration.engine-description',1200,[1,2,3]),
                                   ('calibration.peer-description',2400,[2,4,6])]:
            self.a.insert(1, dict(platform='windows-physx-d3d9',event=event,detail='force',
                                 sample={'mass':mass,'inertia':inertia,'com':[0,0,0]}))
    def test_correct_sdk_peer_description(self):
        self.sdk_pair()
        self.assertTrue(self.run_compare()['exact_parity']['complete'])
    def test_legacy_sdk_peer_reference_rejected(self):
        self.sdk_pair()
        self.a=[r for r in self.a if r['event']!='calibration.peer-description']
        with self.assertRaisesRegex(ValueError,'recapture with the corrected v14 stand'):self.run_compare()
    def test_sdk_peer_assignment_inertia_rejected(self):
        self.sdk_pair()
        next(r for r in self.a if r['event']=='calibration.peer-description')['sample']['inertia']=[.2,.4,.6]
        with self.assertRaisesRegex(ValueError,'wheel-description copy construction'):self.run_compare()
    def test_sdk_actual_peer_mass_rejected(self):
        self.sdk_pair()
        next(r for r in self.a if r['event']=='calibration.peer-description')['sample']['mass']=1200
        with self.assertRaisesRegex(ValueError,'actor mass differs'):self.run_compare()

if __name__=='__main__':unittest.main()

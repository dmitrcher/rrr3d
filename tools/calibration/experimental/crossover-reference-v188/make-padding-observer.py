"""Disable live-code observation by redirecting owned observer points to PE padding.
Original game/SDK files and original observer stay unchanged. Embedded source stamp
is historical; this derived executable is identified by the patch receipt instead.
"""
from pathlib import Path
import struct,json,hashlib
w=Path('work/crossover-reference-v188');source=Path('work/controller-stabilize-v185/native/StabilizeProbeV185.exe');target=w/'prefix/drive_c/MotorRock/game-debug/StabilizeProbeV185PaddingControl.exe';game=Path('work/package/MotorRock-Windows-Debug/Rock3dGame.dll');b=bytearray(source.read_bytes());g=game.read_bytes();nt=struct.unpack_from('<I',g,0x3c)[0];count=struct.unpack_from('<H',g,nt+6)[0];size=struct.unpack_from('<H',g,nt+20)[0];text=None
for i in range(count):
 o=nt+24+size+i*40
 if g[o:o+8].rstrip(b'\0')==b'.text':text=struct.unpack_from('<IIII',g,o+8)
assert text;virtual_size,rva,raw_size,raw_offset=text;assert raw_size-virtual_size>=9
padding=[rva+virtual_size+i for i in range(9)]
original=[0x49847a,0x498496,0x498994,0x4aa815,0x498856,0x49895c,0x94dd0a,0x94cc45,0x94cc87]
offsets=[]
for i,(old,new) in enumerate(zip(original,padding)):
 sig=struct.pack('<I',old);at=b.find(sig);assert at>=4 and b.find(sig,at+1)<0
 assert b[at-4:at]==bytes([0xc7,0x44,0x24,0x18+4*i]),'Unexpected instruction; do not patch'
 offsets.append(at);b[at:at+4]=struct.pack('<I',new)
assert offsets==list(range(offsets[0],offsets[0]+8*9,8))
target.write_bytes(b)
r=dict(scope=__doc__.strip(),original_observer_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),derived_observer_sha256=hashlib.sha256(b).hexdigest(),game_sha256=hashlib.sha256(g).hexdigest(),observer_instruction_immediate_offsets=offsets,validated_instruction="mov dword ptr [esp+disp8], imm32",old_rvas=original,padding_rvas=padding,padding_file_bytes=list(g[raw_offset+virtual_size:raw_offset+virtual_size+9]),outside_original_text_virtual_size=True,original_game_disk_unchanged=True,original_observer_unchanged=True,no_live_code_breakpoints=True,embedded_build_identity_is_historical=True)
(w/'padding-observer-patch.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r,indent=2))

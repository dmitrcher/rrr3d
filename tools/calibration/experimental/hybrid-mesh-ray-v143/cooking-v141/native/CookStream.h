#include <NxStream.h>
#include <vector>
#include <stdexcept>
#include <cstring>
class CalibrationStream : public NxStream {
 std::vector<NxU8> bytes; mutable size_t cursor;
 template<class T> T read() const {T value;readBuffer(&value,sizeof(value));return value;}
 template<class T> NxStream& store(T value){return storeBuffer(&value,sizeof(value));}
public:
 const std::vector<NxU8>& data()const{return bytes;}
 CalibrationStream():cursor(0){}
 virtual NxU8 readByte() const{return read<NxU8>();}
 virtual NxU16 readWord() const{return read<NxU16>();}
 virtual NxU32 readDword() const{return read<NxU32>();}
 virtual NxF32 readFloat() const{return read<NxF32>();}
 virtual NxF64 readDouble() const{return read<NxF64>();}
 virtual void readBuffer(void* data,NxU32 size) const {if(cursor+size>bytes.size())throw std::runtime_error("Cooked mesh stream truncated");std::memcpy(data,&bytes[cursor],size);cursor+=size;}
 virtual NxStream& storeByte(NxU8 v){return store(v);}
 virtual NxStream& storeWord(NxU16 v){return store(v);}
 virtual NxStream& storeDword(NxU32 v){return store(v);}
 virtual NxStream& storeFloat(NxReal v){return store(v);}
 virtual NxStream& storeDouble(NxF64 v){return store(v);}
 virtual NxStream& storeBuffer(const void* data,NxU32 size){const NxU8* p=static_cast<const NxU8*>(data);bytes.insert(bytes.end(),p,p+size);return *this;}
};
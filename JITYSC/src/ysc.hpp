#pragma once
#include <vector>
#include <map>

#define OSTART(size) constexpr int SIZE = size; if (label_mode) {}  else {
#define OEND }; cur_ip += SIZE ;

#pragma pack(push, 1)
struct uint24_t
{
	int value : 24;
};
#pragma pack(pop)

#define PUSH8(t) *(uint8_t*)opcode_stream = (uint8_t)t; opcode_stream += 1;
#define PUSH16(t) *(uint16_t*)opcode_stream = (uint16_t)t; opcode_stream += 2;
#define PUSH32(t) *(uint32_t*)opcode_stream = (uint32_t)t; opcode_stream += 4;
#define PUSH24(t) (*(uint24_t*)opcode_stream).value = (uint32_t)t; opcode_stream += 3;

namespace ysc {
	class program {
		using callback_t = void(*)(program& prog);
		enum class Opcode : uint8_t
		{
			NOP,
			IADD,
			ISUB,
			IMUL,
			IDIV,
			IMOD,
			INOT,
			INEG,
			IEQ,
			INE,
			IGT,
			IGE,
			ILT,
			ILE,
			FADD,
			FSUB,
			FMUL,
			FDIV,
			FMOD,
			FNEG,
			FEQ,
			FNE,
			FGT,
			FGE,
			FLT,
			FLE,
			VADD,
			VSUB,
			VMUL,
			VDIV,
			VNEG,
			IAND,
			IOR,
			IXOR,
			I2F,
			F2I,
			F2V,
			PUSH_CONST_U8,
			PUSH_CONST_U8_U8,
			PUSH_CONST_U8_U8_U8,
			PUSH_CONST_U32,
			PUSH_CONST_F,
			DUP,
			DROP,
			NATIVE,
			ENTER,
			LEAVE,
			LOAD,
			STORE,
			STORE_REV,
			LOAD_N,
			STORE_N,
			ARRAY_U8,
			ARRAY_U8_LOAD,
			ARRAY_U8_STORE,
			LOCAL_U8,
			LOCAL_U8_LOAD,
			LOCAL_U8_STORE,
			STATIC_U8,
			STATIC_U8_LOAD,
			STATIC_U8_STORE,
			IADD_U8,
			IMUL_U8,
			IOFFSET,
			IOFFSET_U8,
			IOFFSET_U8_LOAD,
			IOFFSET_U8_STORE,
			PUSH_CONST_S16,
			IADD_S16,
			IMUL_S16,
			IOFFSET_S16,
			IOFFSET_S16_LOAD,
			IOFFSET_S16_STORE,
			ARRAY_U16,
			ARRAY_U16_LOAD,
			ARRAY_U16_STORE,
			LOCAL_U16,
			LOCAL_U16_LOAD,
			LOCAL_U16_STORE,
			STATIC_U16,
			STATIC_U16_LOAD,
			STATIC_U16_STORE,
			GLOBAL_U16,
			GLOBAL_U16_LOAD,
			GLOBAL_U16_STORE,
			J,
			JZ,
			IEQ_JZ,
			INE_JZ,
			IGT_JZ,
			IGE_JZ,
			ILT_JZ,
			ILE_JZ,
			CALL,
			GLOBAL_U24,
			GLOBAL_U24_LOAD,
			GLOBAL_U24_STORE,
			PUSH_CONST_U24,
			SWITCH,
			STRING,
			STRINGHASH,
			TEXT_LABEL_ASSIGN_STRING,
			TEXT_LABEL_ASSIGN_INT,
			TEXT_LABEL_APPEND_STRING,
			TEXT_LABEL_APPEND_INT,
			TEXT_LABEL_COPY,
			CATCH,
			THROW,
			CALLINDIRECT,
			PUSH_CONST_M1,
			PUSH_CONST_0,
			PUSH_CONST_1,
			PUSH_CONST_2,
			PUSH_CONST_3,
			PUSH_CONST_4,
			PUSH_CONST_5,
			PUSH_CONST_6,
			PUSH_CONST_7,
			PUSH_CONST_FM1,
			PUSH_CONST_F0,
			PUSH_CONST_F1,
			PUSH_CONST_F2,
			PUSH_CONST_F3,
			PUSH_CONST_F4,
			PUSH_CONST_F5,
			PUSH_CONST_F6,
			PUSH_CONST_F7
		};

	public:
		std::vector<uint64_t> native_table{};
		std::uint8_t* opcodes;
		std::uint32_t opcode_size;
		uint32_t current_params{};

		std::vector<const char*> m_strings;
	private:
		std::map<const char*, uint32_t> labels{};
		std::uint32_t cur_ip = 0;
		/////std::uint32_t nsize;
		std::uint8_t* opcode_stream;
		bool label_mode = false;
		int m_string_index;

		inline uint16_t get_or_make_native_idx(uint64_t nhash)
		{
			native_table.emplace_back(nhash);
			for (int i = 0; i < native_table.size(); i++) if (native_table[i] == nhash) return i;
			__assume(0);
		}

		inline uint32_t get_label_ip(const char* label) {
			if (labels.count(label) == 0) throw "cannot find label";
			return labels[label];
		}

	public:
		inline program(callback_t cb) :
			cur_ip(0)
		{
			opcodes = new uint8_t[2048];
			label_mode = true;
			cb(*this);
			label_mode = false;
			cur_ip = 0;
			opcode_stream = opcodes;
			cb(*this);

			opcode_size = (opcode_stream - opcodes) + 1;
		}

		inline ~program()
		{
			delete[] opcodes;
		}

		inline void push(uint32_t val) {
			OSTART(5)
				PUSH8(Opcode::PUSH_CONST_U32)
				PUSH32(val)
			OEND
		}

		inline void pushf(float val) {
			OSTART(5)
				PUSH8(Opcode::PUSH_CONST_U32)
				PUSH32(*(uint32_t*)&val)
			OEND
		}

		inline void iadd() {
			OSTART(1)
				PUSH8(Opcode::IADD)
			OEND
		}

		inline void isub() {
			OSTART(1)
				PUSH8(Opcode::ISUB)
			OEND
		}

		inline void imul() {
			OSTART(1)
				PUSH8(Opcode::IMUL)
			OEND
		}

		inline void idiv() {
			OSTART(1)
				PUSH8(Opcode::IDIV)
			OEND
		}

		inline void fadd() {
			OSTART(1)
				PUSH8(Opcode::FADD)
			OEND
		}

		inline void fsub() {
			OSTART(1)
				PUSH8(Opcode::FSUB)
			OEND
		}

		inline void fmul() {
			OSTART(1)
				PUSH8(Opcode::FMUL)
			OEND
		}

		inline void fdiv() {
			OSTART(1)
				PUSH8(Opcode::FDIV)
			OEND
		}

		inline void vadd() {
			OSTART(1)
				PUSH8(Opcode::VADD)
			OEND
		}

		inline void vsub() {
			OSTART(1)
				PUSH8(Opcode::VSUB)
			OEND
		}

		inline void vmul() {
			OSTART(1)
				PUSH8(Opcode::VMUL)
			OEND
		}

		inline void vdiv() {
			OSTART(1)
				PUSH8(Opcode::VDIV)
			OEND
		}

		inline void ieq() {
			OSTART(1)
				PUSH8(Opcode::IEQ)
			OEND
		}

		inline void inot() {
			OSTART(1)
				PUSH8(Opcode::INOT)
			OEND
		}

		inline void ior() {
			OSTART(1)
				PUSH8(Opcode::IOR)
			OEND
		}

		inline void iand() {
			OSTART(1)
				PUSH8(Opcode::IAND)
			OEND
		}

		inline void label(const char* label) {
			if (label_mode) labels[label] = cur_ip;
		}

		inline void enter(const char* name, uint8_t params, uint16_t locals) {
			if (label_mode) labels[name] = cur_ip;
			OSTART(5)
				PUSH8(Opcode::ENTER)
				PUSH8(params)
				PUSH16(locals)
				PUSH8(0) // funcNameLen

				current_params = params;
			OEND
		}

		inline void leave(uint8_t params, uint8_t returns) {
			OSTART(3)
				PUSH8(Opcode::LEAVE)
				PUSH8(params)
				PUSH8(returns)
			OEND
		}

		inline void drop() {
			OSTART(1)
				PUSH8(Opcode::DROP)
			OEND
		}

		inline void dup() {
			OSTART(1)
				PUSH8(Opcode::DUP)
			OEND
		}

		inline void f2v() {
			OSTART(1)
				PUSH8(Opcode::F2V)
			OEND
		}

		inline void native(uint64_t hash, uint8_t params, uint8_t returns) {
			OSTART(4)
				PUSH8(Opcode::NATIVE)
				PUSH8((params << 2) | (returns & 0x3))
				std::uint32_t native_idx = get_or_make_native_idx(hash);
				PUSH8((native_idx >> 8));
				PUSH8((native_idx & 0x00FF));
			OEND
		}

		inline void call(const char* label) {
			OSTART(4)
				PUSH8(Opcode::CALL)
				PUSH24(get_label_ip(label))
			OEND
		}

		inline void jmp(const char* label) {
			OSTART(3)
				PUSH8(Opcode::J)
				PUSH16(((uint16_t)get_label_ip(label)) - ((uint16_t)cur_ip) - 3)
			OEND
		}

		inline void jz(const char* label) {
			OSTART(3)
				PUSH8(Opcode::JZ)
				PUSH16(((uint16_t)get_label_ip(label)) - ((uint16_t)cur_ip) - 3)
			OEND
		}

		inline void je(const char* label) {
			OSTART(3)
				PUSH8(Opcode::INE_JZ)
				PUSH16(((uint16_t)get_label_ip(label)) - ((uint16_t)cur_ip) - 3)
			OEND
		}

		inline void jne(const char* label) {
			OSTART(3)
				PUSH8(Opcode::IEQ_JZ)
				PUSH16(((uint16_t)get_label_ip(label)) - ((uint16_t)cur_ip) - 3)
			OEND
		}

		inline void load() {
			OSTART(1)
				PUSH8(Opcode::LOAD)
			OEND
		}

		inline void store() {
			OSTART(1)
				PUSH8(Opcode::STORE)
			OEND
		}

		inline void global(uint32_t idx) {
			OSTART(4)
				PUSH8(Opcode::GLOBAL_U24)
				PUSH24(idx)
			OEND
		}

		inline void global_load(uint32_t idx) {
			OSTART(4)
				PUSH8(Opcode::GLOBAL_U24_LOAD)
				PUSH24(idx)
			OEND
		}

		inline void global_store(uint32_t idx) {
			OSTART(4)
				PUSH8(Opcode::GLOBAL_U24_STORE)
				PUSH24(idx)
			OEND
		}

		inline void local(uint16_t idx) {
			OSTART(3)
				PUSH8(Opcode::STATIC_U16)
				PUSH16(idx)
			OEND
		}

		inline void local_load(uint16_t idx) {
			OSTART(3)
				PUSH8(Opcode::STATIC_U16_LOAD)
				PUSH16(idx)
			OEND
		}

		inline void local_store(uint16_t idx) {
			OSTART(3)
				PUSH8(Opcode::STATIC_U16_STORE)
				PUSH16(idx)
			OEND
		}

		inline void offset(uint16_t off) {
			OSTART(3)
				PUSH8(Opcode::IOFFSET_S16)
				PUSH16(off)
			OEND
		}

		inline void offset_load(uint16_t off) 
		{
			OSTART(3)
				PUSH8(Opcode::IOFFSET_S16_LOAD)
				PUSH16(off)
			OEND
		}

		inline void offset_store(uint16_t off) 
		{
			OSTART(3)
				PUSH8(Opcode::IOFFSET_S16_STORE)
				PUSH16(off)
			OEND
		}

		inline void array(uint16_t size) 
		{
			OSTART(3)
				PUSH8(Opcode::ARRAY_U16)
				PUSH16(size)
			OEND
		}

		inline void array_load(uint16_t size) 
		{
			OSTART(3)
				PUSH8(Opcode::ARRAY_U16_LOAD)
				PUSH16(size)
			OEND
		}

		inline void array_store(uint16_t size) 
		{
			OSTART(3)
				PUSH8(Opcode::ARRAY_U16_STORE)
				PUSH16(size)
			OEND
		}

		inline void frame(uint16_t idx)
		{
			OSTART(3)
				PUSH8(Opcode::LOCAL_U16)
				PUSH16(2+current_params+idx)
			OEND
		}

		inline void frame_load(uint16_t idx)
		{
			OSTART(3)
				PUSH8(Opcode::LOCAL_U16_LOAD)
				PUSH16(2 + current_params + idx)
			OEND
		}

		inline void frame_store(uint16_t idx)
		{
			OSTART(3)
				PUSH8(Opcode::LOCAL_U16_STORE)
				PUSH16(2 + current_params + idx)
			OEND
		}
	};
};

#undef OSTART
#undef OEND
#undef PUSH8
#undef PUSH16
#undef PUSH32
#undef PUSH24
#include "script_loader.hpp"
#include "crossmap.hpp"
#include "rage/natives.hpp"
#include "natives.hpp"

#include "program/main.hpp"

namespace ext 
{
	script_loader::script_loader() :
		m_bytecode(build_program()),
		m_native_registration(g_pointers->m_native_registration_table),
		m_required_script($(code_controller))
	{
		cache_handlers();
		find_thread();
		initalize_thread();

		g_script_loader = this;
	}

	script_loader::~script_loader()
	{
		if (m_thread.valid())
			m_thread.set_state(rage::eThreadState::Paused);
		g_script_loader = nullptr;
	}

	void script_loader::find_thread()
	{
		LOG(INFO) << "Waiting for code_controller...";
		while ((!m_thread.valid()) || (!m_program.valid()))
		{
			m_thread = rage::scrThread::get_thread_by_hash(m_required_script);
			m_program = rage::scrProgram::get_program_by_hash(m_required_script);
			::Sleep(100);
		}
	}

	void script_loader::cache_handlers()
	{
		LOG(INFO) << "Caching native handlers...";
		for (auto& p : ysc::crossmap) 
		{
			m_handler_cache[p.second] = m_native_registration.get_entrypoint_from_hash(p.second);
		}
	}

	void script_loader::initalize_thread()
	{
		LOG(INFO) << "Preparing script...";
		m_thread.set_state(rage::eThreadState::Paused);
		g_process->set_paused(true);

		m_thread.reset();

		uint64_t page = m_program.get_code_page(0); // fine for now, later we may need to use multiple pages
		g_process->write_raw(page, m_bytecode.opcode_size, m_bytecode.opcodes);

		uint64_t natives = m_program.get_native_table();

		for (int i = 0; i < m_bytecode.native_table.size(); i++)
		{
			g_process->write<std::uint64_t>(natives + (uint64_t)(i * 8), m_handler_cache[m_bytecode.native_table[i]]);
		}

		auto m_original_vft = m_thread.get_handler_vft();
		m_fake_vft = g_process->allocate(20 * 8);

		LOG(INFO) << GetLastError();
		assert(m_fake_vft != NULL);

		for (uint64_t i = 0; i < 20; i++)
		{
			g_process->write<uint64_t>(m_fake_vft + (i * 8), g_process->read<uint64_t>(m_original_vft + (i * 8)));
		}

		g_process->write<uint64_t>(m_fake_vft + (6 * 8), g_pointers->m_ret_true_function);

		m_thread.set_handler_vft(m_fake_vft);

		m_program.mark_program_as_ours();

		g_process->set_paused(false);
		m_thread.set_state(rage::eThreadState::Running);
	}

	ysc::program script_loader::build_program()
	{
		return ysc::program([](ysc::program& p) {
			program::main(p);
		});
	}

}
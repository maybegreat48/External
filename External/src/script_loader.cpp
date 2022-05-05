#include "script_loader.hpp"
#include "crossmap.hpp"
#include "rage/natives.hpp"
#include "natives.hpp"
#include "ysc_file.hpp"

namespace ext 
{
	script_loader::script_loader() :
		m_native_registration(g_pointers->m_native_registration_table),
		m_required_script($(code_controller))
	{
		find_thread();
		pick_script();
		initalize_thread();

		g_script_loader = this;
	}

	script_loader::~script_loader()
	{
		if (m_thread.valid() && g_process->is_running())
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

	void script_loader::pick_script()
	{
		// https://stackoverflow.com/a/49652087

		memset(&m_ofn, 0, sizeof(m_ofn));
		m_ofn.lStructSize = sizeof(m_ofn);
		m_ofn.hwndOwner = NULL;
		m_ofn.lpstrFile = m_file_name;
		m_ofn.nMaxFile = sizeof(m_file_name);
		m_ofn.lpstrFilter = "Rage Script (no RSC7 header)\0*.YSC;*.YSC.FULL\0\0";
		m_ofn.nFilterIndex = 1;
		m_ofn.lpstrFileTitle = NULL;
		m_ofn.nMaxFileTitle = 0;
		m_ofn.lpstrInitialDir = NULL;
		m_ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (::GetOpenFileNameA(&m_ofn) != TRUE)
		{
			exit(0);
		}
	}

	void script_loader::initalize_thread()
	{
		LOG(INFO) << "Preparing script...";

		std::filesystem::path fpath(m_file_name);
		ysc_file file(fpath);

		LOG(INFO) << "Caching native handlers...";
		for (auto& p : file.m_natives)
		{
			m_handler_cache[p] = m_native_registration.get_entrypoint_from_hash(p);
		}

		m_thread.set_state(rage::eThreadState::Paused);
		g_process->set_paused(true);

		m_thread.reset();

		if (m_program.get_code_size() < file.m_code_length)
			LOG(FATAL) << "Cannot fit " << file.m_code_length << " bytes into program, maximum is " << m_program.get_code_size();

		if (m_program.get_string_size() < file.m_string_size)
			LOG(FATAL) << "Cannot fit " << file.m_string_size << " string chars into program, maximum is " << m_program.get_string_size();

		if (m_program.get_num_natives() < file.m_natives_count)
			LOG(FATAL) << "Cannot fit " << file.m_natives_count << " natives into program, maximum is " << m_program.get_num_natives();

		if ((m_thread.get_stack_size() - 200) < file.m_static_count)
			LOG(FATAL) << "Cannot fit " << file.m_static_count << " statics into stack, maximum is " << (m_thread.get_stack_size() - 200);

		for (int i = 0; i < file.m_code_block_list.size(); i++)
		{
			int tablesize = ((i + 1) * 0x4000 >= file.m_code_length) ? file.m_code_length % 0x4000 : 0x4000;
			uint64_t codepage = m_program.get_code_page(i);
			g_process->write_raw(codepage, tablesize, file.m_code_block_list[i]);
		}

		for (int i = 0; i < file.m_string_block_list.size(); i++)
		{
			int tablesize = ((i + 1) * 0x4000 >= file.m_string_size) ? file.m_string_size % 0x4000 : 0x4000;
			uint64_t stringpage = m_program.get_string_page(i);
			g_process->write_raw(stringpage, tablesize, file.m_string_block_list[i]);
		}

		uint64_t natives = m_program.get_native_table();

		for (int i = 0; i < file.m_natives.size(); i++)
		{
			g_process->write<std::uint64_t>(natives + (uint64_t)(i * 8), m_handler_cache[file.m_natives[i]]);
		}

		g_process->write_raw(m_thread.get_stack(), file.m_static_count * 8, file.m_statics);
		m_thread.set_stack_ptr(file.m_static_count + 1);

		//g_process->write_raw(m_thread.get_name(), strlen(file.m_name), file.m_name);
		//m_thread.set_hash(file.m_name_hash);

		// model spawn bypass

		auto m_original_vft = m_thread.get_handler_vft();
		m_fake_vft = g_process->allocate(20 * 8);

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
}
#include "views/view.hpp"

namespace ext
{
	void view::session() {
#if 0
		for (const SessionType& session_type : sessions)
		{
			components::button(session_type.name, [session_type] {
				session::join_type(session_type);
				});
		}
#endif
	}
}